#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include "GLFW/glfw3.h"

// numeric_limits
#include <limits>

using namespace outlaw;
using namespace uroboro;



double f(double x, double y) {

	return y;
}


double g(double x, double y) {

	return -x;
}


// Function to color-code the vector field
// lambda = |v_i - v_min| / (v_max - v_min) (relative intensity)
vec3 color(double lambda) {
	return vec3(lambda, 2.5 * lambda * (1 - lambda), 1 - lambda);
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
	double trace_timeout = 0.0001;

	// Initialize vectors
	double offset = 1;

	double w = 2;
	double h = 2;

	double scale = 20;

	const size_t lines_w = scale * 1.2;
	const size_t lines_h = scale * 1.2;

	vec2 vectors[lines_w][lines_h];
	vec3 vec_colors[lines_w][lines_h];
	Primitive lines[lines_w][lines_h];

	real v_max = 0;
	real v_min = std::numeric_limits<real>::max();


	for (int i = 0; i < lines_w; ++i) {
		for (int j = 0; j < lines_h; ++j) {
			
			double x = w / (double) lines_w * i - offset;
			double y = h / (double) lines_h * j - offset;

			vectors[i][j] = vec2(f(x, y), g(x, y));
		}
	}


	// Calculate v_max and v_min
	for (int i = 0; i < lines_w; ++i) {
		for (int j = 0; j < lines_h; ++j) {
			v_max = max(v_max, vectors[i][j].magnitude());
			v_min = min(v_min, vectors[i][j].magnitude());
		}
	}


	// Calculate colors
	for (int i = 0; i < lines_w; ++i) {
		for (int j = 0; j < lines_h; ++j) {
			vec_colors[i][j] = color((vectors[i][j].magnitude() - v_min) / (v_max - v_min));
		}
	}


	// Create primitives
	for (int i = 0; i < lines_w; ++i) {
		for (int j = 0; j < lines_h; ++j) {
		
			lines[i][j] = create_line(vec3(0, 0, 0),
							vec3(f(vectors[i][j].x, vectors[i][j].y),
								g(vectors[i][j].x, vectors[i][j].y), 0).normalized() / scale);

			mat4 model = mat4();
			model.translate(w / (double) lines_w * i - offset,
							h / (double) lines_h * j - offset,
							0);

			lines[i][j].model = model;
		}
	}

	// Draw fuller lines
	Renderer::set_line_width(5);


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

		default_shader.setUniform("mesh_color", vec3(0.01f, 0.01f, 0.01f));
		default_shader.setUniform("transform", camera.view);

		// Render X and Y axis
		
		render_primitive(x_axis);
		render_primitive(y_axis);

		// Render vectors		
		for (int i = 0; i < lines_w; ++i) {
			for (int j = 0; j < lines_h; ++j) {

				default_shader.setUniform("transform", camera.view * lines[i][j].model);
				default_shader.setUniform("mesh_color", vec_colors[i][j]);
				render_primitive(lines[i][j]);
			}
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
