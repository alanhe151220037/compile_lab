#ifndef _SYMTABLE_
#define _SYMTABLE_
#include <stdio.h>
#include "gram_tree.h"

#define prop_ID char
#define prop_INT int
#define prop_FLOAT float

typedef struct arry_{
	int nr;
	struct arry_ *next;
}arry;
typedef struct prop_var_{
	prop_ID *id;
	void *type;
	void *init;

	arry *arrylist; 
//	void *belongto;

	struct prop_var_ *next;
}prop_var;

typedef struct prop_type_{
	prop_ID *id;
	prop_var *varlist;
	struct prop_type_ *next;
	
}prop_type;

typedef struct prop_exp_{
	prop_type *type;
	arry *arrylist;
	int if_left;
	struct prop_exp_ *next;
	
}prop_exp;
typedef struct prop_fun_{
	prop_ID *id;
	prop_type *type;
	prop_var *paramlist;
	struct prop_fun_ *next;	
}prop_fun;


//struct prop_exp
typedef struct field_tree_{
	int if_struct;
	prop_var *sym_t;
	prop_type *type_t;
	struct field_tree_ *parent;
	struct field_tree_ *next;
	struct field_tree_ *child;
}field_tree;
field_tree root_field;

void analy_tree(Nod*);
void print_symt(field_tree *,int);
void print_funlist();
#endif
