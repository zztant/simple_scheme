

object* alloc_object(){
	object* obj =(object*)malloc(sizeof(object));
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

object* find_symbol(char* value){
	object* obj = symbol_table;
	while(!is_null_list(obj)){
		if( strcmp(value,car(obj)->data.s_symbol.value) == 0)
			return car(obj);
		obj = cdr(obj);
	}
	return null_list;
}

object* make_symbol(char *value){
	object* obj = find_symbol(value);
	if(!is_null_list(obj))
		return obj;
	obj = alloc_object();
	obj->type = SYMBOL;
	obj->data.s_symbol.value =(char*)malloc(sizeof(strlen(value)+1));
	strcpy(obj->data.s_symbol.value,value);
	
	symbol_table = cons(obj,symbol_table);

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
	return obj;
}


char is_null_list(object* obj){
	return obj==null_list;
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

/* 环境定义以及符号表 */

object* make_env(object* env){
	return cons(env,null_list);
}
