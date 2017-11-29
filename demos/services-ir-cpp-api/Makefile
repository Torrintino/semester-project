.PHONY: all clean test

CXXFLAGS = -std=c++14 -Wall -Wextra -Wno-unused -g -Og

all: libir.so services-program

libir.so: IR.cpp IR.h Makefile
	g++ $(CXXFLAGS) IR.cpp -shared -fpic -lpthread -o libir.so

Services.o: Services.cpp IR.h Makefile
	g++ -c $(CXXFLAGS) Services.cpp  -o Services.o

services-program: Services.o libir.so Makefile
	g++ $(CXXFLAGS) -L. Services.o -lir -o services-program
	
test:
	LD_LIBRARY_PATH=. ./services-program

clean:
	rm -f services-program libir.so Services.o

