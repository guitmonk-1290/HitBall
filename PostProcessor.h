#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "sprite_renderer.h"
#include "Shader.h"

class PostProcessor
{
public:

	Shader shader;
	Texture texture;
	unsigned int width, height;

	bool confuse, chaos, shake;

	PostProcessor() {};
	PostProcessor(Shader shader, unsigned int width, unsigned int height);

	void BeginRender();
	void EndRender();
	void Render(float time);

private:

	unsigned int MSFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;

	void initRenderData();
};

