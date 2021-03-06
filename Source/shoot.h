#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "ships.h"

enum ShootBoardState {
	EMPTY = 0, // пусто
	SHOTED, // точка
	KILLED // крестик
};

typedef struct PlayerStats {
	// [size][0] - на плаву, [size][1] - всего
	int shipCount[4][2];

	int shots;
	int hits;
    int shipsDestroyed;
} PlayerStats;

// Поле для храненя и отображения выстрелови и попаданий игрока
typedef struct ShotBoard {
	int Height;
	int Width;
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


// PlayerStats methods
int countDestryedShips(const PlayerStats* stats);
void initPlayerStats(PlayerStats *stats, const ShipsInfo* shipsInfo);
bool isPlayerWins(const PlayerStats* stats);


// ShotBoard methods
void initShotBoard(ShotBoard* shotBoard, const Board *board);
void clearShotBoard(ShotBoard* shotBoard);

