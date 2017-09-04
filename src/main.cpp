#include "outlaw.h"
#include "help.h"

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create();

	// Load default shader
	Shader s("../res/default.glsl");
	s.bind();

	// Create VAO and set it up
	GPUID VAO = Renderer::create_vao();


	// Prepare buffer of data
	vec3 triangle_vertices[3] = {
		vec3(-1,	-1,		0),
		vec3(0,		1,		0),
		vec3(1,		-1,		0)
	};

	GPUID VBO = Renderer::create_buffer((float*) triangle_vertices,
										sizeof(triangle_vertices));

	Renderer::setup_vao();

	// Game loop (rudimental)
	while(true) {

		if(Input::isPressed(KEY_ESCAPE)) {
			Window::setShouldClose(true);
			break;
		}

		Renderer::clear_screen();

		Renderer::bind_vao(VAO);
		Renderer::draw_buffer(VBO, 3);

		Window::swapBuffers();
		Window::pollEvents();
	}

	return 0;
}
