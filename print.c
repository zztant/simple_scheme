
void print_obj_rational(FILE* output, object* obj){
	int val1 = obj->data.s_rational.value1;
	int val2 = obj->data.s_rational.value2;
	if( (val1>0)!=(val2>0) )
		fprintf(output,"-");
	if(val2 == 1)
		fprintf(output,"%d\n",abs(val1));
	else
		fprintf(output,"%d/%d\n",abs(val1),abs(val2));
}

void print_obj_prim_proc(FILE* output, object* obj){
	fprintf(output,"<primitive procedure>\n");
}

void print_obj_pair(FILE *output, object* pair){
	object* car_obj;
	object* cdr_obj;

	car_obj = car(pair);
	cdr_obj = cdr(pair);
	print_object(output, car_obj);
	if(cdr_obj->type == PAIR){
		fprintf(output, " ");
		print_obj_pair(output,cdr_obj);
	}
	else if(cdr_obj->type == null_list)
		return;
	else{
		fprintf(output, " . ");
		print_object(output, cdr_obj);
	}
}

void print_object(FILE *output, object* obj){
	switch(obj->type){
		case RATIONAL: print_obj_rational(output,obj); return;
		case PRIM_PROC: print_obj_prim_proc(output,obj); return;
		case PAIR: print_obj_pair(output,obj); return;
	}
}

