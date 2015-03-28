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

object* prim_proc_define(object* arguments,object* env){
	if(arguments == null_list){
		fprintf(stderr,"procedure need arguments");
		exit(1);
	}
	object* bind = find_bind(car(arguments),env);
	if(bind == NULL)
		add_bind(car(arguments),cadr(arguments),env);
	else
		set_cdr(bind,cadr(arguments));
	return symbol_ok;
}

object* prim_proc_set(object* arguments, object* env){
	if(arguments == null_list){
		fprintf(stderr,"procedure need arguments");
		exit(1);
	}
	object* bind = find_bind(car(arguments),env);
	if(bind == NULL){
		fprintf(stderr,"unbound variable");
		exit(1);
	}
	else
		set_cdr(bind,cadr(arguments));
	return cdr(bind);
}


object* set_car(object* pair, object* obj){
	pair->data.s_pair.car = obj;
	return symbol_ok;
}

object* set_cdr(object* pair, object* obj){
	pair->data.s_pair.cdr = obj;
	return symbol_ok;
}


object* add_bind(object* var, object* val, object* env){
	set_cdr(env, cons(cons(var,val),cdr(env)));
	return symbol_ok;
}

object* find_bind(object* var, object* env){
	if(is_null_list(env)) return NULL;
	object* first = cdr(env);
	while(!is_null_list(first)){
		if( caar(first) == var )
			return car(first);
		first = cdr(first);
	}
	return find_bind(var,car(env));
}

object* lookup_variable_value(object* var, object* env){
	object* bind = find_bind(var,env);
	if(bind == NULL){
		fprintf(stderr,"undefined symbol");
		exit(1);
	}
	return cdr(bind);
}

object* set(object* var, object* val,object* env){
	object* bind = find_bind(var,env);
	if(bind == NULL){
		fprintf(stderr,"unbound variable");
		exit(1);
	}
	else
		bind->data.s_pair.cdr = val;
	return symbol_ok;
}

char is_definition(object* exp){
	return is_pair(exp) && car(exp) == symbol_define;
}

object* eval_definition(object* exp, object* env){
	object* var = cadr(exp);
	object* arguments = cddr(exp);
	return prim_proc_define(cons(var,eval_arguments(arguments,env)),env);
}

char is_quoted(object* exp){
	return is_pair(exp) && car(exp) == symbol_quote;
}

object* eval_quoted(object* exp){
	return cadr(exp);
}


char is_set(object* exp){
	return is_pair(exp) && car(exp) == symbol_set;
}

object* eval_set(object* exp, object* env){
	object* var = cadr(exp);
	object* arguments = cddr(exp);
	return prim_proc_set(cons(var,eval_arguments(arguments,env)),env);
}

object* eval_arguments(object* arguments, object* env){
	if(is_null_list(arguments))
		return null_list;
	return cons(eval(car(arguments),env),eval_arguments(cdr(arguments),env));
}

object* eval(object* exp,object* env){
	object* proc;
	object* argu;
	object* value;

	if(is_atom(exp))
		return exp;
	else if(is_definition(exp))
		return eval_definition(exp,env);
	else if(is_quoted(exp))
		return eval_quoted(exp);
	else if(is_set(exp))
		return eval_set(exp,env);
	else if(is_symbol(exp))
		return lookup_variable_value(exp,env);
	else if(is_pair(exp)){
		proc = eval(operator(exp),env);
		argu = eval_arguments(operands(exp),env);
		if(is_prim_proc(proc))
			return (*(proc->data.s_prim_proc.func)) (argu) ;
		else if (is_comp_proc(proc)){

		}
	}
}




