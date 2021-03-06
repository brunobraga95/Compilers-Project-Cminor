
cminor: main.o expr.o scanner.o parser.tab.o stmt.o param_list.o type.o decl.o symbol.o hash_table.o scope.o register.o library.o
	gcc main.o expr.o scanner.o parser.tab.o stmt.o param_list.o type.o decl.o symbol.o hash_table.o scope.o register.o -o cminor -lm -g

# This pattern indicates that any .o file depends
# upon the .c file of the same name, and all of the .h files.
# So, if a .o file is needed, it is built automatically.

%.o: %.c *.h 
	gcc -Wall -c $< -o $@ -g

# Only the files generated by flex and bison need explicit rules.

scanner.c: scanner.flex parser.tab.h
	  flex -oscanner.c scanner.flex

parser.tab.c parser.tab.h: grammar.bison
	yacc -d -bparser -v grammar.bison

# clean causes all intermediate files to be deleted.

clean:
	rm -f parser.tab.* parser.output scanner.c *.o cminor cminor.c 

