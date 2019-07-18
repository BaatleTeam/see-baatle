#include "shootWindows.h"

void initWindowsShooting(const ShipsInfo *Ships, const Board *board, 
                        WindowParametres *WBackGround, 
                        WindowParametres *WInfoPlayer, 
                        WindowParametres *WInfoComputer, 
                        WindowParametres *WBoardPlayer,
                        WindowParametres *WBoardComputer)
{
    Indents Indents = {
                    .LeftIndent = 3,
                    .BetweenIndent = 2, 
                    .RightIndent = 2,
                    .TopIndent = 3,
                    .BottomIndent = 3 
    };
    int terminalHeight = 0;
    int termainalWidth = 0;

    WInfoPlayer->Width = 28;
    WInfoPlayer->Height = 20;
    WInfoPlayer->Begin_x = Indents.LeftIndent;
    WInfoPlayer->Begin_y = Indents.TopIndent;
    
    WBoardPlayer->Width = getTableWidth(board);
    WBoardPlayer->Height = getTableHeight(board);
    WBoardPlayer->Begin_x = Indents.LeftIndent + WInfoPlayer->Width + Indents.BetweenIndent;
    WBoardPlayer->Begin_y = Indents.TopIndent;

    WBoardComputer->Width = getTableWidth(board);
    WBoardComputer->Height = getTableHeight(board);
    WBoardComputer->Begin_x = WBoardPlayer->Begin_x + WBoardComputer->Width + Indents.BetweenIndent + 1; 
    WBoardComputer->Begin_y = Indents.TopIndent;

    WInfoComputer->Width = WInfoPlayer->Width;
    WInfoComputer->Height = WInfoPlayer->Height;
    WInfoComputer->Begin_x = WBoardComputer->Begin_x + WBoardComputer->Width + Indents.BetweenIndent;
    WInfoComputer->Begin_y = Indents.TopIndent;


    termainalWidth = WInfoComputer->Begin_x + WInfoComputer->Width + Indents.RightIndent;
    terminalHeight = (WBoardPlayer->Height > WInfoPlayer->Height) ? WBoardPlayer->Height : WInfoPlayer->Height;
    terminalHeight += Indents.TopIndent + Indents.BottomIndent;

    // terminalHeight = 30;
    // termainalWidth = 120;
    resize_term(terminalHeight, termainalWidth);
    WBackGround->Width = COLS;
    WBackGround->Height = LINES;
    WBackGround->Begin_x = 0;
    WBackGround->Begin_y = 0;
    
    initWindow(WBoardPlayer);
    initWindow(WBoardComputer);
    initWindow(WInfoPlayer);
    initWindow(WInfoComputer);
    initWindow(WBackGround);
}

void DrawWInfo_Shoting(WindowParametres *WInfo, const PlayerStats* stats){
    DrawWInfo_Shoting_Default(WInfo);

    mvwprintw(WInfo->ptrWin, 3, 2, "4-size: %d/%d", stats->ship_4[0], stats->ship_4[1]);
    mvwprintw(WInfo->ptrWin, 5, 2, "3-size: %d/%d", stats->ship_3[0], stats->ship_3[1]);
    mvwprintw(WInfo->ptrWin, 3, 15, "2-size: %d/%d", stats->ship_2[0], stats->ship_2[1]);
    mvwprintw(WInfo->ptrWin, 5, 15, "1-size: %d/%d", stats->ship_1[0], stats->ship_1[1]);

    mvwprintw(WInfo->ptrWin, 11, 2, "Shoots: %d", stats->shots);
    mvwprintw(WInfo->ptrWin, 13, 2, "Hits: %d", stats->hits);
    if (stats->shots == 0)
        mvwprintw(WInfo->ptrWin, 15, 2, "Percent: 0.0%%");
    else
        mvwprintw(WInfo->ptrWin, 15, 2, "Percent: %.2f%%", (float)stats->hits / (float)stats->shots);

    wrefresh(WInfo->ptrWin);
}

void DrawWInfo_Shoting_Default(WindowParametres *WInfo){
    wattron(WInfo->ptrWin, COLOR_PAIR(2));
    wbkgdset(WInfo->ptrWin, COLOR_PAIR(2));
    wclear(WInfo->ptrWin);
    box(WInfo->ptrWin, 0, 0);

    mvwprintw(WInfo->ptrWin, 1, 9, "YOUR SHIPS");

    mvwprintw(WInfo->ptrWin, 8, 0, "%c", 195);
    mvwprintw(WInfo->ptrWin, 8, WInfo->Width-1, "%c", 180);
    for (int i = 1; i < WInfo->Width-1; i++)
        mvwprintw(WInfo->ptrWin, 8, i, "%c", 196);
    mvwprintw(WInfo->ptrWin, 9, 9, "STATISTICS");
    
    wrefresh(WInfo->ptrWin);
}

void DrawWBoard_Shoting(const WindowParametres *WBoard, int cur_x, int cur_y, const PlayerShotBoard *board){
    DrawWBoard_Shoting_Default(WBoard);
    // todo draw cursor function
    wrefresh(WBoard->ptrWin);
}

void DrawWBoard_Shoting_Default(WindowParametres *WBoard){
    DrawTableWindow(WBoard);
    wrefresh(WBoard->ptrWin);
}

// ----------------------

void moveCursor_Shooting(Board board, int *cursor_x_position, int *cursor_y_position, int key){
    switch (key){
        case KEY_LEFT:
            if (*cursor_x_position > 0)
                (*cursor_x_position)--;
            break;
        case KEY_RIGHT:
            if (*cursor_x_position < board.Width-1)
                (*cursor_x_position)++;
        case KEY_UP:
            if (*cursor_y_position > 0)
                (*cursor_y_position)--;
            break;
        case KEY_DOWN:
            if (*cursor_y_position < board.Height-1)
                (*cursor_y_position)++;
        default:
            // No Any reaction for another key
            break;
    }
}