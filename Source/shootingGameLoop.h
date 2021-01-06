#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "shoot.h"
#include "shootWindows.h"
#include "ships.h"


void shootingGameLoop(ShipsInfo *ShipsPlayer, ShipsInfo *ShipsComputer, Board *BoardPlayer, Board *BoardComputer);