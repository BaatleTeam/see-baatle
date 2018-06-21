#pragma once
#include <curses.h>
#include "ships.h"

void initWindowsParametres(struct Board board, WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*);
void DrawTableWindow(WindowParametres *Wship);
void DrawMainWindow(WindowParametres *Wmain);
void str_top(WINDOW*, int);
void str_line(WINDOW*,int, int);
void str_numb(WINDOW*,int, int);
void str_bottom(WINDOW*, int, int);