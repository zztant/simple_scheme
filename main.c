#include"header.h"

void init(){
	null_list = make_null_list();
	global_env = make_env(null_list);
	symbol_table = null_list;

	proc_plus = make_prim_proc(prim_proc_plus);
	proc_minus = make_prim_proc(prim_proc_minus);
	proc_mul = make_prim_proc(prim_proc_mul);
	proc_div = make_prim_proc(prim_proc_div);

	symbol_quote = make_symbol("quote");
	symbol_lambda = make_symbol("lambda");
	symbol_define = make_symbol("define");
	symbol_true = make_symbol("#t");
	symbol_false = make_symbol("#f");
	symbol_plus = make_symbol("+");
	symbol_minus = make_symbol("-");
	symbol_mul = make_symbol("*");
	symbol_div = make_symbol("/");
	symbol_ok = make_symbol("#ok");
	symbol_set = make_symbol("set!");
	symbol_begin = make_symbol("begin");
	symbol_else = make_symbol("else");
	symbol_if = make_symbol("if");
	symbol_and = make_symbol("and");
	symbol_or = make_symbol("or");
	
	add_bind(symbol_plus, proc_plus, global_env);
	add_bind(symbol_minus, proc_minus, global_env);
	add_bind(symbol_mul, proc_mul, global_env);
	add_bind(symbol_div, proc_div, global_env);
	
}

int main(){
	FILE* input;
	object* stmt;
	init();
	while(1){
		printf(">");
		stmt = read_exp(stdin);
		if(stmt == NULL) break;
		print_object(stdout,eval(stmt,global_env) );
	}
	return 0;
}

