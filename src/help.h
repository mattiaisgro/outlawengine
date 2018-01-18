#ifndef OUTLAW_HELP_H
#define OUTLAW_HELP_H
#include "outlaw.h"
#include "uroboro/utility.h"

using namespace outlaw;

/*
* This file contains help functions to easily use or test the library
*/

struct Primitive {
	GPUID VAO;
	GPUID VBO;
	mat4 model;
	unsigned int size;
	GLPRIMITIVE primitive_type;

	Primitive(GPUID VAO, GPUID VBO, mat4 model, unsigned int size, GLPRIMITIVE primitive_type) :
				VAO(VAO), VBO(VBO), model(model), size(size), primitive_type(primitive_type) {}
};


void render_primitive(Primitive p) {

	Renderer::bind_vao(p.VAO);
	Renderer::draw_buffer(p.size, p.primitive_type);
}


inline Primitive create_primitive(vec3 vertices[], unsigned int size,
									GLPRIMITIVE primitive_type = GLPRIMITIVE::TRIANGLE_STRIP,
									GLBUFFUSAGE usage = GLBUFFUSAGE::STATIC) {

	GPUID VAO = Renderer::create_vao();
	Renderer::bind_vao(VAO);

	GPUID VBO = Renderer::create_buffer((float*) vertices, size);

	VAOAttrib attributes[] = { VAOAttrib() };

	Renderer::setup_vao(attributes, sizeof(attributes) / sizeof(VAOAttrib));

	return Primitive(VAO, VBO, mat4(), size / sizeof(vec3), primitive_type);
}


inline void update_primitive(Primitive p, vec3 vertices[], unsigned int size) {

	Renderer::update_buffer(p.VBO, GLBUFFTARGET::VBO, 0, size, (void*) vertices);

}


inline Primitive create_rectangle(float width, float height) {

	float hw = width / 2.f;
	float hh = height / 2.f;

	vec3 vertices[] = {
		vec3(-hw, hh, 0),
		vec3(-hw, -hh, 0),
		vec3(hw, hh, 0),
		vec3(hw, -hh, 0)
	};

	return create_primitive(vertices, sizeof(vertices));
}


inline Primitive create_square(float lenght) {
	return create_rectangle(lenght, lenght);
}


inline Primitive create_cube(float width, float height, float depth) {

	float hw = width / 2.f;
	float hh = height / 2.f;
	float hd = depth / 2.f;

	vec3 vertices[] = {
		vec3(-hw, -hh, -hd)
		// TO-DO
	};

	return create_primitive(vertices, sizeof(vertices));
}


inline Primitive create_circle(float radius, int steps = 64) {

	std::vector<vec3> vertices;
	float dx = (2 * PI) / (float) steps;

	vertices.push_back(vec3(0, 0, 0)); // center

	for (float i = 0; i < (2 * PI); i += dx) {

		vertices.push_back(vec3(cos(i) * radius, sin(i) * radius, 0));
		vertices.push_back(vec3(cos(i + dx) * radius, sin(i + dx) * radius, 0));
	}

	return create_primitive(&vertices[0], vertices.size() * 3 * 4, GLPRIMITIVE::TRIANGLE_FAN);
}


inline Primitive create_graph_from_function(float(*func)(float), float a, float b, int steps = 1000, float scale = 1) {

	float dx = (b - a) / (float) steps;

	std::vector<vec3> vertices;
	vec3 prev = vec3(a * scale, func(a) * scale, 0);
	vec3 curr;

	for (float i = a; i < b + dx; i += dx) {
		vertices.push_back(prev);
		curr = vec3(i * scale, func(i) * scale, 0);
		vertices.push_back(curr);
		prev = curr;
	}

	return create_primitive(&vertices[0], vertices.size() * 3 * 4, GLPRIMITIVE::LINES, GLBUFFUSAGE::DYNAMIC);
}


inline void update_graph_from_function(Primitive p, float(*func)(float), float a, float b, int steps = 1000, float scale = 1) {

	float dx = (b - a) / (float) steps;

	std::vector<vec3> vertices;
	vec3 prev = vec3(a * scale, func(a) * scale, 0);
	vec3 curr;

	for (float i = a; i < b + dx; i += dx) {
		vertices.push_back(prev);
		curr = vec3(i * scale, func(i) * scale, 0);
		vertices.push_back(curr);
		prev = curr;
	}

	update_primitive(p, &vertices[0], vertices.size() * 3 * 4);
}


inline Shader load_default_shader() {

	Shader default_shader("../res/default.glsl");
	default_shader.bind();
	return default_shader;
}

#endif
