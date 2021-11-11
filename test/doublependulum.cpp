#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

using namespace outlaw;
using namespace uroboro;


// Simulate a double pendulum system


// Resources:
// https://www.myphysicslab.com/pendulum/double-pendulum-en.html


int main(int argc, char const *argv[]) {

	println("Double Pendulum Simulation");
	println("Press Esc to exit");
	println("Press Space to reset view matrix");
	println("Press L to toggle lines");
	println("Press T to toggle trail");
	println("Press S to toggle spheres");

	Window::create("Double Pendulum Simulation", 720, 720);
	Shader default_shader = Shader("default.glsl");
	default_shader.bind();

	// Create primitives
	Primitive circle1 = create_circle(0.05, 120);
	Primitive circle2 = create_circle(0.05, 120);
	Primitive origin = create_circle(0.01, 64);

	// Initialize camera
	BasicCamera camera = BasicCamera();
	float ratio = Window::getAspectRatio();
	float scene_scale = 1.0;
	camera.ortho(-ratio * scene_scale, ratio * scene_scale,
		-1 * scene_scale, 1 * scene_scale, 0.1f, 100);


	double dt = 0;
	double curr_time = 0;
	double prev_time = glfwGetTime();

	bool trail = true;
	double last_trace_time = curr_time;
	double trace_timeout = 0.001;
	std::vector<Primitive> trace{};
	std::vector<vec3> trace_pos{};

	// Show lines connecting pendulums
	bool lines = true;

	// Show spheres
	bool spheres = true;

	// Physical system setup

	real g = 1; // Gravity

	real theta1 = PI / 1.25; // Theta of the first pendulum
	real theta2 = PI / 1.75; // Theta of the second pendulum

	real L1 = 0.4; // Length of the first pendulum
	real L2 = 0.2; // Length of the second pendulum

	real m1 = 10; // Mass of the first pendulum
	real m2 = 1; // Mass of the second pendulum

	real omega1 = 0; // Omega of the first pendulum
	real omega2 = 0; // Omega of the second pendulum

	// Coordinates of the first pendulum
	real x1 = L1 * sin(theta1);
	real y1 = -L1 * cos(theta1);

	real alfa1 = (-g * (2 * m1 + m2) * sin(theta1) - m2 * g * sin(theta1 - 2 * theta2)
		- 2 * sin(theta1 - theta2) * m2 *(omega2 * omega2 * L2 + omega1 * omega1 * L1 * cos(theta1 - theta2)))
			/ (L1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));

	// Coordinates of the second pendulum
	real x2 = x1 + L2 * sin(theta2);
	real y2 = y2 - L2 * cos(theta2);


	real alfa2 = (2 * sin(theta1 - theta2) * (omega1 * omega1 * L1 * (m1 + m2) + g * (m1 * m2) * cos(theta1) +
			omega2 * omega2 * L2 * m2 * cos(theta1 - theta2)))
			/ (L2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));


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

		// L to toggle lines
		if(Input::isPressed(KEY_L)) {

			lines = lines ? false : true;
		}

		// T to toggle trail
		if(Input::isPressed(KEY_T)) {

			trail = trail ? false : true;
		}

		// S to toggle spheres
		if(Input::isPressed(KEY_S)) {

			spheres = spheres ? false : true;
		}


		// Pendulum Logic

		// Update pendulum alfa
		real alfa1 = (-g * (2 * m1 + m2) * sin(theta1) - m2 * g * sin(theta1 - 2 * theta2)
		- 2 * sin(theta1 - theta2) * m2 *(omega2 * omega2 * L2 + omega1 * omega1 * L1 * cos(theta1 - theta2)))
			/ (L1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));


		real alfa2 = (2 * sin(theta1 - theta2) * (omega1 * omega1 * L1 * (m1 + m2) + g * (m1 * m2) * cos(theta1) +
			omega2 * omega2 * L2 * m2 * cos(theta1 - theta2)))
			/ (L2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));


		// Simple Euler integration
		omega1 += alfa1 * dt;
		omega2 += alfa2 * dt;

		theta1 += omega1 * dt;
		theta2 += omega2 * dt;

		// Update position
		x1 = L1 * sin(theta1);
		y1 = -L1 * cos(theta1);
		x2 = x1 + L2 * sin(theta2);
		y2 = y1 - L2 * cos(theta2);

		// Create a new trail if <trace_timeout> has passed
		if((curr_time > last_trace_time + trace_timeout) && trail) {

			trace.push_back(create_circle(0.005, 16));
			trace_pos.push_back(vec3(x2, y2, 0));
			last_trace_time = curr_time;
		}

		// Render
		Renderer::clear_screen(vec3(0.99f, 0.99f, 0.99f));

		mat4 model = mat4();
		mat4 MVP = mat4();

		default_shader.setUniform("mesh_color", vec3(0.01f, 0.01f, 0.01f));
		default_shader.setUniform("transform", camera.view);

		// Render lines
		if(lines) {
			Primitive line1 = create_line(vec3(0, 0, 0), vec3(x1, y1, 0));
			render_primitive(line1);

			Primitive line2 = create_line(vec3(x1, y1, 0), vec3(x2, y2, 0));
			render_primitive(line2);
		}

		// Render trail
		if(trail) {
			for (int i = 0; i < trace.size(); ++i) {
				MVP = camera.projection * camera.view;
				MVP.translate(trace_pos[i]);
				default_shader.setUniform("transform", MVP);
				render_primitive(trace[i]);

				// TO-DO Line Strip
			}
		}

		// Render origin point
		MVP = camera.projection * camera.view * model;
		default_shader.setUniform("mesh_color", vec3(0.01f, 0.01f, 0.01f));
		default_shader.setUniform("transform", MVP);
		render_primitive(origin);

		// Render spheres
		if(spheres) {
			// Render first pendulum
			MVP = camera.projection * camera.view * model;
			MVP.translate(x1, y1, 0);
			default_shader.setUniform("mesh_color", vec3(50.0 / 255.0, 82.0 / 255.0, 136.0 / 255.0));
			default_shader.setUniform("transform", MVP);
			render_primitive(circle1);

			// Render second pendulum
			MVP = camera.projection * camera.view * model;
			MVP.translate(x2, y2, 0);
			default_shader.setUniform("mesh_color", vec3(182.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0));
			default_shader.setUniform("transform", MVP);
			render_primitive(circle2);
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
