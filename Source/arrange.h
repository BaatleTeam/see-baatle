#pragma once
#include <curses.h>
#include "ships.h"

void DrawDefaltArrangeWindow(WindowParametres*, const ShipsInfo*);
int calculateArrangeHeight(const ShipsInfo *Ships);
int getMaxNumOfShips(const ShipsInfo *ships);

void changeActiveShip(const ShipsInfo *Ship, int*, int*, const int);
void changeActiveShip_LeftKey(const ShipsInfo *Ships, int* currNumber, int* currType);
void changeActiveShip_RightKey(const ShipsInfo *Ships, int* currNumber, int* currType);
void changeActiveShip_DownKey(int maxNumber, int* currNumber);
void changeActiveShip_UpKey(int maxNumber, int* currNumber);

void DrawShips_InArangeWindow(WindowParametres *warr, const ShipsInfo *Ships);
void DrawActiveShip_InArrangeWindow(WindowParametres *warr, int number, int size);
void DrawNewNumberOfStandingShips(WINDOW *WIN, ship* ship, int*);
void DrawErrorMessage_InArrangeWindow(WINDOW*);
void EraseErrorMessage_InArrangeWindow(WINDOW*);
void colorizeCurrShip(WINDOW* WIN, ship Ship);
// void DrawStandingShips(WINDOW* WIN, const ShipsInfo);

int getIndex(const ShipsInfo *Ships, int number, int size);
int getNumberOfShipsFromSize(const ShipsInfo *Ships, int size);


void initCurrActiveShip_Arrange(const ShipsInfo*, int *currShipNumber, int *currShipSize);
