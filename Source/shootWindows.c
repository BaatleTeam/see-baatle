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

    mvwprintw(WInfo->ptrWin, 3, 3, "4-size: %c%c", 32, 32);
    mvwprintw(WInfo->ptrWin, 5, 3, "3-size: %c%c", 32, 32);
    mvwprintw(WInfo->ptrWin, 3, 15, "2-size: %c%c", 32, 32);
    mvwprintw(WInfo->ptrWin, 5, 15, "1-size: %c%c", 32, 32);

    mvwprintw(WInfo->ptrWin, 11, 3, "Shoots: %c%c", 32, 32);
    mvwprintw(WInfo->ptrWin, 13, 3, "Hits: %c%c", 32, 32);
    mvwprintw(WInfo->ptrWin, 15, 3, "Percent: %c%c%c%%", 32, 32, 32);

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

void DrawWBoard_Default(WindowParametres *WBoard){
    DrawTableWindow(WBoard);

    wrefresh(WBoard->ptrWin);
}