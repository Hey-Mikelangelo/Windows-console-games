#include "Headers.h"
#include "SnakeGame.h"
#include "Menu.h"
#include "MenuAdditWindow.h"
#include "deps/wininput/src/wininput.hpp"

using std::cout;
using std::endl;

typedef void(*FunctionPointer)();

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

int currentInput;
bool isKeyPressed;

Menu MenuMain(consoleHandle, &currentInput); //Main menu
Menu MenuGameSelect(consoleHandle, &currentInput);
Menu MenuOptions(consoleHandle, &currentInput);
Menu FuncMainClose;

/*Menu Game Select */ 
Menu MenuSnakeMenu(consoleHandle, &currentInput);
Menu FuncGameSelectClose;

/*Menu Options*/
Menu FuncOptions1;
Menu FuncOptions2;
Menu FuncOptionsClose;

/*Menu Snake Menu*/
Menu FuncSnakeGameStart;
Menu MenuSnakeOptions(consoleHandle, &currentInput);
Menu FuncSnakeMenuClose;

//Menu Snake Options
Menu FuncSnakeWidth;
Menu FuncSnakeHeigh;
Menu FuncSnakeDifficulty;
Menu FuncSnakeOptionsClose;

//Games
SnakeGame SnakeGameWindow(consoleHandle, &currentInput);
int snakeGameWidth = 12;
int snakeGameHeigh = 12;
int snakeGameDifficulty = 10;


std::string MenuMainButtons[] = { "Select Game", "Options", "Exit" };
Menu* MenuMainLinks[] = { &MenuGameSelect, &MenuOptions/*&MenuOptions*/,&FuncMainClose };

std::string MenuOptionsButtons[] = { "Nothing to do", "No Options", "Back"}; 
Menu* MenuOptionsLinks[] = { &FuncOptions1, &FuncOptions2, &FuncOptionsClose };

std::string MenuGameSelectButtons[] = { "Classic Snake", "Back" };
Menu* MenuGameSelectLinks[] = { &MenuSnakeMenu, &FuncGameSelectClose };

std::string MenuSnakeMenuButtons[] = { "Play", "Options", "Back" };
Menu* MenuSnakeMenuLinks[] = { &FuncSnakeGameStart, &MenuSnakeOptions/*&MenuSnakeOptions*/, &FuncSnakeMenuClose };

std::string MenuSnakeOptionsButtons[] = { "Width", "Heigh", "Difficulty", "Back" };
Menu* MenuSnakeOptionsLinks[] = { &FuncSnakeWidth, &FuncSnakeHeigh, &FuncSnakeDifficulty, &FuncSnakeOptionsClose };



void MenuMainClose() {
    MenuMain.close();
}
void MenuGameSelectClose() {
    MenuGameSelect.close();
}
void MenuOptionsClose() {
    MenuOptions.close();
}
void MenuSnakeMenuClose() {
    MenuSnakeMenu.changeSubTitle("");
    MenuSnakeMenu.close();
}

void AdditMenu(std::string varName, int* var, int menuButtonIdx, int min, int max, int step = 1) {
    MenuAdditWindow AdditWindow(consoleHandle, &currentInput);
    AdditWindow.init(varName, var, menuButtonIdx, min, max, step);
    AdditWindow.show();
}
void MenuSnakeOptionsWidth() {
    AdditMenu("", &snakeGameWidth, 0, 5, 30);
}
void MenuSnakeOptionsHeigh() {
    AdditMenu("", &snakeGameHeigh, 1, 5, 30);
}
void MenuSnakeOptionsDifficulty() {
    AdditMenu("", &snakeGameDifficulty, 2, 5, 100, 5);
}
void MenuSnakeOptionsClose() {
    MenuSnakeOptions.close();
}
void SnakeMenuStart() {
    SnakeGameWindow.setWidthHeigh((short)snakeGameWidth, (short)snakeGameHeigh);
    SnakeGameWindow.setDifficulty(snakeGameDifficulty);
    if (SnakeGameWindow.Start() == 0) {
        std::string subtitle = "You lose. Score: " + std::to_string(SnakeGameWindow.getScore());
        MenuSnakeMenu.changeSubTitle(subtitle);
        FuncSnakeGameStart.close();
    }
    else {
        std::string subtitle = "You won. Score: " + std::to_string(SnakeGameWindow.getScore());
        MenuSnakeMenu.changeSubTitle(subtitle);
        FuncSnakeGameStart.close();
    }

}


bool keyHandler(input::KeyData& data) {
    if (data.type == INPUT_TYPE_KEYDOWN) {
        currentInput = (int)data.code;
        isKeyPressed = true;
    }
    else {
        isKeyPressed = false;
    }
    return false;
}

int main()
{
    input::addKeyHandler(keyHandler);

    /*Menu main*/
    MenuMain.initMenu("Main menu", MenuMainButtons, MenuMainLinks, 3);
    FuncMainClose.initFuntion(MenuMainClose);

    //Menu game select
    MenuGameSelect.initMenu("Select a game", MenuGameSelectButtons, MenuGameSelectLinks, 2);
    FuncGameSelectClose.initFuntion(MenuGameSelectClose);

    //Menu options
    MenuOptions.initMenu("Options", MenuOptionsButtons, MenuOptionsLinks, 3);
    FuncOptionsClose.initFuntion(MenuOptionsClose);

    //Menu Snake Menu
    MenuSnakeMenu.initMenu("Snake Game", MenuSnakeMenuButtons, MenuSnakeMenuLinks, 3);
    FuncSnakeGameStart.initFuntion(SnakeMenuStart);
    MenuSnakeOptions.initMenu("Snake Game Options", MenuSnakeOptionsButtons, MenuSnakeOptionsLinks, 4);
    FuncSnakeMenuClose.initFuntion(MenuSnakeMenuClose);

    //Menu Snake Options
    FuncSnakeWidth.initFuntion(MenuSnakeOptionsWidth);
    FuncSnakeHeigh.initFuntion(MenuSnakeOptionsHeigh);
    FuncSnakeDifficulty.initFuntion(MenuSnakeOptionsDifficulty);
    FuncSnakeOptionsClose.initFuntion(MenuSnakeOptionsClose);


    MenuMain.show();
    return 0;
}
