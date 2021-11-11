#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

#include "uroboro/interp.h"

using namespace outlaw;
using namespace uroboro;


int main(int argc, char const *argv[]) {

	println("Bezier Animation");
	println("Press Esc to exit");
	println("Press Space to reset view matrix");

	Window::setSamples(8);
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

	real t;
	const int N = 5;
	std::vector<vec3> points;

	for (int i = 0; i < N; ++i) {
		points.push_back(vec3(
			cos(2 * PI / 5.0 * i - (PI / 3.2)) / 1.25,
			sin(2 * PI / 5.0 * i - (PI / 3.2)) / 1.25, 0));
	}


	Primitive point = create_circle(0.015, 64);


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

		default_shader.setUniform("mesh_color", vec3(0.02f, 0.02f, 0.02f));
		default_shader.setUniform("transform", camera.view);


		t = ((long long int) (glfwGetTime() * 2000) % 10000) / 10000.0;


		vec3 P1 = lerp(points[0], points[1], t);
		vec3 P2 = lerp(points[1], points[2], t);
		vec3 P3 = lerp(points[2], points[3], t);
		vec3 P4 = lerp(points[3], points[4], t);

		vec3 A = lerp(P1, P2, t);
		vec3 B = lerp(P2, P3, t);
		vec3 C = lerp(P3, P4, t);

		vec3 D = lerp(A, B, t);
		vec3 E = lerp(B, C, t);

		vec3 P = lerp(D, E, t);

		MVP = camera.projection * camera.view;
		default_shader.setUniform("transform", MVP);
		render_primitive(create_line(D, E));

		MVP = camera.projection * camera.view;
		MVP.translate(P);
		default_shader.setUniform("mesh_color", vec3(0.8f, 0.05f, 0.05f));
		default_shader.setUniform("transform", MVP);
		render_primitive(point);

		default_shader.setUniform("mesh_color", vec3(0.02f, 0.02f, 0.02f));

		for (int i = 3; i < N; ++i) {

			vec3 P1 = lerp(points[i - 3], points[i - 2], t);
			vec3 P2 = lerp(points[i - 2], points[i - 1], t);
			vec3 P3 = lerp(points[i - 1], points[i], t);

			vec3 A = lerp(P1, P2, t);
			vec3 B = lerp(P2, P3, t);

			vec3 P = lerp(A, B, t);

			MVP = camera.projection * camera.view;
			default_shader.setUniform("transform", MVP);
			render_primitive(create_line(A, B));

			MVP = camera.projection * camera.view;
			MVP.translate(P);
			default_shader.setUniform("transform", MVP);
			render_primitive(point);
		}

		for (int i = 2; i < N; ++i) {

			vec3 P1 = lerp(points[i - 2], points[i - 1], t);
			vec3 P2 = lerp(points[i - 1], points[i], t);

			vec3 P = lerp(P1, P2, t);

			MVP = camera.projection * camera.view;
			default_shader.setUniform("transform", MVP);
			render_primitive(create_line(P1, P2));

			MVP = camera.projection * camera.view;
			MVP.translate(P);
			default_shader.setUniform("transform", MVP);
			render_primitive(point);
		}

		for (int i = 1; i < N; ++i) {
			
			MVP = camera.projection * camera.view;
			default_shader.setUniform("transform", MVP);
			render_primitive(create_line(points[i - 1], points[i]));

			vec3 P = lerp(points[i - 1], points[i], t);

			MVP = camera.projection * camera.view;
			MVP.translate(P);
			default_shader.setUniform("transform", MVP);
			render_primitive(point);
		}


		for (int i = 0; i < N; ++i) {
			MVP = camera.projection * camera.view;
			MVP.translate(points[i]);
			default_shader.setUniform("transform", MVP);
			render_primitive(point);
		}

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
