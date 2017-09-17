#include "outlaw.h"
#include "loadgl.h"

using namespace GL;
using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("GLInfo");

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	Window::destroy();

	println("OpenGL version: ", major, ".", minor);

	println("Extensions:");
	for (auto i = GL::glextensions.begin(); i != GL::glextensions.end(); ++i) {
		println(i->first);
	}

	return 0;
}
