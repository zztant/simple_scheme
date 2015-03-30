#ifndef COMPILE_H
#define COMPILE_H

#define next_code(code) (code->data.code.next)

object* make_code(code_type type, object* argu, object* next);
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


void print_code();


#endif
