#pragma once
#include <curses.h>
#include <stdlib.h>

enum ShootBoardState {
	EMPTY = 0, // пусто
	SHOTED, // точка
	KILLED // крестик
};

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

// Поле для храненя и отображения выстрелови и попаданий игрока игрока
typedef struct PlayerShotBoard {
	unsigned Height;
	unsigned Width;
	enum ShootBoardState **board;
} PlayerShotBoard;

typedef struct ShotResult {
	bool isHit; // попал или нет
	int shipSize; // если потопил полностью - размер потопленого корабля. Иначе - 0
} ShotResult;


void choosing_comp_strategy(bool ship_comp_field[10][15]);

void change_x_leftkey(int* x);
void change_x_rightkey(int* x);
void change_y_topkey(int* y);
void change_y_botkey(int* y);

bool check_border_left_shoot(int y, int x, int field[10][15]);
bool check_border_right_shoot(int y, int x, int field[10][15]);
bool check_border_bot_shoot(int y, int x, int field[10][15]);
bool check_border_top_shoot(int y, int x, int field[10][15]);