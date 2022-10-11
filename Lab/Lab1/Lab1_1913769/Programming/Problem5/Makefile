rating.o: rating.cpp rating.h
	g++ -c rating.cpp

main.o: rating.h main.cpp
	g++ -c main.cpp

all: main.o rating.o
	g++ main.o rating.o -o main

clear: 
	rm -f *.o main 

clear_txt:
	rm -f finaloutput.txt resultFile1.txt resultFile2.txt