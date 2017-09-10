#include "window.h"
#include "GLFW/glfw3.h"
#include "core.h"
#include <cstdlib>
#include "renderer.h"
#include "input.h"

using namespace outlaw;

void* Window::ID;
uint Window::width = 0;
uint Window::height = 0;
std::string Window::title = "";
WindowMode Window::mode = WindowMode::WINDOW_WINDOWED;
bool Window::visible = true;
bool Window::resizable = false;
bool Window::initialized = false;
uint Window::samples = 0;
bool Window::sRGB = false;
bool Window::vsync = false;
Window::WindowNext Window::next;

static void window_close(GLFWwindow* id) {
	//TO-DO User pointer for close callback
	Window::destroy();
	exit(0);
}

#define WINDOWID ((GLFWwindow*) ID)

int outlaw::Window::create(std::string title, uint width, uint height) {

	if(height == 0 || width == 0) {
		Window::height = 800; Window::width = 1200;
		height = 800; width = 1200;
	}

	if(!glfwInit()) {
		printerror("Can't initialize GLFW");
		exit(-1);
	}

	glfwWindowHint(GLFW_RESIZABLE, next.resizable);
	resizable = next.resizable;
	glfwWindowHint(GLFW_VISIBLE, visible);
	glfwWindowHint(GLFW_SAMPLES, next.samples);
	samples = next.samples;
	glfwWindowHint(GLFW_SRGB_CAPABLE, next.sRGB);
	sRGB = next.sRGB;

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	mode = next.mode;
	switch((int) mode) {
		case (int) WindowMode::WINDOW_WINDOWED: ID = (void*) glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		break;
		case (int) WindowMode::WINDOW_WINDOWED_FULLSCREEN:
		/*const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);*/
		break;
		case (int) WindowMode::WINDOW_FULLSCREEN: ID = (void*) glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
		break;
	}

	if (!ID) {
		printerror("Unable to create window");
		glfwTerminate();
		exit(-1);
	}

	initCallbacks();

	glfwMakeContextCurrent(WINDOWID);
	glfwSwapInterval(vsync);

	initialized = true;

	Renderer::init();

	Input::create();

	return 0;
}

void outlaw::Window::swapBuffers() {
	glfwSwapBuffers(WINDOWID);
}

void outlaw::Window::pollEvents() {
	glfwPollEvents();
}

void outlaw::Window::destroy() {
	glfwDestroyWindow(WINDOWID);
	glfwTerminate();
}

void outlaw::Window::focus() {
	//glfwFocusWindow(WINDOWID);
}

void outlaw::Window::maximize() {
	//glfwMaximizeWindow(WINDOWID);
}

void outlaw::Window::iconify() {
	glfwIconifyWindow(WINDOWID);
}

void outlaw::Window::initCallbacks() {
	glfwSetWindowCloseCallback(WINDOWID, window_close);
}

//Setters

void outlaw::Window::setWidth(uint width) {
	if(initialized)
		glfwSetWindowSize(WINDOWID, width, Window::height);

	Window::width = width;
}

void outlaw::Window::setHeight(uint height) {
	if(initialized)
		glfwSetWindowSize(WINDOWID, Window::width, height);

	Window::height = height;
}

void outlaw::Window::setTitle(std::string title) {
	if(initialized)
		glfwSetWindowTitle(WINDOWID, title.c_str());

	Window::title = title;
}

void outlaw::Window::setVisible(bool visible) {

	if(!initialized) {
		Window::visible = visible;
		return;
	}

	if(visible)
		glfwShowWindow(WINDOWID);
	else
		glfwHideWindow(WINDOWID);

	Window::visible = visible;
}

void outlaw::Window::setResizable(bool resizable) {
	glfwWindowHint(GLFW_RESIZABLE, (int) resizable);
	next.resizable = resizable;
}

void outlaw::Window::setShouldClose(bool shouldClose) {
	if(initialized)
		glfwSetWindowShouldClose(WINDOWID, shouldClose);
}

void outlaw::Window::setRatio(int number, int denom) {
	//glfwSetWindowAspectRatio(WINDOWID, number, denom);
}

void outlaw::Window::setPosition(uint x, uint y) {
	if(initialized)
		glfwSetWindowPos(WINDOWID, x, y);
}

void outlaw::Window::setSamples(uint samples) {
	next.samples = samples;
}

void outlaw::Window::setMode(WindowMode mode) {
	next.mode = mode;
}

void outlaw::Window::setsRGB(bool sRGB) {
	next.sRGB = sRGB;
}

void outlaw::Window::setVsync(bool vsync) {
	next.vsync = vsync;
}

void outlaw::Window::setClipboard(std::string s) {
	glfwSetClipboardString(WINDOWID, s.c_str());
}


//Getters

int outlaw::Window::getWidth() {
	int width = 0;
	int height = 0;
	if(initialized) {
		glfwGetWindowSize(WINDOWID, &width, &height);
		Window::height = height;
		Window::width = width;
	}
	return width;
}

int outlaw::Window::getHeight() {
	int width = 0;
	int height = 0;
	if(initialized) {
		glfwGetWindowSize(WINDOWID, &width, &height);
		Window::height = height;
		Window::width = width;
	}
	return height;
}

std::string outlaw::Window::getTitle() {
	return Window::title;
}

bool outlaw::Window::getVisible() {
	return glfwGetWindowAttrib(WINDOWID, GLFW_VISIBLE);
}

bool outlaw::Window::getResizable() {
	return glfwGetWindowAttrib(WINDOWID, GLFW_RESIZABLE);
}

bool outlaw::Window::getShouldClose() {
	return glfwWindowShouldClose(WINDOWID);
}

void outlaw::Window::getRatio(int* number, int* denom) {
	//TO-DO
}

float outlaw::Window::getAspectRatio() {
	return Window::getWidth() / float(Window::getHeight());
}

void outlaw::Window::getPosition(int* x, int* y) {
	glfwGetWindowPos(WINDOWID, x, y);
}

outlaw::uint outlaw::Window::getSamples() {
	return samples;
}

outlaw::WindowMode outlaw::Window::getMode() {
	return Window::mode;
}

bool outlaw::Window::isFocused() {
	return glfwGetWindowAttrib(WINDOWID, GLFW_FOCUSED);
}

bool outlaw::Window::isIconified() {
	return glfwGetWindowAttrib(WINDOWID, GLFW_ICONIFIED);
}

int outlaw::Window::getWindowAttrib(int attrib) {
	return glfwGetWindowAttrib(WINDOWID, attrib);
}

void outlaw::Window::setWindowHint(int hint, int value) {
	glfwWindowHint(hint, value);
}

bool outlaw::Window::getsRGB() {
	return sRGB;
}

bool outlaw::Window::getVsync() {
	return vsync;
}

std::string outlaw::Window::getClipboard() {
	return glfwGetClipboardString(WINDOWID);
}

void outlaw::Window::reshape() {
	int h, w;
	glfwGetFramebufferSize(WINDOWID, &h, &w);
	Renderer::reshape(h, w);
}
