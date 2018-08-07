#pragma once
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "arrange.h"
#include "shoot.h"

#include "table.h"
#include "ships.h"
#include "shootWindows.h"

void arrangingShips(ShipsInfo *ShipsPlayer, Board *BoardPlayer);
void showDebugFieid(struct Board Board);
void podchet_ships(bool number_ships_player[10][15], bool number_ships_comp[10][15]);
void refresh_shoot_player_gpaphics(WINDOW *WIN, int field[10][15], int y, int x);


bool isAllShipsStanding(ShipsInfo ships);
void drawShootWindows(WindowParametres *WMain, WindowParametres *WShip, WindowParametres *WShoot, const Indents *Indents);
