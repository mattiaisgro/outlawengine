#include "input.h"
#include "window.h"
#include "GLFW/glfw3.h"

using namespace outlaw;

void* Input::window_id = nullptr;
int Input::scroll = 0;

#define WINDOWID ((GLFWwindow*) window_id)

void outlaw::Input::create() {
	window_id = Window::getID();
}

void outlaw::Input::destroy() {}

bool outlaw::Input::isPressed(int button) {
	return glfwGetKey(WINDOWID, button) == GLFW_PRESS;
}

bool outlaw::Input::isReleased(int button) {
	return glfwGetKey(WINDOWID, button) == GLFW_RELEASE;
}

int outlaw::Input::getKey(int button) {
	return glfwGetKey(WINDOWID, button);
}

bool outlaw::Input::isMousePressed(int button) {
	return glfwGetMouseButton(WINDOWID, button) == GLFW_PRESS;
}

bool outlaw::Input::isMouseReleased(int button) {
	return glfwGetMouseButton(WINDOWID, button) == GLFW_RELEASE;
}

int outlaw::Input::getMouseKey(int button) {
	return glfwGetMouseButton(WINDOWID, button);
}

vec2 outlaw::Input::getMousePos() {
	double x, y;
	glfwGetCursorPos(WINDOWID, &x, &y);
	return vec2(x, y);
}

void outlaw::Input::setMousePos(double xpos, double ypos) {
	glfwSetCursorPos(WINDOWID, xpos, ypos);
}

void outlaw::Input::setMousePos(vec2 pos) {
	glfwSetCursorPos(WINDOWID, pos.x, pos.y);
}

void outlaw::Input::hideCursor() {

}

void outlaw::Input::showCursor() {

}
