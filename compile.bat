@echo off

set SOURCES=src/*.cpp
set OBJECTS=*.o
set CXXFLAGS=-std=c++11 -O3 -fPIC -c -Iinclude/ -Llib/win32/
set TESTFLAGS=-std=c++11 -Iinclude/ -Llib/win32/ -Isrc/ -Lbin/ -loutlaw
set LIBRARIES=-lopengl32 -lgdi32 -lglfw3
set LIBNAME=outlaw

echo + Compiling...
g++ %CXXFLAGS% %LIBRARIES% %SOURCES%

echo + Linking... [static]
ar rs bin/lib%LIBNAME%.a %OBJECTS%

echo + Linking... [shared]
g++ -shared %OBJECTS% -Llib/win32/ %LIBRARIES% -o bin/lib%LIBNAME%.so

echo + Cleaning...
rm %OBJECTS%

echo + Compiling tests...
g++ test/main.cpp %TESTFLAGS% %LIBRARIES% -o test/main.exe

echo + Copying files...
cp lib/win32/libglfw3.so test/glfw3.dll
cp bin/lib%LIBNAME%.so test/%LIBNAME%.dll

echo + Ok.

pause
