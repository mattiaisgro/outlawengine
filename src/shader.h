#ifndef SHADER_H
#define SHADER_H
#include "types.h"
#include <string>
#include <map>

namespace outlaw {

	class Shader {
		private:
			GPUID ID;

		public:

			inline Shader() {}

			inline Shader(std::string filename) {
				load(filename);
			}

			/*
			* Load a shader from a source file
			* This function uses a special format for shaders,
			* it lets you have a single file for a complete shader
			* EXAMPLE:
			* #vertex
			* vertex shader code...
			*
			* #fragment
			* fragment shader code...
			*
			* Supported identifiers are:
			* #vertex, #fragment, #geometry and #compute
			*
			* 0 is returned on error
			*/
			void load(std::string filename);

			void bind();

			void destroy();

			void setUniform(std::string uniform, vec4 v);

			void setUniform(std::string uniform, vec3 v);

			void setUniform(std::string uniform, mat4 m);


	};

}

#endif
