#include "outlaw.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

#include "uroboro/utility.h"
using namespace uroboro;

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("Outlaw Test");

	// Load default shader
	Shader default_shader = load_default_shader();
	// Create VAO and set it up

	Primitive square = create_square(0.5f);

	// Create default camera
	BasicCamera camera = BasicCamera();
	camera.perspective(90, Window::getAspectRatio(), 0.1f, 100);

	// Game loop (rudimental)
	while(!Window::getShouldClose()) {


		// Update
		if(Input::isPressed(KEY_ESCAPE)) {
			Window::setShouldClose(true);
			break;
		}
		if(Input::isPressed(KEY_W)) {

			camera.view.translate(vec3(0, 0, 0.01f));
		}
		if(Input::isPressed(KEY_S)) {

			camera.view.translate(vec3(0, 0, -0.01f));
		}
		if(Input::isPressed(KEY_A)) {

			camera.view.translate(vec3(0.01f, 0, 0));
		}
		if(Input::isPressed(KEY_D)) {

			camera.view.translate(vec3(-0.01f, 0, 0));
		}
		if(Input::isPressed(KEY_Z)) {

			camera.view.translate(vec3(0, -0.01f, 0));
		}
		if(Input::isPressed(KEY_X)) {

			camera.view.translate(vec3(0, 0.01f, 0));
		}
		if(Input::isPressed(KEY_SPACE)) {

			camera.view.identity();
		}


		// Render
		Renderer::clear_screen();

		square.model = mat4();
		square.model.translate(sin(glfwGetTime() * (10 / (2.f * PI))) / 2.f, cos(glfwGetTime() * (10 / (2.f * PI))) / 2.f, 0);

		mat4 MVP = camera.projection * camera.view * square.model;
		default_shader.setUniform("transform", MVP);

		render_primitive(square);

		// End loop
		Renderer::flush();
		Window::swapBuffers();
		Window::pollEvents();
	}

	return 0;
}
