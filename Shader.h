#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// program ID
	unsigned int ID;

	Shader();
	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	// activate/use the shader
	Shader&	use();

	// utility functions 
	void	setBool		(const char* name, bool value);
	void	setInt		(const char* name, int value);
	void	setFloat	(const char* name, float value);
	void	setVector2f (const char* name, float x, float y);
	void	setVector2f (const char* name, const glm::vec2& value);
	void    setVector3f (const char* name, float x, float y, float z);
	void	setVector3f (const char* name, const glm::vec3& value);
	void	setVector4f (const char* name, float x, float y, float z, float w);
	void	setVector4f (const char* name, const glm::vec4& value);
	void	setMatrix4  (const char* name, const glm::mat4& matrix);

private:

	// print error logs
	void checkErrors(unsigned int object, const std::string type);
};

