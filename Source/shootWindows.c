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

void updateGraphics_Shoting(WindowParametres WBoard, const ShotBoard boardData, Coordinate curPos){
    DrawWBoard_Shoting(WBoard, boardData);
    bool isShotAvailable = checkShotPos(boardData, curPos);
    DrawCursor_Shoting(WBoard, curPos, isShotAvailable);
}

void DrawWInfo_Shoting(WindowParametres *WInfo, const PlayerStats* stats){
    DrawWInfo_Shoting_Default(WInfo);

    mvwprintw(WInfo->ptrWin, 3, 2, "4-size: %d/%d", stats->shipCount[3][0], stats->shipCount[3][1]);
    mvwprintw(WInfo->ptrWin, 5, 2, "3-size: %d/%d", stats->shipCount[2][0], stats->shipCount[2][1]);
    mvwprintw(WInfo->ptrWin, 3, 15, "2-size: %d/%d", stats->shipCount[1][0], stats->shipCount[1][1]);
    mvwprintw(WInfo->ptrWin, 5, 15, "1-size: %d/%d", stats->shipCount[0][0], stats->shipCount[0][1]);

    mvwprintw(WInfo->ptrWin, 11, 2, "Shoots: %d", stats->shots);
    mvwprintw(WInfo->ptrWin, 13, 2, "Hits: %d", stats->hits);
    if (stats->shots == 0)
        mvwprintw(WInfo->ptrWin, 15, 2, "Percent: 0.0%%");
    else
        mvwprintw(WInfo->ptrWin, 15, 2, "Percent: %.2f%%", (float)stats->hits / (float)stats->shots * 100);

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

void DrawWBoard_Shoting(WindowParametres WBoard, ShotBoard boardData){
    DrawWBoard_Shoting_Default(WBoard);
    int charToDraw = 254;
    for (int i = 0; i < boardData.Height; i++){
        for (int j = 0; j < boardData.Width; j++){
            switch (boardData.board[i][j]) {
                case EMPTY:
                    wattron(WBoard.ptrWin, COLOR_PAIR(3));
                    charToDraw = ' ';
                    break;
                case SHOTED:
                    wattron(WBoard.ptrWin, COLOR_PAIR(49));
                    charToDraw = 249;
                    break;
                case KILLED:
                    wattron(WBoard.ptrWin, COLOR_PAIR(49));
                    charToDraw = 'X';
                    break;
                
                default:
                    printf("Error switch value statement in drawing board shooting!\n");
                    break;
            }
            mvwprintw(WBoard.ptrWin, i*2+3, j*2+4, "%c", charToDraw);
        }
    }
    wrefresh(WBoard.ptrWin);
}

void DrawWBoard_Shoting_Default(WindowParametres WBoard){
    DrawTableWindow(&WBoard);
    wrefresh(WBoard.ptrWin);
}

bool checkShotPos(const ShotBoard boardData, Coordinate curPos){
    if (boardData.board[curPos.y][curPos.x] == EMPTY)
        return TRUE;
    else
        return FALSE;
}

ShotResult makeShot(ShipsInfo ShipsComputer, const ShotBoard boardData, Coordinate curPos){
    ShotResult result = {0};
    // найти индекс подбитого корабля
    int shipIndex = -1;
    for (int i = 0; i < getShipsNumber(&ShipsComputer); i++){
        if (isShipHit(&ShipsComputer.Ships[i], curPos.x, curPos.y)) // если попадание
            shipIndex = i;
    }

    // изменить массив-карту
    if (shipIndex != -1){
        result.isHit = TRUE;
        markKILLED(boardData, curPos);
        if (isShipKilled(&ShipsComputer.Ships[shipIndex], boardData)){ // если при этом уничтожен
            result.shipSize = ShipsComputer.Ships[shipIndex].size;
            fillBoardNearKilledShip(ShipsComputer.Ships[shipIndex], boardData);
        }
    }
    else markSHOTED(boardData, curPos);
    return result;
}

// TODO change vars 
void fillBoardNearKilledShip(const ship ship, ShotBoard boardData){
    Coordinate pointToCheck = {0};
    switch (ship.type) {
        case FALSE:
            for (int i = 0; i < ship.size; i++){
                // ряд выше
                initCoordiante(&pointToCheck, ship.x+i, ship.y-1);
                if (isValidBoardCell(boardData, pointToCheck)) 
                    markSHOTED(boardData, pointToCheck);
                // ряд ниже
                initCoordiante(&pointToCheck, ship.x+i, ship.y+1);
                if (isValidBoardCell(boardData, pointToCheck)) 
                    markSHOTED(boardData, pointToCheck);
            }

            for (int i = -1; i < 2; i++){
                // столбец левее
                initCoordiante(&pointToCheck, ship.x-1, ship.y+i);
                if (isValidBoardCell(boardData, pointToCheck)) 
                    markSHOTED(boardData, pointToCheck);
                // столбец правее
                initCoordiante(&pointToCheck, ship.x+ship.size, ship.y+i);
                if (isValidBoardCell(boardData, pointToCheck)) 
                    markSHOTED(boardData, pointToCheck);                
            }
            break;
        case TRUE:
            for (int i = 0; i < ship.size; i++){
                // ряд левее
                initCoordiante(&pointToCheck, ship.x-1, ship.y+i);
                if (isValidBoardCell(boardData, pointToCheck))
                    markSHOTED(boardData, pointToCheck);
                // ряд правее
                initCoordiante(&pointToCheck, ship.x+1, ship.y+i);
                if (isValidBoardCell(boardData, pointToCheck))
                    markSHOTED(boardData, pointToCheck);
            }

            for (int i = -1; i < 2; i++){
                // столбец выше
                initCoordiante(&pointToCheck, ship.x+i, ship.y-1);
                if (isValidBoardCell(boardData, pointToCheck))
                    markSHOTED(boardData, pointToCheck);
                // столбец ниже
                initCoordiante(&pointToCheck, ship.x+i, ship.y+ship.size);
                if (isValidBoardCell(boardData, pointToCheck))
                    markSHOTED(boardData, pointToCheck);                
            }
            break;
    }
}

bool isShipKilled(ship* ship, const ShotBoard boardData){
    switch (ship->type){
        case FALSE:
            for (int i = 0; i < ship->size; i++)
                if (boardData.board[ship->y][ship->x+i] == EMPTY)
                    return FALSE;
            break;
        case TRUE:
            for (int i = 0; i < ship->size; i++)
                if (boardData.board[ship->y+i][ship->x] == EMPTY)
                    return FALSE;
            break;
    }
    return TRUE;
}

void DrawCursor_Shoting(WindowParametres WBoard, Coordinate curPos, bool isActive){
    if (isActive)
        wattron(WBoard.ptrWin, COLOR_PAIR(33));
    else
        wattron(WBoard.ptrWin, COLOR_PAIR(51));
    mvwprintw(WBoard.ptrWin, curPos.y*2+3, curPos.x*2+4, "%c", 254);
    wrefresh(WBoard.ptrWin);
}

// ------------------------------------------------------------

void updateStats(PlayerStats *stats_shots, PlayerStats *stats_shooted, ShotResult shotResult){
    stats_shots->shots++;
    if (shotResult.isHit){
        stats_shots->hits++;
        if (shotResult.shipSize){
            stats_shots->shipsDestroyed++;
            stats_shooted->shipCount[shotResult.shipSize-1][0]--;
        }
    }
}

// ------------------------------------------------------------

void moveCursor_Shooting(Board board, Coordinate *curPos, int key){
    switch (key){
        case KEY_LEFT:
            if (curPos->x > 0)
                (curPos->x)--;
            else if (curPos->x == 0)
                (curPos->x) = board.Width-1;
            break;
        case KEY_RIGHT:
            if (curPos->x < board.Width-1)
                (curPos->x)++;
            else if (curPos->x == board.Width-1)
                (curPos->x) = 0;
            break;
        case KEY_UP:
            if (curPos->y > 0)
                (curPos->y)--;
            else if (curPos->y == 0)
                (curPos->y) = board.Height-1;
            break;
        case KEY_DOWN:
            if (curPos->y < board.Height-1)
                (curPos->y)++;
            else if (curPos->y == board.Height-1)
                (curPos->y) = 0;
            break;
        default:
            // No Any reaction for another key
            break;
    }
}

void markEMPTY(ShotBoard board, Coordinate curPos){
    board.board[curPos.y][curPos.x] = EMPTY;
}

void markSHOTED(ShotBoard board, Coordinate curPos){
    board.board[curPos.y][curPos.x] = SHOTED;
}

void markKILLED(ShotBoard board, Coordinate curPos){
    board.board[curPos.y][curPos.x] = KILLED;
}

bool isValidBoardCell(ShotBoard board, Coordinate point){
    if (point.x < 0 || point.y < 0)
        return FALSE;
    if (point.x >= board.Width || point.y >= board.Height)
        return FALSE;
    return TRUE;
}

// ---------------------------------------------------------------

Coordinate generateShotCoordinate(const ShotBoard boardData, Coordinate prevShot){
    // TODO norm alogrithm
    return (Coordinate) {.x = rand() % boardData.Width, .y = rand() % boardData.Height };
}
