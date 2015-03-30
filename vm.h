#ifndef VM_H
#define VM_H

typedef struct secd_vm{
	object* stack;
	object* env;
	object* code;
	object* dump;
	object* table;
}secd_vm;

secd_vm* global_vm;
secd_vm* init_vm();


#endif
