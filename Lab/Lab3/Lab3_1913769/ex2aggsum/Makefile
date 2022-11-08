main.o: main.c utils.h
	gcc -c main.c 

utils.o: utils.c utils.h
	gcc -c utils.c

all: main.o utils.o
	gcc main.o utils.o -o aggsum
	./aggsum 1000 10   

clear:
	rm -rf *.o 