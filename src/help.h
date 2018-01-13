#ifndef OUTLAW_HELP_H
#define OUTLAW_HELP_H
#include "outlaw.h"

using namespace outlaw;

/*
* This file contains help functions to easily use or test the library
*/

struct Primitive {
	GPUID VAO;
	GPUID VBO;
	mat4 model;
	unsigned int size;

	Primitive(GPUID VAO, GPUID VBO, mat4 model, unsigned int size) : VAO(VAO), VBO(VBO), model(model), size(size) {}
};


void render_primitive(Primitive p, GLPRIMITIVE primitive_type = GLPRIMITIVE::TRIANGLE_STRIP) {

	Renderer::bind_vao(p.VAO);
	Renderer::draw_buffer(p.size, primitive_type);
}


inline Primitive create_primitive(vec3 vertices[], unsigned int size) {
	GPUID VAO = Renderer::create_vao();
	Renderer::bind_vao(VAO);

	GPUID VBO = Renderer::create_buffer((float*) vertices, size);

	VAOAttrib attributes[] = { VAOAttrib() };

	Renderer::setup_vao(attributes, sizeof(attributes) / sizeof(VAOAttrib));

	return Primitive(VAO, VBO, mat4(), size / sizeof(vec3));
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


inline Shader load_default_shader() {

	Shader default_shader("../res/default.glsl");
	default_shader.bind();
	return default_shader;
}

#endif
