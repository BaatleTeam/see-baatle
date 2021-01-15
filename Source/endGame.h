#pragma once
#include "includeCurses.h"
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
WindowString createDerWindowString(WINDOW *parent, WindowParametres wp, const char* text, int begin_x, int begin_y);
void clearWindowString(WindowString* win);
void drawWindowString(const WindowString* win, short colorPair);
void tuneWindowString(WindowParametres *wp, const char *text, int *begin_x, int *begin_y);
