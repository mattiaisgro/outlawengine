#include "outlaw.h"
#include "loadgl.h"
#include "help.h"

#include <vector>
#include <sstream>
#include <string>
#include <fstream>

using namespace outlaw;

int main(int argc, char const *argv[]) {

	if(argc < 2) {
		std::cout << "PlotPoints takes one argument" << std::endl;
		return 1;
	}

	//Init

	const real scale = 1;

	std::vector<vec2> points;

	std::string filename = std::string(argv[1]);
	std::ifstream file(filename);
	std::string line;

	while(getline(file, line)) {

		std::stringstream str(line);

		real x, y;
		str >> x;
		str >> y;

		points.push_back(vec2(x * scale, y * scale));
	}

	Window::create("PlotPoints", 720, 720);

	Shader shader("../res/default.glsl");
	shader.bind();

	Primitive point = create_circle(0.005, 8);
	Primitive line_x = create_line(vec3(-1, 0, 0), vec3(1, 0, 0));
	Primitive line_y = create_line(vec3(0, -1, 0), vec3(0, 1, 0));


	while(!Window::getShouldClose()) {

		// Render
		Renderer::clear_screen(vec3(1, 1, 1));

		shader.setUniform("mesh_color", vec3(0, 0, 0));
		shader.setUniform("transform", mat4());

		render_primitive(line_x);
		render_primitive(line_y);

		for (int i = 0; i < points.size(); ++i) {
			mat4 mvp = mat4();
			mvp.translate(points[i].x, points[i].y, 0);
			shader.setUniform("transform", mvp);
			shader.setUniform("mesh_color", vec3(0.1, 0.1, 0.8));
			render_primitive(point);
		}

		// End loop
		Renderer::flush();
		Window::swapBuffers();
		Window::pollEvents();
	}

	Window::destroy();

	std::cin.get();

	return 0;
}
