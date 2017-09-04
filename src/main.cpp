#include "outlaw.h"
#include "help.h"

using namespace outlaw;

int main(int argc, char const *argv[]) {

	Window::create("Outlaw Rendering Engine", 1200, 800);
	printlog("Opened window");

	Shader default_shader = load_default_shader();
	printlog("Loaded default shader");

	while(true) {

		Window::reshape();
		Renderer::clear_screen();

		// Update

		// Render

		if(Input::isPressed(KEY_ESCAPE)) {
			Window::destroy();
			break;
		}

		Window::pollEvents();
		Window::swapBuffers();
	}

	printlog("Closed window");

	// Wait after window has been closed to read output
	std::cout << "Press enter to exit...";
	std::cin.get();


	return 0;
}
