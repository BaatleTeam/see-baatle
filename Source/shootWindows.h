#include "ships.h"
#include "table.h"
#include "shoot.h"
# define _XOPEN_SOURCE_EXTENDED 1

void initWindowsShooting(const ShipsInfo *Ships, const Board *board, 
                        WindowParametres *WBackGround, 
                        WindowParametres *WInfoPlayer, 
                        WindowParametres *WInfoComputer, 
                        WindowParametres *WBoardPlayer,
                        WindowParametres *WBoardComputer);

void updateGraphics_Shoting(WindowParametres WBoard, const ShotBoard board, int cursor_x_pos, int cursor_y_pos);
void DrawWInfo_Shoting(WindowParametres *WInfo, const PlayerStats *stats);
void DrawWInfo_Shoting_Default(WindowParametres *WShipsPlayer);
void DrawWBoard_Shoting(WindowParametres WBoard, const ShotBoard board);
void DrawWBoard_Shoting_Default(WindowParametres WBoard);
ShotResult makeShot(ShipsInfo ShipsComputer, const ShotBoard boardData, int cursor_x_pos, int cursor_y_pos);
bool checkShotPos(const ShotBoard shotBoard, int cursor_x_pos, int cursor_y_pos);
bool isShipKilled(ship* ship, const ShotBoard boardData);
void fillBoardNearKilledShip(const ship ship, ShotBoard boardData);

// stats and info-window
void updateStats(PlayerStats *stats_1, PlayerStats *stats_2, ShotResult shotResult);

void moveCursor_Shooting(Board board, int *cursor_x_position, int *cursor_y_position, int key);
void DrawCursor_Shoting(WindowParametres WBoard, int cur_x, int cur_y, bool isActive);


void markEMPTY(ShotBoard board, int cursor_x_pos, int cursor_y_pos);
void markSHOTED(ShotBoard board, int cursor_x_pos, int cursor_y_pos);
void markKILLED(ShotBoard board, int cursor_x_pos, int cursor_y_pos);
bool isValidBoardCell(ShotBoard board, int check_x, int check_y);