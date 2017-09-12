#include "outlaw.h"

#include "uroboro/utility.h"
using namespace uroboro;

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("Outlaw Test");
	printlog("Opened window");

	// Load default shader
	Shader default_shader("../res/default.glsl");
	default_shader.bind();
	printlog("Loaded default shader");

	// Create VAO and set it up
	GPUID VAO = Renderer::create_vao();
	printlog("Created default VAO");


	// Prepare buffer of data
	vec3 vertices[] = {
		vec3(-0.5f, 0.5f, 0),
		vec3(-0.5f, -0.5f, 0),
		vec3(0.5f, 0.5f, 0),
		vec3(0.5f, -0.5f, 0)
	};

	GPUID VBO = Renderer::create_buffer((float*) vertices, sizeof(vertices));
	printlog("Created VBO");

	VAOAttrib attributes[] = {
		VAOAttrib() // Default attribute for vertex data
	};

	Renderer::setup_vao(attributes, sizeof(attributes) / sizeof(VAOAttrib));

	// Create default camera
	Camera camera = Camera();
	camera.perspective(90, Window::getAspectRatio(), 0.1f, 100);
	printlog("Created camera");

	printlog("Entering game loop");
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


		// Render
		Renderer::clear_screen();

		mat4 model_m = mat4();

		mat4 MVP = camera.projection * camera.view * model_m;

		default_shader.setUniform("transform", MVP);
		Renderer::bind_vao(VAO);
		Renderer::draw_buffer(sizeof(vertices) / sizeof(vec3), GLPRIMITIVE::TRIANGLE_STRIP);


		// End loop
		Window::swapBuffers();
		Window::pollEvents();
	}

	return 0;
}
