#include <iostream>
#include <windows.h>
#include <conio.h>
#include "SnakeGame.h"
#include <cwchar>
#include <string>

using std::cout;
using std::endl;

typedef void(*FunctionPointer)();

enum GameCode {
    Snake
};

void Init();
void MainMenuLoop();
void SelectGameMenuLoop();
void Input();
void SetMenuChoice();
void DrawMenu(std::string MenuItems[]);
void DrawMenuTitle(std::string title, int offset);
void WriteMenuLine(short xStart, short y, const char* line, short indx);
void gotoxy(short x, short y);
void cls();
void WriteLine(short xStart, short y, const char* line);
void Exit();
void EndGameMenuLoop(int GameExitCode, int score, GameCode game);

void SnakeGameLoop();
void SnakeRestart();
void SnakeDifficulty();
void SnakeExit();

//global 
HANDLE consoleHandle;
CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
CONSOLE_CURSOR_INFO cursorInfo;
CONSOLE_FONT_INFOEX cfi;

//function
COORD gotoCoord;
short screenX, screenY, offset, placeToWrite, keyCode, menuItemCount;
int menuChoice;
char inp;
bool hitEnter;

std::string MainMenuItems[] = {"Select Game", "Options", "Exit"};
std::string GameMenuItems[] = { "Classic Snake", "Back" };

//Snake Game
std::string SnakeMenuItems[] = { "Play", "Difficulty", "Exit" };
std::string SnakeOptionsMenuItems[] = { "Width", "Heigh", "Difficulty", "Back" };
FunctionPointer SnakeButtonFunc[] = { SnakeRestart, SnakeDifficulty, SnakeExit };
short snakeGameWidth, snakeGameHeigh, snakeGameDifficulty, SnakeGameScore;



int main()
{
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    //ConsoleGames games = ConsoleGames(consoleHandle);
    
    Init();
    MainMenuLoop();
    return 0;
}

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
    snakeGameWidth = 15;
    snakeGameHeigh = 12;
    snakeGameDifficulty = 10;
}

void MainMenuLoop() {
    cls();
    hitEnter = false;
    menuItemCount = 3;
    menuChoice = 0;
   do{
        Input();
        SetMenuChoice();
        DrawMenuTitle("Main menu", 0);
        DrawMenu(MainMenuItems);
        Sleep(10);
   } while (!hitEnter);
   gotoxy(0, 0);
   cout << menuChoice;
   switch (menuChoice) {
   case 0: 
        //select game
        SelectGameMenuLoop();
        break;
   case 1: 
        //options
        break;
   case 2: 
        //exit
       Exit();
       break;
   default:
       break;
   }
}
void SelectGameMenuLoop() {
    cls();
    hitEnter = false;
    menuItemCount = 2;
    menuChoice = 0;
    do {
        Input();
        SetMenuChoice();
        DrawMenuTitle("Select Game", 0);
        DrawMenu(GameMenuItems);
        Sleep(10);
    } while (!hitEnter);
    switch (menuChoice) {
    case 0:
        //select game
        EndGameMenuLoop(2, 0, Snake);
        break;
    case 1:
        //Back
        MainMenuLoop();
        break;
    }
}
void EndGameMenuLoop(int GameExitCode, int score, GameCode game) {
    cls();
    hitEnter = false;
    menuChoice = 0;
    std::string* MenuItems;
    std::string menuTitle;
    std::string menuStatus;
    std::string menuStatusMore;
    FunctionPointer* MenuButtonFuntions;
    switch (game) {
    case Snake:
        SnakeGameScore = score;
        menuItemCount = 3;
        MenuItems = SnakeMenuItems;
        menuTitle = "Play Snake";
        MenuButtonFuntions = SnakeButtonFunc;
        break;
    default:
        menuItemCount = 0;
        MenuItems = nullptr;
        MenuButtonFuntions = nullptr;
        break;
    }

    //drawing Message
    if (GameExitCode == 0) {
        menuStatus = "You failed";
        menuStatusMore = std::string("Score: ") + std::to_string(score);
    }
    else if (GameExitCode == 1) {
        menuStatus = "You won!";
        menuStatusMore = std::string("Score: ") + std::to_string(score);

    }
    else if (GameExitCode == 2) {
        menuStatus = menuTitle;
        menuStatusMore = "";
    }

    do {
        Input();
        SetMenuChoice();
        DrawMenuTitle(menuStatus);
        DrawMenuTitle(menuStatusMore, 1);
        DrawMenu(MenuItems);
        Sleep(10);
    } while (!hitEnter);

    for (int i = 0; i < menuItemCount; i++) {
        if (menuChoice == i) {
            MenuButtonFuntions[i]();
            break;
        }
    }
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
void DrawMenuTitle(std::string title, int offsetY = 0){
    offset = (short)(title.length());
    placeToWrite = screenX / 2 - (offset / 2);
    SetConsoleTextAttribute(consoleHandle, 7);
    gotoxy(placeToWrite, (screenY / 2) - 3 + offsetY);
    cout << title;
}
void DrawMenu(std::string MenuItems[]) {
    static short i;
    i = 0;
    while(i < menuItemCount) {
        offset = (short)(MenuItems[i].length());
        placeToWrite = screenX / 2 - (offset / 2);
        WriteMenuLine(placeToWrite, (screenY / 2) + i, MenuItems[i].c_str(), i);
        i++;
    }
}
void WriteMenuLine(short xStart, short y, const char* line, short indx) {
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
    SnakeGameLoop();
}
void SnakeDifficulty() {
    cls();
    hitEnter = false;
    menuChoice = 0;
    menuItemCount = 4;
    //drawing Message
    DrawMenuTitle("Snake Game Options", 0);
    do {
        Input();
        SetMenuChoice();
        DrawMenu(SnakeOptionsMenuItems);
        Sleep(10);
    } while (!hitEnter);

    switch (menuChoice) {
        case 0:
            //change width
            break;
        case 1:
            //change heigh
            break;
        case 2:
            //change difficulty
            break;
        case 3:
            EndGameMenuLoop(2, SnakeGameScore, Snake);
            break;
    }
}
void SnakeExit() {
    SelectGameMenuLoop();
}
void SnakeGameLoop() {
    cls();
    SetConsoleTextAttribute(consoleHandle, 3);
    hitEnter = false;
    SnakeGame snakeGame(consoleHandle, snakeGameWidth, snakeGameHeigh, snakeGameDifficulty);
    int gameExitCode = snakeGame.Start();
    EndGameMenuLoop(gameExitCode, snakeGame.getScore(), Snake);
}

