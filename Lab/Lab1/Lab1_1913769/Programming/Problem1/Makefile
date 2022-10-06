factorial.o: factorial.c factorial.h
	gcc -c factorial.c

main.o: main.c factorial.h
	gcc -c main.c

all: main.o factorial.o
	gcc main.o factorial.o -o factorial

clean: 
	rm -f *.o factorial
