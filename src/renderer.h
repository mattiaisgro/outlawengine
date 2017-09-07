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

	enum class GLTEXTUREWRAP {
		REPEAT = 0x2901,
		MIRRORED_REPEAT = 0x8370,
		CLAMP_TO_EDGE = 0x812F,
		CLAMP_TO_BORDER = 0x812D
	};

	enum class TEXTUREFORMAT {
		RGB = 0x1907,
		RGBA = 0x1908,
		BGR = 0x80E0,
		BGRA = 0x80E1
	};

	enum class TEXTUREFILTER {
		NEAREST = 0x2600,
		LINEAR = 0x2601,
		NEAREST_MIPMAP_NEAREST = 0x2700,
		LINEAR_MIPMAP_NEAREST = 0x2701,
		NEAREST_MIPMAP_LINEAR = 0x2702,
		LINEAR_MIPMAP_LINEAR = 0x2703
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
			static GPUID currentTexture[16];

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

			// Texture functions

			static GPUID create_texture(float data[], size_t height, size_t width,
										TEXTUREFORMAT format = TEXTUREFORMAT::RGB, bool genmipmap = false,
										uint unit = 0);

			static void set_texture_filter(TEXTUREFILTER filter_min, TEXTUREFILTER filter_max);

			static void set_texture_wrap(GLTEXTUREWRAP wrap_s, GLTEXTUREWRAP wrap_t);

			static void set_texture_border_color(vec4 color);

			static void bind_texture(GPUID ID, uint unit);

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
