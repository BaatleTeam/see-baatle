#pragma once
#include <curses.h>
#include "ships.h"

void DrawDefaltArrangeWindow(WINDOW*);
int convert_size(int); // Преобразовывает текущий y в размер корабля
void check_left(int*, int);
void check_right(int *x, int y);
void draw_leftkey(WINDOW*, int, int);
void draw_rightkey(WINDOW *WIN, int y, int x);

void DrawNewNumberOfStandingShips(WINDOW *WIN, ship* ship, int*);