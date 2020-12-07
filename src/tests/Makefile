CC = /usr/bin/gcc
CXX = /usr/bin/g++

CFLAGS = -g -Wall

all: YuvCircularQ.o

CircularQ.o: cqDriver.cpp
	$(CXX) -c cqDriver.cpp

YuvCircularQ.o: YuvDriver.cpp
	$(CXX) -c YuvDriver.cpp

clean:
	rm *.o output