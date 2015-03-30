#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"


void print_obj_rational(FILE* output, object* obj){
	int val1 = obj->data.s_rational.value1;
	int val2 = obj->data.s_rational.value2;
	if( (val1>0)!=(val2>0) )
		fprintf(output,"-");
	if(val2 == 1)
		fprintf(output,"%d",abs(val1));
	else
		fprintf(output,"%d/%d",abs(val1),abs(val2));
}

void print_obj_prim_proc(FILE* output, object* obj){
	fprintf(output,"<primitive procedure>");
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
	else if(cdr_obj == null_list ){
		return;
	}
	else{
		fprintf(output, " . ");
		print_object(output, cdr_obj);
	}
}

void print_obj_symbol(FILE* output, object* obj){
	fprintf(output,"%s",obj->data.s_symbol.value);
}

void print_obj_comp_proc(FILE* output, object* obj){
	fprintf(output,"compound procedure");
}

void print_obj_boolean(FILE* output, object* obj){
	fprintf(output,"#%c",(obj->data.s_boolean.value == 0)?'f':'t');
}

void print_object(FILE *output, object* obj){
	switch(obj->type){
		case RATIONAL: print_obj_rational(output,obj); return;
		case PRIM_PROC: print_obj_prim_proc(output,obj); return;
		case PAIR:
				fprintf(output,"(");	
				print_obj_pair(output,obj); 
				fprintf(output,")");
				return;
		case SYMBOL: print_obj_symbol(output,obj); return;
		case COMP_PROC: print_obj_comp_proc(output,obj); return;
		case BOOLEAN: print_obj_boolean(output,obj); return;
	}
}

