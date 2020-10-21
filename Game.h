#pragma once
#include "Headers.h"
#include "Input.h"

class Game {
protected:
	Game(HANDLE consoleHandle_, int* inputVar_) {
		consoleHandle = consoleHandle_;
		inputVar = inputVar_;
	}
	int Start() {}
	void SetScreen() {
		system("CLS");

		GetConsoleScreenBufferInfo(consoleHandle, &csbiScreenInfo);
		screenWidth = csbiScreenInfo.srWindow.Right;            //updating screen width
		screenHeigh = csbiScreenInfo.srWindow.Bottom;           //updating screen heigh
	}
	void gotoxy(short x, short y)
	{
		gotoCoord = { x, y };
		SetConsoleCursorPosition(consoleHandle, gotoCoord);
	}
	void StopUntilEnter() {
		int key;
		for (;;) {
			if (_kbhit() == 1)
			{
				key = _getch();
				if (key == ENTER || key == SPACE) {
					return;
				}
			}
			Sleep(10);
		}
	}
	enum ColorCode {
		BLUE = 1, GREEN = 2, RED = 4, INTENSITY = 8
	};

	HANDLE consoleHandle;
	int* inputVar;
	int input;
	short screenWidth, screenHeigh;
	COORD gotoCoord;
	CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;

};