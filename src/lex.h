#ifndef LEX_H
#define LEX_H

void read_space(FILE* file);
object* read_pair(FILE* file);
char isdigit(char c);
object* read_digit(FILE* file);
object* read_string(FILE* file);
object* read_symbol(FILE* file);
object* read_exp(FILE* file);

#endif
