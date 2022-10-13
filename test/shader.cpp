#include "outlaw.h"
#include "help.h"

#include "../include/GLFW/glfw3.h"

using namespace outlaw;


int main(int argc, char const *argv[]) {

	unsigned int height = 1024;
	unsigned int width = 1680;

	Window::create("Shader Test", width, height);
	// Window::setSamples(16);

	Shader shader("../res/test.glsl");
	shader.bind();

	//Init

	double dt = 0;
	double curr_time = 0;
	double prev_time = glfwGetTime();

	double shader_load_timeout = 1;
	double last_shader_load = prev_time;

	Primitive square = create_rectangle(2, 2);
	mat4 transform = mat4();
	vec2 scene_scale = vec2(1, 1);
	vec2 iMouse = vec2(0, 0);
	vec3 translation = vec3(0, 0, 0);

	while(!Window::getShouldClose()) {

		// Track the passing of time between frames
		double curr_time = glfwGetTime();
		dt = curr_time - prev_time;


		// Update

		// Escape to exit
		if(Input::isPressed(KEY_ESCAPE)) {
			Window::setShouldClose(true);
			break;
		}

		if(Input::isPressed(KEY_R)) {
			if(curr_time > last_shader_load + shader_load_timeout) {
				shader.destroy();
				shader = Shader("../res/test.glsl");
				shader.bind();
				std::cout << "Reloaded shader" << std::endl;
				last_shader_load = curr_time;
			}
		}

		if(Input::isPressed(KEY_W)) {
			scene_scale.x -= 0.05 * uroboro::sqrt(uroboro::abs(scene_scale.x + 0.01));
			scene_scale.y -= 0.05 * uroboro::sqrt(uroboro::abs(scene_scale.y + 0.01));
		}

		if(Input::isPressed(KEY_S)) {
			scene_scale.x += 0.05 * uroboro::sqrt(uroboro::abs(scene_scale.x + 0.01));
			scene_scale.y += 0.05 * uroboro::sqrt(uroboro::abs(scene_scale.y + 0.01));
		}

		if(Input::isPressed(KEY_A)) {
			translation.x -= 0.1 * uroboro::sqrt(
				uroboro::abs(translation.x + 0.01)) * (uroboro::abs(scene_scale.x) + 0.1);
		}

		if(Input::isPressed(KEY_D)) {
			translation.x += 0.1 * uroboro::sqrt(
				uroboro::abs(translation.x + 0.01)) * (uroboro::abs(scene_scale.x) + 0.1);
		}

		if(Input::isPressed(KEY_Z)) {
			translation.y -= 0.1 * uroboro::sqrt(
				uroboro::abs(translation.y + 0.01)) * (uroboro::abs(scene_scale.y) + 0.1);
		}

		if(Input::isPressed(KEY_X)) {
			translation.y += 0.1 * uroboro::sqrt(
				uroboro::abs(translation.y + 0.01)) * (uroboro::abs(scene_scale.y) + 0.1);
		}

		if(Input::isPressed(KEY_SPACE)) {
			scene_scale = vec2(1, 1);
			translation = vec3(0, 0, 0);
		}

		// Map the window mouse position to the scene
		iMouse = (Input::getMousePos() - vec2(height / 2, width / 2)) * 2;
		iMouse.y *= -1;

		// Render
		Renderer::clear_screen(vec3(0.9f, 0.9f, 0.9f));

		// Set uniform variables in the fragment shader
		shader.setUniform("iTime", (float) curr_time);
		shader.setUniform("iTimeDelta", (float) dt);
		shader.setUniform("iResolution", vec3(height, width, 0));
		shader.setUniform("iMouse", iMouse);
		shader.setUniform("transform", transform);
		shader.setUniform("scene_scale", scene_scale);
		shader.setUniform("translation", translation);

		// Render a square which fills the screen
		render_primitive(square);

		// End loop
		Renderer::flush();
		Window::swapBuffers();
		Window::pollEvents();
	}

	Window::destroy();
	std::cin.get();

	return 0;
}
