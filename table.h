#pragma once
#include <curses.h>

void DrawShipWindow(WINDOW*,int, int);
void str_line(WINDOW*,int, int);
void str_numb(WINDOW*,int, int);
void str_top(WINDOW*, int);
void str_bottom(WINDOW*, int, int);