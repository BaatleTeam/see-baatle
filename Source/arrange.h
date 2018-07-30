#pragma once
#include <curses.h>
#include "ships.h"

void DrawDefaltArrangeWindow(WindowParametres*,struct ShipsInfo);
int calculateArrangeHeight(struct ShipsInfo Ships);
int getMaxNumOfShips(struct ShipsInfo ships);

void changeActiveShip(struct ShipsInfo const Ship, int*, int*, const int);
void changeActiveShip_LeftKey(struct ShipsInfo const Ships, int* currNumber, int* currType);
void changeActiveShip_RightKey(struct ShipsInfo const Ships, int* currNumber, int* currType);
void changeActiveShip_DownKey(int maxNumber, int* currNumber);
void changeActiveShip_UpKey(int maxNumber, int* currNumber);

void DrawShips_InArangeWindow(WindowParametres *warr, struct ShipsInfo Ships);
void DrawActiveShip_InArrangeWindow(WindowParametres *warr, int number, int size);
void DrawNewNumberOfStandingShips(WINDOW *WIN, ship* ship, int*);
void DrawErrorMessage(WINDOW*);
void colorizeCurrShip(WINDOW* WIN, ship Ship);
// void DrawStandingShips(WINDOW* WIN, struct ShipsInfo);

int getIndex(struct ShipsInfo Ships, int number, int size);
int getNumberOfShipsFromSize(struct ShipsInfo const Ships, int size);


void initCurrActiveShip_Arrange(struct ShipsInfo, int *currShipNumber, int *currShipSize);
