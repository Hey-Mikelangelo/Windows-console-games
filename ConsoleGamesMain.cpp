#include <iostream>
#include <windows.h>
#include <conio.h>
#include "SnakeGame.h"
#include <cwchar>
#include <string>
#include "Menu.h"
using std::cout;
using std::endl;

typedef void(*FunctionPointer)();


//enum KeyCode {
//    W = 119, S = 115, A = 97, D = 100, SPACE = 32, ENTER = 13
//};
//void Init();
//void MenuMainLoop();
//void MenuGameSelectLoop();
//void Input();
//void SetMenuChoice();
//void DrawMenu(std::string MenuItems[]);
//void DrawMenuValuesLoop(std::string name, short& value, short step);
//void DrawMenuTitle(std::string title, int offset = 0);
//void DrawMenuLine(short xStart, short y, const char* line, short indx);
//void gotoxy(short x, short y);
//void cls();
//void WriteLine(short xStart, short y, const char* line);
//void Exit();
//void EndGameMenuLoop();
////void SetEndGameMenuLoopParams(int GameExitCode, int score, GameCode game);
//
//void SnakeMenuLoop();
//void SnakeRestart();
//void SnakeWidth();
//void SnakeDifficulty();
//void BackSnake();
//
//////global 
////HANDLE consoleHandle;
////CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
////CONSOLE_CURSOR_INFO cursorInfo;
////CONSOLE_FONT_INFOEX cfi;
//
////function
//COORD gotoCoord;
//short screenX, screenY, offset, placeToWrite, keyCode, menuItemCount;
//int menuChoice;
//char inp;
//bool hitEnter;
////EndGameMenuLoop funtion variables
//int gameExitCode, gameScore;
//GameCode gameCode;










////main menu
//void MenuGameSelectLoop();
//void Exit();
//
////game select menu
//void SnakeMenuLoop();
//void MenuGameSelectBack();
//
////
//void PlaySnake();
////void MenuOptionsSnake();
//void BackSnake();
////void SnakeWidth();
//void SnakeDifficulty();
//
////main menu
//FunctionPointer MenuMainButtonFunc[] = { MenuGameSelectLoop, Exit, Exit };
//
////game select menu
//FunctionPointer MenuGameSelectButtonFunc[] = { SnakeMenuLoop, MenuGameSelectBack };
//
////Snake Game menu
//FunctionPointer SnakeMenuButtonFunc[] = { PlaySnake, MenuOptionsSnake, BackSnake };
//
//short SnakeMenuWidth, SnakeMenuHeigh, SnakeMenuDifficulty, SnakeMenuScore;

//void MenuMainClose();
//void MenuGameSelectClose();
//void MenuSnakeMenuClose();
//void MenuOptionsClose();
//void MenuSnakeMenuClose();
//void SnakeMenuLoop();

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

Menu MenuMain(consoleHandle); //Main menu
Menu MenuGameSelect(consoleHandle);
Menu MenuOptions(consoleHandle);
Menu FuncMainClose;

/*Menu Game Select */ 
Menu MenuSnakeMenu(consoleHandle);
Menu FuncGameSelectClose;

/*Menu Options*/
Menu FuncOptions1;
Menu FuncOptions2;
Menu FuncOptionsClose;

/*Menu Snake Menu*/
Menu FuncSnakeGameStart;
Menu MenuSnakeOptions(consoleHandle);
Menu FuncSnakeMenuClose;

//Menu Snake Options
Menu FuncSnakeWidth;
Menu FuncSnakeHeigh;
Menu FuncSnakeDifficulty;
Menu FuncSnakeOptionsClose;


//Games
SnakeGame SnakeGameWindow(consoleHandle);
short snakeGameWidth = 12;
short snakeGameHeigh = 12; 
short snakeGameDifficulty = 10;


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
void MenuSnakeOptionsClose() {
    MenuSnakeOptions.close();
}
void SnakeMenuStart() {
    SnakeGameWindow.setWidthHeigh(snakeGameWidth, snakeGameHeigh);
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

int main()
{
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
    FuncSnakeOptionsClose.initFuntion(MenuSnakeOptionsClose);


    MenuMain.show();
    return 0;
}
#ifdef FUNTIONAL

void Init() {
    //main console
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!consoleHandle) {
        cout << "no console" << endl;
        return;
    }
    cls();
    //setting initial value
    menuChoice = 0;
    //Make cursor invisible
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);   //copying cursor info
    cursorInfo.bVisible = false;                        //changing cursor visibility to false
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);   //setting back cursor info

    GetConsoleScreenBufferInfo(consoleHandle, &csbiScreenInfo);
    screenX = csbiScreenInfo.srWindow.Right;
    screenY = csbiScreenInfo.srWindow.Bottom;
    //Snake game default variables
    SnakeMenuWidth = 15;
    SnakeMenuHeigh = 12;
    SnakeMenuDifficulty = 10;
}

void MenuSelectLoop(std::string title, std::string additTitle, std::string* MenuItems, int count = menuItemCount) {
    cls();
    hitEnter = false;
    menuChoice = 0;
    menuItemCount = count;

    do {
        Input();
        SetMenuChoice();
        DrawMenuTitle(title, 0);
        DrawMenuTitle(additTitle, 1);
        DrawMenu(MenuItems);
        Sleep(10);
    } while (!hitEnter);

}

void MenuMainLoop() {
    MenuSelectLoop("Main Menu", "", MenuMainButtons, 3);
    MenuMainButtonFunc[menuChoice]();
}
void SelectGameMenuLoop() {
    MenuSelectLoop("Select Game", "", MenuGameSelectButtons, 2);

    //if selected Snake Game, set params for snake game
    if (menuChoice == 0) SetEndGameMenuLoopParams(2, 0, Snake);

    MenuGameSelectButtonFunc[menuChoice]();

}
void EndGameMenuLoop() {
    int GameExitCode = gameExitCode;
    int score = gameScore;
    GameCode game = gameCode;

    std::string* MenuItems;
    std::string menuTitle;
    std::string menuStatus;
    std::string menuAdditStatus;
    FunctionPointer* MenuButtonFuntions;

    switch (game) {
        case Snake:
            SnakeMenuScore = score;
            menuItemCount = 3;
            MenuItems = SnakeMenuButtons;
            menuTitle = "Play Snake";
            MenuButtonFuntions = SnakeMenuButtonFunc;
            break;
        default:
            menuItemCount = 0;
            MenuItems = nullptr;
            MenuButtonFuntions = nullptr;
            break;
    }

    //Message
    switch (GameExitCode) {
    case 0:
        menuStatus = "You failed";
        menuAdditStatus = std::string("Score: ") + std::to_string(score);
        break;
    case 1:
        menuStatus = "You won!";
        menuAdditStatus = std::string("Score: ") + std::to_string(score);
        break;
    case 2:
        menuStatus = menuTitle;
        menuAdditStatus = "";
        break;
    }
    MenuSelectLoop(menuStatus, menuAdditStatus, MenuItems);
    MenuButtonFuntions[menuChoice]();
}

void Input() {
    keyCode = 0;
    if (_kbhit() == 1)
    {
        keyCode = _getch();
    }
    //cout << keyCode << endl;
}
void SetMenuChoice() {
    hitEnter = false;
    //if 's'
    if (keyCode == 115) {
        menuChoice = (menuChoice == menuItemCount - 1) ? 0 : (menuChoice + 1);
    }
    //if 'w'
    else if(keyCode == 119){
        menuChoice = (menuChoice == 0) ? menuItemCount - 1 : (menuChoice - 1);
    }
    //if "ENTER"
    else if (keyCode == 13) {
        hitEnter = true;
    }
    //if "SPACE"
    else if (keyCode == 32) {
        cls();

        GetConsoleCursorInfo(consoleHandle, &cursorInfo);   //copying cursor info
        cursorInfo.bVisible = false;                        //changing cursor visibility to false
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);   //setting back cursor info

        GetConsoleScreenBufferInfo(consoleHandle, &csbiScreenInfo);
        screenX = csbiScreenInfo.srWindow.Right;            //updating screen width
        screenY = csbiScreenInfo.srWindow.Bottom;           //updating screen heigh
    }
    gotoxy(0, 0);
}
void DrawMenuTitle(std::string title, int offsetY ){
    offset = (short)(title.length());
    placeToWrite = screenX / 2 - (offset / 2);
    SetConsoleTextAttribute(consoleHandle, 7);
    gotoxy(placeToWrite, (screenY / 2) - 3 + offsetY);
    cout << title;
}
void DrawMenuValues(std::string name, int menuItemIdx, int xOffset) {
    int yCoord = (screenY / 2) + menuItemIdx;
    int xCoord = screenX / 2 + xOffset;
    
}
void DrawMenu(std::string MenuItems[]) {
    static short i;
    i = 0;
    while(i < menuItemCount) {
        offset = (short)(MenuItems[i].length());
        placeToWrite = screenX / 2 - (offset / 2);
        DrawMenuLine(placeToWrite, (screenY / 2) + i, MenuItems[i].c_str(), i);
        i++;
    }
}
void ChangeMenuValue(short& value, short step, int prevInput) {
    if (prevInput == -1 && keyCode == S) {
        value -= step;
    }
    else  if (prevInput == -1 && keyCode == W) {
        value += step;
    }
}
void DrawMenuValuesLoop(std::string name, short& value, short step = 1) {
    hitEnter = false;
    int prevInput;
    do {
        prevInput = -1;
        Input();
        SetMenuChoice();
        ChangeMenuValue(value, step, prevInput);
        Sleep(20);
    } while (!hitEnter);
}
void DrawMenuLine(short xStart, short y, const char* line, short indx) {
    //FOREGROUND:  BLUE = 1, GREEN = 2, RED = 4, INTENSITY = 8
    if (indx == menuChoice) {
        SetConsoleTextAttribute(consoleHandle, 10);
        gotoxy(screenX / 2 - 10, y);
        cout << "->";
        gotoxy(xStart, y);
        cout << line;
    }
    else {
        SetConsoleTextAttribute(consoleHandle, 6);
        gotoxy(screenX / 2 - 10, y);
        cout << "  ";
        gotoxy(xStart, y);
        cout << line;
    }
   
}
inline void gotoxy(short x, short y)
{
    gotoCoord = { x, y };
    SetConsoleCursorPosition(consoleHandle, gotoCoord);
}
inline void WriteLine(short xStart, short y, const char* line) {
    gotoxy(xStart, y);
    cout << line << '\n';
}
inline void cls() {
    system("CLS");
}
void Exit() {
    cls();
}

void SnakeRestart() {
    SnakeMenuLoop();
}
void SnakeDifficulty() {
    MenuSelectLoop("Snake Game MenuOptions", "", SnakeMenuOptionsMenuBottons, 4);
    switch (menuChoice) {
        case 0:
            //change width
            DrawMenuValuesLoop("Width", SnakeMenuWidth);
            break;
        case 1:
            //change heigh
            DrawMenuValuesLoop("Heigh", SnakeMenuHeigh);
            break;
        case 2:
            //change difficulty
            DrawMenuValuesLoop("Difficulty", SnakeMenuDifficulty);
            break;
        case 3:
            SetEndGameMenuLoopParams(2, SnakeMenuScore, Snake);
            EndGameMenuLoop();
            break;
    }
}
void SnakeWidth() {

}
void BackSnake() {
    SelectGameMenuLoop();
}
void SetEndGameMenuLoopParams(int exitCode, int score, GameCode game) {
    gameExitCode = exitCode;
    gameScore = score;
    gameCode = game;
}
void SnakeMenuLoop() {
    cls();
    SetConsoleTextAttribute(consoleHandle, 3);
    hitEnter = false;
    SnakeMenu SnakeMenu(consoleHandle, SnakeMenuWidth, SnakeMenuHeigh, SnakeMenuDifficulty);
    int exitCode = SnakeMenu.Start();
    SetEndGameMenuLoopParams(exitCode, SnakeMenu.getScore(), Snake);
    EndGameMenuLoop();
}
#endif
