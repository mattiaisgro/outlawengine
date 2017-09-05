#include "outlaw.h"

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create();

	// Load default shader
	Shader default_shader("../res/default.glsl");
	default_shader.bind();

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

	VAOAttrib attributes[] = {
		VAOAttrib()
	};

	Renderer::setup_vao(attributes, sizeof(attributes) / sizeof(VAOAttrib));

	// Game loop (rudimental)
	while(true) {


		// Update
		if(Input::isPressed(KEY_ESCAPE)) {
			Window::setShouldClose(true);
			break;
		}


		// Render
		Renderer::clear_screen();


		mat4 MVP = mat4(1.f);
		default_shader.setUniform("transform", MVP);
		Renderer::bind_vao(VAO);
		Renderer::draw_buffer(VBO, 3);


		// End loop
		Window::swapBuffers();
		Window::pollEvents();
	}

	return 0;
}
