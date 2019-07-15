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
void DrawWBoard_Default(WindowParametres *WBoard);