#include "GameWindow.h"
#include "Game.h"

#include <iostream>

void GameWindow::setRunning(bool newRunning) {
	this->_running = newRunning;
}

bool GameWindow::getRunning() {
	return this->_running;
}

GLFWwindow* GameWindow::getWinObj() {
	return this->window;
}

GameWindow::GameWindow(bool running, const unsigned int width, const unsigned int height, const char* title = ""): _running(running), _width(width), _height(height), _title(title)
{
}

int GameWindow::createWindow() {
	this->window = glfwCreateWindow(this->_width, this->_height, this->_title, NULL, NULL);
	if (this->window == NULL) {
		return -1;
	}
	return 0;
}

void GameWindow::render(Game &game) {
	glClear(GL_COLOR_BUFFER_BIT);
	game.render();
	glfwSwapBuffers(this->window);
}

void GameWindow::update() {

}