#ifndef _IR_H_
#define _IR_H_

#include "sym_tab.h"
#include <stdio.h>
typedef struct operand_{
	int kind;
	char *str;
	int val;
	struct operand_ *next;
}operand;


typedef struct sentence_{
	int kind;
	operand *arg0;
	operand *arg1;
	operand *arg2;
	struct sentence_ *next;
}sentence;

void print_sentence(sentence *);
sentence *translate(Nod *,field_tree *i);
sentence *translate_args(Nod *,field_tree *,operand *);
sentence *translate_exp(Nod *,field_tree *,operand *);
sentence *translate_cond(Nod *,operand *,operand *,field_tree *);

FILE *fp;
#endif
