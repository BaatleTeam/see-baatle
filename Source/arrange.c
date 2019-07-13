#include "arrange.h"
#include "curses.h"

void DrawDefaltArrangeWindow(WindowParametres *Warr, const ShipsInfo *Ships){
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

int calculateArrangeHeight(const ShipsInfo *Ships){
    int Height = 0;
    Height = getMaxNumOfShips(Ships) * 2 + 6 + 4;
    return Height;
}

int getMaxNumOfShips(const ShipsInfo *ships){
    int max = ships->Number_1_Size;
    if (ships->Number_2_Size > max) max = ships->Number_2_Size;
    if (ships->Number_3_Size > max) max = ships->Number_3_Size;
    if (ships->Number_4_Size > max) max = ships->Number_4_Size;
    return max;
}

void initCurrActiveShip_Arrange(const ShipsInfo *Ships, int *currShipNumber, int *currShipSize){
    *currShipSize = 4;
    *currShipNumber = 0;
    while (!getNumberOfShipsFromSize(Ships, *currShipSize)){
        (*currShipSize)--;
        if (*currShipSize < 1) // = 5
            *currShipSize = 4;
    }
}

void changeActiveShip(const ShipsInfo *Ships, int *currNumber, int *currSize, const int key){
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

int getNumberOfShipsFromSize(const ShipsInfo *Ships, int size){
    switch (size){
        case 4: return Ships->Number_4_Size;
        case 3: return Ships->Number_3_Size;
        case 2: return Ships->Number_2_Size;
        case 1: return Ships->Number_1_Size;
        default: return -1;
    }
}

void changeActiveShip_UpKey(int maxNumber, int* currNumber){
    if (*currNumber == 0)
        *currNumber = maxNumber - 1;
    else 
        *currNumber -= 1;
}

void changeActiveShip_DownKey(int maxNumber, int* currNumber){
    if (*currNumber == maxNumber - 1)
        *currNumber = 0;
    else 
        *currNumber += 1;
}

void changeActiveShip_RightKey(const ShipsInfo *Ships, int* currNumber, int* currSize){
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

void changeActiveShip_LeftKey(const ShipsInfo *Ships, int* currNumber, int* currSize){
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

void DrawNewNumberOfStandingShips(WINDOW *WIN, ship* ship, int *number_stand_ships){
    int number_4 = 0;
    int number_3 = 0;
    int number_2 = 0;
    int number_1 = 0;

    for (int i = 0; i < 2; i++)
        if (ship[i].stand == TRUE)
            number_4++;
    for (int i = 2; i < 5; i++)
        if (ship[i].stand == TRUE)
            number_3++;
    for (int i = 5; i < 9; i++)
        if (ship[i].stand == TRUE)
            number_2++;
    for (int i = 9; i < 15; i++)
        if (ship[i].stand == TRUE)
            number_1++;
            
    wattron(WIN, COLOR_PAIR(2));
    mvwprintw(WIN, 5 , 25, "%d / 2", number_4);
    mvwprintw(WIN, 7 , 25, "%d / 3", number_3);
    mvwprintw(WIN, 9 , 25, "%d / 4", number_2);
    mvwprintw(WIN, 11 , 25, "%d / 6", number_1);
    *number_stand_ships = number_1 + number_2 + number_3 + number_4;
    mvwprintw(WIN, 13 , 6, "You have %d ships standing", *number_stand_ships);
    if (*number_stand_ships == 15){
        wattron(WIN, COLOR_PAIR(100));
        mvwprintw(WIN, 15 , 4, "You have arranged all your ships");
        mvwprintw(WIN, 17 , 7, "YOU CAN START THE BATTLE");
        mvwprintw(WIN, 19 , 9, "Press F2 to continue!");
    }
    wrefresh(WIN);
}

void DrawErrorMessage_InArrangeWindow(WINDOW* WIN){
    wattron(WIN, COLOR_PAIR(100));
    mvwprintw(WIN, 12 , 1, "Ships can`t stand near which other!");
    wattron(WIN, COLOR_PAIR(2));
    box(WIN, 0, 0);
    wrefresh(WIN);
}

void EraseErrorMessage_InArrangeWindow(WINDOW* WIN){
    wattron(WIN, COLOR_PAIR(100));
    for (int i = 0; i < WIN->_maxx; i++)
        mvwprintw(WIN, 12 , i, " ");
    wattron(WIN, COLOR_PAIR(2));
    box(WIN, 0, 0);
    wrefresh(WIN);
}

void DrawShips_InArangeWindow(WindowParametres *Warr, const ShipsInfo *Ships){
    char ch = 219;
    int type = 0;
    int x = 0;
    wattron(Warr->ptrWin, COLOR_PAIR(2));

    type = 4;
    x = 3;
    for (size_t number = 0, y = 6; number < Ships->Number_4_Size; number++, y+=2){
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));
        mvwprintw(Warr->ptrWin, y, x,"%c%c%c%c%c", ch, ch, ch, ch, ch);
    }

    
    type = 3;
    x += 9;
    for (size_t number = 0, y = 6; number < Ships->Number_3_Size; number++, y+=2){
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));        
        mvwprintw(Warr->ptrWin, y, x,"%c%c%c%c", ch, ch, ch, ch);
    }
    
    
    type = 2;
    x += 9;
    for (size_t number = 0, y = 6; number < Ships->Number_2_Size; number++, y+=2){
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));        
        mvwprintw(Warr->ptrWin, y, x,"%c%c%c%", ch, ch, ch);
    }

    
    type = 1;
    x += 9;
    for (size_t number = 0, y = 6; number < Ships->Number_1_Size; number++, y+=2){
        if (Ships->Ships[getIndex(Ships, number, type)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));
        mvwprintw(Warr->ptrWin, y, x,"%c%c",ch, ch);
    }

}

void DrawActiveShip_InArrangeWindow(WindowParametres* warr, int number, int size){
    char ch = 219;
    int y = 6 + number*2;
    int x = 0;
    switch (size){
        case 4: x =  3; break;
        case 3: x = 12; break;
        case 2: x = 21; break;
        case 1: x = 30; break;
        default: x = 0;
    }
    wattron(warr->ptrWin, COLOR_PAIR(100));
    for (int i = 0; i < size+1; i++, y)
        mvwprintw(warr->ptrWin, y, x+i,"%c", ch);
    wrefresh(warr->ptrWin);
}

int getIndex(const ShipsInfo *Ships, int number, int shipSize){
    switch(shipSize){
        case 4: return number;
        case 3: return Ships->Number_4_Size + number;
        case 2: return Ships->Number_4_Size + Ships->Number_3_Size + number;
        case 1: return Ships->Number_4_Size + Ships->Number_3_Size + Ships->Number_2_Size + number;
        default: return -1;
    }
}

void colorizeCurrShip(WINDOW* WIN, ship Ship){
    if (Ship.stand == TRUE){
        wattron(WIN, COLOR_PAIR(2));
        char rect = 254;
        int i = Ship.y;
        int j = Ship.x;

        switch(Ship.type){
            case TRUE:
                for (; i < Ship.size + Ship.y; i++){
                    mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
                }
                break;
            case FALSE:
                for (; j < Ship.size + Ship.x; j++){
                    mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
                }
                break;
        }
        wrefresh(WIN);
    }
}