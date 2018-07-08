parser:
	bison -d syntax.y
	flex Lexical.l
	gcc gram_tree.c sym_tab.c syntax.tab.c main.c -lfl -ly -o parser
.PHONY: run clean
test1: parser
	./parser  ./testcase/test_set_1/test_1_1.c
	./parser  ./testcase/test_set_1/test_1_2.c
	./parser  ./testcase/test_set_1/test_1_3.c
	./parser  ./testcase/test_set_1/test_1_4.c
test2: parser
	./parser  ./testcase/test_set_2/test_2_1.c
	./parser  ./testcase/test_set_2/test_2_2.c
	./parser  ./testcase/test_set_2/test_2_3.c
	./parser  ./testcase/test_set_2/test_2_4.c
	./parser  ./testcase/test_set_2/test_2_5.c
	./parser  ./testcase/test_set_2/test_2_6.c
	./parser  ./testcase/test_set_2/test_2_7.c
	./parser  ./testcase/test_set_2/test_2_8.c
	./parser  ./testcase/test_set_2/test_2_9.c
	./parser  ./testcase/test_set_2/test_2_10.c
	./parser  ./testcase/test_set_2/test_2_11.c
	./parser  ./testcase/test_set_2/test_2_12.c
	./parser  ./testcase/test_set_2/test_2_13.c
	./parser  ./testcase/test_set_2/test_2_14.c
	./parser  ./testcase/test_set_2/test_2_15.c
	./parser  ./testcase/test_set_2/test_2_16.c
	./parser  ./testcase/test_set_2/test_2_17.c
	./parser  ./testcase/test_set_2/test_2_18.c
	./parser  ./testcase/test_set_2/test_2_19.c
clean:
	rm parser lex.yy.c syntax.tab.h syntax.tab.c 
