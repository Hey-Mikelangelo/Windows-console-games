#pragma once
//#include "Game.h"

class Pacman : public Game {
	Pacman(HANDLE consoleHandle_, int* inputVar_) : Game(consoleHandle_, inputVar_) {

		consoleHandle = consoleHandle_;
		inputVar = inputVar_;
	}
	int Start() {
		SetScreen();

	}
};