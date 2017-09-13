#include "outlaw.h"
#include "loadgl.h"

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("Outlaw Playground");

	Shader default_shader("../res/default.glsl");
	default_shader.bind();

	//INIT CODE





	while(!Window::getShouldClose()) {


		// Update




		// Render
		Renderer::clear_screen();





		// End loop
		Window::swapBuffers();
		Window::pollEvents();
	}

	return 0;
}
