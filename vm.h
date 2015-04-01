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

void error_handling();
object* lookup_variable_value(object* var, object* env);
object* lookup_bind(object* var, object* env);
void add_bind(object* var, object* val, secd_vm* vm);
void add_bind_ap(object* argu, object* param, secd_vm* vm);
void set_bind(object* var, object* val, object* env);
void eval_ldc(object* code, secd_vm* vm);
void eval_ld(object* code, secd_vm* vm);
void eval_sel(object* code, secd_vm* vm);
void eval_join(object* code, secd_vm* vm);
void eval_ldf(object* code, secd_vm* vm);
void eval_ap(object* code, secd_vm* vm);
void eval_ap_prim(object* code, secd_vm* vm);
void eval_ap_comp(object* code, secd_vm* vm);
void eval_ap(object* code, secd_vm* vm);
void eval_ret(object* code, secd_vm* vm);
void eval_dum(object* code, secd_vm* vm);
void eval_rap(object* code, secd_vm* vm);
void eval_def(object* code, secd_vm* vm);
void eval_set(object* code, secd_vm* vm);
void eval_vm(secd_vm* vm);

void print_ans(FILE* file, object* stack);

#endif
