#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "shoot.h"
#include "shootWindows.h"
#include "ships.h"
#include "menu.h"


GameResults shootingGameLoop(ShipsInfo *ShipsPlayer, ShipsInfo *ShipsComputer, Board *BoardPlayer, Board *BoardComputer);
void freeDataAfterShootingLoop(ShipsInfo *ShipsPlayer, ShipsInfo *ShipsComputer, Board *BoardPlayer, Board *BoardComputer);