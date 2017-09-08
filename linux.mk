		
static: bin/lib${LIBNAME}.a
shared: lib/lib${LIBNAME}.so

lib/lib${LIBNAME}.a: ${OBJECTS}
	@echo + Linking... [static]
	@ar rs $@ $^

lib/lib${LIBNAME}.so: ${OBJECTS}
	@echo + Linking... [shared]
	@${CC} -shared $^ -o $@

${OBJECTS}: all

all: ${SOURCES}
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
