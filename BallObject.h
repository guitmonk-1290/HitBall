#pragma once
#include "GameObject.h"

class BallObject : public GameObject
{
public:

	float radius;
	bool onPaddle;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite, float onpaddle = true);

	// Ball Actions
	glm::vec2 Move(float dt, unsigned int window_width);
	void	  reset(glm::vec2 position, glm::vec2 velocity);
};

