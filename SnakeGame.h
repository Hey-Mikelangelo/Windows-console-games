#pragma once
#include "Headers.h"
#include <array>
#include <random>
#include "Menu.h"
#include "Input.h"

typedef void(*FunctionPointer)();



class SnakeGame{
public:
	SnakeGame(HANDLE consoleHandle_, int* inputVar_) {

		consoleHandle = consoleHandle_;
		inputVar = inputVar_;
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
	short lenght, offsetX, offsetY, screenX, screenY, trailX, trailY, movDir, movDir1, movDir2;
	int input, *inputVar, foodX, foodY, score;;
	bool alive, ateFood, inputToFirstDir, useFirstInp;
	COORD gotoCoord;
	CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
	
	enum ColorCode {
		BLUE = 1, GREEN = 2, RED = 4, INTENSITY = 8
	};

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
		Snake[0] = {width / 2, heigh / 2};
		Snake[1] = { width / 2 + 1, heigh / 2 };
		Snake[2] = { width / 2 + 2, heigh / 2 };
		movDir1 = A;
		movDir2 = A;
		input = A;
		useFirstInp = true;
		inputToFirstDir = true;
		CreateFood();
	}
	void Input() {
		if (_kbhit() == 1)
		{
			input = _getch();
			input = *inputVar;
			//if 'w' after 's'
			if (movDir1 == S && input == W) {
				input = movDir1;
			}
			//if 's' after 'w'
			else if (movDir1 == W && input == S) {
				input = movDir1;
			}
			//if 'a' after 'd'
			else if (movDir1 == D && input == A) {
				input = movDir1;
			}
			//if 'd' after 'a'
			else if (movDir1 == A && input == D) {
				input = movDir1;
			}
			else if(input != A && input != D && input != W && input != S) {
				input = movDir1;
			}

			if (inputToFirstDir) {
				movDir1 = input;
				movDir2 = input;
				useFirstInp = true;
				inputToFirstDir = false;
			}
			else {
				movDir2 = input;
				inputToFirstDir = true;
			}
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
		if (useFirstInp) {
			movDir = movDir1;
			useFirstInp = false;
		}
		else {
			movDir = movDir2;
			movDir1 = movDir2;
			useFirstInp = true;
			inputToFirstDir = true;
		}
		MoveBody();
		switch (movDir) {
			case A: 
				Snake[0] = { Snake[0].X - 1, Snake[0].Y };
				break;
			case D:
				Snake[0] = { Snake[0].X + 1, Snake[0].Y };
				break;
			case W:
				Snake[0] = { Snake[0].X, Snake[0].Y - 1};
				break;
			case S:
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