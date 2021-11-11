#include "outlaw.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("Outlaw Test");

	// Load default shader
	Shader default_shader = load_default_shader();

	// Create VAO and set it up

	Primitive circle = create_circle(1, 1000);
	Primitive square = create_square(0.5f);
	Primitive graph = create_graph_from_function(sin, 0, 2 * PI, 500, 0.25f);

	// Create default camera
	BasicCamera camera = BasicCamera();
	float ratio = Window::getAspectRatio();
	float scene_scale = 2.0f;
	camera.ortho(-ratio * scene_scale, ratio * scene_scale, -1 * scene_scale, 1 * scene_scale, 0.1f, 100);

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
		Renderer::clear_screen(vec3(0.999f, 0.999f, 0.999f));

		double t_1 = glfwGetTime();
		while(t_1 > 2 * PI)
			t_1 -= 2 * PI;

		// update_graph_from_function(graph, sin, glfwGetTime(), glfwGetTime() + (2 * PI), 500, 0.25f);
		// graph.model = mat4();
		// graph.model.translate(-t_1 / 4.f, 0, 0);
		mat4 MVP = camera.projection * camera.view;
		default_shader.setUniform("transform", MVP);
		default_shader.setUniform("mesh_color", vec3(10, 10, 10));
		render_primitive(circle);


		// End loop
		Renderer::flush();
		Window::swapBuffers();
		Window::pollEvents();
	}

	Window::destroy();

	std::cin.get();

	return 0;
}
