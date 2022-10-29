#include "ResourceLoader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

std::map<std::string, Texture> ResourceLoader::Textures;
std::map<std::string, Shader> ResourceLoader::Shaders;

Shader ResourceLoader::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceLoader::getShader(std::string name)
{
	return Shaders[name];
}

Texture ResourceLoader::loadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture ResourceLoader::getTexture(std::string name) {
	return Textures[name];
}

Shader ResourceLoader::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	std::ifstream vShaderifStream;
	std::ifstream fShaderifStream;
	std::ifstream gShaderifStream;
	std::string vCode, fCode, gCode;

	vShaderifStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderifStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderifStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vShaderifStream.open(vShaderFile);
		fShaderifStream.open(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderifStream.rdbuf();
		fShaderStream << fShaderifStream.rdbuf();

		// close files
		vShaderifStream.close();
		fShaderifStream.close();

		vCode = vShaderStream.str();
		fCode = fShaderStream.str();

		if (gShaderFile != nullptr) {
			gShaderifStream.open(gShaderFile);
			std::stringstream gShaderStream; 
			gShaderStream << gShaderifStream.rdbuf();
			gShaderifStream.close();
			gCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_READ_UNSUCCESSFULLY!" << std::endl;
	}

	const char* vShaderSource = vCode.c_str();
	const char* fShaderSource = fCode.c_str();
	const char* gShaderSource = gCode.c_str();

	Shader shader;
	shader.compile(vShaderSource, fShaderSource, gShaderFile!=nullptr ? gShaderSource : nullptr);
	return shader;
}

Texture ResourceLoader::loadTextureFromFile(const char* file, bool alpha)
{
	Texture texture;
	if (alpha) {
		texture.internal_Format = GL_RGBA;
		texture.image_Format = GL_RGBA;
	}

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	// TODO: ERROR CHECKING DATA
	texture.generate(width, height, data);
	stbi_image_free(data);
	return texture;
}