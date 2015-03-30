#ifndef GC_H
#define GC_H

#define MEMSIZE 64

#include"vm.h"

typedef struct garbage_collector{
	object* mem_car;
	object* mem_cdr;
	object* free;
}collector;

collector* global_gc;
collector* init_gc();
char is_full(collector* gc);
void gc_mark(object* obj);
void gc_unmark(object* obj);
char is_marked(object* obj);
object* gc_copy(collector* gc, object* obj);
object* copy_and_mark(collector* gc, object* root);
void do_collect(collector* gc, secd_vm* vm);
object* gc_malloc(collector* gc, secd_vm* vm);
void collect(collector* gc, secd_vm* vm);

#endif
