#include <stdio.h>
#include <stdlib.h>
#include "gram_tree.h"
void insert(Nod *a,Nod *b){
	Nod *c = a->son;
	if(c==NULL){
		a->son = b;
	}
	else{
		while(c->next!=NULL){
			c = c->next;
		}
		c->next = b;
	}
}
extern const char *get_name(int order);
Nod *copy_nod(Nod *a){
	Nod *new_a = (Nod *)malloc(sizeof(Nod));
	new_a->if_lex = a->if_lex;
	new_a->if_null = a->if_null;
	new_a->id = a->id;
	new_a->first_line = a->first_line;
	new_a->str = a->str;
	new_a->son=new_a->next=new_a->self=NULL;


	a->self = new_a;
	return new_a;
}
Nod *trans_nod(Nod *a){
	if(a->if_lex){
		return copy_nod(a);
	}
	else{
		return a->self;
	}
}
void insert_1(Nod *a,Nod *b){
	b=trans_nod(b);
	a->first_line = b->first_line;
	a = copy_nod(a);
	a->son = b;
}
void insert_2(Nod *a,Nod *b1,Nod *b2){
	b1=trans_nod(b1);
	b2=trans_nod(b2);
	a->first_line = b1->first_line;
	a = copy_nod(a);
	a->son = b1;
	b1->next = b2;
}
void insert_3(Nod *a,Nod *b1,Nod *b2,Nod *b3){
	b1=trans_nod(b1);
	b2=trans_nod(b2);
	b3=trans_nod(b3);
	a->first_line = b1->first_line;
	a = copy_nod(a);
	a->son = b1;
	b1->next = b2;
	b2->next = b3;

}
void insert_4(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4){
	b1=trans_nod(b1);
	b2=trans_nod(b2);
	b3=trans_nod(b3);
	b4=trans_nod(b4);
	a->first_line = b1->first_line;
	a = copy_nod(a);
	a->son = b1;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
}
void insert_5(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5){
	b1=trans_nod(b1);
	b2=trans_nod(b2);
	b3=trans_nod(b3);
	b4=trans_nod(b4);
	b5=trans_nod(b5);
	a->first_line = b1->first_line;
	a = copy_nod(a);
	a->son = b1;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
	b4->next = b5;
}
void insert_7(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5,Nod *b6,Nod *b7){
	b1=trans_nod(b1);
	b2=trans_nod(b2);
	b3=trans_nod(b3);
	b4=trans_nod(b4);
	b5=trans_nod(b5);
	b6=trans_nod(b6);
	b7=trans_nod(b7);
	a->first_line = b1->first_line;
	a = copy_nod(a);
	a->son = b1;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
	b4->next = b5;
	b5->next = b6;
	b6->next = b7;
}

void print_tree(Nod *a,int dept){
	if(a->id==-1){
		for(int i=0;i<dept;i++)printf("  ");
		printf("error\n");
		if(a->next!=NULL)	print_tree(a->next,dept);
		return;
	}
	if(!a->if_null){
		for(int i=0;i<dept;i++)printf("  ");
		printf("%s",get_name(2+(1-a->if_lex)*30+a->id));
	}
	if(a->if_lex){
		if(a->str!=NULL)printf(": %s",a->str);
		printf("\n");
	}
	else{
		if(!a->if_null)
		printf(" (%d)\n",a->first_line);
	}

	if(a->son!=NULL){print_tree(a->son,dept+1);}
	if(a->next!=NULL)	print_tree(a->next,dept);
}
