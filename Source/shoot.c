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

bool isPlayerWins(const PlayerStats* const statisticsPlayer){
    int shipsNum = statisticsPlayer->shipCount[0][1] + statisticsPlayer->shipCount[1][1] + statisticsPlayer->shipCount[2][1] + statisticsPlayer->shipCount[3][1];
    if (shipsNum == statisticsPlayer->shipsDestroyed)
        return TRUE;
    else
        return FALSE;
}