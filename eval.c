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

char is_lambda(object* exp){
	return is_pair(exp) && car(exp) == symbol_lambda;
}

object* eval_lambda(object* exp){
	object* parameters = cadr(exp);
	object* body = caddr(exp);
	return make_comp_proc(parameters,body);
}

void add_bind_comp_proc(object* proc,object* val, object* env){
	object* var = proc->data.s_comp_proc.parameters;
	while(val != null_list && var != null_list){
		add_bind(car(var), car(val), env);
		var = cdr(var);
		val = cdr(val);
	}
	if( val != null_list || var!= null_list){
		fprintf(stderr,"wrong for arguments number");
		exit(1);
	}
}


object* eval_comp_proc(object* proc, object* argu, object* env){
	env = make_env(env);
	add_bind_comp_proc(proc,argu,env);
	return eval(proc->data.s_comp_proc.body,env);
}

char is_if(object* exp){
	return is_pair(exp) && car(exp) == symbol_if;
}

object* eval_if(object* exp, object* env){
	object* condition = eval(cadr(exp),env);
	if( is_true(condition) )
		return eval( caddr(exp),env );
	else
		return eval( cadddr(exp),env );
}

object* eval_arguments(object* arguments, object* env){
	if(is_null_list(arguments))
		return null_list;
	return cons(eval(car(arguments),env),eval_arguments(cdr(arguments),env));
}

object* apply(object* proc, object* arguments){
	return (*(proc->data.s_prim_proc.func))(arguments);
}

object* eval(object* exp,object* env){
	object* proc;
	object* argu;
	object* value;

	if(is_atom(exp))
		return exp;
	else if(is_lambda(exp))
		return eval_lambda(exp);
	else if(is_definition(exp))
		return eval_definition(exp,env);
	else if(is_quoted(exp))
		return eval_quoted(exp);
	else if(is_set(exp))
		return eval_set(exp,env);
	else if(is_if(exp))
		return eval_if(exp,env);
	else if(is_symbol(exp))
		return lookup_variable_value(exp,env);
	else if(is_pair(exp)){
		proc = eval(operator(exp),env);
		argu = eval_arguments(operands(exp),env);
		if(is_prim_proc(proc)){
			if(proc->data.s_prim_proc.func == prim_proc_eval)
				return eval(car(argu),env);
			else
				return apply(proc,argu);
		}
		else if (is_comp_proc(proc))
			return eval_comp_proc(proc,argu,env);
	}
	return null_list;
}




