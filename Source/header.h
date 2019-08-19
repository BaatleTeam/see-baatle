#pragma once
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "arrange.h"
#include "shoot.h"

#include "table.h"
#include "ships.h"
#include "shootWindows.h"

typedef struct Pair {
    int first;
    int second;
} Pair;
typedef struct Border {
    Pair pair_x;
    Pair pair_y;
} Border;

void arrangingShips_player(ShipsInfo *ShipsPlayer, Board *BoardPlayer);
void arrangingShips_computer(ShipsInfo *ShipsComputer, Board *BoardComputer);
void chooseFilling(Border borders[4], int width, int height); // выбирает стратегию порядка проверки
bool tryToStandShip(ship* ship, Board* board, int start_x, int end_x, int start_y, int end_y);
void swapBorders(Pair* a, Pair* b);


void drawShootWindows(WindowParametres *WMain, WindowParametres *WShip, WindowParametres *WShoot, const Indents *Indents);
