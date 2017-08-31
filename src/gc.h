#ifndef GC_H
#define GC_H

#define MEMSIZE 100000

#include"vm.h"

typedef struct garbage_collector{
	object** mem_car;
	object** mem_cdr;
	object* free;
	object* scan;
	object* broken_heart;
}collector;

collector* global_gc;
collector* init_gc();
void mark_broken_heart(object* obj, object* pos);
char is_marked(object* obj);
char is_code(object* root);
char is_full(collector* gc);
object* copy_mark(object* free, object* scan);
void collect(collector* gc, secd_vm* vm);
void do_collect(collector* gc, secd_vm* vm);
object* gc_malloc(collector* gc, secd_vm* vm);

#endif
