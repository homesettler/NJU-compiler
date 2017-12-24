all:
	bison -d -v -t syntax.y
	flex lexical.l
	gcc -w -g syntax.tab.c Node.c newHashTable.c semantic.c interCode.c -lfl -o myCompiler
clean:
	rm -f syntax.tab.c syntax.tab.h syntax.output lex.yy.c myCompiler
test:
	./myCompiler 1.txt
	./myCompiler 2.txt 
	./myCompiler 3.txt
	./myCompiler 4.txt
	./myCompiler 5.txt 
	./myCompiler 6.txt 
	./myCompiler 7.txt
	./myCompiler 8.txt 
	./myCompiler 9.txt 
	./myCompiler 10.txt
	./myCompiler 11.txt
	./myCompiler 12.txt
	./myCompiler 13.txt
	./myCompiler 14.txt
	./myCompiler 15.txt
	./myCompiler 16.txt
	./myCompiler 17.txt
testClean:
	rm -rf log/*

