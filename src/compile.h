#ifndef COMPILE_H
#define COMPILE_H

object* make_code(object_type type, object* argu, object* next);
object* compile_atom(object* atom, secd_vm* vm);
object* compile_symbol(object* symbol, secd_vm* vm);
object* compile_def(object* exp, secd_vm* vm);
object* compile_quoted(object* exp, secd_vm* vm);
object* compile_set(object* exp, secd_vm* vm);
object* compile_lambda(object* exp, secd_vm* vm);
object* compile_if(object* exp, secd_vm* vm);
object* compile_pair(object* exp, secd_vm* vm);
object* compile(object* exp, secd_vm* vm);
object* compile_proc(object* exp, secd_vm* vm);
object* compile_argu(object* exp, secd_vm* vm);
void compile_file(FILE* file, secd_vm* vm);
object* pre_macro_list(object* exp, secd_vm* vm);
object* pre_macro(object* exp, secd_vm* vm);

void print_code(object* code);
void print_code_sel(object* code);
void print_code_ldf(object* code);

#endif
