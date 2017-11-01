#include "arrange.h"
#include "curses.h"

void win_arrange_graphics(WINDOW *WIN){
    int x;
    char ch = 219;
    wattron(WIN, COLOR_PAIR(2));
    mvwprintw(WIN,1, 1, "Use TAB and arrows to move.");
    mvwprintw(WIN,2, 1, "Use ENTER to choose the ship.");
    mvwprintw(WIN,3, 1, "Use TAB to change the orientation.");
    mvwprintw(WIN,5,3, "2 ships: ");
    mvwprintw(WIN,7,3, "3 ships: ");
    mvwprintw(WIN,9,3, "4 ships: ");
    mvwprintw(WIN,11,3, "6 ships: ");
    for (int i = 0, x = 12; i < 2; i++, x+=5)
        mvwprintw(WIN,5,x," %c%c%c%c",ch,  ch, ch, ch);
    for (int i = 0, x = 12; i < 3; i++, x+=4)
        mvwprintw(WIN,7,x," %c%c%c",ch, ch, ch);
    for (int i = 0, x = 12; i < 4; i++, x+=3)
        mvwprintw(WIN,9,x," %c%c",ch,ch);
    for (int i = 0, x = 12; i < 6; i++, x+=2)
        mvwprintw(WIN,11,x," %c",ch);    
    wrefresh(WIN);
}

int convert_size(int y){
    switch (y){
        case 5:
            return 5;
        case 7:
            return 4;
            break;
        case 9:
            return 3;
        case 11:
            return 2;
            break;
    } 
}

void check_left(int *x, int y){
    int size;
    size = convert_size(y);       
    if (*x-size < 13)
        *x = 13;
    else 
        *x -= size;
}

void check_right(int *x, int y){
    int size;
    int max;
    switch (y){
        case 5:
            max = 18;
            break;
        case 7:
            max = 21;
            break;
        case 9:
            max = 22;
            break;
        case 11:
            max = 23;
            break;
    } 
    size = convert_size(y);
    if (*x+size > max)
        *x = max;
    else
        *x+=size;
}

void draw_leftkey(WINDOW *WIN, int y, int x){
    int rect = 254;
    int step;
    step = convert_size(y);
    for(int i = 0; i < step-1; i++, x++){
        wattron(WIN, COLOR_PAIR(100));
        mvwprintw(WIN,y,x, "%c", rect);
        wattron(WIN, COLOR_PAIR(2));
        mvwprintw(WIN,y,x+step, "%c", rect);
    }
    wrefresh(WIN);
}

void draw_rightkey(WINDOW *WIN, int y, int x){
    int rect = 254;
    int step;
    step = convert_size(y);
    for(int i = 0; i < step-1; i++, x++){
        wattron(WIN, COLOR_PAIR(100));
        mvwprintw(WIN,y,x, "%c", rect);
        wattron(WIN, COLOR_PAIR(2));
        mvwprintw(WIN,y,x-step, "%c", rect);
    }
    wrefresh(WIN);
}

