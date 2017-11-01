#pragma once
#include <curses.h>
#include "arrange.h"

typedef struct ship_player
{
    int x;
    int y;
    bool type;
    bool stand;
} ship;


void standing_ship(int act_y, ship ship, bool field[10][15]);
void deleting_ship(int act_y, ship ship, bool field[10][15]);
void refresh_ship_player_gpaphics(WINDOW *WIN, bool field[10][15]);

bool check_border_left(ship ship);
bool check_border_right(int, ship ship);
bool check_border_top(ship ship);
bool check_border_bot(int,ship ship);

bool check_ship_left(int cur_y, ship ship, bool field[10][15]);
bool check_ship_right(int cur_y, ship ship, bool field[10][15]);
bool check_ship_top(int act_y, ship ship, bool field[10][15]);
bool check_ship_bot(int act_y, ship ship, bool field[10][15]);

int convert_ship_index(int, int);
int convert_index_to_active_y(int index);

bool check_ship_borders_top_bottom_horizontal(int size_of_ship, ship ship, bool field[10][15]);
bool check_ship_borders_left_right_horizontal(int size_of_ship, ship ship, bool field[10][15]);
bool check_ship_borders_left_right_vertical(int size_of_ship, ship ship, bool field[10][15]);
bool check_ship_borders_top_bottom_vertical(int size_of_ship, ship ship, bool field[10][15]);

void refresh_number_arrange(WINDOW *WIN, ship* ship, int*);
