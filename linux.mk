include config.mk

default_target: all

static: bin/lib${LIBNAME}.a
shared: bin/lib${LIBNAME}.so

bin/lib${LIBNAME}.a: ${OBJECTS}
	@echo + Linking... [static]
	@ar rs $@ $^

bin/lib${LIBNAME}.so: ${OBJECTS}
	@echo + Linking... [shared]
	@${CC} -shared $^ -o $@

${OBJECTS}: ${SOURCES}
	@echo + Compiling...
	@${CC} -c src/*.cpp ${CXXFLAGS}

clean:
	@echo + Cleaning...
	@rm *.o

test: static shared
	@echo + Compiling tests...
	@${CC} test/main.cpp ${TESTFLAGS} ${LIBRARIES} -o test/main

copy:
	@cp lib/linux/libglfw3.so test/glw3.so
	@cp bin/lib${LIBNAME}.so test/lib${LIBNAME}.so

all: static shared clean test copy
	@echo Ok.

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... test"
	@echo "... copy"

