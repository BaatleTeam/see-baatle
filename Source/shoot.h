#pragma once
#include <curses.h>
#include <stdlib.h>

typedef struct PlayerStats {
	// [0] - на плаву, [1] - всего
	unsigned ship_4[2];
	unsigned ship_3[2];
	unsigned ship_2[2];
	unsigned ship_1[2];

	unsigned shots;
	unsigned hits;
    unsigned shipsDestroyed;
} PlayerStats;

void choosing_comp_strategy(bool ship_comp_field[10][15]);

void change_x_leftkey(int* x);
void change_x_rightkey(int* x);
void change_y_topkey(int* y);
void change_y_botkey(int* y);

bool check_border_left_shoot(int y, int x, int field[10][15]);
bool check_border_right_shoot(int y, int x, int field[10][15]);
bool check_border_bot_shoot(int y, int x, int field[10][15]);
bool check_border_top_shoot(int y, int x, int field[10][15]);