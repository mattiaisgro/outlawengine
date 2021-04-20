# Outlaw Rendering Engine

A C++ rendering engine using **OpenGL** with support for window and input handling

### How to compile
* First of all you need GLFW binaries, you can download pre-compiled binaries or compile them by yourself.
All instructions on how to compile GLFW are [here](http://glfw.org)
* When you have the binaries, place them in `outlawengine/lib/win32` or `outlawengine/lib/linux` folder in the root of the repo depending on your system
* Run the Makefile using the command `make -f linux.mk` or `make -f windows.mk`

### Dependencies
* Uroboro _(included)_
* GLFW 3 _(only headers included)_
* loadgl _(included)_
