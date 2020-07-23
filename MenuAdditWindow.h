#pragma once
#include "Headers.h"
#include "Menu.h"

class MenuAdditWindow : public Menu {
public:
	MenuAdditWindow(HANDLE consoleHandle_, int* inputVar_) : Menu(consoleHandle_, inputVar_){
		initiated = false;
	}
	void init(std::string valueName_, int* value_, int menuButtonindex_, int min_, int max_, int step_ = 1) {
		valueName = valueName_;
		value = value_;
		menuButtonindex = menuButtonindex_;
		min = min_;
		max = max_;
		step = step_;
		initiated = true;
		changeChoice = 0;
		valueNameLength = (short)(valueName.length());
	}
	void show() {
		if (!initiated) {
			system("CLS");
			cout << this << " not initiated";
			return;
		}
		hitEnter = false;
		ChangeValueLoop();
	}
	
private:
	std::string valueName;
	int* value;
	int step, min, max;
	int changeChoice, valueNameLength, menuButtonindex;
	bool initiated, hitEnter;

	void setChangeChoice() {
		hitEnter = false;
		if (input == S) {
			if (*value - step < min) {
				*value = max;
			}
			else {
				*value -= step;
			}
		}
		else if (input == W) {
			if (*value + step > max) {
				*value = min;
			}
			else {
				*value += step;
			}
		}
		else if (input == ENTER) {
			hitEnter = true;
		}
		else {
			changeChoice = 0;
		}
	}
	void close() {
		DrawAdditWindow(true);
	}
	void DrawAdditWindow(bool erease = false) {
		short xToWrite;
		if (!erease) {
			SetConsoleTextAttribute(consoleHandle, RED | BLUE);
			xToWrite = screenWidth / 2 + 15 - (valueNameLength / 2) - 1;
			gotoxy(xToWrite, (screenHeigh / 2) + menuButtonindex);
			cout << "               ";
			xToWrite = screenWidth / 2 + 15;
			gotoxy(xToWrite, (screenHeigh / 2) - 2 + menuButtonindex);
			cout << "+";
			gotoxy(xToWrite, (screenHeigh / 2) + 2 + menuButtonindex);
			cout << "-";

			xToWrite = screenWidth / 2 + 15 - (valueNameLength / 2) - 1;
			gotoxy(xToWrite, (screenHeigh / 2) + menuButtonindex);
			cout << valueName << " "<< *value;
		}
		else {
			xToWrite = screenWidth / 2 + 15;
			gotoxy(xToWrite, (screenHeigh / 2) - 2 + menuButtonindex);
			cout << " ";
			gotoxy(xToWrite, (screenHeigh / 2) + 2 + menuButtonindex);
			cout << " ";

			xToWrite = screenWidth / 2 + 15 - (valueNameLength / 2) - 1;
			gotoxy(xToWrite, (screenHeigh / 2) + menuButtonindex);
			cout << "                                ";
		}
		
		
	}
	void ChangeValueLoop() {
		updateScreenDimInfo();
		anchorCorner();
		setChangeChoice();
		DrawAdditWindow();

		while (!hitEnter) {
			Input();
			gotoxy(0, 4);
			cout << input;
			if (input != -1) {
				anchorCorner();
				setChangeChoice();
				DrawAdditWindow();
			}
			Sleep(10);
		}
		close();
		
	}
};