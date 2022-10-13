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

#define PI 3.141592653589793
#define PHI 1.618033988749895
#define MAX_MARCHING_STEPS 1000
#define MIN_MARCHING_DIST 0.00025

in vec2 fragCoord;
out vec4 fragColor;

uniform float iTime;
uniform float iTimeDelta;
uniform vec3 iResolution;
uniform vec2 iMouse;
// uniform float iAnimationVelocity;

uniform vec3 translation;
uniform vec2 scene_scale;

bool lighting = false;
bool glow = false;
vec4 glow_color = normalize(vec4(0.5, 0.2, 0.8, 1));
bool border = false;


vec4 invcol(vec4 c) {
	return vec4(1, 1, 1, 1) - c;
}

// Common functions

float square(float x) {
	return x * x;
}

bool close_enough(float x, float y, float tol) {
	return abs(x - y) < tol;
}

float length4d(vec4 v) {
	return sqrt(square(v.x) + square(v.y) + square(v.z) + square(v.w));
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

float sgn(float x) {
	return x > 0 ? 1 : (x < 0 ? -1 : 0);
}


float clamp(float x, float min, float max) {
	return x < min ? min : (x > max ? max : x);
}


vec3 lerp(vec3 a, vec3 b, float t) {
	return (a + (b - a) * t);
}


// Geometrical transformations

vec2 rotate2d(vec2 v, float theta) {
	return vec2(
		 cos(theta) * v.x + sin(theta) * v.y,
		-sin(theta) * v.x + cos(theta) * v.y);
}


vec2 circle_inversion(vec2 point, vec2 center, float r) {

	vec2 OP = point - center;

	return center + normalize(OP) * square(r) / square(length2d(OP));
}

vec3 sphere_inversion(vec3 point, vec3 center, float r) {

	vec3 OP = point - center;

	return center + normalize(OP) * square(r) / square(length3d(OP));
}

// Distance Estimators

struct DEObject {
	float distance;
	int material;
	vec3 position;
	vec3 normal;
	vec4 color;
};


// Distance estimators
DEObject obj_sphere(vec3 pos, float radius, vec4 color) {

	DEObject sphere;
	sphere.distance = (length3d(pos) - radius);
	sphere.material = 1;
	sphere.color = color;

	return sphere;
}

DEObject obj_plane(vec3 pos, vec3 normal, float h, vec4 color) {

	DEObject plane;
	plane.distance = dot(pos, normal) + h;
	plane.color = color;
	plane.normal = normal;

	return plane;
}

DEObject obj_torus(vec3 pos, float radius, float section_radius, vec4 color) {

	DEObject torus;

	vec2 q = vec2(length(pos.xz) - radius, pos.y);

	torus.distance = length(q) - section_radius;
	torus.material = 2;
	torus.color = color;

	return torus;
}

DEObject obj_union(DEObject a, DEObject b) {

	if(a.distance > b.distance)
		return b;
	else
		return a;
}

DEObject obj_diff(DEObject a, DEObject b) {

	if(-a.distance > b.distance) {
		a.distance *= -1;
		return a;
	} else {
		return b;
	}
}

DEObject obj_intersect(DEObject a, DEObject b) {

	if(a.distance > b.distance)
		return a;
	else
		return b;
}

DEObject obj_blend(DEObject a, DEObject b, float t) {

	DEObject obj;
	float h = clamp(0.5 + 0.5 * (b.distance - a.distance) / t, 0.0, 1.0);

	obj.distance = mix(b.distance, a.distance, h) - t * h * (1.0 - h);
	obj.position = mix(b.position, a.position, h) - t * h * (1.0 - h);
	obj.color = mix(b.color, a.color, h);
	obj.normal = normalize(mix(b.normal, a.normal, h));
	obj.material = max(a.material, b.material);

	return obj;
}

// // Distance estimator
// DEObject DE(vec3 pos) {
// 	return obj_intersect(obj_torus(pos, 1.0, 0.4, vec4(0, 0, 1, 1)),
// 		obj_sphere(pos + vec3(1, 0, 1) * cos(iTime), 0.5, vec4(1, 0, 0, 1)));

// 	// return obj_torus(pos, 1.0, 0.4, vec4(0, 0, 1, 1));
// }

DEObject mandelbulb(vec3 pos) {

	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	float power = (3 * square(sin(iTime / 10)) + 1);
	// int power = 2;

	for (int i = 0; i < 10; i++) {

		r = length(z);

		// Escape radius is 2
		if (r > 2)
			break;
		
		// Polar coordinates
		float theta = acos(z.z / r);
		float phi = atan(z.y, z.x);

		float r_pow_1 = pow(r, power - 1.0);

		dr = r_pow_1 * power * dr + 1;
		
		// Scale and rotate the point
		float zr = r_pow_1 * r;
		theta = theta * power;
		phi = phi * power;
		
		// Cartesian coordinates
		z = zr * vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
		z += pos;
	}


	DEObject obj;
	obj.distance = 0.5 * log(r) * r / dr;
	// obj.color.rgb = mix(
	// 	normalize(vec3(0.2, 0.6, 0.2)),
	// 	normalize(vec3(0.4, 0, 0.3)),
	// 	square(length3d(pos)) - 0.2) * 1.2;

	// obj.color = vec4(1, 1, 1, 1);

	obj.color = vec4(0.5, 1, 0.4, 1)
		+ square(length3d(pos))
			* normalize(vec4(sin(iTime * PHI) * 0.5 + 0.5, -0.5, sin(iTime * PI) * 0.5 + 0.5, 1));

	return obj;
}


DEObject DE(vec3 pos) {
	return mandelbulb(pos);
}


// Raymarching procedure
DEObject raymarch(vec3 camera, vec3 direction) {

	int i = 0;
	float tot_distance = 0;
	float min_distance = 999999;
	DEObject obj;
	vec3 pos;

	for (i = 0; i < MAX_MARCHING_STEPS; i++) {

		// Compute the current position
		pos = camera + direction * tot_distance;

		// Estimate the distance from the current position
		// obj = DE(vec3(circle_inversion(pos.xy, vec2(0, 0), 1), pos.z));
		obj = DE(pos);
		tot_distance += obj.distance;
		obj.position = pos;

		if(obj.distance < min_distance)
			min_distance = obj.distance;

		// Stop when close enough
		if (obj.distance < MIN_MARCHING_DIST * clamp(scene_scale.x, 0.001, 10))
			break;
	}

	obj.distance = tot_distance;

	if(i == MAX_MARCHING_STEPS) {

		obj.material = 0;

		if(glow) {

			// Glow
			float glow = (1 - min_distance * 50);
			obj.color = glow_color * glow;

		} else if(border) {

			// Soft border
			float border_gradient = (1 - min_distance * 100);
			obj.color *= border_gradient;

		} else {
			obj.color = vec4(0, 0, 0, 1);
		}

	} else {

		obj.color *= pow(1 - float(i) / MAX_MARCHING_STEPS, 10);

		// Finite differences method for normal approximation
		if(lighting) {
			obj.normal = normalize(
			vec3(DE(pos + vec3(1, 0, 0)).distance - DE(pos - vec3(1, 0, 0)).distance,
				DE(pos + vec3(0, 1, 0)).distance - DE(pos - vec3(0, 1, 0)).distance,
				DE(pos + vec3(0, 0, 1)).distance - DE(pos - vec3(0, 0, 1)).distance));
		}
	}

	// if(i != MAX_MARCHING_STEPS) {
	// 	obj.color = vec4(1, 1, 1, 1);
	// }

	return obj;
}


// Complex functions

float complex_arg(vec2 z) {
	return atan(z.y, z.x);
}

float complex_abs(vec2 z) {
	return sqrt(square(z.x) + square(z.y));
}

vec2 complex_conjugate(vec2 z) {
	return vec2(z.x, -z.y);
}

vec2 complex_mul(vec2 z, vec2 w) {
	return vec2(
		z.x * w.x - z.y * w.y,
		z.x * w.y + z.y * w.x);
}

vec2 complex_div(vec2 z, vec2 w) {
	return complex_mul(z, complex_conjugate(w));
}

vec2 complex_pow(vec2 z, float p) {

	float rho = complex_abs(z);
	float theta = complex_arg(z);

	return vec2(
		pow(rho, p) * cos(theta * p),
		pow(rho, p) * sin(theta * p));
}


vec3 arg_color_map(vec2 z) {

	float arg = complex_arg(z);

	return vec3(1 - (arg / PI), 0, abs(arg / PI) - 1) * complex_abs(z);
}


void main() {

	// Background color
	fragColor = vec4(0, 0, 0, 1);

	float aspect_ratio = iResolution.x / iResolution.y;
	
	// Compute scene coordinates
	float x = (fragCoord.x * scene_scale.x + translation.x);
	float y = (fragCoord.y * scene_scale.y * aspect_ratio + translation.y);
	// float z = 2;

	// // Rendering using raymarching
	// // Viewpoint
	// vec3 camera = vec3(x, y, z);
	// vec3 view_direction = normalize(vec3(0, 0, -1));

	// // Lighting
	// vec3 diffuse_color = normalize(vec3(1, 1, 1));

	// // Raymarch and return the hit object
	// DEObject obj = raymarch(camera, view_direction);

	// if(length3d(obj.normal) != 0 && lighting) {
	// 	fragColor.rgb = obj.color.rgb + diffuse_color * dot(obj.normal, -view_direction);
	// } else {
	// 	fragColor.rgb = obj.color.rgb;
	// }


	// 2D Fractal rendering
	vec2 pos = vec2(x, y);

	// Transform point
	// pos = circle_inversion(pos, vec2(0, 0), 1);
	// pos = rotate2d(pos, (sin(iTime) / (length2d(pos) * 2 + 0.1)));

	// Displacement
	float c_a = -0.8 * (1 - 0.05 * abs(cos(iTime)));
	float c_b = 0.156 * (1 - 0.05 * abs(cos(iTime)));


	// float c_a = pos.x;
	// float c_b = pos.y;

	// z(n = 0)
	float d_a = pos.x;
	float d_b = pos.y;

	// Power
	float d = 2;

	// Escape Radius
	float R = pow(2, 1/(d - 1));

	// Power
	// float n = sin(iTime / 10) * 3 + 5;
	float n = 7;

	int iteration = 0;
	float tmp;


	float z_a = d_a;
	float z_b = d_b;

	// while (iteration < MAX_MARCHING_STEPS)  {

	// 	float rho_sqr = square(z_a) + square(z_b);

	// 	if(rho_sqr > R * R)
	// 		break;

	// 	float arg = 0;

	// 	if(z_a != 0) {
	// 		arg = atan(z_b, z_a);
	// 	}

	// 	// z_a = rho * cos(arg) + c_a;
	// 	// z_b = rho * sin(arg) + c_b;

	// 	float rho = pow(rho_sqr, n / 2.0);

	// 	z_b = rho * sin(n * arg) + c_b;
	// 	z_a = rho * cos(n * arg) + c_a;

	// 	iteration++;
	// }

	float dist1 = R;
	float dist2 = R;
	float dist3 = R;
	float dist4 = R;


	// Power 2 Julia/Mandelbrot set
	while (z_a * z_a + z_b * z_b < (R * R) && iteration < MAX_MARCHING_STEPS)  {

		tmp = z_a * z_a - z_b * z_b;
		z_b = 2 * z_a * z_b + c_b;
		z_a = tmp + c_a;

		iteration++;

		dist1 = min(dist1, length2d(vec2(z_a - 0.0, z_b - 0.0)));
		dist2 = min(dist2, length2d(vec2(z_a - 0.1, z_b - 0.1)));
		dist3 = min(dist3, length2d(vec2(z_a - 0.2, z_b - 0.2)));
		dist4 = min(dist4, length2d(vec2(z_a - 0.3, z_b - 0.3)));
	}

	float intensity_factor = (iteration
		- log(log(length2d(vec2(z_a, z_b))) / log(R)) / log(d))
			/ float(MAX_MARCHING_STEPS);

	// intensity_factor *= clamp(abs(log(intensity_factor)), 0, 10);

	vec3 base_color = vec3(0x9b, 0x5d, 0xe5);
	vec3 trap1 = vec3(0xf1, 0x5b, 0xb5);
	vec3 trap2 = vec3(0xfe, 0x00, 0x40);
	vec3 trap3 = vec3(0x0f, 0xbb, 0xb9);
	vec3 trap4 = vec3(0xff, 0x00, 0x6e);

	// base_color /= length3d(base_color);
	// trap1 /= length3d(trap1);
	// trap2 /= length3d(trap2);
	// trap3 /= length3d(trap3);
	// trap4 /= length3d(trap4);

	vec3 color = lerp(base_color, trap1, dist1);
	color = lerp(color, trap2, dist2);
	color = lerp(color, trap4, dist4);

	// vec3 color = vec3(dist1, dist2, dist3);
	color *= intensity_factor / 10;

	// Adjust intensity
	// if(length3d(color) < 2) {
	// 	color *= 2;
	// }

	// if(length3d(color) > 3.5) {
	// 	color /= 1.5;
	// }

	color /= clamp(length2d(vec2(d_a, d_b)), 0.8, 100);

	fragColor.rgb = color;


	// Complex function domain coloring
	// vec2 z = vec2(x, y);

	// fragColor.rgb = arg_color_map(complex_pow(z, 0.5));
}
