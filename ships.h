#pragma once
#include <curses.h>

typedef struct ship_player
{
    int x;
    int y;
    int size;
    bool type;
    bool stand;
} ship;

extern int convert_size(int);
void standing_ship(int act_y, ship* ship, bool field[10][15]);
void deleteShipFromField(ship* ship, bool field[10][15]);
void refresh_ship_player_gpaphics(WINDOW *WIN, bool field[10][15]);

bool checkBorderLeft(ship* ship);
bool checkBorderRight(ship* ship);
bool checkBorderTop(ship* ship);
bool checkBorderBot(ship* ship);

bool check_ship_left(int cur_y, ship ship, bool field[10][15]);
bool check_ship_right(int cur_y, ship ship, bool field[10][15]);
bool check_ship_top(int act_y, ship ship, bool field[10][15]);
bool check_ship_bot(int act_y, ship ship, bool field[10][15]);

int convert_ship_index(int, int);
int convert_index_to_active_y(int index);

bool check_ship_borders_top_bottom_horizontal(ship* ship, bool field[10][15]);
bool check_ship_borders_left_right_horizontal(ship* ship, bool field[10][15]);
bool check_ship_borders_left_right_vertical(ship* ship, bool field[10][15]);
bool check_ship_borders_top_bottom_vertical(ship* ship, bool field[10][15]);
bool checkItself(ship* ship, bool field[10][15]);