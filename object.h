#ifndef OBJECT_H
#define OBJECT_H


typedef enum{
	/* scheme_type */
	BOOLEAN,PAIR,SYMBOL,RATIONAL,REAL,CHAR,STRING,VECTOR,PORT,
	PRIM_PROC,COMP_PROC,NULL_LIST,
	/* code_type */
	NIL,LDC,LD,SEL,JOIN,LDF,AP,RET,DUM,RAP,
	DEFINE,SET,LIST,
	/* gc */
	BROKEN_HEART
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
			struct object* env;
		}s_comp_proc;
		struct{
			struct object* (*func)(struct object* arguments);
		}s_prim_proc;
	}data;
}object;

object* alloc_object();
object* make_boolean(char value);
object* make_pair(object* car, object* cdr);
object* car(object* obj);
object* cdr(object* obj);
object* find_symbol(char *value, object* table);
object* make_symbol(char *value);
int gcd(int val1, int val2);
char value_equal(object* val1, object* val2);
char value_lower(object* val1, object* val2);
object* make_rational(int value1, int value2);
object* make_real(double value);
object* make_char(char value);
object* make_string(char* value);
object* make_port(char* filename);
object* make_comp_proc(object* parameters, object* body, object* env);
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
#define true symbol_true
#define false symbol_false

#endif

