#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "shoot.h"
#include "shootWindows.h"
#include "menu.h"

void drawTitle_YouWin(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouLose(WINDOW* win_hello, int smbl); // Рисует приветствие.
// doSingleLineBorder


void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued);

void drawStaticEndGameWindow(enum playerEndGameStatus status);
void createWindowWithText(WindowParametres *wp, const char* text, short colorPair);
