#include "sym_tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
prop_fun *funlist=NULL;
field_tree *current_field=NULL;


int idcmp(prop_ID *id1,prop_ID *id2){
	if(id1==NULL||id2==NULL)return 1;
	return strcmp(id1,id2);
}

prop_var *create_var(prop_ID *id_){
	prop_var *new = (prop_var *)malloc(sizeof(prop_var));
	new->id=id_;
	new->type=NULL;
	new->init=NULL;
	new->arrylist=NULL;
	//new->belongto=NULL;
	new->next=NULL;
	return new;
}

	prop_var *copy_varlist(prop_var *var_){
		if(var_==NULL)return NULL;
		prop_var *new = (prop_var *)malloc(sizeof(prop_var));
		new->id=var_->id;
		new->type=var_->type;
		new->init=var_->init;
		new->arrylist=var_->arrylist;
	//	new->belongto=var_->belongto;
		new->next=copy_varlist(var_->next);
		return new;
	}

prop_fun *create_fun(prop_ID *id_,prop_var *var_){
	prop_fun *new = (prop_fun *)malloc(sizeof(prop_fun));
	new->id=id_;
	new->type=NULL;
	prop_var *var__=copy_varlist(var_);
	new->paramlist=var__;
	new->next=NULL;
	return new;
}


void add_var2var(prop_var *var1,prop_var *var2){
	prop_var *p=var1;
	while(p->next!=NULL)p=p->next;
	p->next=var2;
}


void add_exp2exp(prop_exp *exp1,prop_exp *exp2){
	prop_exp *p=exp1;
	while(p->next!=NULL)p=p->next;
	p->next=exp2;
}

void add_arry1var(prop_var *var,prop_INT *n){
	arry *new=(arry*)malloc(sizeof(arry));
	new->nr=*n;
	new->next=NULL;
	if(var->arrylist==NULL)var->arrylist=new;
	else{
		arry *p=var->arrylist;
		while(p->next!=NULL)p=p->next;
		p->next=new;
	}
}

void set_type1var(prop_var *var_,prop_type *type_){
	prop_var *p= var_;
	while(p->next!=NULL){
		p->type=(void *)type_;
		p=p->next;
	}
	p->type=(void *)type_;
}
void set_type1fun(prop_fun *fun_,prop_type *type_){
	fun_->type=type_;
}


int add_sym(prop_var *var_){
	int label=1;
	prop_var *p=current_field->sym_t;
	if(p==NULL){
		current_field->sym_t=var_;	
	}
	else{
		while(p->next!=NULL){
			prop_var *q=var_;
			while(q!=NULL){
				if(idcmp(p->id,q->id)==0)label=0;
				q=q->next;
			}
			p=p->next;	
		}
		prop_var *q=var_;
		while(q!=NULL){
			if(idcmp(p->id,q->id)==0)label=0;
			q=q->next;
		}
		p->next=var_;
	}
	return label;
}

int add_fun(prop_fun *fun_){
	int label=1;
	if(funlist==NULL){
		funlist=fun_;
	}
	else{
		prop_fun *p=funlist;
		while(p->next!=NULL){
			if(idcmp(p->id,fun_->id)==0)label=0;
			p=p->next;
		}
		if(idcmp(p->id,fun_->id)==0)label=0;
		p->next=fun_;
	}
	return label;
}

prop_type *add_type(prop_ID *id_){
	prop_type *new=(prop_type *)malloc(sizeof(prop_type));
	new->id=id_;
	new->varlist=current_field->sym_t;
	new->next=NULL;
	field_tree *p=current_field;
	while(p->if_struct==1){
		p=p->parent;
	}
	prop_type *q=(prop_type *)p->type_t;
	if(q==NULL)p->type_t=new;
	else{
		while(q->next!=NULL){
			if(idcmp(q->id,id_)==0)return NULL;
			q=q->next;
		}
		if(idcmp(q->id,id_)==0)return NULL;
		q->next=new;
	}
	
	return new;
}

prop_type *get_type(prop_ID *id_){
	field_tree *p=current_field;
	prop_type *q=NULL;
	int if_find=0;
	while(p!=NULL){
		q=p->type_t;
		while(q!=NULL){
			if(idcmp(id_,q->id)==0)break;
			q=q->next;	
		}
		if(q!=NULL)break;
		p=p->parent;
	}
	return q;
}

int produc(arry *l){
	int res=1;
	while(l!=NULL){
		res*=l->nr;
		l=l->next;
	}
	return res;
}

int check_match(prop_exp* exp1,prop_exp* exp2){
	if(exp1->type==exp2->type){
		if(produc(exp1->arrylist)==produc(exp2->arrylist))return 1;
	}
	return 0;
}
int check_int(prop_exp* exp_){
	if(exp_->arrylist==NULL&&exp_->type==root_field.type_t)return 1;
	return 0;
}
int check_float(prop_exp* exp_){
	if(exp_->arrylist==NULL&&exp_->type==root_field.type_t->next)return 1;
	return 0;
}
int check_num(prop_exp* exp_){
	return (check_int(exp_))|(check_float(exp_));
}
int check_struct(prop_exp* exp_){
	if(exp_->arrylist==NULL){
		if(exp_->type!=root_field.type_t && exp_->type!=root_field.type_t->next)return 1;
	}
	return 0;
}
int check_param(prop_var *var_,prop_exp *exp_){
	while(var_!=NULL&&exp_!=NULL){
		if(var_->type!=exp_->type||var_->arrylist!=exp_->arrylist)break;
		var_=var_->next;
		exp_=exp_->next;	
	}
	if(var_==NULL&&exp_==NULL)return 1;
	return 0;
}

prop_fun* find_func(prop_ID *id_){
	prop_fun *p=funlist;
	while(p!=NULL){
		if(idcmp(id_,p->id)==0)return p;
		p=p->next;
	}	
	return NULL;
}

prop_var* find_sym(prop_ID *id_){
	field_tree *q=current_field;
	while(q!=NULL){
		prop_var *p=q->sym_t;
		while(p!=NULL){
			if(idcmp(id_,p->id)==0)return p;
			p=p->next;
		}
		q=q->parent;
	}
	return NULL;
}

prop_var* find_sym_instruct(prop_type *type_,prop_ID *id_){
	prop_var *p=type_->varlist;
	while(p!=NULL){
		if(idcmp(id_,p->id)==0)return p;
		p=p->next;
	}
	return NULL;
}

prop_exp* create_exp(prop_type *type_,arry *l,int label){
	prop_exp *new=(prop_exp *)malloc(sizeof(prop_exp));
	new->type=type_;
	new->if_left=label;
	new->arrylist=NULL;
	if(l!=NULL){
		new->arrylist=(arry *)malloc(sizeof(arry));
		new->arrylist->nr=l->nr;
		new->arrylist->next=NULL;
		l=l->next;
		arry *p=new->arrylist;
		while(l!=NULL){
			p->next=(arry *)malloc(sizeof(arry));
			p->next->nr=l->nr;
			p->next->next=NULL;
			p=p->next;
			l=l->next;
		}
	}
	return new;
}

void find_error(int n,int line){
	printf("Error type %d at line %d:",n,line);
	switch(n){
		case 1: printf("Undefined variable\n");break;
		case 2: printf("Undefined function\n");break;
		case 3: printf("Redefined variable\n");break;
		case 4: printf("Redefined function\n");break;
		case 5: printf("Type mismatched for assignment\n");break;
		case 6: printf("Lvalue required as left operand of assignment\n");break;
		case 7: printf("Type mismatched for operands\n");break;
		case 8: printf("Type mismatched for return\n");break;
		case 9: printf("Arguments mismatched for function\n");break;
		case 10: printf("Not an array\n");break;
		case 11: printf("Not a function\n");break;
		case 12: printf("Not a integer for array index\n");break;
		case 13: printf("Illegal use of \".\"\n");break;
		case 14: printf("Non-existent field\n");break;
		case 15: printf("Redefined field or initialized field\n");break;
		case 16: printf("Duplicated struct name\n");break;
		case 17: printf("Undefined structure\n");break;
	}
}










void entre_field(int label){
	field_tree *new=(field_tree *)malloc(sizeof(field_tree));
	new->if_struct=label;
	new->sym_t=NULL;
	new->type_t=NULL;
	new->parent=current_field;
	new->next=new->child=NULL;
	field_tree *p=current_field->child;
	if(p==NULL){
		current_field->child=new;
	}
	else{
		while(p->next!=NULL){
			p=p->next;
		}
		p->next=new;
	}
	current_field=new;
}

void quit_field(){
	current_field=current_field->parent;
}

void analy_tree(Nod *nod){
//	printf("%d %d\n",nod->token_id,nod->pattern_id);
	if(nod==root){
		root_field.if_struct=0;
		root_field.sym_t=NULL;
		root_field.type_t=NULL;
		root_field.parent=root_field.next=root_field.child=NULL;
		current_field=&root_field;
		add_type("int");
		add_type("float");
	}
	

	Nod *q=nod->son;
	while(q!=NULL){
		analy_tree(q);
		q=q->next;
	}
		
	int *nr;void *p;
	switch (nod->token_id){
		case 1:
			nod->prop=(void *)get_type((char *)nod->prop);
				break;
		case 7:
			nr=(int *)malloc(sizeof(int));
			*nr=atoi((char *)nod->prop);
			nod->prop=(void *)nr;
			break;
		case 26:
			if(nod->parent->token_id==36){
				entre_field(1);
			}
			else if(nod->parent->parent->token_id!=33){
				entre_field(0);
			}
			break;
		case 27:
			if(nod->parent->token_id==43){
				quit_field(0);
			}

			break;
		case 33:
			switch (nod->pattern_id){
				case 1:
					set_type1var((prop_var *)(nod->son->next->prop),(prop_type *)(nod->son->prop));	
					if(add_sym((prop_var *)(nod->son->next->prop))==0)find_error(3,nod->first_line);
				break;
				case 3:	
					set_type1fun((prop_fun *)(nod->son->next->prop),(prop_type *)(nod->son->prop));	
					if(add_fun((prop_fun *)(nod->son->next->prop))==0)find_error(4,nod->first_line);
				break;
					
			}
			break;
		case 34:
			switch (nod->pattern_id){
				case 1:
					nod->prop=nod->son->prop;
				break;
				case 2:
					add_var2var((prop_var *)(nod->son->prop),(prop_var *)(nod->son->next->next->prop));
					nod->prop=nod->son->prop;
			}	
			break;
		case 35:
			nod->prop=nod->son->prop;
			break;
		case 36:
			switch (nod->pattern_id){
				case 1:
					nod->prop=(void *)add_type((prop_ID *)(nod->son->next->prop));
					if(nod->prop==NULL)find_error(16,nod->first_line);
					quit_field();	
					break;
				case 2:
					nod->prop=get_type((prop_ID *)(nod->son->next->prop));
					if(nod->prop==NULL)find_error(17,nod->first_line);
					break;
			}
			break;
		case 37:
			switch (nod->pattern_id){
				case 1:
					nod->prop=NULL;
					break;
				case 2:
					nod->prop=nod->son->prop;
					break;
			}
			break;
		case 38:
			nod->prop=nod->son->prop;
			break;
		case 39:
			switch (nod->pattern_id){
				case 1:
					nod->prop=create_var((prop_ID *)(nod->son->prop));
					break;
				case 2:
					
					add_arry1var((prop_var *)(nod->son->prop),(prop_INT *)(nod->son->next->next->prop));
					nod->prop=nod->son->prop;
					break;
			}
			break;
		case 40:
			switch (nod->pattern_id){
				case 1:
					nod->prop=create_fun((prop_ID *)(nod->son->prop),(prop_var *)(nod->son->next->next->prop));
					entre_field(0);
					if(add_sym((prop_var *)(nod->son->next->next->prop))==0)find_error(3,nod->first_line);
					break;
				case 2:
					nod->prop=create_fun((prop_ID *)(nod->son->prop),NULL);
					entre_field(0);
					break;
			}
			break;
		case 41:
			switch (nod->pattern_id){
				case 1:
					add_var2var((prop_var *)(nod->son->prop),(prop_var *)(nod->son->next->next->prop));
					nod->prop=nod->son->prop;
				break;
				case 2:
					nod->prop=nod->son->prop;
			}	
			break;
		case 42:
			set_type1var((prop_var *)(nod->son->next->prop),(prop_type *)(nod->son->prop));	
			nod->prop=nod->son->next->prop;
			break;
		case 45:
			if(nod->pattern_id==3){
				p=(void *)nod;
				while(((Nod *)p)->token_id!=33)p=((Nod *)(p))->parent;
				if(((prop_exp *)(nod->son->next->prop))->arrylist!=NULL||((Nod *)(p))->son->prop!=((prop_exp *)(nod->son->next->prop))->type)
				find_error(8,nod->first_line);
			}
			break;
		case 47:
			switch (nod->pattern_id){
				case 1:
					set_type1var((prop_var *)(nod->son->next->prop),(prop_type *)(nod->son->prop));		
					if(add_sym((prop_var *)(nod->son->next->prop))==0){
						if(current_field->if_struct==1)find_error(15,nod->first_line);
						else
							find_error(3,nod->first_line);
						break;
					}
			}		
			break;
		case 48:
			switch (nod->pattern_id){
				case 1:
					nod->prop=nod->son->prop;
					break;
				case 2:		
					add_var2var((prop_var *)(nod->son->prop),(prop_var *)(nod->son->next->next->prop));
					nod->prop=nod->son->prop;
					break;
			}	
			break;
		case 49:
			switch (nod->pattern_id){
				case 1:
					nod->prop=nod->son->prop;
					break;
				case 2:
					nod->prop=nod->son->prop;
					break;
			}
			break;
		case 50:
			switch(nod->pattern_id){
				case 1:
					if(nod->son->prop!=NULL&&nod->son->next->next->prop!=NULL){
						if(((prop_exp *)nod->son->prop)->if_left==0){
							find_error(6,nod->first_line);nod->prop=NULL;
						}
						else if(check_match(nod->son->prop,nod->son->next->next->prop)==0){
							find_error(5,nod->first_line);nod->prop=NULL;
						}
						else{
							nod->prop=nod->son->prop;
							((prop_exp *)(nod->prop))->if_left=0;
						}
					}else
						nod->prop=NULL;
					break;
				case 2:
				case 18:
					if(nod->son->prop!=NULL&&nod->son->next->next->prop!=NULL){
						if(check_int(nod->son->prop)==0||check_int(nod->son->next->next->prop)==0){
							find_error(7,nod->first_line);nod->prop=NULL;
						}
						else{
							nod->prop=nod->son->prop;
							((prop_exp *)(nod->prop))->if_left=0;
						}
					}
					else 
						nod->prop=NULL;
					break;
				case 3:
					if(nod->son->prop!=NULL&&nod->son->next->next->prop!=NULL){
						if((check_int(nod->son->next->next->prop)&check_int(nod->son->prop))==1||(check_float(nod->son->next->next->prop)&check_float(nod->son->prop))==1){
						nod->prop=(void *)create_exp(root_field.type_t,NULL,0);
						}
						else{
							find_error(7,nod->first_line);nod->prop=NULL;
						}
					}
					else 
						nod->prop=NULL;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					if(nod->son->prop!=NULL&&nod->son->next->next->prop!=NULL){
						if((check_int(nod->son->next->next->prop)&check_int(nod->son->prop))==1||(check_float(nod->son->next->next->prop)&check_float(nod->son->prop))==1){
							nod->prop=nod->son->prop;
							((prop_exp *)(nod->prop))->if_left=0;
						}
						else{
							find_error(7,nod->first_line);nod->prop=NULL;
						}
					}
					else 
						nod->prop=NULL;
					break;
				case 8:
					nod->prop=nod->son->next->prop;
					break;
				case 9:
					if(nod->son->next->prop!=NULL){
						if(check_num(nod->son->next->prop)==0){
							find_error(7,nod->first_line);nod->prop=NULL;
						}
						else{
							nod->prop=nod->son->next->prop;
							((prop_exp *)(nod->prop))->if_left=0;
						}
					}
					else 
						nod->prop=NULL;
					break;
				case 10:
					if(nod->son->next->prop!=NULL){
						if(check_int(nod->son->next->prop)==0){
							find_error(7,nod->first_line);nod->prop=NULL;
						}
						else{
							nod->prop=nod->son->next->prop;
							((prop_exp *)(nod->prop))->if_left=0;
						}
					}
					else 
						nod->prop=NULL;
					break;

				case 11:
					p=(void *)find_func(nod->son->prop);
					if(p!=NULL){
						if(check_param(((prop_fun *)p)->paramlist,nod->son->next->next->prop)==0){
							find_error(9,nod->first_line);
						}
							nod->prop=(void *)create_exp(((prop_fun *)p)->type,NULL,1);
					}
					else if(find_sym(nod->son->prop)==NULL){
						find_error(2,nod->first_line);nod->prop=NULL;
					}
					else{
						find_error(11,nod->first_line);nod->prop=NULL;
					}
					break;
				case 12:
					p=(void *)find_func(nod->son->prop);
					if(p!=NULL){
						if(((prop_fun *)p)->paramlist!=NULL){
							find_error(9,nod->first_line);
						}
							nod->prop=(void *)create_exp(((prop_fun *)(p))->type,NULL,1);
					}
					else if(find_sym(nod->son->prop)==NULL){
						find_error(2,nod->first_line);nod->prop=NULL;
					}
					else{
						find_error(11,nod->first_line);nod->prop=NULL;
					}

					break;
				case 13:
					if(nod->son->prop!=NULL)
						if(((prop_exp *)(nod->son->prop))->arrylist==NULL){
							find_error(10,nod->first_line);nod->prop=NULL;
						}
						else{
							((prop_exp*)(nod->son->prop))->arrylist=((prop_exp*)(nod->son->prop))->arrylist->next;
							nod->prop=nod->son->prop;							
							if(check_int(nod->son->next->next->prop)==0)
								find_error(12,nod->first_line);
						}
					else
						nod->prop=NULL;
					break;
				case 14:
					if(nod->son->prop!=NULL){
						if(check_struct(nod->son->prop)==0){find_error(13,nod->first_line);nod->prop=NULL;}
						else{
							p=(void *)find_sym_instruct(((prop_exp*)(nod->son->prop))->type,nod->son->next->next->prop);
							if(p==NULL){find_error(14,nod->first_line);nod->prop=NULL;}
							else
								nod->prop=(void *)create_exp(((prop_var *)p)->type,((prop_var *)p)->arrylist,1);
						}
					}
					else
						nod->prop=NULL;
					break;
				case 15:
					p=(void *)find_sym(nod->son->prop);
					if(p==NULL){find_error(1,nod->first_line);nod->prop=NULL;}
					else
						nod->prop=(void *)create_exp(((prop_var *)p)->type,((prop_var *)p)->arrylist,1);
					break;
				case 16:
						nod->prop=(void *)create_exp(root_field.type_t,NULL,0);
					break;
				case 17:
						nod->prop=(void *)create_exp(root_field.type_t->next,NULL,0);
					break;
			}
			break;
		case 51:
			switch(nod->pattern_id){
				case 1:
					if(nod->son->prop!=NULL){
						add_exp2exp((prop_exp*)nod->son->prop,(prop_exp*)nod->son->next->next->prop);
						nod->prop=nod->son->prop;
					}
					else nod->prop=nod->son->next->next->prop;	
					break;	
				case 2:
					nod->prop=nod->son->prop;
					break;
			}
	}

}
void print_funlist(){
	prop_fun *p=funlist;
	while(p!=NULL){
		printf("%s %s (",p->type->id,p->id);
		prop_var *q=p->paramlist;
		while(q!=NULL){
			if(q->type!=NULL)
			printf("%s %s,",((prop_type *)(q->type))->id,q->id);
			q=q->next;
		}
		printf(")\n");
		p=p->next;
	}
	printf("-----------------\n");
}
void print_symt(field_tree *f,int d){
	if(f->if_struct==0){
	prop_var *a=f->sym_t;
	for(int i=0;i<d;i++)printf("	");
	printf("sym_tab:\n");
	while(a!=NULL){
		if(a->type!=NULL){
		for(int i=0;i<d;i++)printf("	");
		printf("	%s (%s)",a->id,((prop_type *)(a->type))->id);
		if(a->arrylist!=NULL){
			arry *p=a->arrylist;
			while(p!=NULL){printf("[%d]",p->nr);p=p->next;}
		}
		printf("\n");
		}	
		a=a->next;
	}
	prop_type *b=f->type_t;
	for(int i=0;i<d;i++)printf("	");
	printf("type_tab:\n");
	while(b!=NULL){
		if(b->id!=NULL){
			for(int i=0;i<d;i++)printf("	");
			printf("	%s:\n",(char *)b->id);
		}
		prop_var *c=b->varlist;
		while(c!=NULL){
			if(c->type!=NULL){
			for(int i=0;i<d;i++)printf("	");
			printf("		%s (%s)",c->id,((prop_type *)(c->type))->id);
			if(c->arrylist!=NULL){
				arry *p=c->arrylist;
				while(p!=NULL){printf("[%d]",p->nr);p=p->next;}
			}
			printf("\n");
			}
			c=c->next;
		}
		b=b->next;
	}
	for(int i=0;i<d;i++)printf("	");
	printf("~~~~~~~~~~~\n");
	if(f->child!=NULL)print_symt(f->child,d+1);
	}
	if(f->next!=NULL)print_symt(f->next,d);
}
