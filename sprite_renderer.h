#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class spriteRenderer
{
public:

	spriteRenderer(Shader& shader);
	~spriteRenderer();

	void drawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:

	Shader shader;
	unsigned int quadVAO;
	void initRenderData();
};

