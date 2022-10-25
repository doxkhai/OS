gettime.o: gettime.c gettime.h
	gcc -c gettime.c -lm

pi_multi-thread.o: pi_multi-thread.c gettime.h
	gcc -c pi_multi-thread.c

pi_multi-thread: pi_multi-thread.o gettime.o
	gcc pi_multi-thread.o gettime.o -o pi_multi-thread -lm

pi_serial.o: pi_serial.c gettime.h
	gcc -c pi_serial.c

pi_serial: pi_serial.o gettime.o
	gcc pi_serial.o gettime.o -o pi_serial -lm

# usage: make all points=100000000
all: pi_multi-thread pi_serial
	rm -f runtime.txt
	./pi_multi-thread $(points)
	./pi_serial $(points)

clear: 
	rm -rf *.o

clearall: 
	rm -rf *.o pi_multi-thread pi_serial
	