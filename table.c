#include "table.h"

void DrawShipWindow(WINDOW *WIN, int width, int height){
    wattron(WIN,COLOR_PAIR(2));
    str_top(WIN, width);
    wattron(WIN,COLOR_PAIR(3));
    for (int i = 2; i < height-1; i+=2){
        str_line(WIN, width, i);
        str_numb(WIN, width, i+1);
    }
    str_bottom(WIN, width, height);
    wrefresh(WIN);
}

void str_numb(WINDOW *WIN, int width, int x){
    int lin = 179;
    mvwprintw(WIN, x, 0, "%c%d",lin, (int)x/2-1);
    for (int i = 2; i < width; i++){
        if (i % 2 == 0)
            mvwprintw(WIN, x, i, "%c", lin);
        else 
            mvwprintw(WIN, x, i, " ");
    }
}

void str_line(WINDOW *WIN, int width, int x){
    int chert = 195;
    int lin = 196;
    int krest = 197; 
    mvwprintw(WIN, x, 0, "%c%c", chert, lin);
    for (int i = 2; i < width; i++){
        if (i % 2 == 0 )
            mvwprintw(WIN, x, i, "%c", krest);
        else
            mvwprintw(WIN, x, i, "%c", lin);
    }
    lin = 180;
    mvwprintw(WIN, x, width-1, "%c", lin);
}

void str_top(WINDOW *WIN, int width){
    int ugol = 218;
    int chert = 196;
    int chert_t = 194;
    int word = 65;
    mvwprintw(WIN, 0,0, "%c", ugol);
    for (int i = 1; i< width-1; i++){
        if (i % 2 != 0)
            mvwprintw(WIN, 0, i, "%c", chert);
        else
            mvwprintw(WIN, 0, i, "%c", chert_t);
    }
    ugol = 191;
    mvwprintw(WIN, 0, width-1, "%c", ugol);
    chert = 179;
    mvwprintw(WIN, 1, 0, "%c ",chert);
    for (int i = 2; i < width-1; i+=2, word++){
        mvwprintw(WIN, 1, i, "%c%c",chert, word);
    }
    mvwprintw(WIN, 1, width-1, "%c",chert);
}

void str_bottom(WINDOW *WIN, int width, int height){
    int ugol = 192;
    int chert = 196;
    int chert_t = 193;
    mvwprintw(WIN, height-1, 0, "%c", ugol);
    for (int i = 1; i< width-1; i++){
        if (i % 2 != 0)
            mvwprintw(WIN, height-1, i, "%c", chert);
        else
            mvwprintw(WIN, height-1, i, "%c", chert_t);
    }
    ugol = 217;
    mvwprintw(WIN, height-1, width-1, "%c", ugol);
}