parser:
	bison -d syntax.y
	flex Lexical.l
	gcc gram_tree.c syntax.tab.c main.c -lfl -ly -o parser
.PHONY: run clean
run: parser
	./parser test1.c
	./parser test2.c
	./parser test3.c
	./parser test4.c
clean:
	rm parser lex.yy.c syntax.tab.h syntax.tab.c 
