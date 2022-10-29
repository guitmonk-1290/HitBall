#include "BallObject.h"

BallObject::BallObject()
	: GameObject(), radius(12.5f), onPaddle(true)
{}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite, float onpaddle)
	: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), radius(radius), onPaddle(true)
{}

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
	if (!onPaddle)
	{
		position += velocity * dt;

		if (position.x <= 0.0f)
		{
			velocity.x = -velocity.x;
			position.x = 0.0f;
		}
		else if (position.x + size.x >= window_width)
		{
			velocity.x = -velocity.x;
			position.x = window_width - size.x;
		}
		if (position.y <= 0.0f)
		{
			velocity.y = -velocity.y;
			position.y = 0.0f;
		}
	}
	return position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->onPaddle = true;
}