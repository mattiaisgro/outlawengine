#include "shader.h"
#include "renderer.h"
#include "core.h"
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>

using namespace outlaw;

void outlaw::Shader::load(std::string filename) {

	std::vector<std::string> lines;
	std::ifstream file(filename);
	std::string line;

	if(!file.is_open()) {
		ID = 0;
		return;
	}

	while(std::getline(file, line))
		lines.emplace_back(line);

	std::string vertex_code = "";
	std::string fragment_code = "";
	std::string geometry_code = "";
	std::string compute_code = "";
	std::string* current = &vertex_code;

	bool compute = false;

	for (unsigned int i = 0; i < lines.size(); ++i) {
		if(lines[i] == "#vertex")
			current = &vertex_code;
		else if(lines[i] == "#fragment")
			current = &fragment_code;
		else if(lines[i] == "#geometry") {
			current = &geometry_code;
		} else if(lines[i] == "#compute") {
			current = &compute_code;
			compute = true;
		} else
			*current += lines[i] + "\n";
	}

	if(!compute)
		ID = Renderer::create_shader(vertex_code, fragment_code, geometry_code);
	else
		ID = Renderer::create_shader(compute_code);
}

void outlaw::Shader::bind() {
	Renderer::bind_shader(ID);
}

void outlaw::Shader::destroy() {
	Renderer::destroy_shader(ID);
}

void outlaw::Shader::setUniform(std::string uniform, vec4 v) {
	Renderer::set_shader_uniform(ID, uniform, v);
}

void outlaw::Shader::setUniform(std::string uniform, vec3 v) {
	Renderer::set_shader_uniform(ID, uniform, v);
}

void outlaw::Shader::setUniform(std::string uniform, mat4 m) {
	Renderer::set_shader_uniform(ID, uniform, m);
}
