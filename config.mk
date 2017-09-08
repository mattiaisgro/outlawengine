LIBNAME = outlaw
CC = g++
CXXFLAGS = -Iinclude/ -Wall -std=c++11 -O2 -fPIC
TESTFLAGS = ${CXXFLAGS} -Isrc/ -L.
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp,%.o, $(SOURCES))

