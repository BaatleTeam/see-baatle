#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "shoot.h"
#include "shootWindows.h"
#include "menu.h"

typedef struct WindowString {
    WindowParametres wp;
    int string_begin_y;
    int string_begin_x;
    const char* string;
} WindowString;

typedef enum userChoice_EndGame {
    END_GAME, CONTINUE_GAME, CHOICE_ENDGAME_UNKNOWN
} userChoice_EndGame;

void drawTitle_YouWin(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouLose(WINDOW* win_hello, int smbl); // Рисует приветствие.
// doSingleLineBorder


void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued);
void iniEndGameWindows(WindowParametres *win_bg, WindowParametres *win_title, WindowString *win_enter, WindowString *win_anykey, enum playerEndGameStatus status);
void drawEndGameBgWindows(const WindowParametres *win_bg, const WindowParametres *win_title, enum playerEndGameStatus status);
void drawEndGameDynamicWindows(const WindowString *win_enter, const WindowString *win_anykey, userChoice_EndGame choice);


// --------------  WindowString methods ----------------------------
WindowString createWindowString(WindowParametres wp, const char* text, int begin_x, int begin_y);
void cleanWindowString(WindowString* win);
void drawWindowString(const WindowString* win, short colorPair);
