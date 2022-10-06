factorial.o: factorial.c factorial.h
	gcc -c factorial.c

readline.o: readline.c readline.h
	gcc -c readline.c

main.o: main.c factorial.h readline.h
	gcc -c main.c

all: main.o readline.o factorial.o
	gcc main.o readline.o factorial.o -o myfactorial

clean:
	rm -f *.o myfactorial
