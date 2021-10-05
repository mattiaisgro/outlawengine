#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

using namespace outlaw;
using namespace uroboro;


// Generate Lissajous curves


int main(int argc, char const *argv[]) {

	// Physical system setup

	double x = 0;
	double y = 0;

	double Ax = 0.5;
	double Ay = 0.5;

	double omegax = 1;
	double omegay = 1;

	double phix = 0.5;
	double phiy = -1;

	// Input Omega variables
	std::cout << "OmegaX = ";
	std::cin >> omegax;

	std::cout << "OmegaY = ";
	std::cin >> omegay;

	// Initialize window
	Window::create("Lissajous Curves", 720, 720);
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
	double trace_timeout = 0.0001;
	std::vector<Primitive> trace{};
	std::vector<vec3> trace_pos{};


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


		// Update position
		x = Ax * cos(omegax * curr_time + phix);
		y = Ay * cos(omegay * curr_time + phiy);


		// Create a new trail if <trace_timeout> has passed
		if(curr_time > last_trace_time + trace_timeout) {

			trace.push_back(create_circle(0.005, 16));
			trace_pos.push_back(vec3(x, y, 0));
			last_trace_time = curr_time;
		}

		// Render
		Renderer::clear_screen(vec3(0.99f, 0.99f, 0.99f));

		mat4 model = mat4();
		mat4 MVP = mat4();

		default_shader.setUniform("mesh_color", vec3(0.01f, 0.01f, 0.01f));
		default_shader.setUniform("transform", camera.view);

		// Render X and Y axis
		
		render_primitive(x_axis);
		render_primitive(y_axis);

		// Render trail
		for (int i = 0; i < trace.size(); ++i) {
			MVP = camera.projection * camera.view;
			MVP.translate(trace_pos[i]);
			default_shader.setUniform("transform", MVP);
			render_primitive(trace[i]);
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
