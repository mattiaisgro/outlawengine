#ifndef OUTLAW_RENDERER_H
#define OUTLAW_RENDERER_H
#include "types.h"
#include "vaoattrib.h"
#include <vector>

namespace outlaw {

	enum class GLBUFFUSAGE {
		STATIC = 0x88E4,
		DYNAMIC = 0x88E8,
		STREAM = 0x88E0
	};

	enum class GLBUFFTARGET {
		VBO = 0x8892,
		EBO = 0x8893
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

	enum class GLTEXTUREFORMAT {
		RGB = 0x1907,
		RGBA = 0x1908,
		BGR = 0x80E0,
		BGRA = 0x80E1
	};

	enum class GLTEXTUREFILTER {
		NEAREST = 0x2600,
		LINEAR = 0x2601,
		NEAREST_MIPMAP_NEAREST = 0x2700,
		LINEAR_MIPMAP_NEAREST = 0x2701,
		NEAREST_MIPMAP_LINEAR = 0x2702,
		LINEAR_MIPMAP_LINEAR = 0x2703
	};

	enum class GLFACE {
		BACK = 0x0405,
		FRONT = 0x0404,
		FRONT_AND_BACK = 0x0408
	};

	enum class GLFRAMEBUFFERACCESS {
		ALL = 0x8D40,
		READ = 0x8CA8,
		DRAW = 0x8CA9
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
			static GPUID currentFramebuffer;

		public:

			static void init();

			// VAO functions

			/*
			* Creates a new VAO buffer
			*/
			static GPUID create_vao();

			static void setup_vao(VAOAttrib attributes[], uint count);

			//VAO attributes

			static void bind_vao(GPUID ID);

			static void destroy_vao(GPUID ID);

			// VBO functions

			/*
			* Creates a VBO and a VAO for mesh rendering
			* Returns 0 on success
			* Returns -1 if parameters are invalid
			*/
			static int create_mesh_buffer(const std::vector<vec3>& vertices,
											const std::vector<vec2>& UVs,
											const std::vector<vec3>& normals,
											GPUID* VBO,
											GPUID* VAO
											);

			static int create_mesh_buffer(const std::vector<vec3>& vertices,
											const std::vector<uint>& indices,
											const std::vector<vec2>& UVs,
											const std::vector<vec3>& normals,
											GPUID* VBO,
											GPUID* VAO,
											GPUID* EBO
											);

			static GPUID create_buffer(float data[], size_t size, GLBUFFUSAGE usage = GLBUFFUSAGE::STATIC);

			static void bind_buffer(GPUID ID);

			static void destroy_buffer(GPUID ID);

			static void draw_buffer(uint count, GLPRIMITIVE primitive = GLPRIMITIVE::TRIANGLES);

			static void copy_buffer(GPUID dest, GPUID src, uint size);

			static void update_buffer(GPUID ID, GLBUFFTARGET target, uint offset, uint size, void* data);

			static void* map_buffer(GPUID ID, GLBUFFTARGET target = GLBUFFTARGET::VBO);

			static bool unmap_buffer(GLBUFFTARGET target = GLBUFFTARGET::VBO);

			// EBO functions

			// TO-DO Support for different types of data (GL_UNSIGNED_SHORT ...)
			static GPUID create_ebo(uint data[], size_t size, GLBUFFUSAGE usage = GLBUFFUSAGE::STATIC);

			static void bind_ebo(GPUID ID);

			static void destroy_ebo(GPUID ID);

			static void draw_ebo(GPUID ID, uint count, GLPRIMITIVE primitive = GLPRIMITIVE::TRIANGLES,
								 const void* pointer = nullptr);

			// Texture functions

			static GPUID create_texture(float data[], size_t height, size_t width,
										GLTEXTUREFORMAT format = GLTEXTUREFORMAT::RGB, bool genmipmap = false,
										uint unit = 0);

			static void set_texture_filter(GLTEXTUREFILTER filter_min, GLTEXTUREFILTER filter_max);

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

			static void set_shader_uniform(GPUID ID, std::string uniform, int value);

			// Framebuffer functions

			static GPUID create_framebuffer();

			static void bind_framebuffer(GPUID ID, GLFRAMEBUFFERACCESS access = GLFRAMEBUFFERACCESS::ALL);

			static void read_pixels(GPUID ID, float x, float y, float width, float height, float* data);

			// Window functions

			static void reshape(uint width, uint height);

			static void clear_screen(vec3 color = vec3(0, 0, 0));

			static void flush(bool wait = false);

			// State functions

			static void set_depth_test(bool b);

			static void set_culling(bool b);

			static void set_culling_face(GLFACE face = GLFACE::FRONT);

			static void set_culling_front_face(bool clockwise = false);


	};

}

#endif
