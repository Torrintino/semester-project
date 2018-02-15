.PHONY: all clean

HEADER_FILES = networking.hpp led-event-status.hpp interface-to-services.hpp gpio-pins.h ../driver/IrRecieveD.h api.proto Makefile
OBJ_FILES = networking.o led-event-status.o interface-to-services.o IrReceiveD.o api.pb.o main.o

CXX      = g++
CC       = gcc
CFLAGS   = -std=c99 -O3 -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-security -Wnull-dereference \
           -Winit-self -Wuninitialized -Wstrict-overflow=5 -Warray-bounds=2 -Wundef -Wshadow \
           -Wframe-larger-than=2048 -Wstack-usage=2048 -Wbad-function-cast -Wcast-qual \
           -Wcast-align -Wconversion
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-security -Wnull-dereference \
           -Winit-self -Wuninitialized -Wstrict-overflow=5 -Warray-bounds=2 -Wshadow \
           -Wframe-larger-than=16384 -Wstack-usage=16384 -Wcast-qual -Wcast-align -Wconversion

# -Wundef deactivated because protoc generates shitty code...

all: hardware-api

api.pb.h: api.proto Makefile
	protoc --proto_path=. --cpp_out=. api.proto

api.pb.o: api.pb.h Makefile
	$(CXX) -c $(CXXFLAGS) -Wno-unused api.pb.cc -o api.pb.o

IrReceiveD.o: ../driver/IrRecieveD.c ../driver/IrRecieveD.h Makefile
	$(CC) -c $(CFLAGS) ../driver/IrRecieveD.c -o IrReceiveD.o

networking.o: networking.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) networking.cpp -o networking.o

led-event-status.o: led-event-status.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) led-event-status.cpp -o led-event-status.o

interface-to-services.o: interface-to-services.cpp $(HEADER_FILES) api.pb.h
	$(CXX) -c $(CXXFLAGS) interface-to-services.cpp -o interface-to-services.o

main.o: main.cpp $(HEADER_FILES)
	$(CXX) -c $(CXXFLAGS) -Wno-unused-parameter main.cpp -o main.o

hardware-api: $(OBJ_FILES) Makefile
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -lprotobuf -llirc_client -lwiringPi -o hardware-api

clean:
	rm -f hardware-api $(OBJ_FILES) api.pb.h api.pb.cc

