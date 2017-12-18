.PHONY: all clean

HEADER_FILES = ../../networking.hpp ../common/tcp-networking.hpp ../../api.proto Makefile
OBJ_FILES = networking.o tcp-networking.o api.pb.o main.o

CXX      = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g -Og

all: client-stub

api.pb.h: ../../api.proto Makefile
	protoc --proto_path=../../ --cpp_out=. ../../api.proto

api.pb.o: api.pb.h Makefile
	$(CXX) -c $(CXXFLAGS) -Wno-unused api.pb.cc -o api.pb.o

networking.o: ../../networking.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) ../../networking.cpp -o networking.o

tcp-networking.o: ../common/tcp-networking.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) ../common/tcp-networking.cpp -o tcp-networking.o

main.o: main.cpp $(HEADER_FILES) api.pb.h
	$(CXX) -c $(CXXFLAGS) -Wno-unused main.cpp -o main.o

client-stub: $(OBJ_FILES) Makefile
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -lprotobuf -o client-stub

clean:
	rm -f client-stub $(OBJ_FILES) api.pb.h api.pb.cc

