include config.mk

.PHONY: static shared clean test

default_target: all

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

rename:
	@cp lib/win32/glfw3.dll lib/win32/libglfw3.so

clean:
	@echo + Cleaning...
	@rm *.o

test: static shared
	@echo + Compiling tests...
	@${CC} test/main.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/main.exe

glinfo:
	@echo + Compiling GLInfo...
	@${CC} test/glinfo.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/glinfo.exe

doublependulum:
	@echo + Compiling DoublePendulum...
	@${CC} test/doublependulum.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/doublependulum.exe

lissajous:
	@echo + Compiling Lissajous...
	@${CC} test/lissajous.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/lissajous.exe

diffeq:
	@echo + Compiling DifferentialEquations...
	@${CC} test/diffeq.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/diffeq.exe

diffeq_trace:
	@echo + Compiling DifferentialEquationsTrace...
	@${CC} test/diffeq_trace.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/diffeq_trace.exe

bezier:
	@echo + Compiling BezierCurves...
	@${CC} test/bezier.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/bezier.exe

bezier_animation:
	@echo + Compiling BezierAnimation...
	@${CC} test/bezier_animation.cpp ${TESTFLAGS} ${LIBRARIES}  -o test/bezier_animation.exe

copy:
	@cp lib/win32/libglfw3.so test/glfw3.dll
	@cp bin/lib${LIBNAME}.so test/${LIBNAME}.dll

all: rename static shared clean test copy
	@echo Ok.

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... test"
	@echo "... copy"
