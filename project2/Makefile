all:
	bison -d -v -t syntax.y
	flex lexical.l
	gcc -g syntax.tab.c Node.c newHashTable.c semantic.c -lfl -o myCompiler
clean:
	rm -f syntax.tab.c syntax.tab.h syntax.output lex.yy.c myCompiler
test:
	./myCompiler pretest/1.txt > log/log1.txt
	./myCompiler pretest/2.txt > log/log2.txt
	./myCompiler pretest/3.txt > log/log3.txt
	./myCompiler pretest/4.txt > log/log4.txt
	./myCompiler pretest/5.txt > log/log5.txt
	./myCompiler pretest/6.txt > log/log6.txt
	./myCompiler pretest/7.txt > log/log7.txt
	./myCompiler pretest/8.txt > log/log8.txt
	./myCompiler pretest/9.txt > log/log9.txt
	./myCompiler pretest/10.txt > log/log10.txt
testClean:
	rm -rf log/*

