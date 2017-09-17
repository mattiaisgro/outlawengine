#include "renderer.h"
#include "window.h"
#include "loadgl.h"
#include "core.h"

using namespace GL;
using namespace outlaw;

GPUID Renderer::currentVBO = 0;
GPUID Renderer::currentVAO = 0;
GPUID Renderer::currentShader = 0;
GPUID Renderer::currentEBO = 0;
GPUID Renderer::currentTexture[16] = {0};
GPUID Renderer::currentFramebuffer = 0;


void outlaw::Renderer::init() {

	int res = GL::loadgl();
	res |= GL::loadglext();

	if(res) {
		printerror("Unable to load OpenGL");
		Window::destroy();
		exit(-2);
	}

	int major, minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printlog("OpenGL version is ", major, ".", minor);

	if(major < 4) {
		printerror("OpenGL version is too low");
		Window::destroy();
		exit(-3);
	}

}

// VAO functions

GPUID outlaw::Renderer::create_vao() {

	GPUID ID;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	return ID;
}

void outlaw::Renderer::setup_vao(VAOAttrib attributes[], uint count) {

	for (uint i = 0; i < count; ++i) {
		const void* ptr = attributes[i].pointer;
		glVertexAttribPointer(i,
								attributes[i].size,
								(GLenum) attributes[i].type,
								attributes[i].normalized,
								attributes[i].stride,
								ptr);
		glEnableVertexAttribArray(i);
	}
}

void outlaw::Renderer::bind_vao(GPUID ID) {

	if(currentVAO != ID) {
		glBindVertexArray(ID);
		currentVAO = ID;
	}
}

void outlaw::Renderer::destroy_vao(GPUID ID) {

	glDeleteVertexArrays(1, &ID);
}

// VBO functions

// void create_mesh_buffer(std::vector<vec3>* vertices, std::vector<vec2>* UVs, std::vector<vec3>* normals,
// 						GPUID* VBO, GPUID* VAO) {

// 	*VAO = Renderer::create_vao();




// 	VAOAttrib attributes[] = {
// 		VAOAttrib(),
// 		VAOAttrib(),
// 		VAOAttrib()
// 	}

// }

// void create_mesh_buffer(std::vector<vec3>* vertices, std::vector<uint>* indices, std::vector<vec2>* UVs,
// 			std::vector<vec3>* normals, GPUID* VBO, GPUID* VAO, GPUID* EBO) {



// }

GPUID outlaw::Renderer::create_buffer(float data[], size_t size, GLBUFFUSAGE usage) {

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	if(!VBO)
		return 0;

	Renderer::bind_buffer(VBO);

	glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum) usage);

	return VBO;
}

void outlaw::Renderer::bind_buffer(GPUID ID) {

	if(currentVBO != ID) {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		Renderer::currentVBO = ID;
	}
}

void outlaw::Renderer::destroy_buffer(GPUID ID) {

	glDeleteBuffers(1, &ID);
}

void outlaw::Renderer::draw_buffer(uint count, GLPRIMITIVE primitive) {

	glDrawArrays((GLenum) primitive, 0, count);
}

void outlaw::Renderer::copy_buffer(GPUID dest, GPUID src, uint size) {

	glBindBuffer(GL_COPY_READ_BUFFER, src);
	glBindBuffer(GL_COPY_WRITE_BUFFER, dest);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
}

void outlaw::Renderer::update_buffer(GPUID ID, GLBUFFTARGET target, uint offset, uint size, void* data) {

	if(target == GLBUFFTARGET::VBO)
		Renderer::bind_buffer(ID);
	else if(target == GLBUFFTARGET::EBO)
		Renderer::bind_ebo(ID);

	glBufferSubData((GLenum) target, offset, size, data);
}

void* outlaw::Renderer::map_buffer(GPUID ID, GLBUFFTARGET target) {

	if(target == GLBUFFTARGET::VBO)
		Renderer::bind_buffer(ID);
	else if(target == GLBUFFTARGET::EBO)
		Renderer::bind_ebo(ID);

	void* ptr = glMapBuffer((GLenum) target, GL_WRITE_ONLY);
	return ptr;
}

bool outlaw::Renderer::unmap_buffer(GLBUFFTARGET target) {

	return glUnmapBuffer((GLenum) target);
}

// EBO functions

GPUID outlaw::Renderer::create_ebo(uint data[], size_t size, GLBUFFUSAGE usage) {

	GPUID ID;
	glCreateBuffers(1, &ID);

	if(!ID)
		return 0;

	Renderer::bind_ebo(ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, (GLenum) usage);

	return ID;
}

void outlaw::Renderer::bind_ebo(GPUID ID) {

	if(ID != currentEBO) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		currentEBO = ID;
	}
}

void outlaw::Renderer::destroy_ebo(GPUID ID) {

	glDeleteBuffers(1, &ID);
}

void outlaw::Renderer::draw_ebo(GPUID ID, uint count, GLPRIMITIVE primitive, const void* pointer) {

	Renderer::bind_ebo(ID);
	glDrawElements((GLenum) primitive, count, GL_UNSIGNED_INT, pointer);
}

// Texture functions

GPUID outlaw::Renderer::create_texture(float data[], size_t height, size_t width, GLTEXTUREFORMAT format, bool genmipmap, uint unit) {

	unsigned int ID;
	glGenTextures(1, &ID);

	if(!ID)
		return 0;

	Renderer::bind_texture(ID, unit);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	if(genmipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	return ID;
}

void outlaw::Renderer::set_texture_filter(GLTEXTUREFILTER filter_min, GLTEXTUREFILTER filter_max) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) filter_max);
}

void outlaw::Renderer::set_texture_wrap(GLTEXTUREWRAP wrap_s, GLTEXTUREWRAP wrap_t) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum) wrap_t);
}

void outlaw::Renderer::set_texture_border_color(vec4 color) {

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*) &color);
}

void outlaw::Renderer::bind_texture(GPUID ID, uint unit) {

	if(currentTexture[unit] != ID) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, ID);
		currentTexture[unit] = ID;
	}
}

// Shader functions

GPUID outlaw::Renderer::create_shader(std::string vertex_code,
					std::string fragment_code,
					std::string geometry_code) {

	GPUID vertexID = glCreateShader(GL_VERTEX_SHADER);
	GPUID fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	GPUID geometryID = 0;

	if(geometry_code != "")
		geometryID = glCreateShader(GL_GEOMETRY_SHADER);

	char const* vertex_code_ptr = vertex_code.c_str();
	glShaderSource(vertexID, 1, &vertex_code_ptr, nullptr);
	glCompileShader(vertexID);

	int message_lenght;

	glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &message_lenght);
	if(message_lenght) {
		char* message = new char[message_lenght + 1];
		message[message_lenght] = '\0';
		glGetShaderInfoLog(vertexID, message_lenght, nullptr, message);
		printerror("SHADER: ", message);
		delete[] message;
	}

	char const* fragment_code_ptr = fragment_code.c_str();
	glShaderSource(fragmentID, 1, &fragment_code_ptr, nullptr);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &message_lenght);
	if(message_lenght) {
		char* message = new char[message_lenght + 1];
		message[message_lenght] = '\0';
		glGetShaderInfoLog(fragmentID, message_lenght, nullptr, message);
		printerror("SHADER: ", message);
		delete[] message;
	}

	if(geometryID) {

		char const* geometry_code_ptr = geometry_code.c_str();
		glShaderSource(geometryID, 1, &geometry_code_ptr, nullptr);
		glCompileShader(geometryID);

		glGetShaderiv(geometryID, GL_INFO_LOG_LENGTH, &message_lenght);
		if(message_lenght) {
			char* message = new char[message_lenght + 1];
			message[message_lenght] = '\0';
			glGetShaderInfoLog(geometryID, message_lenght, nullptr, message);
			printerror("SHADER: ", message);
			delete[] message;
		}

	}

	GPUID programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	if(geometryID)
		glAttachShader(programID, geometryID);
	glLinkProgram(programID);

	int result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &message_lenght);
	if(message_lenght) {
		char* message = new char[message_lenght + 1];
		message[message_lenght] = '\0';
		glGetProgramInfoLog(programID, message_lenght, nullptr, message);
		printerror("SHADER: ", message);
		delete[] message;
	}

	if(message_lenght) {
		printerror("Error while loading shader");
		return 0;
	}

	return programID;
}

GPUID outlaw::Renderer::create_shader(std::string compute_code) {

	GPUID computeID = glCreateShader(GL_COMPUTE_SHADER);

	char const* compute_code_ptr = compute_code.c_str();
	glShaderSource(computeID, 1, &compute_code_ptr, nullptr);
	glCompileShader(computeID);

	int message_lenght;

	glGetShaderiv(computeID, GL_INFO_LOG_LENGTH, &message_lenght);
	if(message_lenght) {
		char* message = new char[message_lenght + 1];
		message[message_lenght] = '\0';
		glGetShaderInfoLog(computeID, message_lenght, nullptr, message);
		printerror("SHADER: ", message);
		delete[] message;
	}

	GPUID programID = glCreateProgram();
	glAttachShader(programID, computeID);
	glLinkProgram(programID);

	int result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &message_lenght);
	if(message_lenght) {
		char* message = new char[message_lenght + 1];
		message[message_lenght] = '\0';
		glGetProgramInfoLog(programID, message_lenght, nullptr, message);
		printerror("SHADER: ", message);
		delete[] message;
	}

	if(message_lenght) {
		printerror("Error while loading shader");
		return 0;
	}

	return programID;
}

void outlaw::Renderer::bind_shader(GPUID ID) {

	if(currentShader != ID) {
		glUseProgram(ID);
		currentShader = ID;
	}
}

void outlaw::Renderer::destroy_shader(GPUID ID) {

	glDeleteShader(ID);
}

void outlaw::Renderer::set_shader_uniform(GPUID ID, std::string uniform, vec3 value) {

	unsigned int location = glGetUniformLocation(ID, uniform.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void outlaw::Renderer::set_shader_uniform(GPUID ID, std::string uniform, vec4 value) {

	unsigned int location = glGetUniformLocation(ID, uniform.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void outlaw::Renderer::set_shader_uniform(GPUID ID, std::string uniform, mat4 value) {

	unsigned int location = glGetUniformLocation(ID, uniform.c_str());
	// Does not support double precision
	// TO-DO
	const float* data_ptr = (float*) value.data;
	glUniformMatrix4fv(location, 1, GL_FALSE, data_ptr);
}

void outlaw::Renderer::set_shader_uniform(GPUID ID, std::string uniform, int value) {

	unsigned int location = glGetUniformLocation(ID, uniform.c_str());
	glUniform1i(location, value);
}

// Framebuffer functions

GPUID Renderer::create_framebuffer() {

	unsigned int ID;
	glGenFramebuffers(1, &ID);

	return ID;
}

void Renderer::bind_framebuffer(GPUID ID, GLFRAMEBUFFERACCESS access) {

	if(currentFramebuffer != ID) {
		glBindFramebuffer((GLenum) access, ID);
		currentFramebuffer = ID;
	}
}

void Renderer::read_pixels(GPUID ID, float x, float y, float width, float height, float* data) {
	bind_framebuffer(ID);
	glReadPixels(x, y, width, height, GL_RGB, GL_FRAMEBUFFER, (void*) data);
}


// Window functions

void outlaw::Renderer::reshape(uint width, uint height) {
	glViewport(0, 0, width, height);
}

void outlaw::Renderer::clear_screen(vec3 color) {
	glClearColor(color.x, color.y, color.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void outlaw::Renderer::flush(bool wait) {

	if(wait)
		glFinish();
	else
		glFlush();
}

// State functions

void outlaw::Renderer::set_depth_test(bool b) {

	if(b)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void outlaw::Renderer::set_culling(bool b) {

	if(b)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void set_culling_face(GLFACE face) {

	glCullFace((GLenum) face);
}

void set_culling_front_face(bool clockwise) {

	if(clockwise)
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);
}
