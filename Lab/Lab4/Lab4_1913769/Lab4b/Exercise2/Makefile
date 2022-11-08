mem.o: mem.c mem.h
	gcc -c mem.c

main.o: main.c mem.h
	gcc -c main.c

all: main.o mem.o
	gcc main.o mem.o -o main

clear:
	rm -rf *.o