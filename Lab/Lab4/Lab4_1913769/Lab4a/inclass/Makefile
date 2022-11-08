
sched.o: sched.c structs.h queue.h
	gcc -c sched.c

queue.o: queue.c structs.h queue.h
	gcc -c queue.c

sched: queue.o sched.o input.txt
	gcc queue.o sched.o -o sched
	./sched < input.txt

clear:
	rm -rf *.o