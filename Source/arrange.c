#include "arrange.h"

static int getMaxNumOfShips(const ShipsInfo *ships);
static void changeActiveShip_LeftKey(const ShipsInfo *Ships, int* currNumber, int* currType);
static void changeActiveShip_RightKey(const ShipsInfo *Ships, int* currNumber, int* currType);
static void changeActiveShip_DownKey(int maxNumber, int* currNumber);
static void changeActiveShip_UpKey(int maxNumber, int* currNumber);

static int getNumberOfShipsFromSize(const ShipsInfo *Ships, int size);


void DrawDefaultArrangeWindow(WindowParametres *Warr, const ShipsInfo *Ships){
    wbkgdset(Warr->ptrWin, COLOR_PAIR(2));
    wclear(Warr->ptrWin);
    box(Warr->ptrWin, 0, 0);

    mvwprintw(Warr->ptrWin, 1, 9, " ARRANGE YOUR SHIPS! ");
    mvwprintw(Warr->ptrWin, 4,  2, " 4-size");
    mvwprintw(Warr->ptrWin, 4, 10, " 3-size");
    mvwprintw(Warr->ptrWin, 4, 19, " 2-size");
    mvwprintw(Warr->ptrWin, 4, 28, " 1-size");

    DrawShips_InArangeWindow(Warr, Ships);

    if (!Ships->Number_4_Size){
        mvwprintw(Warr->ptrWin, 6, 5, "NO");
        mvwprintw(Warr->ptrWin, 8, 4, "SHIPS");
    }

    if (!Ships->Number_3_Size){
        mvwprintw(Warr->ptrWin, 6, 13, "NO");
        mvwprintw(Warr->ptrWin, 8, 12, "SHIPS");
    }


    wrefresh(Warr->ptrWin);
}

int calculateArrangeHeight(const ShipsInfo *Ships) {
    // TODO 
    return getMaxNumOfShips(Ships) * 2 + 6 + 4;
}

int getMaxNumOfShips(const ShipsInfo *ships) {
    int max = ships->Number_1_Size;
    if (ships->Number_2_Size > max) max = ships->Number_2_Size;
    if (ships->Number_3_Size > max) max = ships->Number_3_Size;
    if (ships->Number_4_Size > max) max = ships->Number_4_Size;
    return max;
}

void initCurrActiveShip_Arrange(const ShipsInfo *Ships, int *currShipNumber, int *currShipSize) {
    *currShipSize = 4;
    *currShipNumber = 0;
    while (!getNumberOfShipsFromSize(Ships, *currShipSize)){
        (*currShipSize)--;
        if (*currShipSize < 1) // = 5
            *currShipSize = 4;
    }
}

void changeActiveShip(const ShipsInfo *Ships, int *currNumber, int *currSize, const int key) {
    switch (key){
        case KEY_LEFT:
            changeActiveShip_LeftKey(Ships, currNumber, currSize);
            break;
        case KEY_RIGHT:
            changeActiveShip_RightKey(Ships, currNumber, currSize);
            break;
        case KEY_UP:
            changeActiveShip_UpKey(getNumberOfShipsFromSize(Ships, *currSize), currNumber);
            break;
        case KEY_DOWN:
            changeActiveShip_DownKey(getNumberOfShipsFromSize(Ships, *currSize), currNumber);
            break;
        default: 
            break;
    }
}

int getNumberOfShipsFromSize(const ShipsInfo *Ships, int size) {
    switch (size){
        case 4: return Ships->Number_4_Size;
        case 3: return Ships->Number_3_Size;
        case 2: return Ships->Number_2_Size;
        case 1: return Ships->Number_1_Size;
        default: return -1;
    }
}

void changeActiveShip_UpKey(int maxNumber, int* currNumber) {
    if (*currNumber == 0)
        *currNumber = maxNumber - 1;
    else 
        *currNumber -= 1;
}

void changeActiveShip_DownKey(int maxNumber, int* currNumber) {
    if (*currNumber == maxNumber - 1)
        *currNumber = 0;
    else 
        *currNumber += 1;
}

void changeActiveShip_RightKey(const ShipsInfo *Ships, int* currNumber, int* currSize) {
    int maxNumber = 0;
    do {
        (*currSize)--;
        if (*currSize < 1) // = 0
            *currSize = 4;
        maxNumber = getNumberOfShipsFromSize(Ships, *currSize);
    }
    while (!maxNumber);
    if (*currNumber > maxNumber - 1)
        *currNumber = maxNumber - 1;
}

void changeActiveShip_LeftKey(const ShipsInfo *Ships, int* currNumber, int* currSize) {
    int maxNumber = 0;
    do {
        (*currSize)++;
        if (*currSize > 4) // = 5
            *currSize = 1;
        maxNumber = getNumberOfShipsFromSize(Ships, *currSize);
    }
    while (!maxNumber);
    if (*currNumber > maxNumber - 1)
        *currNumber = maxNumber - 1;
}

void DrawShips_InArangeWindow(WindowParametres *Warr, const ShipsInfo *Ships) {
    int ch = 219;
    int type = 0;
    int x = 0;
    wattron(Warr->ptrWin, COLOR_PAIR(2));

    type = 4;
    x = 3;
    for (int number = 0, y = 6; number < Ships->Number_4_Size; number++, y+=2) {
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));
        mvwprintw(Warr->ptrWin, y, x,"%c%c%c%c%c", ch, ch, ch, ch, ch);
    }

    
    type = 3;
    x += 9;
    for (int number = 0, y = 6; number < Ships->Number_3_Size; number++, y+=2) {
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));        
        mvwprintw(Warr->ptrWin, y, x,"%c%c%c%c", ch, ch, ch, ch);
    }
    
    
    type = 2;
    x += 9;
    for (int number = 0, y = 6; number < Ships->Number_2_Size; number++, y+=2) {
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));        
        mvwprintw(Warr->ptrWin, y, x,"%c%c%c%", ch, ch, ch);
    }

    
    type = 1;
    x += 9;
    for (int number = 0, y = 6; number < Ships->Number_1_Size; number++, y+=2) {
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));
        mvwprintw(Warr->ptrWin, y, x,"%c%c",ch, ch);
    }

}

int getIndex(const ShipsInfo *Ships, int number, int shipSize) {
    switch (shipSize) {
        case 4: return number;
        case 3: return Ships->Number_4_Size + number;
        case 2: return Ships->Number_4_Size + Ships->Number_3_Size + number;
        case 1: return Ships->Number_4_Size + Ships->Number_3_Size + Ships->Number_2_Size + number;
        default: return -1;
    }
}