#pragma once

#include <iostream>
#include "GameObject.h"
#include "GameLevel.h"

#include <GLFW/glfw3.h>

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Game
{
private:
	enum GameState {
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};

public:
	GameState State;
	bool keys[1024];
	unsigned int width, height;
	std::vector<GameLevel> levels;
	unsigned int level;

	Game(unsigned int width, unsigned int height);
	~Game();

	// load all shaders/textures/levels
	void Init();

	void processInput(float dt);
	void DoCollisions();
	void update(float dt);
	void render();
};

