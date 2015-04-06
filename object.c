#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"


object* alloc_object(){
	object* obj =(object*)gc_malloc(global_gc,global_vm);
	return obj;
}

object* make_continuation(object* value){
	object* obj = alloc_object();
	obj->type = CONTINUATION;
	obj->data.s_continuation.value = value;
	return obj;
}

object* make_broken_heart(){
	object* obj = (object*)malloc(sizeof(object));
	obj->type = BROKEN_HEART;
	return obj;
}

object* make_boolean(char value){
	object* obj = alloc_object();
	obj->type = BOOLEAN;
	obj->data.s_boolean.value = value;
	return obj;
}

object* make_pair(object* car, object* cdr){
	object* obj = alloc_object();
	obj->type = PAIR;
	obj->data.s_pair.car = car;
	obj->data.s_pair.cdr = cdr;
	return obj;
}

object* car(object* obj){
	return obj->data.s_pair.car;
}

object* cdr(object* obj){
	return obj->data.s_pair.cdr;
}

object* find_symbol(char *value, object* table){
	if(is_null_list(table))
		return make_null_list();
	if(strcmp(value,car(table)->data.s_symbol.value)==0)
		return car(table);
	return find_symbol(value,cdr(table));
}

object* make_symbol(char *value){
	object* obj = find_symbol(value,global_vm->table);
	if(!is_null_list(obj))
		return obj;
	obj = alloc_object();
	obj->type = SYMBOL;
	obj->data.s_symbol.value =(char*)malloc(sizeof(strlen(value)+1));
	obj->data.s_symbol.value = strcpy(obj->data.s_symbol.value,value);
	
	global_vm->table = cons(obj,global_vm->table);

	return obj;
}

int gcd(int val1, int val2){
	int rem;
	val1 = abs(val1);
	val2 = abs(val2);
	while(val2>0){
		rem = val1 % val2;
		val1 = val2;
		val2 = rem;
	}
	return val1;
}

object* make_rational(int value1, int value2){
	object* obj = alloc_object();
	obj->type = RATIONAL;
	obj->data.s_rational.value1 = value1 / gcd(value1,value2);
	obj->data.s_rational.value2 = value2 / gcd(value1,value2);
	return obj;
}

//need change
char value_equal(object* val1, object* val2){
	return val1->data.s_rational.value1 == val2->data.s_rational.value1
		&& val1->data.s_rational.value2 == val2->data.s_rational.value2;
}

char value_lower(object* val1, object* val2){
	int value1 = val1->data.s_rational.value1 *
		         val2->data.s_rational.value2
				 - val1->data.s_rational.value2 *
				   val2->data.s_rational.value1;
	int value2 = val1->data.s_rational.value2 *
				 val2->data.s_rational.value2;
	return (value1>0&&value2<0) || (value1<0&&value2>0);
}

char value_greater(object* val1, object* val2){
	int value1 = val1->data.s_rational.value1 *
		         val2->data.s_rational.value2
				 - val1->data.s_rational.value2 *
				   val2->data.s_rational.value1;
	int value2 = val1->data.s_rational.value2 *
				 val2->data.s_rational.value2;
	return (value1>0&&value2>0) || (value1<0&&value2<0);
}

object* make_real(double value){
	object* obj = alloc_object();
	obj->type = REAL;
	obj->data.s_real.value = value;
	return obj;
}

object* make_char(char value){
	object* obj = alloc_object();
	obj->type = CHAR;
	obj->data.s_char.value = value;
	return obj;
}

object* make_string(char* value){
	object* obj = alloc_object();
	obj->type = STRING;
	obj->data.s_string.value =(char*)malloc(strlen(value)+1);
	strcpy(obj->data.s_string.value,value);
	return obj;
}

object* make_port(char* filename){
	object* obj = alloc_object();
	obj->type = PORT;
	obj->data.s_port.value = fopen(filename,"rw");
	return obj;
}

object* make_comp_proc(object* parameters,
		object* body, object* env){
	object* obj = alloc_object();
	obj->type = COMP_PROC;
	obj->data.s_comp_proc.parameters =parameters;
	obj->data.s_comp_proc.body = body;
	obj->data.s_comp_proc.env = env;
	return obj;
}

object* make_prim_proc(object* (*func)(object* arguments)){
	object* obj = alloc_object();
	obj->type = PRIM_PROC;
	obj->data.s_prim_proc.func = func;
	return obj;
}

object* make_null_list(){
	object* obj = alloc_object();
	obj->type = NULL_LIST;
	obj->data.s_pair.car = 0;
	obj->data.s_pair.cdr = 0;
	return obj;
}

object* make_macro(object* value){
	object* obj = alloc_object();
	obj->type = MACRO;
	obj->data.s_macro.value = value;
	return obj;
}

char is_macro(object* obj){
	return obj->type == MACRO;
}

char is_continuation(object* obj){
	return obj->type == CONTINUATION;
}

char is_null_list(object* obj){
	return obj->type == NULL_LIST;
}

char is_boolean(object* obj){
	return obj->type == BOOLEAN;
}

char is_symbol(object* obj){
	return obj->type == SYMBOL;
}

char is_rational(object* obj){
	return obj->type == RATIONAL;
}

char is_real(object* obj){
	return obj->type == REAL;
}

char is_char(object* obj){
	return obj->type == CHAR;
}

char is_string(object* obj){
	return obj->type == STRING;
}

char is_vector(object* obj){
	return obj->type == VECTOR;
}

char is_port(object* obj){
	return obj->type == PORT;
}

char is_prim_proc(object* obj){
	return obj->type == PRIM_PROC;
}

char is_comp_proc(object* obj){
	return obj->type == COMP_PROC;
}

char is_pair(object* obj){
	return obj->type == PAIR;
}

char is_atom(object* obj){
	if(obj->type == PRIM_PROC ||
		   	obj->type == COMP_PROC ||
		   	obj->type == PAIR ||
			obj->type == SYMBOL)
		return 0;
	else
		return 1;
}


char is_number(object* obj){
	return is_rational(obj) || is_real(obj);
}


