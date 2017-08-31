#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))
#define cons(obj1,obj2) make_pair(obj1,obj2)
#define operator(obj) car(obj)
#define operands(obj) cdr(obj)

typedef enum{
	BOOLEAN,PAIR,SYMBOL,RATIONAL,REAL,CHAR,STRING,VECTOR,PORT,
	PRIM_PROC,COMP_PROC,NULL_LIST
}object_type;

typedef struct object{
	object_type type;
	union{
		struct{
			char value;
		}s_boolean;
		struct{
			struct object* car;
			struct object* cdr;
		}s_pair;
		struct{
			char* value;
		}s_symbol;
		struct{
			int value1;
			int value2;
		}s_rational;
		struct{
			double value;
		}s_real;
		struct{
			char value;
		}s_char;
		struct{
			char* value;
		}s_string;
		struct{
			FILE* value;
		}s_port;
		struct{
			struct object* parameters;
			struct object* body;
		}s_comp_proc;
		struct{
			struct object* (*func)(struct object* arguments);
		}s_prim_proc;
	}data;
}object;

object* symbol_table;
object* null_list;
object* global_env;

object* symbol_quote;
object* symbol_lambda;
object* symbol_define;
object* symbol_true;
object* symbol_false;
object* symbol_if;
object* symbol_and;
object* symbol_or;
object* symbol_ok;
object* symbol_set;
object* symbol_begin;


//lex.c
void read_space(FILE* file);
object* read_pair(FILE* file);
char isdigit(char c);
object* read_digit(FILE* file);
object* read_string(FILE* file);
object* read_symbol(FILE* file);
object* read_exp(FILE* file);

//eval.c
object* prim_proc_plus(object* arguments);
object* prim_proc_minus(object* arguments);
object* prim_proc_mul(object* arguments);
object* prim_proc_div(object* arguments);
object* prim_proc_define(object* arguments,object* env);
object* prim_proc_set(object* arguments, object* env);
object* prim_proc_and(object* argu);
object* prim_proc_or(object* argu);
object* prim_proc_equal(object* argu);
object* prim_proc_lower(object* argu);
object* prim_proc_greater(object* argu);
object* prim_proc_cons(object* arguments);
object* prim_proc_is_null(object* exp);
object* prim_proc_is_proc(object* exp);
object* prim_proc_is_pair(object* exp);
object* prim_proc_is_string(object* exp);
object* prim_proc_is_boolean(object* exp);
object* prim_proc_is_symbol(object* exp);
object* prim_proc_eq(object* exp);
object* prim_proc_car(object* exp);
object* prim_proc_cdr(object* exp);
object* prim_proc_apply(object* exp);
object* prim_proc_eval();
object* set(object* var, object* val,object* env);
object* set_cdr(object* pair, object* obj);
object* set_car(object* pair, object* obj);
object* add_bind(object* var, object* val, object* env);
object* find_bind(object* var, object* env);
object* lookup_variable_value(object* var, object* env); 
char is_definition(object* exp);
object* eval_definition(object* exp, object* env);
char is_quoted(object* exp);
object* eval_quoted(object* exp);
char is_set(object* exp);
object* eval_set(object* exp, object* env);
char is_lambda(object* exp);
object* eval_lambda(object* exp);
void add_bind_comp_proc(object* proc,object* val, object* env);
object* eval_comp_proc(object* proc, object* argu, object* env);
char is_if(object* exp);
object* eval_if(object* exp, object* env);
object* eval(object* exp, object* env);
object* apply(object* proc, object* arguments);
object* eval_arguments(object* exp, object* env);



//objct.c
object* alloc_object();
object* make_boolean(char value);
object* make_pair(object* car, object* cdr);
object* car(object* obj);
object* cdr(object* obj);
object* find_symbol(char *value);
object* make_symbol(char *value);
int gcd(int val1, int val2);
char value_equal(object* val1, object* val2);
char value_lower(object* val1, object* val2);
object* make_rational(int value1, int value2);
object* make_real(double value);
object* make_char(char value);
object* make_string(char* value);
object* make_port(char* filename);
object* make_comp_proc(object* parameters, object* body);
object* make_null_list();
char is_null_list(object* obj);
char is_boolean(object* obj);
char is_symbol(object* obj);
char is_rational(object* obj);
char is_real(object* obj);
char is_char(object* obj);
char is_string(object* obj);
char is_vector(object* obj);
char is_port(object* obj);
char is_prim_proc(object* obj);
char is_comp_proc(object* obj);
char is_pair(object* obj);
char is_atom(object* obj);
char is_true(object* obj);
char is_false(object* obj);
char is_number(object* obj);
object* make_env(object* env);
//print
void print_obj_pair(FILE* output, object* obj);
void print_obj_prim_proc(FILE* output, object* obj);

void print_obj_rational(FILE* output, object* obj);
void print_object(FILE* output, object* obj);
void print_obj_symbol(FILE* output, object* obj);
void print_obj_comp_proc(FILE* output, object* obj);
void print_obj_boolean(FILE* output, object* obj);


#include"eval.c"
#include"object.c"
#include"lex.c"
#include"print.c"
