#ifndef OUTLAW_RENDERER_H
#define OUTLAW_RENDERER_H
#include "types.h"

namespace outlaw {

	class Renderer {
		private:
			Renderer();
			~Renderer();

			// OpenGL state
			static GPUID currentVBO;
			static GPUID currentVAO;
			static GPUID currentShader;

		public:

			enum class BUFF_USAGE {
				STATIC,
				DYNAMIC,
				STREAM
			};

			enum class VAO_TYPE {
				FLAT,
				INTERLEAVED
			};

			static void init();

			// VAO functions

			static GPUID create_vao(VAO_TYPE type);

			static void bind_vao(GPUID ID);

			static void destroy_vao(GPUID ID);

			// VBO functions

			static GPUID create_buffer(float data[], size_t size, BUFF_USAGE usage);

			static void bind_buffer(GPUID ID);

			static void destroy_buffer(GPUID ID);

			static void draw_buffer(GPUID ID, uint count);

			// Shader functions

			static GPUID create_shader(std::string vertex_code,
								std::string fragment_code,
								std::string geometry_code = "");

			static GPUID create_shader(std::string compute_code);

			static void bind_shader(GPUID ID);

			static void destroy_shader(GPUID ID);

			static void set_shader_uniform(GPUID ID, std::string uniform, vec3 value);

			static void set_shader_uniform(GPUID ID, std::string uniform, vec4 value);

			static void set_shader_uniform(GPUID ID, std::string uniform, mat4 value);

			// Window functions

			static void reshape(uint width, uint height);

			static void clear_screen(vec3 color = vec3(0, 0, 0));

	};

}

#endif
