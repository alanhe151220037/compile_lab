#ifndef GRAM_TREE
#define GRAM_TREE
typedef struct Node{
	int if_lex;
	int if_null;
	struct Node *self;

	int id;
	int first_line;
	char *str;

	struct Node *son,*next;
}Nod;

Nod *root;
void insert(Nod *a,Nod *b);
Nod *copy_nod(Nod *a);
Nod *trans_nod(Nod *a);
void insert_1(Nod *a,Nod *b);
void insert_2(Nod *a,Nod *b1,Nod *b2);
void insert_3(Nod *a,Nod *b1,Nod *b2,Nod *b3);
void insert_4(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4);
void insert_5(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5);
void insert_7(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5,Nod *b6,Nod *b7);
void print_tree(Nod *a,int dept);
#endif
