#pragma once
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "arrange.h"
#include "shoot.h"

#include "table.h"
#include "ships.h"
void tmp_otladchik(bool field[10][15]);





int begin_coord(int curr_y, ship* ship, bool field[10][15]);
bool dop_begin(int x, int y, int size, bool field[10][15]);







void podchet_ships(bool number_ships_player[10][15], bool number_ships_comp[10][15]);
void refresh_shoot_player_gpaphics(WINDOW *WIN, int field[10][15], int y, int x);

bool check_ship_borders(int act_y, ship ship, bool field[10][15]);


void refresh_ship_player_array(ship* ship, bool field[10][15]);