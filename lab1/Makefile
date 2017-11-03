all:
	bison -d -v syntax.y
	flex lexical.l
	gcc -g -w syntax.tab.c Node.c -lfl -o myCompiler
clean:
	rm -f syntax.tab.c syntax.tab.h syntax.output lex.yy.c myCompiler