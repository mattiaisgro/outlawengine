#ifndef OUTLAW_RENDERER_H
#define OUTLAW_RENDERER_H
#include "types.h"
#include "vaoattrib.h"

namespace outlaw {

	enum class BUFF_USAGE {
		STATIC,
		DYNAMIC,
		STREAM
	};

	enum class GLPRIMITIVE {
		POINTS = 0x0000,
		LINES = 0x0001,
		LINE_LOOP = 0x0002,
		LINE_STRIP = 0x0003,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006
	};

	class Renderer {
		private:
			Renderer() = delete;
			~Renderer() = delete;

			// OpenGL state
			static GPUID currentVBO;
			static GPUID currentVAO;
			static GPUID currentShader;
			static GPUID currentEBO;

		public:

			static void init();

			// VAO functions

			static GPUID create_vao();

			static void setup_vao(VAOAttrib attributes[], uint count);

			//TO-DO VAO attributes

			static void bind_vao(GPUID ID);

			static void destroy_vao(GPUID ID);

			// VBO functions

			static GPUID create_buffer(float data[], size_t size, BUFF_USAGE usage = BUFF_USAGE::STATIC);

			static void bind_buffer(GPUID ID);

			static void destroy_buffer(GPUID ID);

			static void draw_buffer(uint count, GLPRIMITIVE primitive = GLPRIMITIVE::TRIANGLES);

			// EBO functions

			// TO-DO Support for different types of data (GL_UNSIGNED_SHORT ...)
			static GPUID create_ebo(uint data[], size_t size, BUFF_USAGE usage = BUFF_USAGE::STATIC);

			static void bind_ebo(GPUID ID);

			static void destroy_ebo(GPUID ID);

			static void draw_ebo(GPUID ID, uint count, GLPRIMITIVE primitive = GLPRIMITIVE::TRIANGLES,
								 const void* pointer = nullptr);

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
