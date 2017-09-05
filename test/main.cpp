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
	vec3 vertices[] = {
		vec3(-0.5f, 0.5f, 0),
		vec3(-0.5f, -0.5f, 0),
		vec3(0.5f, 0.5f, 0),
		vec3(0.5f, -0.5f, 0)
	};

	GPUID VBO = Renderer::create_buffer((float*) vertices, sizeof(vertices));

	VAOAttrib attributes[] = {
		VAOAttrib() // Default attribute for vertex data
	};

	Renderer::setup_vao(attributes, sizeof(attributes) / sizeof(VAOAttrib));

	// Game loop (rudimental)
	while(!Window::getShouldClose()) {


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
		Renderer::draw_buffer(sizeof(vertices) / sizeof(vec3), GLPRIMITIVE::TRIANGLE_STRIP);


		// End loop
		Window::swapBuffers();
		Window::pollEvents();
	}

	return 0;
}
