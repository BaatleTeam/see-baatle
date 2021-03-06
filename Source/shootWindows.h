#include "ships.h"
#include "table.h"
#include "shoot.h"
# define _XOPEN_SOURCE_EXTENDED 1

extern FILE* db_out;

void initWindowsShooting(const Board *board, 
                         WindowParametres *WBackGround, 
                         WindowParametres *WInfoPlayer, 
                         WindowParametres *WInfoComputer, 
                         WindowParametres *WBoardPlayer,
                         WindowParametres *WBoardComputer);

void updateGraphics_Shoting(WindowParametres WBoard, const ShotBoard board, Coordinate curPos);
void DrawWInfo_Shoting(WindowParametres *WInfo, const PlayerStats *stats);
void DrawWInfo_Shoting_Default(WindowParametres *WShipsPlayer);
void DrawWBoard_Shoting(WindowParametres WBoard, const ShotBoard board);
void DrawWBoard_Shoting_Default(WindowParametres WBoard);
ShotResult makeShot(ShipsInfo ShipsComputer, const ShotBoard boardData, Coordinate curPos);
bool checkShotPos(const ShotBoard shotBoard, Coordinate curPos);
bool isShipKilled(Ship* ship, const ShotBoard boardData);
void fillBoardNearKilledShip(const Ship ship, ShotBoard boardData);

// stats and info-window
void updateStats(PlayerStats *stats_1, PlayerStats *stats_2, ShotResult shotResult);

void moveCursor_Shooting(Board board, Coordinate *curPos, int key);
void DrawCursor_Shoting(WindowParametres WBoard, Coordinate curPos, bool isActive);


void markEMPTY(ShotBoard board, Coordinate curPos);
void markSHOTED(ShotBoard board, Coordinate curPos);
void markKILLED(ShotBoard board, Coordinate curPos);
bool isValidBoardCell(ShotBoard board, Coordinate checkPos);


// ai
// enum PrevShootStatus {PAST, HIT_NO_DIRECT, HIT_HORIZ, HIT_VERT, HIT_LAST} sas;
Coordinate generateShotCoordinate(const ShotBoard* const, Coordinate, const PlayerStats* const);
Coordinate getRandomCoordinate(const ShotBoard* const boardData);
Coordinate getDirPerspectiveCoordinate(const ShotBoard* const boardData, Coordinate prevShot, Coordinate firstHit, bool (*dirs)[4], DIR priorityDIR, DIR* prevDIR);

int numberOfPerspectiveDirs(const bool dirs[]);
DIR getRandomDirect(const bool dirs[]);
Coordinate getCoordinateOfDir(DIR dir, const ShotBoard* const boardData, Coordinate prevShot);