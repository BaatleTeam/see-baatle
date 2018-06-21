#include "table.h"


void initWindowsParametres(struct Board board, WindowParametres *mainW, WindowParametres *arrange, WindowParametres *ship, WindowParametres *shoot, WindowParametres* help){
    // Отступы между окнами и краями главного окна.
    size_t LeftIndent    = 4;
    size_t BetweenIndent = 3;
    size_t RightIndent   = 4;
    size_t TopIndent     = 3;
    size_t BottomIndent  = 4;

    // Зависимости от размера отступов.
	ship->Begin_y = TopIndent;
	ship->Begin_x = LeftIndent;
    ship->Height = 3 + board.Height * 2;
	ship->Width  = 3 + board.Width  * 2;

    // Окно arrange на одном уровне с ship.
    arrange->Begin_y = TopIndent;
    arrange->Begin_x = LeftIndent + ship->Width + BetweenIndent;
    arrange->Height = 21; // min
    arrange->Width = 38; // min

    // Размеры окна под стрельбу должны соотвествовать окну ship.
    shoot->Begin_y = arrange->Begin_y;
    shoot->Begin_x = arrange->Begin_x;
    shoot->Height = ship->Height;
    shoot->Width = ship->Width;

    // Новое главное окно // - ?
    mainW->Width = arrange->Begin_x + arrange->Width + RightIndent;
    mainW->Height = TopIndent + ship->Height + BottomIndent;
    mainW->Begin_y = 0;
    mainW->Begin_x = 0;

    ship->ptrWin = newwin(ship->Height, ship->Width, ship->Begin_y, ship->Begin_x);
    arrange->ptrWin = newwin(arrange->Height, arrange->Width, arrange->Begin_y, arrange->Begin_x);
    shoot->ptrWin = newwin(shoot->Height, shoot->Width, shoot->Begin_y, shoot->Begin_x);
    mainW->ptrWin = newwin(mainW->Height, mainW->Width, mainW->Begin_y, mainW->Begin_x);
    // + help

    if (mainW->Height < arrange->Height + 4)
 		mainW->Height = arrange->Height + 4;
    resize_term(mainW->Height, mainW->Width);
}

void DrawMainWindow(WindowParametres *Wmain){
    wbkgdset(Wmain->ptrWin, COLOR_PAIR(200));
    wclear(Wmain->ptrWin); 
    wrefresh(Wmain->ptrWin);
    refresh();
}

void DrawTableWindow(WindowParametres *Wship){
    wattron(Wship->ptrWin,COLOR_PAIR(3));
    str_top(Wship->ptrWin, Wship->Width);
    for (int i = 2; i < Wship->Height-1; i+=2){
        str_line(Wship->ptrWin, Wship->Width, i);
        str_numb(Wship->ptrWin, Wship->Width, i+1);
    }
    str_bottom(Wship->ptrWin, Wship->Width, Wship->Height);
    wrefresh(Wship->ptrWin);
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