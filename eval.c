#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"

object* prim_proc_plus(object* arguments){
	int value1=0,value2=1;
	object* obj_car;
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_rational(obj_car)){
			fprintf(stderr,"Wrong type for + procedure!");
			exit(1);
		}
		value1 = value1 * (obj_car->data.s_rational.value2)
			    +value2 * (obj_car->data.s_rational.value1);
		value2 = value2 * (obj_car->data.s_rational.value2);
		arguments = cdr(arguments);
	}
	return make_rational(value1,value2);
}

object* prim_proc_minus(object* arguments){
	int value1=0,value2=1;
	int flag = 0;
	object* obj_car;
	if(is_null_list(arguments)){
		fprintf(stderr,"procedure need arguments");
		exit(1);
	}
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_rational(obj_car)){
			fprintf(stderr,"Wrong type for - procedure!");
			exit(1);
		}
		flag ++ ;
		if( flag == 1 ){
			value1 = obj_car->data.s_rational.value1;
			value2 = obj_car->data.s_rational.value2;
			arguments = cdr(arguments);
			continue;
		}
		value1 = value1 * (obj_car->data.s_rational.value2)
			    -value2 * (obj_car->data.s_rational.value1);
		value2 = value2 * (obj_car->data.s_rational.value2);
		arguments = cdr(arguments);
	}
	if(flag == 1)
		value1 = -value1;
	return make_rational(value1,value2);
}

object* prim_proc_mul(object* arguments){
	int value1=1,value2=1;
	object* obj_car;
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_rational(obj_car)){
			fprintf(stderr,"Wrong type for + procedure!");
			exit(1);
		}
		value1 = value1 * obj_car->data.s_rational.value1;
		value2 = value2 * obj_car->data.s_rational.value2;
		arguments = cdr(arguments);
	}
	return make_rational(value1,value2);
}

object* prim_proc_div(object* arguments){
	int value1=1,value2=1;
	object* obj_car;
	int flag = 0;
	if(is_null_list(arguments)){
		fprintf(stderr,"procedure need arguments");
		exit(1);
	}
	while(!is_null_list(arguments)){
		obj_car = car(arguments);
		if(!is_rational(obj_car)){
			fprintf(stderr,"Wrong type for + procedure!");
			exit(1);
		}
		flag++;
		if(flag == 1){
			value1 = obj_car->data.s_rational.value1;
			value2 = obj_car->data.s_rational.value2;
			arguments = cdr(arguments);
			continue;
		}
		value1 = value1 * obj_car->data.s_rational.value2;
		value2 = value2 * obj_car->data.s_rational.value1;
		arguments = cdr(arguments);
	}
	if(flag == 1){
		int tmp = value1;
		value1 = value2;
		value2 = tmp;
	}
	return make_rational(value1,value2);
}

object* prim_proc_and(object* argu){
	object* and = symbol_true;
	while(argu != null_list){
		and = car(argu);
		if( and == symbol_false)
			return symbol_false;
		argu = cdr(argu);
	}
	return and;
}

object* prim_proc_or(object* argu){
	object* or = symbol_false;
	while(argu != null_list){
		or = car(argu);
		if( or != symbol_false)
			return or;
		argu = cdr(argu);
	}
	return symbol_false;
}

object* prim_proc_equal(object* argu){
	if(is_null_list(argu)) 
		return symbol_true;
	object *obj1, *obj2;
    obj1 = car(argu);
	argu = cdr(argu);
	while(!is_null_list(argu)){
		obj2 = car(argu);
		if ( !is_number(obj1) || !is_number(obj2)){
			fprintf(stderr,"wrong arguments for '=' procedure");
			exit(1);
		}
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
		if ( !is_number(obj1) || !is_number(obj2)){
			fprintf(stderr,"wrong arguments for '=' procedure");
			exit(1);
		}
		if( !value_lower(obj1,obj2) )
			return symbol_false;
		obj1 = obj2;
		argu = cdr(argu);
	}
	return symbol_true;
}

object* prim_proc_greater(object* argu){
	if( is_false(prim_proc_equal(argu)) && is_false(prim_proc_lower(argu)))
		return symbol_true;
	else
		return symbol_false;
}

object* prim_proc_cons(object* arguments){
	return cons(car(arguments),cadr(arguments));
}

object* prim_proc_is_null(object* exp){
	return is_null_list(exp) ? symbol_true : symbol_false;
}

object* prim_proc_is_proc(object* exp){
	return (is_prim_proc(exp) || is_comp_proc(exp))?symbol_true:symbol_false;
}

object* prim_proc_is_pair(object* exp){
	return is_pair(exp)?symbol_true:symbol_false;
}

object* prim_proc_is_string(object* exp){
	return is_string(exp)?symbol_true:symbol_false;
}

object* prim_proc_is_boolean(object* exp){
	return is_boolean(exp)?symbol_true:symbol_false;
}

object* prim_proc_is_symbol(object* exp){
	return is_symbol(exp)?symbol_true:symbol_false;
}

object* prim_proc_eq(object* exp){
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
	return caar(exp);
}

object* prim_proc_cdr(object* exp){
	return cdar(exp);
}

object* prim_proc_apply(object* exp){
	object* proc = car(exp);
	object* argu = cadr(exp);
	return apply(proc,argu);
}

object* prim_proc_eval(){
	return null_list;
}

char is_definition(object* exp){
	return is_pair(exp) && car(exp) == symbol_define;
}

char is_quoted(object* exp){
	return is_pair(exp) && car(exp) == symbol_quote;
}

char is_set(object* exp){
	return is_pair(exp) && car(exp) == symbol_set;
}


char is_lambda(object* exp){
	return is_pair(exp) && car(exp) == symbol_lambda;
}



char is_if(object* exp){
	return is_pair(exp) && car(exp) == symbol_if;
}


object* apply(object* proc, object* arguments){
	return (*(proc->data.s_prim_proc.func))(arguments);
}




