#pragma once
#include <curses.h>
#include "ships.h"

void DrawDefaltArrangeWindow(WINDOW*);
int convert_size(int); // Преобразовывает текущий y в размер корабля

void changeActiveShip(int*, int*, const int);
void changeActiveShip_LeftKey(int*, int*);
void changeActiveShip_RightKey(int *x, int *y);
void changeActiveShip_UpKey(int*, int*);
void changeActiveShip_DownKey(int *x, int *y);

void draw_leftkey(WINDOW*, int, int);
void draw_rightkey(WINDOW *WIN, int y, int x);

void DrawNewNumberOfStandingShips(WINDOW *WIN, ship* ship, int*);
void DrawErrorMessage(WINDOW*);
void DrawStandingShips(WINDOW* WIN, ship* ship);
void DrawActiveShip(WINDOW* WIN, int y, int x);