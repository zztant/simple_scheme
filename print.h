#ifndef EVAL_H
#define EVAL_H

void print_obj_pair(FILE* output, object* obj);
void print_obj_prim_proc(FILE* output, object* obj);

void print_obj_rational(FILE* output, object* obj);
void print_object(FILE* output, object* obj);
void print_obj_symbol(FILE* output, object* obj);
void print_obj_comp_proc(FILE* output, object* obj);
void print_obj_boolean(FILE* output, object* obj);

#endif
