#include <stdio.h>
#include <stdlib.h>
#include "gram_tree.h"
extern const char *get_name(int order);
Nod *create_gnod(int token,int pattern){
	Nod *new=(Nod *)malloc(sizeof(Nod));	
	if(token>0)token+=30;
	new->token_id=token;
	new->pattern_id=pattern;
	new->prop=NULL;
	new->parent=new->son=new->next=NULL;
	return new;
}
void insert_1(Nod *a,Nod *b){
	a->first_line = b->first_line;
	a->son = b;
	b->parent=a;
}
void insert_2(Nod *a,Nod *b1,Nod *b2){
	a->first_line = b1->first_line;
	a->son = b1;
	b1->next = b2;
	b1->parent=a;
	b2->parent=a;
}
void insert_3(Nod *a,Nod *b1,Nod *b2,Nod *b3){
	a->first_line = b1->first_line;
	a->son = b1;
	b1->next = b2;
	b2->next = b3;

	b1->parent=a;
	b2->parent=a;
	b3->parent=a;
}
void insert_4(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4){
	a->first_line = b1->first_line;
	a->son = b1;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
	b1->parent=a;
	b2->parent=a;
	b3->parent=a;
	b4->parent=a;
}
void insert_5(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5){
	a->first_line = b1->first_line;
	a->son = b1;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
	b4->next = b5;
	b1->parent=a;
	b2->parent=a;
	b3->parent=a;
	b4->parent=a;
	b5->parent=a;
}
void insert_7(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5,Nod *b6,Nod *b7){
	a->first_line = b1->first_line;
	a->son = b1;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
	b4->next = b5;
	b5->next = b6;
	b6->next = b7;
	b1->parent=a;
	b2->parent=a;
	b3->parent=a;
	b4->parent=a;
	b5->parent=a;
	b6->parent=a;
	b7->parent=a;
}

void print_tree(Nod *a,int dept){
	if(a->token_id!=0){
		for(int i=0;i<dept;i++)printf("  ");
		printf("%d %s",a->token_id,get_name(2+a->token_id));
	
	
		if(a->token_id>30){
			printf(" (%d)",a->first_line);
		}	
		else if(a->token_id!=-1){
			if(a->prop!=NULL)printf(": %s",(char *)a->prop);
		}
		printf("\n");
	}
	if(a->son!=NULL){print_tree(a->son,dept+1);}
	if(a->next!=NULL)	print_tree(a->next,dept);
}






