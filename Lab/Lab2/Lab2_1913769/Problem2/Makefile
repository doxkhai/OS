incircle.o: incircle.c incircle.h
	gcc -c incircle.c -lm

pi_multi-thread.o: pi_multi-thread.c incircle.h
	gcc -c pi_multi-thread.c

pi_multi-thread: pi_multi-thread.o incircle.o
	gcc pi_multi-thread.o incircle.o -o pi_multi-thread -lm

pi_serial.o: pi_serial.c incircle.h
	gcc -c pi_serial.c

pi_serial: pi_serial.o incircle.o
	gcc pi_serial.o incircle.o -o pi_serial -lm

all: pi_multi-thread pi_serial
	./pi_multi-thread $(points)
	./pi_serial $(points)

clear: 
	rm -rf *.o
