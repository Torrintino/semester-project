.PHONY: all clean

HEADER_FILES = ../../networking.hpp ../common/tcp-networking.hpp ../../api.proto Makefile
OBJ_FILES = networking.o tcp-networking.o api.pb.o main.o

CXX      = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-security -Wnull-dereference \
           -Winit-self -Wuninitialized -Wstrict-overflow=5 -Warray-bounds=2 -Wshadow \
           -Wframe-larger-than=16384 -Wstack-usage=16384 -Wcast-qual -Wcast-align -Wconversion

# -Wundef deactivated because protoc generates shitty code...

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

