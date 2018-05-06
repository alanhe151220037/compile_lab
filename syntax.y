%{
#include <stdio.h>
#include "gram_tree.h"
#define YYSTYPE Nod
#define YYDEBUG 1
#define set_gnod(nod,gram_code) \
	do{\
		nod.if_null=0;\
		nod.if_lex=0;\
		nod.id = gram_code;\
		nod.str=NULL;\
		nod.son=nod.next=nod.self=NULL;\
	}while(0)
extern char *yytext;
extern int yylineno;
int success = 1;
int yyerror(char *msg){
	success=0;
	fprintf(stderr,"ERROR type B at Line %d: '%s' nearby\n",yylineno,yytext);
	return 0;
}
extern int yylex();
%}


/*declared tokens*/
%token TYPE IF STRUCT RETURN ELSE WHILE
%token INT FLOAT ID 
%token SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token LP RP LB RB LC RC 

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LP RP LB RB




/*declared non-terminals*/

%%
/*High-level Definitions*/
Program	: ExtDeflist {set_gnod($$,1); insert_1(&$$,&$1); root=$$.self;}
	;
ExtDeflist	:	{set_gnod($$,2);$$.if_null = 1;copy_nod(&$$);}
	| ExtDef ExtDeflist {set_gnod($$,2); insert_2(&$$,&$1,&$2);}
	;
ExtDef	: Specifier ExtDecList SEMI {set_gnod($$,3); insert_3(&$$,&$1,&$2,&$3);}
       	| Specifier SEMI {set_gnod($$,3); insert_2(&$$,&$1,&$2);}
	| Specifier FunDec CompSt {set_gnod($$,3); insert_3(&$$,&$1,&$2,&$3);}
	| error SEMI {set_gnod($$,3);set_gnod($1,-1);copy_nod(&$1);insert_2(&$$,&$1,&$2);}
	;
ExtDecList	: VarDec {set_gnod($$,4); insert_1(&$$,&$1);}
	| VarDec COMMA ExtDecList {set_gnod($$,4); insert_3(&$$,&$1,&$2,&$3);}
	;
/*Specifiers*/
Specifier	: TYPE {set_gnod($$,5); insert_1(&$$,&$1);}
	| StructSpecifier {set_gnod($$,5); insert_1(&$$,&$1);}
	;
StructSpecifier	: STRUCT OptTag LC DefList RC {set_gnod($$,6); insert_5(&$$,&$1,&$2,&$3,&$4,&$5);}
	| STRUCT Tag {set_gnod($$,6); insert_2(&$$,&$1,&$2);}
	;
OptTag	:{set_gnod($$,7);$$.if_null = 1;copy_nod(&$$);}
	| ID {set_gnod($$,7); insert_1(&$$,&$1);}
	;
Tag	: ID {set_gnod($$,8); insert_1(&$$,&$1);}
    	;
/*Declarators*/
VarDec	: ID {set_gnod($$,9); insert_1(&$$,&$1);}
	| VarDec LB INT RB {set_gnod($$,9); insert_4(&$$,&$1,&$2,&$3,&$4);}
	;
FunDec	: ID LP VarList RP {set_gnod($$,10); insert_4(&$$,&$1,&$2,&$3,&$4);}
	| ID LP RP {set_gnod($$,10); insert_3(&$$,&$1,&$2,&$3);}
	;
VarList	: ParamDec COMMA VarList {set_gnod($$,11); insert_3(&$$,&$1,&$2,&$3);}
	| ParamDec {set_gnod($$,11); insert_1(&$$,&$1);}
	;
ParamDec: Specifier VarDec {set_gnod($$,12); insert_2(&$$,&$1,&$2);}
	;
/*Statements*/
CompSt	: LC DefList StmtList RC {set_gnod($$,13); insert_4(&$$,&$1,&$2,&$3,&$4);}
	| error RC {printf("wrong\n");set_gnod($$,13);set_gnod($1,-1);copy_nod(&$1);insert_2(&$$,&$1,&$2);}
	;
StmtList: {set_gnod($$,14);$$.if_null = 1;copy_nod(&$$);}
	| Stmt StmtList {set_gnod($$,14); insert_2(&$$,&$1,&$2);}
	;
Stmt	: Exp SEMI {set_gnod($$,15); insert_2(&$$,&$1,&$2);}
     	| CompSt {set_gnod($$,15); insert_1(&$$,&$1);}
	| RETURN Exp RP Stmt {set_gnod($$,15); insert_4(&$$,&$1,&$2,&$3,&$4);}
	| IF LP Exp RP Stmt ELSE Stmt {set_gnod($$,15); insert_7(&$$,&$1,&$2,&$3,&$4,&$5,&$6,&$7);}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {set_gnod($$,15); insert_5(&$$,&$1,&$2,&$3,&$4,&$5);}
	| WHILE LP Exp RP Stmt {set_gnod($$,15); insert_5(&$$,&$1,&$2,&$3,&$4,&$5);}
	| error SEMI {set_gnod($$,15);set_gnod($1,-1);copy_nod(&$1);insert_2(&$$,&$1,&$2);}
	;
/*Local Definitions*/
DefList : {set_gnod($$,16);$$.if_null = 1;copy_nod(&$$);}
	| Def DefList {set_gnod($$,16); insert_2(&$$,&$1,&$2);}
	;
Def	: Specifier DecList SEMI {set_gnod($$,17); insert_3(&$$,&$1,&$2,&$3);}
    	| error SEMI {set_gnod($$,17);set_gnod($1,-1);copy_nod(&$1);insert_2(&$$,&$1,&$2);}
	;
DecList	: Dec {set_gnod($$,18); insert_1(&$$,&$1);}
    	| Dec COMMA DecList {set_gnod($$,18); insert_3(&$$,&$1,&$2,&$3);}
	;
Dec	: VarDec {set_gnod($$,19); insert_1(&$$,&$1);}
    	| VarDec ASSIGNOP Exp {set_gnod($$,19); insert_3(&$$,&$1,&$2,&$3);}
	
	;
Exp	: Exp ASSIGNOP Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
    	| Exp AND Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| Exp RELOP Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| Exp PLUS Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| Exp MINUS Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| Exp STAR Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| Exp DIV Exp {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| LP Exp RP {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| MINUS Exp {set_gnod($$,20); insert_2(&$$,&$1,&$2);}
	| NOT Exp {set_gnod($$,20); insert_2(&$$,&$1,&$2);}
	| ID LP Args RP {set_gnod($$,20); insert_4(&$$,&$1,&$2,&$3,&$4);}
	| ID LP RP {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| Exp LB Exp RB {set_gnod($$,20); insert_4(&$$,&$1,&$2,&$3,&$4);}
	| Exp DOT ID {set_gnod($$,20); insert_3(&$$,&$1,&$2,&$3);}
	| ID {set_gnod($$,20); insert_1(&$$,&$1);}
	| INT {set_gnod($$,20); insert_1(&$$,&$1);}
	| FLOAT {set_gnod($$,20); insert_1(&$$,&$1);}
	;
Args	: Exp COMMA Args {set_gnod($$,21); insert_3(&$$,&$1,&$2,&$3);}
     	| Exp {set_gnod($$,21); insert_1(&$$,&$1);}
	;

/*
Cal	: 
     	| Exp {printf("= %d\n",$1);}
 	;
Exp 	: Factor
	| Exp ADD Factor { $$ = $1 + $3;}
	| Exp SUB Factor { $$ = $1 - $3;}
	;
Factor 	: Term
	| Factor MUL Term { $$ = $1 * $3;}
	| Factor DIV Term { $$ = $1 / $3;}
	;
Term	: INT
     	;
*/
%%
const char *get_name(int order){
	return yytname[order];
}

