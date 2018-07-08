%{
#include <stdio.h>
#include "gram_tree.h"
#define YYSTYPE Nod *
#define YYDEBUG 1
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
Program	: ExtDeflist {$$=create_gnod(1,1); insert_1($$,$1); root=$$;}
	;
ExtDeflist	:	{$$=create_gnod(2,1);Nod *p=create_gnod(0,0);insert_1($$,p);}
	| ExtDef ExtDeflist {$$=create_gnod(2,2); insert_2($$,$1,$2);}
	;
ExtDef	: Specifier ExtDecList SEMI {$$=create_gnod(3,1); insert_3($$,$1,$2,$3);}
       	| Specifier SEMI {$$=create_gnod(3,2); insert_2($$,$1,$2);}
	| Specifier FunDec CompSt {$$=create_gnod(3,3); insert_3($$,$1,$2,$3);}
	| error SEMI {$$=create_gnod(3,4);$1=create_gnod(-1,0);insert_2($$,$1,$2);}
	;
ExtDecList	: VarDec {$$=create_gnod(4,1); insert_1($$,$1);}
	| VarDec COMMA ExtDecList {$$=create_gnod(4,2); insert_3($$,$1,$2,$3);}
	;
/*Specifiers*/
Specifier	: TYPE {$$=create_gnod(5,1); insert_1($$,$1);}
	| StructSpecifier {$$=create_gnod(5,2); insert_1($$,$1);}
	;
StructSpecifier	: STRUCT OptTag LC DefList RC {$$=create_gnod(6,1); insert_5($$,$1,$2,$3,$4,$5);}
	| STRUCT Tag {$$=create_gnod(6,2); insert_2($$,$1,$2);}
	;
OptTag	:{$$=create_gnod(7,1);Nod *p=create_gnod(0,0);insert_1($$,p);}
	| ID {$$=create_gnod(7,2); insert_1($$,$1);}
	;
Tag	: ID {$$=create_gnod(8,1); insert_1($$,$1);}
    	;
/*Declarators*/
VarDec	: ID {$$=create_gnod(9,1); insert_1($$,$1);}
	| VarDec LB INT RB {$$=create_gnod(9,2); insert_4($$,$1,$2,$3,$4);}
	;
FunDec	: ID LP VarList RP {$$=create_gnod(10,1); insert_4($$,$1,$2,$3,$4);}
	| ID LP RP {$$=create_gnod(10,2); insert_3($$,$1,$2,$3);}
	;
VarList	: ParamDec COMMA VarList {$$=create_gnod(11,1); insert_3($$,$1,$2,$3);}
	| ParamDec {$$=create_gnod(11,2); insert_1($$,$1);}
	;
ParamDec: Specifier VarDec {$$=create_gnod(12,1); insert_2($$,$1,$2);}
	;
/*Statements*/
CompSt	: LC DefList StmtList RC {$$=create_gnod(13,1); insert_4($$,$1,$2,$3,$4);}
	| error RC {$$=create_gnod(13,2);$1=create_gnod(-1,0);insert_2($$,$1,$2);}
	;
StmtList: {$$=create_gnod(14,1);Nod *p=create_gnod(0,0);insert_1($$,p);}
	| Stmt StmtList {$$=create_gnod(14,2); insert_2($$,$1,$2);}
	;
Stmt	: Exp SEMI {$$=create_gnod(15,1); insert_2($$,$1,$2);}
     	| CompSt {$$=create_gnod(15,2); insert_1($$,$1);}
	| RETURN Exp SEMI{$$=create_gnod(15,3); insert_3($$,$1,$2,$3);}
	| IF LP Exp RP Stmt ELSE Stmt {$$=create_gnod(15,4); insert_7($$,$1,$2,$3,$4,$5,$6,$7);}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=create_gnod(15,5); insert_5($$,$1,$2,$3,$4,$5);}
	| WHILE LP Exp RP Stmt {$$=create_gnod(15,6); insert_5($$,$1,$2,$3,$4,$5);}
	| error SEMI {$$=create_gnod(15,7);$1=create_gnod(-1,0);insert_2($$,$1,$2);}
	;
/*Local Definitions*/
DefList : {$$=create_gnod(16,1);Nod *p=create_gnod(0,0);insert_1($$,p);}
	| Def DefList {$$=create_gnod(16,2); insert_2($$,$1,$2);}
	;
Def	: Specifier DecList SEMI {$$=create_gnod(17,1); insert_3($$,$1,$2,$3);}
    	| error SEMI {$$=create_gnod(17,2);$1=create_gnod(-1,0);insert_2($$,$1,$2);}
	;
DecList	: Dec {$$=create_gnod(18,1); insert_1($$,$1);}
    	| Dec COMMA DecList {$$=create_gnod(18,2); insert_3($$,$1,$2,$3);}
	;
Dec	: VarDec {$$=create_gnod(19,1); insert_1($$,$1);}
    	| VarDec ASSIGNOP Exp {$$=create_gnod(19,2); insert_3($$,$1,$2,$3);}
	
	;
Exp	: Exp ASSIGNOP Exp {$$=create_gnod(20,1); insert_3($$,$1,$2,$3);}
    	| Exp AND Exp {$$=create_gnod(20,2); insert_3($$,$1,$2,$3);}
	| Exp RELOP Exp {$$=create_gnod(20,3); insert_3($$,$1,$2,$3);}
	| Exp PLUS Exp {$$=create_gnod(20,4); insert_3($$,$1,$2,$3);}
	| Exp MINUS Exp {$$=create_gnod(20,5); insert_3($$,$1,$2,$3);}
	| Exp STAR Exp {$$=create_gnod(20,6); insert_3($$,$1,$2,$3);}
	| Exp DIV Exp {$$=create_gnod(20,7); insert_3($$,$1,$2,$3);}
	| LP Exp RP {$$=create_gnod(20,8); insert_3($$,$1,$2,$3);}
	| MINUS Exp {$$=create_gnod(20,9); insert_2($$,$1,$2);}
	| NOT Exp {$$=create_gnod(20,10); insert_2($$,$1,$2);}
	| ID LP Args RP {$$=create_gnod(20,11); insert_4($$,$1,$2,$3,$4);}
	| ID LP RP {$$=create_gnod(20,12); insert_3($$,$1,$2,$3);}
	| Exp LB Exp RB {$$=create_gnod(20,13); insert_4($$,$1,$2,$3,$4);}
	| Exp DOT ID {$$=create_gnod(20,14); insert_3($$,$1,$2,$3);}
	| ID {$$=create_gnod(20,15); insert_1($$,$1);}
	| INT {$$=create_gnod(20,16); insert_1($$,$1);}
	| FLOAT {$$=create_gnod(20,17); insert_1($$,$1);}
    	| Exp OR Exp {$$=create_gnod(20,18); insert_3($$,$1,$2,$3);}
	;
Args	: Exp COMMA Args {$$=create_gnod(21,1); insert_3($$,$1,$2,$3);}
     	| Exp {$$=create_gnod(21,2); insert_1($$,$1);}
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

