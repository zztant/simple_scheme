#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"
#include"macro.h"

#define symbol_true (make_boolean(1))
#define symbol_false (make_boolean(0))

void error_type(char* proc_name){
	fprintf(stderr,"ERROR:: wrong type of argument for `%s' procedure\n",proc_name);
	exit(1);
}

void error_argu_num(char* proc_name){
	fprintf(stderr,"ERROR:: wrong number of arguments for `%s' procedure\n",proc_name);
	exit(1);
}

char check_argu_num(object* arguments, char n){
	if(is_null_list(arguments) && n == 0)
		return 1;
	else if(!is_null_list(arguments) && n!=0 )
		return check_argu_num(cdr(arguments),n-1);
	else
		return 0;
}

object* ratio_to_real(object* obj){
	return make_real((double)obj->data.s_rational.value1
			        /(double)obj->data.s_rational.value2);
}

object* plus_ratio_ratio(object* obj1, object* obj2){
	int value1= obj1->data.s_rational.value1;
	int value2= obj1->data.s_rational.value2;
	value1 = value1 * (obj2->data.s_rational.value2)
		    +value2 * (obj2->data.s_rational.value1);
	value2 = value2 * (obj2->data.s_rational.value2);
	return make_rational(value1,value2);
}

object* plus_real_real(object* obj1, object* obj2){
	double sum = obj1->data.s_real.value
		        +obj2->data.s_real.value;
	return make_real(sum);
}

object* plus_ratio_real(object* obj1, object* obj2){
	double value2 = obj2->data.s_real.value;
	obj1= ratio_to_real(obj1);
	double value1 = obj1->data.s_real.value;
	return make_real(value1+value2);
}

object* minus_ratio_ratio(object* obj1, object* obj2){
	int value1=obj1->data.s_rational.value1;
	int value2=obj1->data.s_rational.value2;
	value1 = value1 * (obj2->data.s_rational.value2)
		    -value2 * (obj2->data.s_rational.value1);
	value2 = value2 * (obj2->data.s_rational.value2);
	return make_rational(value1,value2);
}

object* minus_ratio_real(object* obj1, object* obj2){
	obj1 = ratio_to_real(obj1);
	return make_real(obj1->data.s_real.value
			        -obj2->data.s_real.value);
}

object* minus_real_ratio(object* obj1, object* obj2){
	obj2 = ratio_to_real(obj2);
	return make_real(obj1->data.s_real.value
			        -obj2->data.s_real.value);
}

object* minus_real_real(object* obj1, object* obj2){
	return make_real(obj1->data.s_real.value
			        -obj2->data.s_real.value);
}



object* mul_ratio_real(object* obj1, object* obj2){
	obj1 = ratio_to_real(obj1);
	return make_real(obj1->data.s_real.value
			        *obj2->data.s_real.value);
}

object* mul_ratio_ratio(object* obj1, object* obj2){
	int value1 = obj1->data.s_rational.value1
		        *obj2->data.s_rational.value1;
	int value2 = obj1->data.s_rational.value2
		        *obj2->data.s_rational.value2;
	return make_rational(value1,value2);
}

object* mul_real_real(object* obj1, object* obj2){
	return make_real(obj1->data.s_real.value
			        *obj2->data.s_real.value);
}

object* div_ratio_real(object* obj1, object* obj2){
	obj1 = ratio_to_real(obj1);
	return make_real(obj1->data.s_real.value
			        /obj2->data.s_real.value);
}

object* div_real_ratio(object* obj1, object* obj2){
	obj2 = ratio_to_real(obj2);
	return make_real(obj1->data.s_real.value
			        /obj2->data.s_real.value);
}

object* div_ratio_ratio(object* obj1, object* obj2){
	int value1 = obj1->data.s_rational.value1
		        *obj2->data.s_rational.value2;
	int value2 = obj1->data.s_rational.value2
		        *obj2->data.s_rational.value1;
	return make_rational(value1,value2);
}

object* div_real_real(object* obj1, object* obj2){
	return make_real(obj1->data.s_real.value
					/obj2->data.s_real.value);
}

object* prim_proc_plus(object* arguments){
	object* obj = make_rational(0,1);
	object* obj_car;
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_number(obj_car))
			error_type("+");
		if(is_rational(obj)&&is_rational(obj_car))
			obj = plus_ratio_ratio(obj,obj_car);
		else if(is_rational(obj)&&is_real(obj_car))
			obj = plus_ratio_real(obj,obj_car);
		else if(is_real(obj)&&is_rational(obj_car))
			obj = plus_ratio_real(obj_car,obj);
		else
			obj = plus_real_real(obj,obj_car);
		arguments = cdr(arguments);
	}
	return obj;
}

object* prim_proc_minus(object* arguments){
	object* obj;
	int flag = 0;
	object* obj_car;
	if(is_null_list(arguments))
		error_argu_num("-");
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_number(obj_car))
			error_type("-");
		flag ++ ;
		if( flag == 1 ){
			obj = obj_car;
			arguments = cdr(arguments);
			continue;
		}
		if(is_rational(obj)&&is_rational(obj_car))
			obj = minus_ratio_ratio(obj,obj_car);
		else if(is_rational(obj)&&is_real(obj_car))
			obj = minus_ratio_real(obj,obj_car);
		else if(is_real(obj)&&is_rational(obj_car))
			obj = minus_real_ratio(obj,obj_car);
		else
			obj = minus_real_real(obj,obj_car);
		arguments = cdr(arguments);
	}
	return obj;
}

object* prim_proc_mul(object* arguments){
	object* obj = make_rational(1,1);
	object* obj_car;
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_number(obj_car))
			error_type("*");
		if(is_rational(obj)&&is_rational(obj_car))
			obj = mul_ratio_ratio(obj,obj_car);
		else if(is_rational(obj)&&is_real(obj_car))
			obj = mul_ratio_real(obj,obj_car);
		else if(is_real(obj)&&is_rational(obj_car))
			obj = mul_ratio_real(obj_car,obj);
		else
			obj = mul_real_real(obj,obj_car);
		arguments = cdr(arguments);
	}
	return obj;
}

object* prim_proc_div(object* arguments){
	object* obj;
	object* obj_car;
	int flag = 0;
	if(is_null_list(arguments))
		error_argu_num("/");
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_number(obj_car))
			error_type("/");
		flag++;
		if(flag == 1){
			obj = obj_car;
			arguments = cdr(arguments);
			continue;
		}
		if(is_rational(obj)&&is_rational(obj_car))
			obj = div_ratio_ratio(obj,obj_car);
		else if(is_rational(obj)&&is_real(obj_car))
			obj = div_ratio_real(obj,obj_car);
		else if(is_real(obj)&&is_rational(obj_car))
			obj = div_real_ratio(obj,obj_car);
		else
			obj = div_real_real(obj,obj_car);
		arguments = cdr(arguments);
	}
	return obj;
}

object* prim_proc_not(object* argu){
	if( check_argu_num(argu,0) || check_argu_num(argu,2))
		error_argu_num("not");
	argu = car(argu);
	if(is_false(argu))
		return symbol_true;
	else
		return symbol_false;
}

//need change
char value_equal(object* val1, object* val2){
	if(is_rational(val1))
		val1 = ratio_to_real(val1);
	if(is_rational(val2))
		val2 = ratio_to_real(val2);
	return val1->data.s_real.value == val2->data.s_real.value;
}

char value_lower(object* val1, object* val2){
	if(is_rational(val1))
		val1 = ratio_to_real(val1);
	if(is_rational(val2))
		val2 = ratio_to_real(val2);
	return val1->data.s_real.value < val2->data.s_real.value;
}

char value_greater(object* val1, object* val2){
	if(is_rational(val1))
		val1 = ratio_to_real(val1);
	if(is_rational(val2))
		val2 = ratio_to_real(val2);
	return val1->data.s_real.value > val2->data.s_real.value;
}

object* prim_proc_equal(object* argu){
	if(is_null_list(argu)) 
		return symbol_true;
	object *obj1, *obj2;
    obj1 = car(argu);
	argu = cdr(argu);
	while(!is_null_list(argu)){
		obj2 = car(argu);
		if ( !is_number(obj1) || !is_number(obj2))
			error_type("=");
		if( !value_equal(obj1,obj2) )
			return symbol_false;
		obj1 = obj2;
		argu = cdr(argu);
	}
	return symbol_true;
}

object* prim_proc_lower(object* argu){
	if(is_null_list(argu)) 
		return symbol_true;
	object *obj1, *obj2;
    obj1 = car(argu);
	argu = cdr(argu);
	while(!is_null_list(argu)){
		obj2 = car(argu);
		if ( !is_number(obj1) || !is_number(obj2))
			error_type("<");
		if( !value_lower(obj1,obj2) )
			return symbol_false;
		obj1 = obj2;
		argu = cdr(argu);
	}
	return symbol_true;
}

object* prim_proc_greater(object* argu){
	if(is_null_list(argu)) 
		return symbol_true;
	object *obj1, *obj2;
    obj1 = car(argu);
	argu = cdr(argu);
	while(!is_null_list(argu)){
		obj2 = car(argu);
		if ( !is_number(obj1) || !is_number(obj2))
			error_type(">");
		if( !value_greater(obj1,obj2) )
			return symbol_false;
		obj1 = obj2;
		argu = cdr(argu);
	}
	return symbol_true;
}

object* prim_proc_cons(object* arguments){
	if( check_argu_num(arguments,0) || check_argu_num(arguments,3))
		error_argu_num("cons");
	return cons(car(arguments),cadr(arguments));
}


object* prim_proc_is_null(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("null?");
	exp = car(exp);
	if(is_null_list(exp)) 
		return symbol_true;
	else
		return symbol_false;
}

object* prim_proc_is_proc(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("procedure?");
	exp = car(exp);
	return (is_prim_proc(exp) || is_comp_proc(exp))?symbol_true:symbol_false;
}

object* prim_proc_is_pair(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("pair?");
	exp = car(exp);
	return is_pair(exp)?symbol_true:symbol_false;
}

object* prim_proc_is_string(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("string?");
	exp = car(exp);
	return is_string(exp)?symbol_true:symbol_false;
}

object* prim_proc_is_boolean(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("boolean?");
	exp = car(exp);
	return is_boolean(exp)?symbol_true:symbol_false;
}

object* prim_proc_is_symbol(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("symbol?");
	exp = car(exp);
	return is_symbol(exp)?symbol_true:symbol_false;
}


object* prim_proc_is_number(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("number?");
	exp = car(exp);
	return is_number(exp)?symbol_true:symbol_false;
}

object* prim_proc_eq(object* exp){
	if( check_argu_num(exp,0) || check_argu_num(exp,1) ||
	    check_argu_num(exp,3))
		error_argu_num("eq?");
	object* obj1 = car(exp);
	object* obj2 = cadr(exp);

	if(obj1->type != obj2->type)
		return symbol_false;
	switch(obj1->type){
		case RATIONAL: return prim_proc_equal(exp);
		case STRING:
			return (strcmp(obj1->data.s_string.value,
						   obj2->data.s_string.value) == 0)?
					symbol_true:symbol_false;
		default:
			return (obj1 == obj2) ? symbol_true:symbol_false;
	}
}

object* prim_proc_car(object* exp){
	if(check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("car");
	if(!is_pair(car(exp)))
		error_type("car");
	return caar(exp);
}

object* prim_proc_cdr(object* exp){
	if(check_argu_num(exp,0) || check_argu_num(exp,2))
		error_argu_num("cdr");
	if(!is_pair(car(exp)))
		error_type("cdr");
	return cdar(exp);
}

object* prim_proc_apply(object* exp){
	if(check_argu_num(exp,0) || check_argu_num(exp,1) ||
	   check_argu_num(exp,3))
		error_argu_num("apply");	
	object* proc = car(exp);
	if(!is_prim_proc(proc)&&!is_comp_proc(proc))
		error_type("apply");
	object* argu = cadr(exp);
	return apply(proc,argu);
}

object* prim_proc_eval(object* exp){
	if(check_argu_num(exp,0) || check_argu_num(exp,1) ||
	   check_argu_num(exp,3))
		error_argu_num("eval");	
	exp = caar(exp);
	return make_null_list();
}

object* prim_proc_load(object* arguments){
	if(check_argu_num(arguments,0) || check_argu_num(arguments,2))
		error_argu_num("load");
	object* file_obj = car(arguments);
	if(!is_string(file_obj))
		error_type("load");
	char* file_name = file_obj->data.s_string.value;
	object* stmt;
	FILE* file = fopen(file_name,"rw");
	if(file==NULL){
		fprintf(stderr,"ERROR:: open file `%s' error\n",file_name);
		return make_symbol("#wrong");
	}
	compile_file(file,global_vm);
	print_code(global_vm->code);
	eval_vm(global_vm);
	fclose(file);
	return make_symbol("#ok");
}

object* prim_proc_display(object* arguments){
	if(check_argu_num(arguments,0) || check_argu_num(arguments,2))
		error_argu_num("display");
	object* str = car(arguments);
	print_object(stdout,str);
	return make_symbol("#ok");
}

object* prim_proc_newline(object* arguments){
	if(check_argu_num(arguments,1))
		error_argu_num("newline");
	fprintf(stdout,"\n");
	return make_symbol("#ok");
}

object* prim_proc_read(object* arguments){
	if(check_argu_num(arguments,1))
		error_argu_num("read");
	return read_exp(stdin);
}

char symbol_compare(object* exp, char* value){
	return is_symbol(exp) && strcmp(exp->data.s_symbol.value,value) == 0;
}

char is_definition(object* exp){
	return is_pair(exp) && (symbol_compare(car(exp),"define") ||
			                symbol_compare(car(exp),"define-syntax"));
}

char is_quoted(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"quote");
}

char is_set(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"set!");
}

char is_begin(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"begin");
}

char is_lambda(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"lambda");
}

char is_if(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"if");
}

char is_eval(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"eval");
}

char is_callcc(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"call/cc");
}

char is_syntax(object* exp){
	return is_pair(exp) && symbol_compare(car(exp),"syntax-rules");
}


char is_false(object* obj){
	return obj->type == BOOLEAN &&
		   obj->data.s_boolean.value == 0;
}

char is_true(object* obj){
	return !is_false(obj);
}

object* apply(object* proc, object* arguments){
	return (*(proc->data.s_prim_proc.func))(arguments);
}




