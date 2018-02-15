.PHONY: all clean

HEADER_FILES = ../../networking.hpp ../common/tcp-networking.hpp Makefile
OBJ_FILES = networking.o tcp-networking.o main.o

CXX      = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O3

all: server-stub

networking.o: ../../networking.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) ../../networking.cpp -o networking.o

tcp-networking.o: ../common/tcp-networking.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) ../common/tcp-networking.cpp -o tcp-networking.o

main.o: main.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) -Wno-unused main.cpp -o main.o

server-stub: $(OBJ_FILES) Makefile
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o server-stub

clean:
	rm -f server-stub $(OBJ_FILES) api.pb.h api.pb.cc

