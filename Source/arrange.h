#pragma once
// #include <ncurses.h>
#include "includeCurses.h"
#include <stdlib.h>
#include "ships.h"
#include "table.h"

typedef struct Pair {
    int first;
    int second;
} Pair;
typedef struct Border {
    Pair pair_x;
    Pair pair_y;
} Border;


bool arrangingShips_ByPlayer(CorePlayerGameData *playerGameData);
void arrangingShips_ByComputer(CorePlayerGameData *playerGameData);
int calculateArrangeHeight(const ShipsInfo *Ships);


void initCurrActiveShip_Arrange(const ShipsInfo*, int *currShipNumber, int *currShipSize);
void DrawDefaultArrangeWindow(WindowParametres*, const ShipsInfo*);
void changeActiveShip(const ShipsInfo *Ship, int*, int*, const int);
void DrawShips_InArangeWindow(WindowParametres *warr, const ShipsInfo *Ships);
int getIndex(const ShipsInfo *Ships, int number, int size);
