#vertex
#version 400 core

// Pass-through vertex shader
layout(location = 0) in vec3 position;
uniform mat4 transform;
out vec2 fragCoord;

void main() {
	gl_Position = transform * vec4(position, 1.0f);
	fragCoord = gl_Position.xy;
}

#fragment
#version 400 core

#define PI 3.14

in vec2 fragCoord;
out vec4 fragColor;

uniform float iTime;
uniform float iTimeDelta;
uniform vec3 iResolution;
uniform vec2 iMouse;

uniform vec2 scene_scale;


vec4 invcol(vec4 c) {
	return vec4(1, 1, 1, 1) - c;
}


float square(float x) {
	return x * x;
}

bool close_enough(float x, float y, float tol) {
	return abs(x - y) < tol;
}

float length3d(vec3 v) {
	return sqrt(square(v.x) + square(v.y) + square(v.z));
}

float length2d(vec2 v) {
	return sqrt(square(v.x) + square(v.y));
}

float distance2d(vec2 a, vec2 b) {
	return sqrt(square(a.x - b.x) + square(a.y - b.y));
}

float distance3d(vec3 a, vec3 b) {
	return length3d(a - b);
}
	
vec4 draw_function(float f_x, float y, float tolerance) {
	float col = 1;
	if(close_enough(f_x, y, tolerance))
		col = sqrt(abs(y - f_x) / (tolerance));
	return vec4(col, col, col, 1.0);
}


vec4 draw_constraint(float x, float f_x, float y, float f_y, float tolerance) {
	float col = 1;
	if(close_enough(f_x, x, tolerance) && close_enough(f_y, y, tolerance))
		col = sqrt(length2d(vec2(x - f_x, y - f_y)) / (tolerance));
	return vec4(col, col, col, 1.0);
}


float sgn(float x) {
	return x > 0 ? 1 : (x < 0 ? -1 : 0);
}


float clamp(float x, float min, float max) {
	return x < min ? min : (x > max ? max : x);
}


float atan2(float x, float y) {
	return atan(y / x) - clamp(sgn(x), -1, 0) * PI * sgn(y);
}


void main() {

	fragColor = vec4(0, 0, 0, 1);
	
	float x = (fragCoord.x * scene_scale.x);
	float y = (fragCoord.y * scene_scale.y);

	// Circle
	fragColor += invcol(draw_function(square(x) + square(y), 1, 0.025));

	// Flower
	fragColor += invcol(draw_function(square(x) + square(y), square(sin(atan2(-x, y) * sin(iTime / 5) * 10)) * 0.5 + 0.1, 0.05));

	// Lissajous trajectory
	fragColor += invcol(draw_constraint(x, sin(17 * iTime / 2) * 0.5, y, sin(7 * iTime / 2) * 0.5, 0.02)) * 2;

	// Pointy hyperbola
	fragColor += invcol(draw_function(square(x) - square(y), square(sin(atan2(-x, y) * sin(iTime / 5) * 5)) * 0.2 + 0.05, 0.02));

	// Highlight mouse position
	fragColor += invcol(draw_constraint(x, iMouse.x / iResolution.x, y, iMouse.y / iResolution.y, 0.05));

	// Animated concentric circles 
	float gs = abs(sin(sqrt(length2d(vec2(x, y)) * 5000 * abs(sin(iTime)))));
	fragColor += vec4(gs, gs, gs, 1);
}
