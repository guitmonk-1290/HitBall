#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"

class ResourceLoader
{
public:

	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static Shader getShader(std::string name);
	static Texture loadTexture(const char* file, bool alpha, std::string name);
	static Texture getTexture(std::string name);
	static void clear();

private:

	ResourceLoader();
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static Texture loadTextureFromFile(const char* file, bool alpha);
};

