CC = /usr/bin/gcc
CXX = /usr/bin/g++

CFLAGS = -g -Wall

all: output

output: CircularQ.o
	$(CXX) CircularQ.o -o output

CircularQ.o: ../libUtil/CircularQ.h
	$(CXX) -c ../libUtil/CircularQ.h

clean:
	rm *.o output