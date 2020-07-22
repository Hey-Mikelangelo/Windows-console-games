#pragma once
#include <windows.h>
#include <iostream>
#include <array>
#include <random>
#include "Menu.h"
typedef void(*FunctionPointer)();

class SnakeGame{
public:
	SnakeGame(HANDLE consoleHandle_) {
		consoleHandle = consoleHandle_;
	}
	//returning 0 if fail, 1 if win
	int Start() {
		SetScreen();
		CalculateOffsets();
		DrawWalls();
		InitSnake();
		return SnakeGameLoop();
	}
	void setWidthHeigh(short width_, short heigh_) {
		width = width_;
		heigh = heigh_;
	}
	void setDifficulty(short difficulty_) {
		difficulty = difficulty_;
	}
	short getScore() {
		return score;
	}
	short getWidth() {
		return width;
	}
	short getHeigh() {
		return heigh;
	}
	short getDifficulty() {
		return difficulty;
	}
private:
	short  width = 10, heigh = 10, difficulty = 10;
	HANDLE consoleHandle;
	std::array<COORD, 100> Snake;
	short score, lenght, offsetX, offsetY, screenX, screenY, trailX, trailY, movDir;
	int keyCode, foodX, foodY;;
	bool alive, ateFood;
	COORD gotoCoord;
	CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
	
	void Draw(short coordX, short coordY, char symbol) {
		SetConsoleCursorPosition(consoleHandle, {coordX * 2 + offsetX, coordY + offsetY });
		std::cout << symbol;
	}

	void SetScreen() {
		system("CLS");

		GetConsoleScreenBufferInfo(consoleHandle, &csbiScreenInfo);
		screenX = csbiScreenInfo.srWindow.Right;            //updating screen width
		screenY = csbiScreenInfo.srWindow.Bottom;           //updating screen heigh
	}
	void CalculateOffsets() {
		offsetX = screenX/2 - width;
		offsetY = (screenY / 2) - (heigh / 2);
	}
	void DrawWalls() {
		SetConsoleTextAttribute(consoleHandle, 3);
		for (int i = 0; i < width; i++) {
			Draw(i, 0, '#');
		}
		for (int i = 1; i < heigh-1; i++) {
			Draw(0, i, '#');
			Draw(width-1, i, '#');
		}
		for (int i = 0; i < width; i++) {
			Draw(i, heigh-1, '#');
		}
	}
	void InitSnake() {
		score = 0;
		lenght = 3;
		alive = true;
		movDir = 97;
		Snake[0] = {width / 2, heigh / 2};
		Snake[1] = { width / 2 + 1, heigh / 2 };
		Snake[2] = { width / 2 + 2, heigh / 2 };
		keyCode = 97; //input 'a'
		CreateFood();
	}
	void Input() {
		if (_kbhit() == 1)
		{
			keyCode = _getch();
			//if 'w' after 's'
			if (movDir == 115 && keyCode == 119) {
				keyCode = movDir;
			}
			//if 's' after 'w'
			else if (movDir == 119 && keyCode == 115) {
				keyCode = movDir;
			}
			//if 'a' after 'd'
			else if (movDir == 100 && keyCode == 97) {
				keyCode = movDir;
			}
			//if 'd' after 'a'
			else if (movDir == 97 && keyCode == 100) {
				keyCode = movDir;
			}
			else if(keyCode != 97 && keyCode != 100 && keyCode != 115 && keyCode != 119) {
				keyCode = movDir;
			}
			gotoxy(5, 8);
		}
	}
	void CreateFood() {
		bool ok = false;
		while (!ok) {
			foodX = rand() % (width-2) + 1;
			foodY = rand() % (heigh-2) + 1;
			if (CheckForSnake(foodX, foodY)) {
				ok = true;
			}
		}		
	}
	//true if there is no snake in coords
	bool CheckForSnake(int x, int y, int startIdx = 0) {
		for (int i = 0 + startIdx; i < lenght; i++) {
			if (Snake[i].X == x && Snake[i].Y == y) {
				return false;
			}
		}
		return true;
	}
	inline void gotoxy(short x, short y)
	{
		gotoCoord = { x, y };
		SetConsoleCursorPosition(consoleHandle, gotoCoord);
	}
	void MoveBody() {
		trailX = Snake[lenght - 1].X;
		trailY = Snake[lenght - 1].Y;
		for (int i = lenght-1; i > 0; i--) {
			Snake[i] = Snake[i - 1];
		}
	}
	void MoveSnake() {
		movDir = keyCode;
		MoveBody();
		switch (movDir) {
			//A
			case 97: 
				Snake[0] = { Snake[0].X - 1, Snake[0].Y };
				break;
			//D
			case 100:
				Snake[0] = { Snake[0].X + 1, Snake[0].Y };
				break;
			//W
			case 119:
				Snake[0] = { Snake[0].X, Snake[0].Y - 1};
				break;
			//S
			case 115:
				Snake[0] = { Snake[0].X, Snake[0].Y + 1 };
				break;

		}
	}
	void AddSegment() {
		Snake[lenght] = { trailX, trailY };
		lenght++;
	}
	void CheckSnake() {
		if (Snake[0].X <= 0 || Snake[0].X >= width-1 || Snake[0].Y <= 0 || Snake[0].Y >= heigh-1) {
			alive = false;
		}
		else if (Snake[0].X == foodX && Snake[0].Y == foodY) {
			ateFood = true;
			score++;
			CreateFood();
			AddSegment();
		}
		int headX, headY;
		headX = Snake[0].X;
		headY = Snake[0].Y;
		if (!CheckForSnake(headX, headY, 1)) {
			alive = false;
		}
	}
	void DrawDynObj() {
		if (alive) {
			SetConsoleTextAttribute(consoleHandle, 5);
			Draw(foodX, foodY, '@');

			SetConsoleTextAttribute(consoleHandle, 4);
			Draw(Snake[0].X, Snake[0].Y, 'O');
			SetConsoleTextAttribute(consoleHandle, 12);
			for (int i = 1; i < lenght; i++) {
				Draw(Snake[i].X, Snake[i].Y, 'O');
			}
			if (!ateFood) {
				Draw(trailX, trailY, ' ');	//ereasing trail
			}
			else {
				ateFood = false;			//if ate foot - leaving trail (new tail segment)
			}
		}
	}
	void StopUntilEnter() {
		int key;
		for (;;) {
			if (_kbhit() == 1)
			{
				key = _getch();
				if (key == 13) {
					return;
				}
			}
			Sleep(10);
		}
	}
	int SnakeGameLoop() {
		int i = 0;
		do {
			Input();
			if (i * difficulty >= 100) {
				i = 0;
				MoveSnake();
				CheckSnake();
				DrawDynObj();
			}
			Sleep(10);
			i++;
			if (lenght >= 100) {
				break;
			}
		} while (alive);
		StopUntilEnter();
		if (!alive) {
			return 0;
		}
		else {
			return 1;
		}
		
	}

};