#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"


secd_vm* init_vm(){
	secd_vm* vm = (secd_vm*)malloc(sizeof(secd_vm));
	vm->stack = null_list;
	vm->code = null_list;
	vm->dump = null_list;
	vm->table = null_list;
	vm->env = make_env(null_list);
	return vm;
}



