#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"

collector* init_gc(){
	collector* gc = (collector*)malloc(sizeof(collector));
	gc->mem_car = (object*)malloc(MEMSIZE*sizeof(object));
	gc->mem_cdr = (object*)malloc(MEMSIZE*sizeof(object));
	gc->free = gc->mem_car;
	return gc;
}

void print_mem(){
	object* p1 = global_gc->mem_car;
	object* p2 = global_gc->mem_cdr;
	printf("\n");
	for(;p1!=global_gc->mem_car + MEMSIZE;p1++)
		printf("#");
}

char is_full(collector* gc){
	return gc->free >= gc->mem_car + MEMSIZE;
}

void gc_mark(object* obj){
	obj->marked = 1;
}

void gc_unmark(object* obj){
	obj->marked = 0;
}

char is_marked(object* obj){
	return obj->marked;
}

object* gc_copy(collector* gc, object* obj){
	*(gc->free) = (*obj);
	if( is_symbol(obj) )
		fprintf(stdout,"%s,",obj->data.s_symbol.value);
	else if (is_rational(obj))
		fprintf(stdout,"%d,",obj->data.s_rational.value1);
	return gc->free++;
}

object* copy_and_mark(collector* gc, object* root){
	if ( is_marked(root) || is_null_list(root) ) return root;
	else if ( is_pair(root) ){
		gc_mark(root);
		root = gc_copy(gc,root);
		root->data.s_pair.car = copy_and_mark(gc,car(root));
		root->data.s_pair.cdr = copy_and_mark(gc,cdr(root));
	}
	else{
		gc_mark(root);
		root = gc_copy(gc,root);
	}
	return root;
}


void do_collect(collector* gc, secd_vm* vm){
	gc->free = gc->mem_cdr;
	vm->stack = copy_and_mark(gc,vm->stack);
	vm->env = copy_and_mark(gc,vm->env);
	vm->code = copy_and_mark(gc,vm->code);
	vm->dump = copy_and_mark(gc,vm->dump);
	vm->table = copy_and_mark(gc,vm->table);
	object* tmp = gc->mem_car;
	gc->mem_car = gc->mem_cdr;
	gc->mem_cdr = tmp;
	
	object* obj = gc->mem_car;
	while( obj != gc->free ){
		gc_unmark(obj);
		obj = obj + 1;
	}
}

void collect(collector* gc, secd_vm* vm){
	if( is_full(gc) ){
		fprintf(stderr,"memory is overflow!!!!!\n");
		exit(1);
	}
	if( gc->free >= gc->mem_car + (MEMSIZE/4*3) )
		do_collect(gc,vm);
}

object* gc_malloc(collector* gc, secd_vm* vm){
	object* obj = gc->free++;
	printf("\ncan have %d object\n",MEMSIZE+((int)gc->mem_car-(int)gc->free)/sizeof(object));
	obj->marked = 0;
	return obj;
}



