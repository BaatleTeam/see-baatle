#include "ships.h"
#include "table.h"
# define _XOPEN_SOURCE_EXTENDED 1

void initWindowsShooting(const ShipsInfo *Ships, const Board *board, 
                        WindowParametres *WBackGround, 
                        WindowParametres *WInfoPlayer, 
                        WindowParametres *WInfoComputer, 
                        WindowParametres *WBoardPlayer,
                        WindowParametres *WBoardComputer);

void DrawWInfo_Default(WindowParametres *WShipsPlayer);
void DrawWBoard_Default(WindowParametres *WBoard);