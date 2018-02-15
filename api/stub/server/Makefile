.PHONY: all clean

HEADER_FILES = ../../networking.hpp ../common/tcp-networking.hpp Makefile
OBJ_FILES = networking.o tcp-networking.o main.o

CXX      = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-security -Wnull-dereference \
           -Winit-self -Wuninitialized -Wstrict-overflow=5 -Warray-bounds=2 -Wundef -Wshadow \
           -Wframe-larger-than=16384 -Wstack-usage=16384 -Wcast-qual -Wcast-align -Wconversion

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

