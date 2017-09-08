LIBNAME=outlaw
SOURCES=src/*.cpp
OBJECTS=*.o
CC=g++
CXXFLAGS="-std=c++11 -O2 -fPIC -Wall -c -Iinclude/ -Llib/linux/"
TESTFLAGS="-std=c++11 -Iinclude/ -Llib/linux/ -Isrc/ -Lbin -loutlaw"
LIBRARIES=-lopengl32 -lgdi32 -lglfw3

echo + Compiling...
$CC $CXXFLAGS $LIBRARIES $SOURCES

echo + Linking... \[static\]
ar rs bin/lib$LIBNAME.a $OBJECTS

echo + Linking... \[shared\]
$CC -shared $OBJECTS -Llib/linux/ $LIBRARIES -o bin/lib$LIBNAME.so

echo + Cleaning...
rm $OBJECTS

echo + Compiling tests...
$CC test/main.cpp $TESTFLAGS $LIBRARIES -o test/main

echo + Copying files...
cp lib/linux/libglfw3.so test/glw3.so
cp bin/lib$LIBNAME.so test/lib$LIBNAME.so

echo + Ok.

