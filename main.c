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



void init(){
	null_list = (object*)malloc(sizeof(object));
	null_list->type = NULL_LIST;
	global_gc = init_gc();
	global_vm = init_vm();

	symbol_quote = make_symbol("quote");
	symbol_lambda = make_symbol("lambda");
	symbol_define = make_symbol("define");
	symbol_true = make_symbol("#t");
	symbol_false = make_symbol("#f");
	symbol_set = make_symbol("set!");
	symbol_begin = make_symbol("begin");
	symbol_if = make_symbol("if");
	symbol_ok = make_symbol("#ok");
}


int main(){
	FILE* input;
	object* stmt;
	init();
	while(1){
		printf("\n>");
		stmt = read_exp(stdin);
		compile(stmt,global_vm);
		print_code();
		printf("\n");
		collect(global_gc,global_vm);
	}
	return 0;
}

