include config.mk

LIBRARIES = -L./lib/win32/ -lopengl32 -lgdi32 -lglfw3

static: bin/lib${LIBNAME}.a
shared: bin/lib${LIBNAME}.so

bin/lib${LIBNAME}.a: ${OBJECTS}
	@echo + Linking... [static]
	@ar rs $@ $^

bin/lib${LIBNAME}.so: ${OBJECTS}
	@echo + Linking... [shared]
	@${CC} -shared $^ ${LIBRARIES} -o $@

${OBJECTS}: ${SOURCES}
	@echo + Compiling...
	@${CC} -c src/*.cpp ${CXXFLAGS}

clean:
	@echo + Cleaning...
	@rm *.o

test: static shared
	@echo + Compiling tests...
	@${CC} test/main.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/main.exe

copy:
	@cp lib/win32/libglfw3.so test/glfw3.dll
	@cp bin/lib${LIBNAME}.so test/${LIBNAME}.dll

all: static shared clean test copy
	@echo Ok.
