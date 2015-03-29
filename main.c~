#include"header.h"

#define add_procedure(scheme_name, c_name) \
	add_bind(make_symbol(scheme_name),     \
			 make_prim_proc(c_name),       \
			 global_env);
#define make_scheme_proc(c_procedure) \
			
void init(){
	null_list = make_null_list();
	global_env = make_env(null_list);
	symbol_table = null_list;

	symbol_quote = make_symbol("quote");
	symbol_lambda = make_symbol("lambda");
	symbol_define = make_symbol("define");
	symbol_true = make_boolean(1);
	symbol_false = make_boolean(0);
	symbol_set = make_symbol("set!");
	symbol_begin = make_symbol("begin");
	symbol_if = make_symbol("if");
	symbol_ok = make_symbol("#ok");

	add_procedure("and",prim_proc_and);
	add_procedure("or",prim_proc_or);
	add_procedure("+",prim_proc_plus);
	add_procedure("-",prim_proc_minus);
	add_procedure("*",prim_proc_mul);
	add_procedure("/",prim_proc_div);
	add_procedure("=",prim_proc_equal);
	add_procedure("<",prim_proc_lower);
	add_procedure(">",prim_proc_greater);
    add_procedure("cons",prim_proc_cons);
	add_procedure("car",prim_proc_car);
	add_procedure("cdr",prim_proc_cdr);

	add_procedure("null?",prim_proc_is_null);
	add_procedure("procedure?",prim_proc_is_proc);
	add_procedure("pair?",prim_proc_is_pair);
//	add_procedure("vector?",prim_proc_is_vector);
	add_procedure("string?",prim_proc_is_string);
	add_procedure("boolean?",prim_proc_is_boolean);
	add_procedure("char?",prim_proc_is_boolean);
	add_procedure("symbol?",prim_proc_is_symbol);
	add_procedure("eq?",prim_proc_eq);

	add_procedure("apply",prim_proc_apply);
	add_procedure("eval",prim_proc_eval);
}


int main(){
	FILE* input;
	object* stmt;
	init();
	while(1){
		printf("\n>");
		stmt = read_exp(stdin);
		if(stmt == NULL) break;
		print_object(stdout,eval(stmt,global_env) );
	}
	return 0;
}

