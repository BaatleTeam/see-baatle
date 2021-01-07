#include "shoot.h"

void drawShootWindows(WindowParametres *WMain, WindowParametres *WShip, WindowParametres *WShoot, const Indents *Indents){
    delwin(WMain->ptrWin);
    WMain->Width = Indents->LeftIndent + 2*WShip->Width + Indents->BetweenIndent + Indents->RightIndent;
    WMain->Height = Indents->TopIndent + WShip->Height + Indents->BottomIndent;
    WMain->ptrWin = newwin(WMain->Height, WMain->Width, WMain->Begin_y, WMain->Begin_x);

    resize_term(WMain->Height, WMain->Width);
    DrawMainWindow(WMain);

    delwin(WShip->ptrWin);
    WShip->ptrWin = newwin(WShip->Height, WShip->Width, WShip->Begin_y, WShip->Begin_x);
    DrawTableWindow(WShip);

    delwin(WShoot->ptrWin);
    WShoot->ptrWin = newwin(WShoot->Height, WShoot->Width, WShoot->Begin_y, WShoot->Begin_x);
    DrawTableWindow(WShoot);
}

void initCoordiante(Coordinate *coord, int x, int y){
    coord->x = x;
    coord->y = y;
}


void initPlayerStats(PlayerStats *stats, const ShipsInfo* shipsInfo) {
    stats->hits = 0;
    stats->shots = 0;
    stats->shipsDestroyed = 0;
    stats->shipCount[3][1] = shipsInfo->Number_4_Size;
    stats->shipCount[3][0] = shipsInfo->Number_4_Size;
    stats->shipCount[2][1] = shipsInfo->Number_3_Size;
    stats->shipCount[2][0] = shipsInfo->Number_3_Size;
    stats->shipCount[1][1] = shipsInfo->Number_2_Size;
    stats->shipCount[1][0] = shipsInfo->Number_2_Size;
    stats->shipCount[0][1] = shipsInfo->Number_1_Size;
    stats->shipCount[0][0] = shipsInfo->Number_1_Size;
}

int countDestryedShips(const PlayerStats* stats) {
    return stats->shipCount[0][1] + stats->shipCount[1][1] + stats->shipCount[2][1] + stats->shipCount[3][1];
}

bool isPlayerWins(const PlayerStats* const stats){
    int shipsNum = countDestryedShips(stats);
    if (shipsNum == stats->shipsDestroyed)
        return TRUE;
    else
        return FALSE;
}



void initShotBoard(ShotBoard* shotBoard, const Board *board) {
    enum ShootBoardState **boardArrayPlayer = malloc(board->Height * sizeof(enum ShootBoardState*));
    for (int i = 0; i < board->Height; i++)
        boardArrayPlayer[i] = calloc(board->Width, sizeof(enum ShootBoardState));
    
    shotBoard->Height = board->Height;
    shotBoard->Width = board->Width;
    shotBoard->board = boardArrayPlayer;
}