#ifndef OUTLAW_HELP_H
#define OUTLAW_HELP_H
#include "outlaw.h"

using namespace outlaw;

/*
* This file contains help functions to easily use or test the library
*/

inline Shader load_default_shader() {

	Shader default_shader;
	default_shader.load("../res/default.glsl");
	default_shader.bind();
	return default_shader;
}

#endif
