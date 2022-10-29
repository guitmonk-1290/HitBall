#pragma once

#include "GameObject.h"
#include <iostream>
#include <vector>

class GameLevel
{
public:

	std::vector<GameObject> Bricks;

	GameLevel() {}

	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(spriteRenderer &renderer);

	// checks if the level is completed
	bool isCompleted();

private:

	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

