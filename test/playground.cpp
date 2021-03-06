#include "outlaw.h"
#include "loadgl.h"

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("Outlaw Playground");

	Shader default_shader("../res/default.glsl");
	default_shader.bind();

	//Init





	while(!Window::getShouldClose()) {


		// Update




		// Render
		Renderer::clear_screen();





		// End loop
		Renderer::flush();
		Window::swapBuffers();
		Window::pollEvents();
	}

	Window::destroy();

	std::cin.get();

	return 0;
}
