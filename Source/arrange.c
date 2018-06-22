#include "arrange.h"
#include "curses.h"

void DrawDefaltArrangeWindow(WindowParametres *Warr, struct ShipsInfo Ships){
    wbkgdset(Warr->ptrWin, COLOR_PAIR(2));
    wclear(Warr->ptrWin);
    box(Warr->ptrWin, 0, 0);
    mvwprintw(Warr->ptrWin, 1, 9, " ARRANGE YOUR SHIPS! ");

    DrawShips_InArangeWindow(Warr, Ships);
  
    //mvwprintw(Warr->ptrWin, 1, 2, "Use ARROWS to move.");
    // mvwprintw(Warr->ptrWin, 2, 2, "Use ENTER to choose the ship.");
    // mvwprintw(Warr->ptrWin, 3, 2, "Use TAB to change the orientation.");
    // mvwprintw(Warr->ptrWin, 3, 2, "");
    // mvwprintw(Warr->ptrWin, 5, 2, "%d ships: ", Ships.Number_4_Size);
    // mvwprintw(Warr->ptrWin, 7, 2, "%d ships: ", Ships.Number_3_Size);
    // mvwprintw(Warr->ptrWin, 9, 2, "%d ships: ", Ships.Number_3_Size);
    // mvwprintw(Warr->ptrWin, 11,2, "%d ships: ", Ships.Number_1_Size);
    // for (int i = 0, x = 12; i < Ships.Number_4_Size; i++, x+=5)
    //     mvwprintw(Warr->ptrWin,5,x," %c%c%c%c",ch,  ch, ch, ch);
    // for (int i = 0, x = 12; i < Ships.Number_3_Size; i++, x+=4)
    //     mvwprintw(Warr->ptrWin,7,x," %c%c%c",ch, ch, ch);
    // for (int i = 0, x = 12; i < Ships.Number_2_Size; i++, x+=3)
    //     mvwprintw(Warr->ptrWin,9,x," %c%c",ch,ch);
    // for (int i = 0, x = 12; i < Ships.Number_1_Size; i++, x+=2)
    //     mvwprintw(Warr->ptrWin,11,x," %c",ch);    
    wrefresh(Warr->ptrWin);
}

int getSize(int y){
    switch (y){
        case 5:  return 4;
        case 7:  return 3;
        case 9:  return 2;
        case 11: return 1;
    } 
}

void changeActiveShip(struct ShipsInfo const Ships, int *currNumber, int *currSize, const int key){
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

int getNumberOfShipsFromSize(struct ShipsInfo const Ships, int size){
    switch (size){
        case 4: return Ships.Number_4_Size;
        case 3: return Ships.Number_3_Size;
        case 2: return Ships.Number_2_Size;
        case 1: return Ships.Number_1_Size;
    }
}

void changeActiveShip_UpKey(int maxNumber, int* currNumber){
    if (*currNumber == 1)
        *currNumber = maxNumber;
    else 
        *currNumber -= 1;
}

void changeActiveShip_DownKey(int maxNumber, int* currNumber){
    if (*currNumber == maxNumber)
        *currNumber = 1;
    else 
        *currNumber += 1;
}

void changeActiveShip_RightKey(struct ShipsInfo const Ships, int* currNumber, int* currSize){
    do {
        (*currSize)++;
        if (*currSize > 4) // = 5
            *currSize = 0;
    }
    while (!getNumberOfShipsFromSize(Ships, *currSize));
}

void changeActiveShip_LeftKey(struct ShipsInfo const Ships, int* currNumber, int* currSize){
    do {
        (*currSize)--;
        if (*currSize < 1) // = 0
            *currSize = 4;
    }
    while (!getNumberOfShipsFromSize(Ships, *currSize));
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

void DrawErrorMessage(WINDOW* WIN){
    wattron(WIN, COLOR_PAIR(100));
    mvwprintw(WIN, 15 , 1, "Ships can not stand near which other!");
    wattron(WIN, COLOR_PAIR(2));
    box(WIN, 0, 0);
    wrefresh(WIN);
}

void DrawShips_InArangeWindow(WindowParametres *Warr, struct ShipsInfo Ships){
    char ch = 219;
    int type = 0;
    wattron(Warr->ptrWin, COLOR_PAIR(2));

    mvwprintw(Warr->ptrWin, 4, 2, " 4-size");
    type = 4;
    for (size_t number = 0, y = 6; number < Ships.Number_4_Size; number++, y+=2){
        if (Ships.Ships[getIndex(Ships, type, number)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));
        mvwprintw(Warr->ptrWin,y,2," %c%c%c%c%c",ch, ch, ch, ch, ch);
    }

    mvwprintw(Warr->ptrWin, 4, 10, " 3-size");
    for (size_t number = 0, y = 6; number < Ships.Number_3_Size; number++, y+=2){
        if (Ships.Ships[getIndex(Ships, type, number)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));        
        mvwprintw(Warr->ptrWin,y,11," %c%c%c%c",ch, ch, ch, ch);
    }
    
    mvwprintw(Warr->ptrWin, 4, 19, " 2-size");
    for (size_t number = 0, y = 6; number < Ships.Number_2_Size; number++, y+=2){
        if (Ships.Ships[getIndex(Ships, type, number)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));        
        mvwprintw(Warr->ptrWin,y,20," %c%c%c%",ch, ch, ch);
    }

    mvwprintw(Warr->ptrWin, 4, 28, " 1-size");
    for (size_t number = 0, y = 6; number < Ships.Number_1_Size; number++, y+=2){
                if (Ships.Ships[getIndex(Ships, type, number)].stand == TRUE)
            wattron(Warr->ptrWin, COLOR_PAIR(55));
        else
            wattron(Warr->ptrWin, COLOR_PAIR(2));
        mvwprintw(Warr->ptrWin,y,29," %c%c",ch, ch);
    }

}

// void DrawStandingShips(WINDOW* WIN, struct ShipsInfo Ships){
//     char ch = 219;
//     for (int i = 0, x = 12; i < Ships.Number_4_Size; i++, x+=5){
//         if (Ships.Ships[getIndex(5, i, Ships)].stand == TRUE)
//             wattron(WIN, COLOR_PAIR(55));
//         else
//             wattron(WIN, COLOR_PAIR(2));
//         mvwprintw(WIN,5,x," %c%c%c%c",ch, ch, ch, ch);
//     }
//     for (int i = 0, x = 12; i < Ships.Number_3_Size; i++, x+=4){
//         if (Ships.Ships[getIndex(7, i, Ships)].stand == TRUE)
//             wattron(WIN, COLOR_PAIR(55));
//         else
//             wattron(WIN, COLOR_PAIR(2));
//         mvwprintw(WIN,7,x," %c%c%c",ch, ch, ch);
//     }
//     for (int i = 0, x = 12; i < Ships.Number_2_Size; i++, x+=3){
//         if (Ships.Ships[getIndex(9, i, Ships)].stand == TRUE)
//         wattron(WIN, COLOR_PAIR(55));
//     else
//         wattron(WIN, COLOR_PAIR(2));
//         mvwprintw(WIN,9,x," %c%c",ch,ch);
//     }
//     for (int i = 0, x = 12; i < Ships.Number_1_Size; i++, x+=2){
//         if (Ships.Ships[getIndex(11, i, Ships)].stand == TRUE)
//         wattron(WIN, COLOR_PAIR(55));
//     else
//         wattron(WIN, COLOR_PAIR(2));
//         mvwprintw(WIN,11,x," %c",ch); 
//     }
//     wrefresh(WIN);
//     wattron(WIN, COLOR_PAIR(2));
// }

void DrawActiveShip_InArrangeWindow(WindowParametres* warr, int number, int size){
    char ch = 219;
    int y = 4 + number*2;
    wattron(warr->ptrWin, COLOR_PAIR(100));
    for (int i = 0; i < size; i++, y)
        mvwprintw(warr->ptrWin, y, i+2,"%c",ch);
    wrefresh(warr->ptrWin);
}

int getIndex(struct ShipsInfo Ships, int number, int shipSize){
    switch(shipSize){
        case 4: return  number;
        case 3: return  Ships.Number_4_Size + number;
        case 2: return  Ships.Number_4_Size + Ships.Number_3_Size + number;
        case 1: return Ships.Number_4_Size + Ships.Number_3_Size + Ships.Number_2_Size + number;
        default: return -1;
    }
}
