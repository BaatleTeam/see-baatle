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
void DrawWBoard_Shoting(WindowParametres *WBoard, int cur_x, int cur_y, const PlayerShotBoard *board);
void DrawWBoard_Shoting_Default(WindowParametres *WBoard);

void moveCursor_Shooting(Board board, int *cursor_x_position, int *cursor_y_position, int key);


void markEMPTY(PlayerShotBoard board, int cursor_x_pos, int cursor_y_pos);
void markSHOTED(PlayerShotBoard board, int cursor_x_pos, int cursor_y_pos);
void markKILLED(PlayerShotBoard board, int cursor_x_pos, int cursor_y_pos);