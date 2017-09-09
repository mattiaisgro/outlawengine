include config.mk

static: bin/lib${LIBNAME}.a
shared: bin/lib${LIBNAME}.so

bin/lib${LIBNAME}.a: ${OBJECTS}
	@echo + Linking... [static]
	@ar rs $@ $^

bin/lib${LIBNAME}.so: ${OBJECTS}
	@echo + Linking... [shared]
	@${CC} -shared $^ -o $@

all: ${SOURCES}
	@echo + Compiling...
	@${CC} -c src/*.cpp ${CXXFLAGS}

clean:
	@echo + Cleaning...
	@rm *.o

test: static shared
	@echo + Compiling tests...
	@${CC} test/main.cpp ${TESTFLAGS} ${LIBRARIES} -o test/main.exe

copy:
	@cp lib/win32/libglfw3.so test/glw3.so
	@cp bin/lib${LIBNAME}.so test/lib${LIBNAME}.so

all: static shared clean test copy
	@echo Ok.
