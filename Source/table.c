#include "table.h"

void initWindowsParametres(const CorePlayerGameData *playerGameData, WindowParametres *mainW, WindowParametres *arrange, WindowParametres *ship, WindowParametres* help){
    // Объявление размеров отступов.
    Indents indents = {.LeftIndent    = 4,
                       .BetweenIndent = 5,
                       .RightIndent   = 4,
                       .TopIndent     = 3,
                       .BottomIndent  = 4 };

    // Зависимости от размера отступов.
	ship->Begin_y = indents.TopIndent;
	ship->Begin_x = indents.LeftIndent;
    ship->Height = getTableHeight(&playerGameData->board);
	ship->Width  = getTableWidth(&playerGameData->board);

    // Окно arrange на одном уровне с ship.
    arrange->Begin_y = indents.TopIndent;
    arrange->Begin_x = indents.LeftIndent + ship->Width + indents.BetweenIndent;
    arrange->Height = calculateArrangeHeight(&playerGameData->ships);
    arrange->Width = 38; // min

    // Новое главное окно // - ?
    mainW->Width = arrange->Begin_x + arrange->Width + indents.RightIndent;
    mainW->Height = indents.TopIndent + ship->Height + indents.BottomIndent;
    mainW->Begin_y = 0;
    mainW->Begin_x = 0;

    if (arrange->Height > ship->Height)
        mainW->Height += ship->Height - arrange->Height;
    if (mainW->Height <= 25)
        mainW->Height = 25;

    initWindow(ship);
    initWindow(arrange);
    initWindow(mainW);
    help->ptrWin = NULL;

    resize_term(mainW->Height, mainW->Width);
    clear();
    refresh();
}

void DrawMainWindow(WindowParametres *Wmain){
    wbkgdset(Wmain->ptrWin, COLOR_PAIR(200));
    clear();
    refresh();
    wclear(Wmain->ptrWin); 
    wrefresh(Wmain->ptrWin);
}

void DrawTableWindow(WindowParametres *Wship){
    wattron(Wship->ptrWin, COLOR_PAIR(3));
    str_top(Wship->ptrWin, Wship->Width);
    for (int i = 2; i < Wship->Height-1; i+=2){
        str_line(Wship->ptrWin, Wship->Width, i);
        str_numb(Wship->ptrWin, Wship->Width, i+1);
    }
    str_bottom(Wship->ptrWin, Wship->Width, Wship->Height);
    wrefresh(Wship->ptrWin);
}

int getTableWidth(const Board *board){
    return 4 + board->Width * 2;
}

int getTableHeight(const Board *board){
    return 3 + board->Height * 2;
}

void str_numb(WINDOW *WIN, int width, int x){
    int lin = 179;
    int LineNumber = (int)x/2-1;
    if (LineNumber < 10)
        mvwprintw(WIN, x, 0, "%c %d", lin, LineNumber);
    else
        mvwprintw(WIN, x, 0, "%c%d", lin, LineNumber);
    for (int i = 3; i < width; i++){
        if (i % 2 != 0)
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
        if (i % 2 != 0 )
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
    mvwprintw(WIN, 0,1, "%c", chert);
    for (int i = 2; i < width-1; i++)
        if (i % 2 == 0)
            mvwprintw(WIN, 0, i, "%c", chert);
        else
            mvwprintw(WIN, 0, i, "%c", chert_t);

    ugol = 191;
    chert = 179;
    mvwprintw(WIN, 0, width-1, "%c", ugol);
    mvwprintw(WIN, 1, 0, "%c ",chert);
    mvwprintw(WIN, 1, 2, " ");
    for (int i = 3; i < width-1; i+=2, word++)
        mvwprintw(WIN, 1, i, "%c%c",chert, word);
    mvwprintw(WIN, 1, width-1, "%c",chert);
}

void str_bottom(WINDOW *WIN, int width, int height){
    int ugol = 192;
    int chert = 196;
    int chert_t = 193;
    mvwprintw(WIN, height-1, 0, "%c", ugol);
    mvwprintw(WIN, height-1, 1, "%c", chert);
    for (int i = 2; i< width-1; i++){
        if (i % 2 == 0)
            mvwprintw(WIN, height-1, i, "%c", chert);
        else
            mvwprintw(WIN, height-1, i, "%c", chert_t);
    }
    ugol = 217;
    mvwprintw(WIN, height-1, width-1, "%c", ugol);
}

void doDoubleLineBorder(WINDOW* win_ptr) {
    wborder(win_ptr, 186, 186, 205, 205, 201, 187, 200, 188);   
}

void doSingleLineBorder(WINDOW* win_ptr) {
    wborder(win_ptr, 179, 179, 196, 196, 218, 191, 192, 217);   
}
