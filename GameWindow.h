#pragma once

#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GameWindow
{
private:
	GLFWwindow* window;
	bool _running;
	GLfloat _width;
	GLfloat _height;
	const char* _title;

public:

	void setRunning(bool newRunning);
	bool getRunning();
	GLFWwindow* getWinObj();

	GameWindow(bool running, const unsigned int width, const unsigned int height, const char* title);

	int createWindow();
	void render(Game &game);
	void update();
};

