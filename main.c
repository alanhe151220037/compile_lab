#include <stdio.h>
#include "lex.yy.c"
#include "sym_tab.h"
extern int success;
int main(int argc, char **argv){
	if(argc<=1){	
		printf("here\n");
		yyparse();
		return 1;
	}
	FILE *f = fopen(argv[1],"r");
	if(!f) {
		perror(argv[1]);
		return 1;		
	}
	yyrestart(f);
	yyparse();
	if(success){
	//	print_tree(root,0);
//		printf("////////////////////////////////\n");
		analy_tree(root);	
//		printf("//////////////\n");
//		print_funlist();
//		print_symt(&root_field,0);
	}
		
	return 0;
}

