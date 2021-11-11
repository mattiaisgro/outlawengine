#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include "GLFW/glfw3.h"

// numeric_limits
#include <limits>

using namespace outlaw;
using namespace uroboro;



double f(double x, double y) {

	return 5 * x;
}


double g(double t) {

	return 1;
}



// Render differential equations


int main(int argc, char const *argv[]) {

	// Initialize window
	Window::create("Differential Equations", 720, 720);
	Shader default_shader = Shader("default.glsl");
	default_shader.bind();

	// Create primitives
	Primitive x_axis = create_line(vec3(-1, 0, 0), vec3(1, 0, 0));
	Primitive y_axis = create_line(vec3(0, -1, 0), vec3(0, 1, 0));

	// Initialize camera
	BasicCamera camera = BasicCamera();
	float ratio = Window::getAspectRatio();
	float scene_scale = 1.0;
	camera.ortho(-ratio * scene_scale, ratio * scene_scale,
		-1 * scene_scale, 1 * scene_scale, 0.1f, 100);

	double dt = 0;
	double curr_time = 0;
	double prev_time = glfwGetTime();

	double last_trace_time = curr_time;
	double trace_timeout = 0.00001;

	// Initialize vectors
	double offset = 1;

	double w = 2;
	double h = 2;

	double scale = 20;

	const size_t lines_w = 10;
	const size_t lines_h = 10;

	vec2 particles[lines_h * lines_w];
	std::vector<vec2> trace_pos;
	Primitive trace_point = create_circle(0.005, 16);


	int index = 0;
	for (int i = 0; i < lines_w; ++i) {
		for (int j = 0; j < lines_h; ++j) {
			
			double x =  ((w / lines_w) * (i + 1) - offset) * scale;
			double y = ((h / lines_h) * (j + 1) - offset) * scale;

			particles[index] = vec2(x, y);
			trace_pos.push_back(vec2(x, y));

			index++;
		}
	}


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



		// Update particles' position
		if(dt < 0.1) {
			for (int i = 0; i < lines_h * lines_w; ++i) {

				double a = f(particles[i].x, particles[i].y) * g(curr_time);
				particles[i].y += a * dt;
				particles[i].x += particles[i].y * dt;
			}
		}

		// Generate new trace points
		if(curr_time > last_trace_time + trace_timeout) {
			for (int i = 0; i < lines_h * lines_w; ++i) {
				trace_pos.push_back(vec2(particles[i].x, particles[i].y));
			}

			last_trace_time = curr_time;
		}

		// Render
		Renderer::clear_screen(vec3(0.99f, 0.99f, 0.99f));

		default_shader.setUniform("mesh_color", vec3(0.01f, 0.01f, 0.01f));
		default_shader.setUniform("transform", camera.view);

		// Render X and Y axis
		
		render_primitive(x_axis);
		render_primitive(y_axis);

		// Render trail
		for (int i = 0; i < trace_pos.size(); ++i) {

			mat4 MVP = mat4();
			MVP.translate(vec3(trace_pos[i].x / scale, trace_pos[i].y / scale, 0));

			default_shader.setUniform("transform", MVP);
			render_primitive(trace_point);
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
