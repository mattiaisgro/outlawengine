LIBNAME = outlaw
CC = g++
CXXFLAGS = -Iinclude/ -Wall -std=c++11 -O2 -fPIC
TESTFLAGS = -std=c++11 -Iinclude/ -Isrc/ -Lbin/ -loutlaw
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp,%.o, $(SOURCES))
