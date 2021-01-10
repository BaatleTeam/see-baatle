#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "menu.h"

typedef struct WindowString {
    WindowParametres wp;
    int string_begin_y;
    int string_begin_x;
    const char* string;
} WindowString;

void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued);


// --------------  WindowString methods ----------------------------
WindowString createWindowString(WindowParametres wp, const char* text, int begin_x, int begin_y);
void cleanWindowString(WindowString* win);
void drawWindowString(const WindowString* win, short colorPair);
