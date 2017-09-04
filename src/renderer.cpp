#include "renderer.h"
#include "window.h"
#include "loadgl.h"
#include "core.h"

using namespace GL;
using namespace outlaw;

GPUID Renderer::currentVBO = 0;
GPUID Renderer::currentVAO = 0;
GPUID Renderer::currentShader = 0;


void outlaw::Renderer::init() {}

// VAO functions

GPUID outlaw::Renderer::create_vao(VAO_TYPE type) {

	if(type == VAO_TYPE::FLAT) {

		GLuint ID;
		glGenVertexArrays(1, &ID);
		glBindVertexArray(ID);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*) 0
		);

	} else if(type == VAO_TYPE::INTERLEAVED) {
		return 0;
	}
}

void outlaw::Renderer::bind_vao(GPUID ID) {

	if(currentVAO != ID)
		glBindVertexArray(ID);
}

void outlaw::Renderer::destroy_vao(GPUID ID) {

	glDeleteVertexArrays(1, &ID);
}

// VBO functions

GPUID outlaw::Renderer::create_buffer(float data[], size_t size, BUFF_USAGE usage) {

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	if(!VBO)
		return 0;

	Renderer::bind_buffer(VBO);

	int usage_const = 0;

	if(usage == BUFF_USAGE::STATIC)
		usage_const = GL_STATIC_DRAW;
	else if(usage == BUFF_USAGE::DYNAMIC)
		usage_const = GL_DYNAMIC_DRAW;
	else if(usage == BUFF_USAGE::STREAM)
		usage_const = GL_STREAM_DRAW;


	glBufferData(GL_ARRAY_BUFFER, size, data, usage_const);

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

void outlaw::Renderer::draw_buffer(GPUID ID, uint count) {

	Renderer::bind_buffer(ID);
	glDrawArrays(GL_TRIANGLES, 0, count);
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

	// TO-DO
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

void outlaw::Renderer::reshape(uint width, uint height) {
	glViewport(0, 0, width, height);
}

void outlaw::Renderer::clear_screen(vec3 color) {
	glClearColor(color.x, color.y, color.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
