#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

Shader::Shader()
{
	
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	unsigned int vertex, fragment, geometry;

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	checkErrors(vertex, "VERTEX");

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	checkErrors(fragment, "FRAGMENT");

	if (geometrySource != nullptr) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometrySource, NULL);
		glCompileShader(geometry);
		checkErrors(geometry, "GEOMETRY");
	}

	// shader Program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (geometrySource != nullptr) {
		glAttachShader(this->ID, geometry);
	}
	glLinkProgram(this->ID);
	checkErrors(this->ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometrySource != nullptr) {
		glDeleteShader(geometry);
	}
}

Shader& Shader::use() {
	glUseProgram(this->ID);
	return *this;
}

void Shader::setBool(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(this->ID, name), (int)value);
}

void Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::setVector2f(const char* name, float x, float y) 
{
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::setVector2f(const char* name, const glm::vec2& value)
{
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::setVector3f(const char* name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::setVector3f(const char* name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const char* name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::setVector4f(const char* name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const char* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkErrors(unsigned int object, const std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_ERROR" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINKING_ERROR" << infoLog << std::endl;
		}
	}
}