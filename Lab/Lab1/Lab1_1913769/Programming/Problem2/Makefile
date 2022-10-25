readline.o: readline.c readline.h
	gcc -c readline.c

main.o: main.c readline.h
	gcc -c main.c

all: main.o readline.o
	gcc main.o readline.o -o readline

clean:
	rm -f *.o 
