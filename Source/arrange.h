#pragma once
#include <curses.h>
#include "ships.h"

void DrawDefaltArrangeWindow(WINDOW*,struct ShipsInfo);

void changeActiveShip(struct ShipsInfo Ship,int*, int*, const int);
void changeActiveShip_LeftKey(int maxNumber, int *x, int *y);
void changeActiveShip_DownKey(int maxNumber, int *x, int *y);
void changeActiveShip_RightKey(int maxNumber, int *x, int *y);
void changeActiveShip_UpKey(int maxNumber, int *x, int *y);

void DrawNewNumberOfStandingShips(WINDOW *WIN, ship* ship, int*);
void DrawErrorMessage(WINDOW*);
void DrawStandingShips(WINDOW* WIN, struct ShipsInfo);
void DrawActiveShip(WINDOW* WIN, int y, int x);

int getIndex(int lineY, int currNumber, struct ShipsInfo Ships);
int getSize(int); // Преобразовывает текущий y в размер корабля
int getMaxNumberOfShipsFromSize(struct ShipsInfo Ships, int lineY);