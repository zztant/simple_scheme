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
	gc->mem_car = (object**)malloc(MEMSIZE*sizeof(object*));
	gc->mem_cdr = (object**)malloc(MEMSIZE*sizeof(object*));
	gc->broken_heart = make_broken_heart();
	for(int i=0; i< MEMSIZE ;i++){
		gc->mem_car[i] = (object*)malloc(sizeof(object));
	}
	for(int i=0; i< MEMSIZE ;i++){
		gc->mem_cdr[i] = (object*)malloc(sizeof(object));
	}
	gc->free = gc->mem_car[0];
	return gc;
}

char is_full(collector* gc){
	return gc->free >= gc->mem_car[0] + MEMSIZE;
}


char is_code(object* root){
	return root->type >= NIL && root->type <= CALLCC ;
}

/*破碎的心，car做标记，cdr指向新的位置*/
void mark_broken_heart(object* obj, object* pos){

	obj->data.s_pair.car = global_gc->broken_heart;
	obj->data.s_pair.cdr = pos;
}

char is_marked(object* obj){
	return /*obj->type!=NULL_LIST &&*/
		   car(obj) == global_gc->broken_heart;
}

void copy_obj(object* obj1, object* obj2){
	obj1->type = obj2->type;
	obj1->data = obj2->data;
}


object* copy_mark(object* free, object* scan){
	object* obj1;
	object* obj2;
	object* obj3;
	while(free != scan ){
		while( !is_continuation(scan) && !is_pair(scan) && !is_code(scan) && !is_comp_proc(scan)) {
		//	print_object(stdout,scan);
		//	printf(",");
			scan++;
			if(free == scan)
				return free;
		}
		if( is_pair(scan) || is_code(scan) ){
			obj1 = car(scan);
			obj2 = cdr(scan);
			if( !is_marked(obj1) ){
				copy_obj(free,obj1);
				mark_broken_heart(obj1,free);
				scan->data.s_pair.car = free;
				free++;
			}
			else
				scan->data.s_pair.car = cdr(obj1);
			if( !is_marked(obj2) ){
				copy_obj(free,obj2);
				mark_broken_heart(obj2,free);
				scan->data.s_pair.cdr = free;
				free++;
			}	
			else
				scan->data.s_pair.cdr = cdr(obj2);
		}
		else if(is_comp_proc(scan)){
			obj1 = scan->data.s_comp_proc.parameters;
			obj2 = scan->data.s_comp_proc.body;
			obj3 = scan->data.s_comp_proc.env;
			if( !is_marked(obj1) ){
				copy_obj(free,obj1);
				mark_broken_heart(obj1,free);
				scan->data.s_comp_proc.parameters = free;
				free++;
			}
			else
				scan->data.s_comp_proc.parameters = cdr(obj1);
			if( !is_marked(obj2) ){
				copy_obj(free,obj2);
				mark_broken_heart(obj2,free);
				scan->data.s_comp_proc.body = free;
				free++;
			}	
			else
				scan->data.s_comp_proc.body = cdr(obj2);
			if( !is_marked(obj3) ){
				copy_obj(free,obj3);
				mark_broken_heart(obj3,free);
				scan->data.s_comp_proc.env = free;
				free++;
			}	
			else
				scan->data.s_comp_proc.env = cdr(obj3);
		}
		else{
			obj1 = scan->data.s_continuation.value;
			if( !is_marked(obj1)){
				copy_obj(free,obj3);
				mark_broken_heart(obj1,free);
				scan->data.s_continuation.value = free;
				free++;
			}
			else
				scan->data.s_continuation.value = cdr(obj1);
		}
		scan++;
	}
	return free;
}	

void do_collect(collector* gc, secd_vm* vm){
	printf("\nhave %d object\n",gc->free - gc->mem_car[0]);
	
	object* root =	cons(cons(cons(vm->env,vm->code),
							  cons(vm->dump,vm->stack)),
						 vm->table);
	
	gc->scan = gc->mem_cdr[0];
	gc->free = gc->mem_cdr[0];

	copy_obj(gc->free,root);
	mark_broken_heart(root,gc->free);
	root = gc->free;
	gc->free++;	
	
	gc->free = copy_mark(gc->free,gc->scan);
	
	vm->env = caaar(root);
	vm->code = cdaar(root);
	vm->stack = cddar(root);
	vm->dump = cadar(root);
	vm->table = cdr(root);

	object** tmp = gc->mem_car;
	gc->mem_car = gc->mem_cdr;
	gc->mem_cdr = tmp;
	printf("\nhave %d object\n",gc->free - gc->mem_car[0]);
}

void collect(collector* gc, secd_vm* vm){
	if( is_full(gc) ){
		fprintf(stderr,"memory is overflow!!!!!\n");
		exit(1);
	}
	if( gc->free >= gc->mem_car[0] + (MEMSIZE/4*3) ){
	printf("\nhave %d object\n",gc->free - gc->mem_car[0]);
		do_collect(gc,vm);
	printf("\nhave %d object\n",gc->free - gc->mem_car[0]);
	}
}

object* gc_malloc(collector* gc, secd_vm* vm){
	object* obj = gc->free++;
	return obj;
}



