#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

#include "uroboro/interp.h"

using namespace outlaw;
using namespace uroboro;


inline Primitive create_cubic_bezier(vec3 P0, vec3 P1, vec3 P2, vec3 P3, unsigned int steps = 100) {

	std::vector<vec3> vertices;
	vec3 prev = P0;
	vec3 curr;

	for (int i = 0; i < steps; ++i) {
		vertices.push_back(prev);
		curr = cubic_bezier(P0, P1, P2, P3, i / (real) steps);
		vertices.push_back(curr);
		prev = curr;
	}

	return create_primitive(&vertices[0], vertices.size() * 3 * 4,
		GLPRIMITIVE::LINES, GLBUFFUSAGE::DYNAMIC);
}


inline Primitive create_quadratic_bezier(vec3 P0, vec3 P1, vec3 P2, unsigned int steps = 100) {

	std::vector<vec3> vertices;
	vec3 prev = P0;
	vec3 curr;

	for (int i = 0; i < steps; ++i) {
		vertices.push_back(prev);
		curr = quadratic_bezier(P0, P1, P2, i / (real) steps);
		vertices.push_back(curr);
		prev = curr;
	}

	return create_primitive(&vertices[0], vertices.size() * 3 * 4,
		GLPRIMITIVE::LINES, GLBUFFUSAGE::DYNAMIC);
}


int main(int argc, char const *argv[]) {

	println("Bezier Curves");
	println("Press Esc to exit");
	println("Press Space to reset view matrix");
	println("Press L to toggle lines");
	println("Press T to toggle trail");
	println("Press S to toggle spheres");

	Window::create("Bezier Curves", 720, 720);
	Shader default_shader = Shader("default.glsl");
	default_shader.bind();

	// Initialize camera
	BasicCamera camera = BasicCamera();
	float ratio = Window::getAspectRatio();
	float scene_scale = 1.0;
	camera.ortho(-ratio * scene_scale, ratio * scene_scale,
		-1 * scene_scale, 1 * scene_scale, 0.1f, 100);


	double dt = 0;
	double curr_time = 0;
	double prev_time = glfwGetTime();

	vec3 P0 = vec3(-0.8, -0.8, 0);
	vec3 P1 = vec3(0.35, 0.75, 0);
	vec3 P2 = vec3(0.9, 0.55, 0);
	vec3 P3 = vec3(0.8, -0.8, 0);

	Primitive bezier1 = create_cubic_bezier(P0, P1, P2, P3);

	while(!Window::getShouldClose()) {


		// Track the passing of time between frames
		double curr_time = glfwGetTime();
		dt = curr_time - prev_time;


		// Input handling

		// Escape to exit
		if(Input::isPressed(KEY_ESCAPE)) {
			Window::setShouldClose(true);
			break;
		}

		// Space to reset view matrix
		if(Input::isPressed(KEY_SPACE)) {

			camera.view.identity();
		}

		// Render
		Renderer::clear_screen(vec3(0.99f, 0.99f, 0.99f));

		mat4 model = mat4();
		mat4 MVP = mat4();

		default_shader.setUniform("mesh_color", vec3(0.01f, 0.01f, 0.01f));
		default_shader.setUniform("transform", camera.view);

		MVP = camera.projection * camera.view;
		default_shader.setUniform("transform", MVP);
		render_primitive(bezier1);

		// End loop
		Renderer::flush();
		Window::swapBuffers();
		Window::pollEvents();

		prev_time = curr_time;
	}

	Window::destroy();

	std::cin.get();

	return 0;
}
