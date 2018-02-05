.PHONY: all clean

HEADER_FILES = ../../networking.hpp ../../api.proto Makefile
OBJ_FILES = networking.o api.pb.o main.o

CXX      = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g -Og

all: led-trigger

api.pb.h: ../../api.proto Makefile
	protoc --proto_path=../../ --cpp_out=. ../../api.proto

api.pb.o: api.pb.h Makefile
	$(CXX) -c $(CXXFLAGS) -Wno-unused api.pb.cc -o api.pb.o

networking.o: ../../networking.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) ../../networking.cpp -o networking.o

main.o: main.cpp $(HEADER_FILES) api.pb.h
	$(CXX) -c $(CXXFLAGS) -Wno-unused main.cpp -o main.o

led-trigger: $(OBJ_FILES) Makefile
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -lprotobuf -o led-trigger

clean:
	rm -f led-trigger $(OBJ_FILES) api.pb.h api.pb.cc

