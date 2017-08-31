#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"eval.h"
#include"lex.h"
#include"print.h"
#include"gc.h"
#include"vm.h"
#include"compile.h"
#include"macro.h"

char is_variable(object* obj, object* literals){
	if(is_null_list(literals))
		return 1;
	else{
		object* obj2 = car(literals);
		/* 这里要小心，因为垃圾回收和符号表可以保证地址相同 */
		if( obj == obj2 )
			return 0;
		return is_variable(obj,cdr(literals));
	}
}



/*match_list 组织为 ( ( . ) ( . ) ( . ) ...) 的形式*/
char match_pattern(object* exp, object* pattern, 
		object* literals, object** match_list){
/*	print_object(stdout,exp);
	printf("\n");
	print_object(stdout,pattern);
	printf("\n");
*/
	if(is_pair(pattern)){
		/* 如果有... */
		if(!is_null_list(cdr(pattern))&&symbol_compare(cadr(pattern),"...")){
			(*match_list) = cons(cons(exp,pattern),(*match_list));
			return 1;
		}
		return match_pattern(car(exp),car(pattern),literals,match_list) &&
			   match_pattern(cdr(exp),cdr(pattern),literals,match_list);
	}
	else{
		switch(pattern->type){
			case BOOLEAN: return exp->data.s_boolean.value 
						  == pattern->data.s_boolean.value;
			case RATIONAL: return value_equal(exp,pattern);
			case STRING: return strcmp(exp->data.s_string.value,
							       pattern->data.s_string.value)==0;
			case NULL_LIST: return is_null_list(exp);
			case SYMBOL: 
				if(symbol_compare(pattern,"_"))
					return 1;
				if(is_variable(pattern,literals)){
					(*match_list) = cons(cons(exp,pattern),(*match_list));
					return 1;
				}
				else if(pattern == exp)
					return 1;
				else
					return 0;
			default: return 0;
		}
	}
}

char is_same_object(object* pair1, object* pair2){
	if(pair1->type!=pair2->type)
		return 0;
	if(is_pair(pair1))
		return is_same_object(car(pair1),car(pair2)) &&
			   is_same_object(cdr(pair1),cdr(pair2));
	switch(pair1->type){
		case SYMBOL: return pair1==pair2;
		case RATIONAL: return value_equal(pair1,pair2);
		default: return 1;
	}
}

char is_in_pair(object* template, object* pair){
	if(is_same_object(template,pair)) return 1;
	if(is_pair(pair))
		return is_in_pair(template,car(pair))||is_in_pair(template,cdr(pair));
	else
		return 0;
}

object* map_car(object* list){
	if(is_null_list(list))
		return list;
	else
		return cons(caar(list),map_car(cdr(list)));
}

object* map_cdr(object* list){
	if(is_null_list(list))
		return list;
	else
		return cons(cdar(list),map_cdr(cdr(list)));
}


void together_trans(object* unit, object* variable, 
		object* template, object** trans_ans){
	/*		printf("variable:");
			print_object(stdout,variable);
			printf("\n");
			printf("template:");
			print_object(stdout,template);
			printf("\n");*/
	if(!is_pair(template)) return;
	if(is_same_object(unit,car(template))){
		(*trans_ans) = variable;
		return;
	}
	else if(is_pair(car(template))){
		together_trans(unit,map_car(variable),map_car(template),trans_ans);
		together_trans(unit,map_cdr(variable),map_cdr(template),trans_ans);
	}
}
/* template 在cdr的位置处 */
object* find_and_trans(object* unit, object* match_list){
	if(is_null_list(match_list))
		return make_null_list();
	/*		printf("unit:::");
			print_object(stdout,unit);
			printf("cdarmatch_list:::");
			print_object(stdout,cdar(match_list));
			printf("\n");*/
	if(is_in_pair(unit,cdar(match_list))){
		object* variable = caar(match_list);
		object* template = cons(cadar(match_list),make_null_list());
/*			printf("variable:::");
			print_object(stdout,variable);
			printf("\n");
			printf("template:::");
			print_object(stdout,template);
			printf("\n");*/
		object* trans_ans = NULL;
		together_trans(unit,variable,template,&trans_ans);
/*			printf("trans_ans:::");
			print_object(stdout,trans_ans);
			printf("\n");*/
		if(trans_ans!=NULL)
			return trans_ans;
		else{
			fprintf(stderr,"ERROR:: syntax error during compile\n");
			exit(1);
		}
	}
	return find_and_trans(unit,cdr(match_list));
}

object* find_pattern(object* unit, object* match_list){
	if(is_null_list(match_list))
		return match_list;
	else if(unit == cdar(match_list))
		return caar(match_list);
	else
		return find_pattern(unit,cdr(match_list));
}

object* do_trans(object* match_list, object* template, object* literals){
	object* exp = template;
	if(is_pair(template)){
		/* 如果有... */
		if(!is_null_list(cdr(template))&&symbol_compare(cadr(template),"...")){
			/* 在匹配链中找到对应匹配，进行转换 */
			return find_and_trans(car(template),match_list);
		}
		exp = cons( do_trans(match_list,car(template),literals),
				    do_trans(match_list,cdr(template),literals));
		return exp;
	}
	if(is_symbol(template)&&is_variable(template,literals)){
		exp = find_pattern(template,match_list);
		if(is_null_list(exp))
			return template;
		else
			return exp;
	}
	return exp;
}


object* transform(object* exp, object* macro){
	object* macro_name = car(exp);
	object* literals = car(macro->data.s_macro.value);
	object* trans_list = cdr(macro->data.s_macro.value);
	while(!is_null_list(trans_list)){
		object* pattern = caar(trans_list);
		object* template = cadar(trans_list);
		object* match_list = make_null_list();
		if(match_pattern(exp,pattern,literals,&match_list))
			return do_trans(match_list,template,literals);
		trans_list = cdr(trans_list);
	}
	fprintf(stderr,"ERROR:: syntax error during compile\n");
	exit(1);
}
