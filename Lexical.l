%{
#include "gram_tree.h"
#define YYSTYPE Nod
#include "syntax.tab.h"
#include <stdlib.h>
#include <string.h>

//int yycolumn = 1;
/*#define YY_USER_ACTION \
	do {	yylloc.first_line = yyloc.last_line = yylineno;\
		yylloc.first_column = yycolumn;\
		yylloc.last_column = yycolumn + yyleng - 1;\
		yycolum += yyleng;\
	}while(0);
*/
void setlval(int token_code){
	yylval.if_lex=1;
	yylval.if_null=0;
	yylval.id = token_code;
	yylval.first_line = yylineno;
	if(token_code==1||token_code==7||token_code==8||token_code==9){
		yylval.str = (char *)malloc(1+strlen(yytext));
		strcpy(yylval.str,yytext);
	}
	else	yylval.str=NULL;
}
extern int success;
%}
%%
"int"|"float" { setlval(1);return TYPE;}
"if" { setlval(2);return IF;}
"struct" { setlval(3);return STRUCT;}
"return" { setlval(4);return RETURN;}
"else" { setlval(5);return ELSE;}
"while" { setlval(6);return WHILE;}
([0-9]+)|(0[0-7]+)|(0x[0-9a-f]+)|(0X[0-9A_F]+) { setlval(7);return INT;}
([0-9]+\.[0-9]+) { setlval(8);return FLOAT;}
[_a-zA-Z][_0-9a-zA-Z]* { setlval(9);return ID;}
";" { setlval(10);return SEMI;}
"," { setlval(11);return COMMA;}
"=" { setlval(12);return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" { setlval(13);return RELOP;}
"+" { setlval(14);return PLUS;}
"-" { setlval(15);return MINUS;}
"*" { setlval(16);return STAR;}
"/" { setlval(17);return DIV;}
"&&" { setlval(18);return AND;}
"||" { setlval(19);return OR;}
"." { setlval(20);return DOT;}
"!" { setlval(21);return NOT;}
"(" { setlval(22);return LP;}
")" { setlval(23);return RP;} 
"[" { setlval(24);return LB;}
"]" { setlval(25);return RB;}
"{" { setlval(26);return LC;} 
"}" { setlval(27);return RC;} 
\n  {yylineno++;}
(" "|\t|\v)+ {}
. {success = 0;printf("ERROR type A at Line %d: '%s'\n",yylineno,yytext);}
%%
