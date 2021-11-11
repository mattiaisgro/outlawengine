#ifndef UROBORO_INTERP_H
#define UROBORO_INTERP_H

#include "common.h"

namespace uroboro {

	inline real lerp(real number1, real number2, real interp) {
		return (number1 + interp * (number2 - number1));
	}

	inline vec3 lerp(vec3 number1, vec3 number2, real interp) {
		return (number1 + (number2 - number1) * interp);
	}

	inline real smoothstep(real number1, real number2, real interp) {
		const real result = clamp((interp - number1) / (number2 - number1), 0.0, 1.0);
		return result * result * (3 - 2 * result);
	}

	inline real smootherstep(real number1, real number2, real interp) {
		const real result = clamp((interp - number1) / (number2 - number1), 0.0, 1.0);
		return result * result * result * (result * (result * 6 - 15) + 10);
	}


	// Quadratic Bezier curve
	inline vec3 quadratic_bezier(vec3 P0, vec3 P1, vec3 P2, real t) {

		vec3 A = lerp(P0, P1, t);
		vec3 B = lerp(P1, P2, t);

		return lerp(A, B, t);
	}


	// Cubic Bezier curve
	inline vec3 cubic_bezier(vec3 P0, vec3 P1, vec3 P2, vec3 P3, real t) {

		vec3 A = lerp(P0, P1, t);
		vec3 B = lerp(P1, P2, t);
		vec3 C = lerp(P2, P3, t);
		
		vec3 D = lerp(A, B, t);
		vec3 E = lerp(B, C, t);

		return lerp(D, E, t);
	}


}

#endif
