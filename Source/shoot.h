#pragma once
#include <curses.h>
#include <stdlib.h>
#include "ships.h"

enum ShootBoardState {
	EMPTY = 0, // пусто
	SHOTED, // точка
	KILLED // крестик
};

typedef struct PlayerStats {
	// [size][0] - на плаву, [size][1] - всего
	unsigned shipCount[4][2];

	unsigned shots;
	unsigned hits;
    unsigned shipsDestroyed;
} PlayerStats;

// Поле для храненя и отображения выстрелови и попаданий игрока
typedef struct ShotBoard {
	unsigned Height;
	unsigned Width;
	enum ShootBoardState **board;
} ShotBoard;

typedef struct ShotResult {
	bool isHit; // попал или нет
	int shipSize; // если потопил полностью - размер потопленого корабля. Иначе - 0
} ShotResult;

typedef struct Coordinate {
	int x;
	int y;
} Coordinate;

typedef enum DIR {NO_DIR = -1, UP = 0, RIGHT, DOWN, LEFT} DIR;

extern void DrawMainWindow(WindowParametres *Wmain);
extern void DrawTableWindow(WindowParametres *Wship);
void initCoordiante(Coordinate *coord, int x, int y);
void drawShootWindows(WindowParametres *WMain, WindowParametres *WShip, WindowParametres *WShoot, const Indents *Indents);
bool isPlayerWins(const PlayerStats* const statisticsPlayer);