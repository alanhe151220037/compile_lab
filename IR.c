#include "IR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int Counter=0;
int Counter_=0;
char *itoa_(int a){
	int nr=11;
	char *str=(char *)malloc(12*sizeof(char));
	if(a==0)str[--nr]='0';
	while(a!=0){
		str[--nr]='0'+a%10;
		a/=10;
	}
	return ((char *)str+nr);
}
operand *create_operand(int k,char *str_,int val_){
	operand *new=(operand *)malloc(sizeof(operand));
	new->kind=k;new->next=NULL;
	if(k==1){
		//const
		char *d=(char *)malloc(16*sizeof(char)); 
		d[0]='#';d[1]='\0';char *x=itoa_(val_);
		strcat(d,x);
		new->str=d;
		new->val=val_;
	}
	else if(k==2){
		//var
		char *d=(char *)malloc(16*sizeof(char)); 
		d[0]='v';d[1]='\0';char *x=itoa_(++Counter);
		strcat(d,x);
		new->str=d;
	}
	else if(k==3){
		//fun
		new->str=str_;
	}
	else if(k==4){
		//var
		char *d=(char *)malloc(16*sizeof(char)); 
		d[0]='l';d[1]='a';d[2]='b';d[3]='e';d[4]='l';d[5]='\0';char *x=itoa_(++Counter_);
		strcat(d,x);
		new->str=d;
	}

	return new;
}

sentence *create_sen(int k,operand *a1,operand *a2,operand *a3){
	sentence *new=(sentence *)malloc(sizeof(sentence));
	new->kind = k;
	new->arg0=a1;
	new->arg1=a2;
	new->arg2=a3;
	return new;
}

sentence *connect_sen(sentence *s1,sentence *s2){
	if(s1==NULL)return s2;
	sentence *p=s1;
	while(p->next!=NULL){
		p=p->next;	
	}
	p->next=s2;
	return s1;
}


void print_sentence(sentence *s){
	if(s!=NULL){
		switch(s->kind){
			case 1:
				fprintf(fp,"LABEL %s :\n",s->arg0->str);
				break;
			case 2:
				fprintf(fp,"FUNCTION %s :\n",s->arg0->str);
				break;
			case 3:
				fprintf(fp,"%s := %s\n",s->arg0->str,s->arg1->str);
				break;
			case 4:
				fprintf(fp,"%s := %s + %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;
			case 5:
				fprintf(fp,"%s := %s - %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;
			case 6:
				fprintf(fp,"%s := %s * %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;
			case 7:
				fprintf(fp,"%s := %s / %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;
			case 8:
				fprintf(fp,"%s := &%s\n",s->arg0->str,s->arg1->str);
				break;
			case 9:
				fprintf(fp,"%s := *%s\n",s->arg0->str,s->arg1->str);
				break;
			case 10:
				fprintf(fp,"*%s := %s\n",s->arg0->str,s->arg1->str);
				break;
			case 11:
				fprintf(fp,"GOTO %s\n",s->arg0->str);
				break;
			case 12:
				fprintf(fp,"IF %s > %s GOTO %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;	
			case 13:
				fprintf(fp,"IF %s < %s GOTO %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;	
			case 14:
				fprintf(fp,"IF %s >= %s GOTO %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;	
			case 15:
				fprintf(fp,"IF %s <= %s GOTO %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;	
			case 16:
				fprintf(fp,"IF %s == %s GOTO %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;	
			case 17:
				fprintf(fp,"IF %s != %s GOTO %s\n",s->arg0->str,s->arg1->str,s->arg2->str);
				break;	
			case 18:
				fprintf(fp,"RETURN %s\n",s->arg0->str);
				break;
			case 19:
				fprintf(fp,"DEC %s [%d]\n",s->arg0->str,s->arg1->val);
				break;
			case 20:
				fprintf(fp,"ARG %s\n",s->arg0->str);	
				break;
			case 21:
				fprintf(fp,"%s := CALL %s\n",s->arg0->str,s->arg1->str);
				break;
			case 22:
				fprintf(fp,"PARAM %s\n",s->arg0->str);
				break;
			case 23:
				fprintf(fp,"READ %s\n",s->arg0->str);
				break;
			case 24:
				fprintf(fp,"WRITE %s\n",s->arg0->str);

		}
		print_sentence(s->next);
	}
}
int get_relop(char *str){
	if(strcmp(str,">")==0)return 12;
	else if(strcmp(str,"<")==0)return 13;
	else if(strcmp(str,">=")==0)return 14;
	else if(strcmp(str,"<=")==0)return 15;
	else if(strcmp(str,"==")==0)return 16;
	else if(strcmp(str,"!=")==0)return 17;
}
sentence *translate_param(prop_fun *fun_){
	field_tree *f_=fun_->f;
	prop_var *p=fun_->paramlist;
	prop_var *q=f_->sym_t;
	sentence *s=NULL;
	while(p!=NULL){
		q->op=create_operand(2,NULL,0);
		s=connect_sen(s,create_sen(22,(operand *)q->op,NULL,NULL));
		p=p->next;
		q=q->next;
	}
	return s;
}

void connect_ope(operand *op1,operand *op2){
	op1->next=op2->next;
	op2->next=op1;
}
sentence *translate_args(Nod *nod,field_tree *f,operand *list){
	operand *p;
	sentence *s;
	if(nod->pattern_id==2){
		p=create_operand(2,NULL,0);
		s=translate_exp(nod->son,f,p);
		connect_ope(p,list);
		return s;
	}
	else if(nod->pattern_id==1){
		p=create_operand(2,NULL,0);
		s=translate_exp(nod->son,f,p);
		connect_ope(p,list);
		connect_sen(s,translate_args(nod->son->next->next,f,list));
		return s;
	}
	return NULL;
}
sentence *translate_cond(Nod *nod,operand *label_t,operand *label_f,field_tree *f){
printf("%d %d in \n",nod->token_id,nod->pattern_id);
	operand *p1;
	operand *p2;
	sentence *s;
	switch(nod->pattern_id){
		case 3:
			p1=create_operand(2,NULL,0);
			p2=create_operand(2,NULL,0);
			s=translate_exp(nod->son,f,p1);
			connect_sen(s,translate_exp(nod->son->next->next,f,p2));
			connect_sen(s,create_sen(get_relop(nod->son->next->prop),p1,p2,label_t));
			connect_sen(s,create_sen(11,label_f,NULL,NULL));
			return s;
		case 10:
			return translate_cond(nod->son->next,label_f,label_t,f);
		case 2:
			p1=create_operand(4,NULL,0);
			s=translate_cond(nod->son,p1,label_f,f);
			connect_sen(s,create_sen(1,p1,NULL,NULL));
			connect_sen(s,translate_cond(nod->son->next->next,label_t,label_f,f));
			return s;
		case 18:
			p1=create_operand(4,NULL,0);
			s=translate_cond(nod->son,label_t,p1,f);
			connect_sen(s,create_sen(1,p1,NULL,NULL));
			connect_sen(s,translate_cond(nod->son->next->next,label_t,label_f,f));
			return s;
		default:
			p1=create_operand(2,NULL,0);
			s=translate_exp(nod, f,p1);
			connect_sen(s,create_sen(17,p1,create_operand(1,NULL,0),label_t));
			connect_sen(s,create_sen(11,label_f,NULL,NULL));
			return s;
	}
}

operand *lookup(field_tree *f,char *id_){
	prop_var *p=f->sym_t;
	while(p!=NULL){
		if(strcmp(p->id,id_)==0){
			if(p->op==NULL)p->op=create_operand(2,NULL,0);
			return p->op;
		}
		p=p->next;
	}
}
sentence *translate_exp(Nod *nod,field_tree *f,operand *place){
printf("%d %d\n",nod->token_id,nod->pattern_id);
	if(place==NULL)place=create_operand(2,NULL,0);
	operand *p1;
	operand *p2;
	operand *p3;
	sentence *s;
	switch(nod->pattern_id){
		case 16:
			p1=create_operand(1,NULL,*(int *)(nod->son->prop));
			return create_sen(3,place,p1,NULL);
		case 15:
			return create_sen(3,place,lookup(f,nod->son->prop),NULL);
		case 1:
			switch(nod->son->pattern_id){
				case 15:
					p1=lookup(f,nod->son->son->prop);
					p2=create_operand(2,NULL,0);		
					s=translate_exp(nod->son->next->next,f,p2);
					connect_sen(s,create_sen(3,p1,p2,NULL));			
					connect_sen(s,create_sen(3,place,p1,NULL));			
					return s;
				default:
					return NULL;
			}
		case 4:
			p1=create_operand(2,NULL,0);			
			p2=create_operand(2,NULL,0);		
			s=translate_exp(nod->son,f,p1);
			connect_sen(s,translate_exp(nod->son->next->next,f,p2));
			connect_sen(s,create_sen(4,place,p1,p2));
			return s;
		case 5:
			p1=create_operand(2,NULL,0);			
			p2=create_operand(2,NULL,0);		
			s=translate_exp(nod->son,f,p1);
			connect_sen(s,translate_exp(nod->son->next->next,f,p2));
			connect_sen(s,create_sen(5,place,p1,p2));
			return s;
		case 6:
			p1=create_operand(2,NULL,0);			
			p2=create_operand(2,NULL,0);		
			s=translate_exp(nod->son,f,p1);
			connect_sen(s,translate_exp(nod->son->next->next,f,p2));
			connect_sen(s,create_sen(6,place,p1,p2));
			return s;
		case 7:
			p1=create_operand(2,NULL,0);			
			p2=create_operand(2,NULL,0);		
			s=translate_exp(nod->son,f,p1);
			connect_sen(s,translate_exp(nod->son->next->next,f,p2));
			connect_sen(s,create_sen(7,place,p1,p2));
			return s;
		case 8:
			return translate_exp(nod->son->next,f,place);
		case 9:
			p1=create_operand(2,NULL,0);
			s=translate_exp(nod->son->next,f,p1);
			connect_sen(s,create_sen(5,place,create_operand(1,NULL,0),p1));
			return s;
		case 2:
		case 3:
		case 10:
		case 18:
			p1=create_operand(4,NULL,0);
			p2=create_operand(4,NULL,0);
			s=create_sen(3,place,create_operand(1,NULL,0),NULL);
			connect_sen(s,translate_cond(nod,p1,p2,f));
			connect_sen(s,create_sen(1,p1,NULL,NULL));
			connect_sen(s,create_sen(3,place,create_operand(1,NULL,1),p1));
			connect_sen(s,create_sen(1,p2,NULL,NULL));
			return s;	
		case 12:
			if(strcmp(nod->son->prop,"read")==0)return create_sen(23,place,NULL,NULL);
			return create_sen(21,place,create_operand(3,nod->son->prop,0),NULL);
		case 11:
			p1=create_operand(1,NULL,0);
			s=translate_args(nod->son->next->next,f,p1);
			p1=p1->next;
			if(strcmp(nod->son->prop,"write")==0){
				connect_sen(s,create_sen(24,p1,NULL,NULL));
				return s;
			}
			else{
				while(p1!=NULL){
					connect_sen(s,create_sen(20,p1,NULL,NULL));
					p1=p1->next;
				}
				connect_sen(s,create_sen(21,place,create_operand(3,nod->son->prop,0),NULL));
				return s;
			}
		case 13://TODO
			return NULL;
	}
}


sentence *translate(Nod *nod,field_tree *f){	
printf("%d %d\n",nod->token_id,nod->pattern_id);
	sentence *p;
	operand *q1;
	operand *q2;
	operand *q3;
	switch(nod->token_id){
		case 31: 
			return translate(nod->son,NULL);
		case 32:
			if(nod->pattern_id==2)
				return connect_sen(translate(nod->son,NULL),translate(nod->son->next,NULL));
			else
				return NULL;
		case 33:
			if(nod->pattern_id==3){
				p=create_sen(2,create_operand(3,((prop_fun *)(nod->son->next->prop))->id,0),NULL,NULL);
				connect_sen(p,translate_param((prop_fun *)(nod->son->next->prop)));
				return connect_sen(p,translate(nod->son->next->next,((prop_fun *)(nod->son->next->prop))->f));
	
			}
			else
				return NULL;
		case 43:
			if(nod->pattern_id==1){
				return connect_sen(translate(nod->son->next,f),translate(nod->son->next->next,f));
			}
			else
				return NULL;
		case 44:
			if(nod->pattern_id==2){
				return connect_sen(translate(nod->son,f),translate(nod->son->next,f));
			}
			else
				return NULL;
		case 45:
			switch(nod->pattern_id){
				case 1:
					return translate_exp(nod->son,f,NULL);
				case 2:
					return translate(nod->son,f);
				case 3:
					q1=create_operand(2,NULL,0);
					return connect_sen(translate_exp(nod->son->next,f,q1),create_sen(18,q1,NULL,NULL));
				case 4:
					q1=create_operand(4,NULL,0);
					q2=create_operand(4,NULL,0);
					q3=create_operand(4,NULL,0);
					p=translate_cond(nod->son->next->next,q1,q2,f);
					connect_sen(p,create_sen(1,q1,NULL,NULL));
					connect_sen(p,translate(nod->son->next->next->next->next,f));
					connect_sen(p,create_sen(11,q3,NULL,NULL));
					connect_sen(p,create_sen(1,q2,NULL,NULL));
					connect_sen(p,translate(nod->son->next->next->next->next->next->next,f));
					connect_sen(p,create_sen(1,q3,NULL,NULL));
					return p;
				case 5:
					q1=create_operand(4,NULL,0);
					q2=create_operand(4,NULL,0);
					p=translate_cond(nod->son->next->next,q1,q2,f);
					
					connect_sen(p,create_sen(1,q1,NULL,NULL));
					connect_sen(p,translate(nod->son->next->next->next->next,f));
					connect_sen(p,create_sen(1,q2,NULL,NULL));
					return p;
				case 6:
					q1=create_operand(4,NULL,0);
					q2=create_operand(4,NULL,0);
					q3=create_operand(4,NULL,0);
					p=create_sen(1,q1,NULL,NULL);
					connect_sen(p,translate_cond(nod->son->next->next,q2,q3,f));
					connect_sen(p,create_sen(1,q2,NULL,NULL));
					connect_sen(p,translate(nod->son->next->next->next->next,f));
					connect_sen(p,create_sen(11,q1,NULL,NULL));
					connect_sen(p,create_sen(1,q3,NULL,NULL));
					return p;
			}
		case 46:
			if(nod->pattern_id==2){
				return connect_sen(translate(nod->son,f),translate(nod->son->next,f));
			}
			else
				return NULL;

		case 47:
			return translate(nod->son->next,f);	
		case 48:
			if(nod->pattern_id==1){
				if(((prop_var *)(nod->son->prop))->arrylist==NULL)return NULL;
				else{
					((prop_var *)(nod->son->prop))->op=create_operand(2,NULL,0);
					return create_sen(19,((prop_var *)(nod->son->prop))->op,create_operand(1,NULL,((prop_var *)(nod->son->prop))->arrylist->nr*4),NULL);
				}
			}
			else if(nod->pattern_id==2){
				if(((prop_var *)(nod->son->prop))->arrylist==NULL)p=NULL;
				else{
					((prop_var *)(nod->son->prop))->op=create_operand(2,NULL,0);
					p=create_sen(19,((prop_var *)(nod->son->prop))->op,create_operand(1,NULL,((prop_var *)(nod->son->prop))->arrylist->nr*4),NULL);
				}
				return connect_sen(p,translate(nod->son->next->next,f));
			}
		
	}
}
