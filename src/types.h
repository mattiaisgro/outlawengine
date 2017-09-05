#ifndef OUTLAW_TYPES_H
#define OUTLAW_TYPES_H
#include <string>
#include "uroboro/vec.h"
#include "uroboro/mat.h"

namespace outlaw {

	using uint = unsigned int;

	using GPUID = uint;

	enum class GLTYPE {
			BYTE = 0x1400,
			UNSIGNED_BYTE = 0x1401,
			SHORT = 0x1402,
			UNSIGNED_SHORT = 0x1403,
			INT = 0x1404,
			UNSIGNED_INT = 0x1405,
			FLOAT = 0x1406,
			DOUBLE = 0x140A
	};

}

using uroboro::vec3;
using uroboro::vec4;
using uroboro::vec2;
using uroboro::mat4;

using outlaw::GPUID;

#endif
