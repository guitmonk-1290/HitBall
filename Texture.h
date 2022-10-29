#pragma once

#include <glad/glad.h>

class Texture
{
public:

	unsigned int ID;

	unsigned int width, height;

	unsigned int internal_Format;
	unsigned int image_Format;

	// texture configuration
	unsigned int wrap_s;
	unsigned int wrap_t;
	unsigned int filter_min;
	unsigned int filter_max;

	// constructor
	Texture();

	// methods
	void generate(unsigned int width, unsigned int height, unsigned char* data);
	void bind() const;
};

