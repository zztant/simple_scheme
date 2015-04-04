#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"
#include"compile.h"
#include"object.c"
#include"eval.c"
#include"lex.c"
#include"print.c"
#include"gc.c"
#include"vm.c"
#include"compile.c"

#define add_procedure(scheme_name,c_name,vm) \
	vm->env = add_bind(make_symbol(scheme_name),        \
			  make_prim_proc(c_name),     \
			  vm->env);      

void init(){
	global_gc = init_gc();
	global_vm = init_vm();
	
	add_procedure("+",prim_proc_plus,global_vm);
	add_procedure("-",prim_proc_minus,global_vm);
	add_procedure("*",prim_proc_mul,global_vm);
	add_procedure("/",prim_proc_div,global_vm);
	add_procedure("not",prim_proc_not,global_vm);
	add_procedure("and",prim_proc_and,global_vm);
	add_procedure("or",prim_proc_or,global_vm);
	add_procedure("=",prim_proc_equal,global_vm);
	add_procedure("<",prim_proc_lower,global_vm);
	add_procedure(">",prim_proc_greater,global_vm);
	add_procedure("cons",prim_proc_cons,global_vm);
	add_procedure("list",prim_proc_list,global_vm);
	add_procedure("null?",prim_proc_is_null,global_vm);
	add_procedure("procedure?",prim_proc_is_proc,global_vm);
	add_procedure("pair?",prim_proc_is_pair,global_vm);
	add_procedure("string?",prim_proc_is_string,global_vm);
	add_procedure("boolean?",prim_proc_is_boolean,global_vm);
	add_procedure("symbol?",prim_proc_is_symbol,global_vm);
	add_procedure("eq?",prim_proc_eq,global_vm);
	add_procedure("car",prim_proc_car,global_vm);
	add_procedure("cdr",prim_proc_cdr,global_vm);
	add_procedure("apply",prim_proc_apply,global_vm);
	add_procedure("eval",prim_proc_eval,global_vm); 
	add_procedure("load",prim_proc_load,global_vm);
	add_procedure("display",prim_proc_display,global_vm);
	add_procedure("newline",prim_proc_newline,global_vm);

	FILE* fp = fopen("init.scm","rw");
	compile_file(fp,global_vm);
	eval_vm(global_vm);
	global_vm->stack = make_null_list();
	fclose(fp);
}

int main(){
	FILE* input;
	object* stmt;
	init();
	while(1){
		printf("\nscheme@zztant>");
		stmt = read_exp(stdin);
	//	print_object(stdout,stmt);
	//	printf("\n");
		compile(stmt,global_vm);
	//	print_code(global_vm->code);
	//	printf("\n");
		eval_vm(global_vm);
		print_ans(stdout,global_vm->stack);
		global_vm->stack = make_null_list();
	}
	return 0;
}

