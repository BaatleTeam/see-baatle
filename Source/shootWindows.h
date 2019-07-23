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

void DrawWInfo_Shoting(WindowParametres *WInfo, const PlayerStats *stats);
void DrawWInfo_Shoting_Default(WindowParametres *WShipsPlayer);
void DrawWBoard_Shoting(WindowParametres WBoard, const PlayerShotBoard board);
void DrawWBoard_Shoting_Default(WindowParametres WBoard);
void makeShot(ShipsInfo ShipsComputer, const PlayerShotBoard boardData, int cursor_x_pos, int cursor_y_pos);
bool checkShotPos(const PlayerShotBoard shotBoard, int cursor_x_pos, int cursor_y_pos);
bool isShipKilled(ship* ship, const PlayerShotBoard boardData);
void fillBoardNearKilledShip(const ship ship, PlayerShotBoard boardData);

void moveCursor_Shooting(Board board, int *cursor_x_position, int *cursor_y_position, int key);
void DrawCursor_Shoting(WindowParametres WBoard, int cur_x, int cur_y, bool isActive);


void markEMPTY(PlayerShotBoard board, int cursor_x_pos, int cursor_y_pos);
void markSHOTED(PlayerShotBoard board, int cursor_x_pos, int cursor_y_pos);
void markKILLED(PlayerShotBoard board, int cursor_x_pos, int cursor_y_pos);
bool isValidBoardCell(PlayerShotBoard board, int check_x, int check_y);