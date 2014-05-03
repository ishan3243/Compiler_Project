all: hello
	gcc driver.c parser.o stack.o set.o hash.o hash2.o lexer.o ast.o typeChecker.o symbolTable.o -o myCompiler

hello: driver.o parser.o stack.o set.o hash.o hash2.o lexer.o ast.o symbolTable.o typeChecker.o

main.o: driver.c
	gcc -c driver.c
	
parser.o: parser.c
	gcc -c parser.c
	
stack.o: stack.c
	gcc -c stack.c
	
set.o: set.c
	gcc -c set.c
	
hash.o: hash.c
	gcc -c hash.c
	
hash2.o: hash2.c
	gcc -c hash2.c
	
lexernew.o: lexer.c
	gcc -c lexer.c
	
ast.o: ast.c
	gcc -c ast.c
	
symbolTable.o: symbolTable.c
	gcc -c symbolTable.c	

typeChecker.o: typeChecker.c
	gcc -c typeChecker.c
clean:
	rm -rf *o hello
