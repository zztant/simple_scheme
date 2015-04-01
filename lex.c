#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"

void read_space(FILE* file){
	char c;
	c = getc(file);
	while( isspace(c) )
		c = getc(file);
	ungetc(c,file);
}

object* read_pair(FILE* file){
	char c;
	object* car_obj;
	object* cdr_obj;
	read_space(file);
	c = getc(file);
	if( c == '.' ){
		cdr_obj = read_exp(file);
		read_space(file);
		getc(file);
		return cdr_obj;
	}
	else if(  c == ')' )
		return make_null_list();
	else
		ungetc(c,file);
	car_obj = read_exp(file);
	cdr_obj = read_pair(file);
	return cons(car_obj, cdr_obj);
}

char isdigit(char c){
	return ( c>='0' && c<='9' );
}


object* read_digit(FILE* file){
	char c;
	c = getc(file);
	int value1=0,value2=0;

	while(isdigit(c)){
		value1 = value1*10 + c-'0';
		c = getc(file);
	}
	if(c=='/'){
		c = getc(file);
		if(!isdigit(c)){
			fprintf(stderr,"wrong number expression!\n>");
			fflush(file);
			return read_exp(file);
		}
		while(isdigit(c)){
			value2 = value2*10 + c-'0';
			c = getc(file);
		}
		ungetc(c,file);
	}
	else 
		ungetc(c,file);
	if(value2 == 0) value2 = 1;
	return make_rational(value1,value2);
}

object* read_string(FILE* file){
	char c;
	char tmp[100];
	int i = 0;
	while( (c=getc(file))!= '"')
		tmp[i++] = c;
	tmp[i] = '\0';
	return make_string(tmp);
}

object* read_symbol(FILE* file){
	char tmp[100];
	int i = 0;
	char c;
	while( !isspace(c=getc(file)) && c!=')' )
		tmp[i++] = c;
	tmp[i] = '\0';
	ungetc(c,file);
	return make_symbol(tmp);
}

object* read_exp(FILE* file){
	char c;
	object* car_obj;
	fflush(stdin);
	if( (c=getc(file))!=EOF ){
		ungetc(c,file);
		read_space(file);
		c = getc(file);
		if(c=='(')
			return read_pair(file);
		else if(c=='#'){
			if( (c=getc(file)) == 't')
				return symbol_true;
			else if ( c == 'f' )
				return symbol_false;
			else{
				fprintf(stderr,"wrong identifier for #\n>");
				fflush(file);
				return read_exp(file);
			}
		}
		else if(isdigit(c)){
			ungetc(c,file);
			return read_digit(file);
		}
		else if(c=='"')
			return read_string(file);
		else if(c=='\'')
			return cons(make_symbol("quote"), cons(read_exp(file),make_null_list()));
		else if(c==')'){
			fprintf(stderr,"wrong expression for )\n>");
			fflush(file);
			return read_exp(file);
		}
		else {
			ungetc(c,file);
			return read_symbol(file);
		}
	}
	return NULL;
}
