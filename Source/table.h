#pragma once
#include "includeCurses.h"
#include "ships.h"

extern int calculateArrangeHeight(const ShipsInfo *Ships);

void initWindowsParametres(const ShipsInfo *ships, const Board *board, WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*);
void DrawTableWindow(WindowParametres *Wship);
void DrawMainWindow(WindowParametres *Wmain);
int getTableWidth(const Board *board);
int getTableHeight(const Board *board);
void str_top(WINDOW*, int);
void str_line(WINDOW*, int, int);
void str_numb(WINDOW*, int, int);
void str_bottom(WINDOW*, int, int);

void doDoubleLineBorder(WINDOW* win_ptr);
void doSingleLineBorder(WINDOW* win_ptr);
