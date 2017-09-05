#ifndef OUTLAW_VAOATTRIB_H
#define OUTLAW_VAOATTRIB_H
#include "types.h"

/*
* This file contains the VAOAttrib structure
* Its purpose is to provide a simple way
* to customize VAO attributes
* NOTE: The location of the VAO is determined by the Renderer
*/


namespace outlaw {

	struct VAOAttrib {

		VAOAttrib(uint size = 3,
					GLTYPE type = GLTYPE::FLOAT,
					bool normalized = false,
					uint stride = 0,
					void* pointer = nullptr) : size(size),
											type(type),
											normalized(normalized),
											stride(stride),
											pointer(pointer) {}

		uint size;
		GLTYPE type;
		bool normalized;
		uint stride;
		void* pointer;


	};


}

#endif
