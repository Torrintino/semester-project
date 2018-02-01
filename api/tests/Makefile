.PHONY: all clean

CXX      = g++
CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -g -Og
CXXFLAGS = -std=c++14 -Wall -Wextra -g -Og

all: gpio-test

gpio-test: gpio_test.cpp Makefile
	$(CXX) $(CXXFLAGS) -Wno-unused-parameter gpio_test.cpp -lwiringPi -o gpio-test

clean:
	rm -f gpio-test

