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


void drawTitle_YouWin(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouLose(WINDOW* win_hello, int smbl); // Рисует приветствие.
// doSingleLineBorder


void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued);

void drawStaticEndGameWindow(enum playerEndGameStatus status);
WindowString createWindowString(WindowParametres wp, const char* text, int begin_x, int begin_y);
void drawWindowString(WindowString win, short colorPair);
