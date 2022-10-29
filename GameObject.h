#pragma once

#include "sprite_renderer.h"
#include "Texture.h"

class GameObject
{
public:

	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	bool isSolid;
	bool Destroyed;
	Texture sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(spriteRenderer& renderer);
};

