#ifndef GRAM_TREE
#define GRAM_TREE
typedef struct Node{
	int token_id,pattern_id;
	int first_line;
	void *prop;

	struct Node *parent,*son,*next;
}Nod;

Nod* root;
Nod* create_gnod(int,int);
void insert_1(Nod *a,Nod *b);
void insert_2(Nod *a,Nod *b1,Nod *b2);
void insert_3(Nod *a,Nod *b1,Nod *b2,Nod *b3);
void insert_4(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4);
void insert_5(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5);
void insert_7(Nod *a,Nod *b1,Nod *b2,Nod *b3,Nod *b4,Nod *b5,Nod *b6,Nod *b7);
void print_tree(Nod *a,int dept);
#endif
