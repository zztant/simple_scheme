#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"


secd_vm* init_vm(){
	secd_vm* vm = (secd_vm*)malloc(sizeof(secd_vm));
	vm->stack = make_null_list();
	vm->code = make_null_list();
	vm->dump = make_null_list();
	vm->table = make_null_list();
	vm->env = make_null_list();
	return vm;
}

void error_handling(){
	printf("\nhas some error during eval!!");
	exit(1);
}

object* lookup_variable_value(object* var, object* env){
	if(!is_null_list(env)){
		if(var == caar(env))
			return cdar(env);
		else
			return lookup_variable_value(var,cdr(env));
	}
	return make_null_list();
}

object* lookup_bind(object* var, object* env){
	if(!is_null_list(env)){
		if(var == caar(env))
			return car(env);
		else
			return lookup_bind(var,cdr(env));
	}
	return make_null_list();
}

object* pop(secd_vm* vm){
	object* val = car(vm->stack);
	vm->stack = cdr(vm->stack);
	return val;
}

object* add_bind(object* var, object* val, object* env){
	object* bind = lookup_bind(var,env);
	if(is_null_list(bind)){
		env = cons(cons(var,val),env);
		return env;
	}
	else{
		bind->data.s_pair.cdr = val;
		return env;
	}
}

void set_bind(object* var, object* val, object* env){
	object* bind = lookup_bind(var,env);
	if(is_null_list(bind))
		error_handling();
	else
		bind->data.s_pair.cdr = val;
}

void eval_ldc(object* code, secd_vm* vm){
	object* argu = car(code);
	vm->stack = cons(argu,vm->stack);
}

void eval_ld(object* code, secd_vm* vm){
	object* var = car(code);
	object* val = lookup_variable_value(var,vm->env);
	if(is_null_list(val)){
		object* dump = vm->dump;
		while(!is_null_list(dump)){
			if(!is_code(car(dump)))
				val = lookup_variable_value(var,cadar(dump));
			if(!is_null_list(val))
				break;
			dump = cdr(dump);
		}
	}
	if(is_null_list(val))
		error_handling();
	vm->stack = cons(val,vm->stack);
}


void eval_sel(object* code, secd_vm* vm){
	object* cond = pop(vm);
	object* code_true = caar(code);
	object* code_false = cdar(code);
	object* code_next = cdr(code);
	vm->dump = cons(code_next,vm->dump);
	if( is_true(cond) )
		vm->code = code_true;
	else
		vm->code = code_false;
}

void eval_join(object* code, secd_vm* vm){
	vm->code = car(vm->dump);
	vm->dump = cdr(vm->dump);
}

void eval_ldf(object* code, secd_vm* vm){
	object* param = caar(code);
	object* body = cdar(code);
	object* env = vm->env;
	object* closure = make_comp_proc(param, body, env);
	vm->stack = cons(closure,vm->stack);
}

object* add_bind_ap(object* param, object* argu, object* env){
	if(!is_null_list(argu) && !is_null_list(param)){
		env = cons(cons(car(param), car(argu)), env);
		env = add_bind_ap(cdr(param), cdr(argu), env);
		return env;
	}
	return env;
}


void eval_ap_prim(object* code, secd_vm* vm){
	object* proc = pop(vm);
	object* argu = pop(vm);
	vm->stack = cons(apply(proc,argu),vm->stack);
}


void eval_ret(object* code, secd_vm* vm){
	object* val = pop(vm);
	vm->stack = caar(vm->dump);
	vm->env = cadar(vm->dump);
	vm->code = caddar(vm->dump);
	vm->stack = cons(val,vm->stack);
	vm->dump = cdr(vm->dump);
}

void eval_ap_comp(object* code, secd_vm* vm){
	object* closure = pop(vm);
	object* argu = pop(vm);
	object* param = closure->data.s_comp_proc.parameters;
	object* body = closure->data.s_comp_proc.body;
	object* env = closure->data.s_comp_proc.env;
	
	/*清空寄存器*/
	vm->dump = cons(cons(vm->stack,cons(vm->env,cons(vm->code,make_null_list()))),vm->dump);
	vm->stack = make_null_list();
	/*安装闭包环境,添加对自由变量的绑定*/
	vm->env = add_bind_ap(param,argu,env);
	vm->code = body;
}

void eval_ap(object* code, secd_vm* vm){
	object* proc = car(vm->stack);
	if(is_prim_proc(proc))
		eval_ap_prim(code,vm);
	else if(is_comp_proc(proc))
		eval_ap_comp(code,vm);
}

void eval_dum(object* code, secd_vm* vm){
	vm->env = cons(make_null_list(),vm->env);	
}

void eval_rap(object* code, secd_vm* vm){
	object* closure = pop(vm);
	object* argu = pop(vm);
	object* param = closure->data.s_comp_proc.parameters;
	object* body = closure->data.s_comp_proc.body;
	object* env = closure->data.s_comp_proc.env;
	if(is_null_list(car(vm->env)))
		vm->env = cdr(vm->env);

	vm->dump = cons(vm->stack,cons(vm->env,cons(vm->code,vm->dump)));
	vm->stack = make_null_list();

	vm->code = body;
	vm->env = add_bind_ap(param,argu,vm->env);

}

void eval_def(object* code, secd_vm* vm){
	object* val = pop(vm);
	object* var = car(code);
	vm->env = add_bind(var,val,vm->env);
}

void eval_set(object* code, secd_vm* vm){
	object* val = pop(vm);
	object* var = car(code);
	set_bind(var,val,vm->env);
}

object* get_argu_list(char argu_num, secd_vm* vm){
	if( argu_num == 0 )
		return make_null_list();
	else{
		object* argu = pop(vm);
		return cons(argu, get_argu_list(argu_num-1, vm));
	}
}

void eval_list(object* code, secd_vm* vm){
	char argu_num = car(code)->data.s_rational.value1;
	object* argu_list = get_argu_list(argu_num,vm);
	vm->stack = cons(argu_list,vm->stack);
}

void eval_vm(secd_vm* vm){
start:
	if(is_null_list(vm->code))
		return;
	collect(global_gc,vm);
	object* code = vm->code;
	vm->code = cdr(vm->code);
	switch(code->type){
		case NIL: break;
		case LDC: eval_ldc(code,vm); break;
		case LD:  eval_ld(code,vm); break;
		case SEL: eval_sel(code,vm); break;
		case JOIN:eval_join(code,vm); break;
		case LDF: eval_ldf(code,vm); break;
		case AP:  eval_ap(code,vm); break;
		case RET: eval_ret(code,vm); break;
		case DUM: eval_dum(code,vm); break;
		case RAP: eval_rap(code,vm); break;
		case DEFINE: eval_def(code,vm); break;
		case SET: eval_set(code,vm); break;
		case LIST: eval_list(code,vm); break;
		default: fprintf(stderr,"Wrong vm_code type!%d\n",code->type); exit(1);
	}
	goto start;
}

void print_ans(FILE* file, object* stack){
	if(is_null_list(stack))
		return;
	print_ans(file,cdr(stack));
	print_object(file,car(stack));
}
