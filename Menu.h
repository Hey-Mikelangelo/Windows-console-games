#pragma once
#include "Headers.h"
#include "Input.h"
using std::cout;

typedef void(*FunctionPointer)();
class Menu {

public:
	Menu() {
		initiated = false;
	}
	Menu(HANDLE consoleHandle_, int* inputVar_)
	{
		consoleHandle = consoleHandle_;
		initiated = false;
		inputVar = inputVar_;
	}
	void initFuntion(FunctionPointer Function_) {
		Function = Function_;
		isAFunction = true;
		initiated = true;
	}
	void initMenu(std::string menuTitle_, std::string* MenuButtons_,
		Menu** MenuLinks_, short buttonCount_, std::string menuSubTitle_ = "")
	{
		title = menuTitle_;
		Buttons = MenuButtons_;
		MenuLinks = MenuLinks_;
		buttonCount = buttonCount_;
		subtitle = menuSubTitle_;

		titleLength = (short)(title.length());
		menuChoice = 0;
		hitEnter = false;
		closeMenu = false;
		isAFunction = false;
		initiated = true;
	}
	void show() {
		if (!initiated) {
			system("CLS");
			cout << this << " not initiated";
			return;
		}
		closeMenu = false;
		if (!isAFunction) {
			while (!closeMenu) {
				resizeScreen();
				cls();
				menuSelectLoop();
			}
		}
		else {
			if (Function == nullptr) {
				cls();
				cout << "No function";
			}
			else {
				Function();
			}
		}
	}
	void close() {
		closeMenu = true;
	}
	void changeSubTitle(std::string newSubTitle) {
		subtitle = newSubTitle;
	}
protected:
	HANDLE consoleHandle;
	int input, *inputVar;
	short screenWidth, screenHeigh;

	

	enum ColorCode {
		BLUE = 1, GREEN = 2, RED = 4, INTENSITY = 8
	};

	void cls() {
		system("CLS");
	}
	void gotoxy(short x, short y)
	{
		static COORD gotoCoord;
		gotoCoord = { x, y };
		SetConsoleCursorPosition(consoleHandle, gotoCoord);
	}
	void anchorCorner() {
		gotoxy(0, 0);
		cout << " ";
	}

	void Input() {
		input = -1;
		if (_kbhit() == 1)
		{
			input = _getch();
			input = *inputVar;
		}
	}
	void updateScreenDimInfo() {
		CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &csbiScreenInfo);
		screenWidth = csbiScreenInfo.srWindow.Right;
		screenHeigh = csbiScreenInfo.srWindow.Bottom;
	}
private:
	std::string title;
	std::string* Buttons;
	Menu** MenuLinks;
	FunctionPointer* ButtonFunc;
	short buttonCount;
	std::string subtitle;

	short menuChoice;
	short subtitleLength, titleLength;
	
	bool hitEnter, closeMenu, isAFunction, initiated;
	FunctionPointer Function;

	void resizeScreen() {
		CONSOLE_CURSOR_INFO cursorInfo;
		//Make cursor invisible
		GetConsoleCursorInfo(consoleHandle, &cursorInfo);   //copying cursor info
		cursorInfo.bVisible = false;                        //changing cursor visibility to false
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);   //setting back cursor info

		updateScreenDimInfo();
	}
	
	void setMenuChoice() {
		hitEnter = false;
		//if 's'
		if (input == S) {
			menuChoice = (menuChoice == buttonCount - 1) ? 0 : (menuChoice + 1);
		}
		//if 'w'
		else if (input == W) {
			menuChoice = (menuChoice == 0) ? buttonCount - 1 : (menuChoice - 1);
		}
		//if "ENTER"
		else if (input == ENTER) {
			hitEnter = true;
		}
		//if "SPACE"
		else if (input == SPACE) {
			cls();
			resizeScreen();
			drawTitle();
			drawSubtitle();
		}
	}
	
	void drawTitle() {
		short xToWrite = screenWidth / 2 - (titleLength / 2);
		SetConsoleTextAttribute(consoleHandle, 7);
		gotoxy(0, 0);
		gotoxy(xToWrite, (screenHeigh / 2) - 3);
		cout << title;
	}
	void drawSubtitle() {
		subtitleLength = (short)(subtitle.length());
		short xToWrite = screenWidth / 2 - (subtitleLength / 2);
		SetConsoleTextAttribute(consoleHandle, 7);
		gotoxy(xToWrite, (screenHeigh / 2) - 2);
		cout << subtitle;
	}
	void drawMenuButtons() {
		for (short i = 0; i < buttonCount; i++) {
			short length = (short)(Buttons[i].length());
			short x = screenWidth / 2 - (length / 2);
			short y = screenHeigh / 2 + i;

			if (i == menuChoice) {
				SetConsoleTextAttribute(consoleHandle, GREEN | INTENSITY);
				gotoxy(screenWidth / 2 - 10, y);
				cout << "->";
				gotoxy(x, y);
				cout << Buttons[i];

			}
			else {
				SetConsoleTextAttribute(consoleHandle, RED | GREEN);
				gotoxy(screenWidth / 2 - 10, y);
				cout << "  ";
				gotoxy(x, y);
				cout << Buttons[i];
			}
		}
	}
	void menuSelectLoop() {
		drawTitle();
		drawSubtitle();
		do {
			anchorCorner();
			Input();
			setMenuChoice();
			drawMenuButtons();
			Sleep(10);
		} while (!hitEnter);
		//ButtonFunc[menuChoice]();
		(MenuLinks[menuChoice])->show();
	}

};