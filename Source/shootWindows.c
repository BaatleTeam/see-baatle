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

Coordinate generateShotCoordinate(const ShotBoard* const boardData, Coordinate prevShot, const PlayerStats* const newStats){
    static Coordinate coordOfFirstHit = (Coordinate) {-1, -1};
    static unsigned destroyedShipsOnPrevShot = 0;
    static bool perspectiveDirs[4] = {FALSE, FALSE, FALSE, FALSE};
    static DIR PrevDir = NO_DIR;
    bool isPrevShotWasLast = FALSE;
    if (newStats->shipsDestroyed - destroyedShipsOnPrevShot > 0){
        isPrevShotWasLast = TRUE;
        coordOfFirstHit = (Coordinate) {-1, -1};
        for (int i = 0; i < 4; i++)
            perspectiveDirs[i] = FALSE;
        PrevDir = NO_DIR;
    }
    destroyedShipsOnPrevShot = newStats->shipsDestroyed;
    
    static enum PrevShotStatus {PAST, HIT} prevShotStatus = PAST;
    if (prevShot.x == -1) // если первый выстрел.
        prevShotStatus = PAST;
    else if (boardData->board[prevShot.y][prevShot.x] == SHOTED)
        prevShotStatus = PAST;
    else if (boardData->board[prevShot.y][prevShot.x] == KILLED)
        prevShotStatus = HIT;

    if (prevShotStatus == HIT && PrevDir == NO_DIR && !isPrevShotWasLast)
        coordOfFirstHit = prevShot;
    // fprintf(db_out, "Data operating:\n");
    // fprintf(db_out, "PrevShot: (%d, %d)\n", prevShot.x, prevShot.y);
    // fprintf(db_out, "PrevShotStatus: %d\n", prevShotStatus);
    // fprintf(db_out, "coordOfFirstHit: (%d, %d)\n", coordOfFirstHit.x, coordOfFirstHit.y);
    // fprintf(db_out, "PrevDir: %d\n", PrevDir);
    // fprintf(db_out, "destroyedShipsOnPrevShot: %d\n", destroyedShipsOnPrevShot);
    // fprintf(db_out, "PerspectiveDirs: ");
    // for (int i = 0; i < 4; i++)
    //     fprintf(db_out, "%d ", perspectiveDirs[i]);
    // fprintf(db_out, "\n");

    if (prevShot.x == -1 && prevShot.y == -1){ 
        // fprintf(db_out, "Random_1\n\n");
        return getRandomCoordinate(boardData);
    }
    else
    if (prevShotStatus == PAST && numberOfPerspectiveDirs(perspectiveDirs) == 0){
        // fprintf(db_out, "Random_2\n\n");
        PrevDir = NO_DIR;
        return getRandomCoordinate(boardData);
    }
    else
    if (prevShotStatus == PAST && numberOfPerspectiveDirs(perspectiveDirs) != 0){
        perspectiveDirs[PrevDir] = FALSE;
        // fprintf(db_out, "Persp_1\n\n");
        return getDirPerspectiveCoordinate(boardData, prevShot, coordOfFirstHit, &perspectiveDirs, NO_DIR, &PrevDir);
    }
    else
    if (prevShotStatus == HIT && PrevDir != NO_DIR){
        if (isPrevShotWasLast){ // если попали и добили
            // fprintf(db_out, "Random_3\n\n");
            return getRandomCoordinate(boardData);
        }
        else {
            perspectiveDirs[PrevDir] = TRUE;
            // fprintf(db_out, "Persp_2\n\n");
            return getDirPerspectiveCoordinate(boardData, prevShot, coordOfFirstHit, &perspectiveDirs, PrevDir, &PrevDir);
        }
    }
    else
    if (prevShotStatus == HIT && PrevDir == NO_DIR){
        if (isPrevShotWasLast){ // если попали в единичку (сразу убили)
            // fprintf(db_out, "Random_4\n\n");
            return getRandomCoordinate(boardData);
        }
        else { // Попали случайно в первый раз в большой корабль
            for (int i = 0; i < 4; i++)
                perspectiveDirs[i] = TRUE;
            // fprintf(db_out, "Persp_3\n\n");
            return getDirPerspectiveCoordinate(boardData, prevShot, coordOfFirstHit, &perspectiveDirs, NO_DIR, &PrevDir);
        }
    }
    return (Coordinate){-1, -1}; // Non-reached block
}

Coordinate getRandomCoordinate(const ShotBoard* const boardData){
    while (TRUE) {
        int x = rand() % boardData->Width;
        int y = rand() % boardData->Height;
        if (boardData->board[y][x] == EMPTY)
            return (Coordinate){x,y};
    }
}

Coordinate getDirPerspectiveCoordinate(const ShotBoard* const boardData, Coordinate prevShot, Coordinate firstHit, bool (*dirs)[4], DIR priorityDIR, DIR* prevDIR){
    // Если есть приоритет, стреляем по приоритету, иначе выбираем случайное возможное направление.
    // Может получиться, что приоритетное направление невозможно. Тогда также выбираем случайное возможное.
    Coordinate nextHitCoord = (Coordinate){-1, -1};
    do {
        assert(numberOfPerspectiveDirs(*dirs) != 0);
        if (priorityDIR != NO_DIR)
            *prevDIR = priorityDIR;
        else {
            *prevDIR = getRandomDirect(*dirs);
            prevShot = firstHit;
        }
        (*dirs)[*prevDIR] = FALSE;
        nextHitCoord = getCoordinateOfDir(*prevDIR, boardData, prevShot);

        if (nextHitCoord.x == -1){
            prevShot = firstHit; // Если направление провалилось, то нужно 
            // стрелять в противоположном направлении, начиная с места первого попадания.
            if (priorityDIR == UP || priorityDIR == DOWN){
                if (priorityDIR == DOWN)
                    priorityDIR = UP;
                else
                    priorityDIR = DOWN;
                (*dirs)[LEFT] = FALSE;
                (*dirs)[RIGHT] = FALSE;
            } else
            if (priorityDIR == LEFT || priorityDIR == RIGHT){
                if (priorityDIR == LEFT)
                    priorityDIR = RIGHT;
                else
                    priorityDIR = LEFT;
                (*dirs)[UP] = FALSE;
                (*dirs)[DOWN] = FALSE;
            }
        }
    }
    while (nextHitCoord.x == -1); // если вернулось (-1,-1), то выстрел в этом направлении невозможен. Ищем другое направление.
    return nextHitCoord;
}

int numberOfPerspectiveDirs(const bool dirs[]){
    int number = 0;
    for (int i = 0; i < 4; i++)
        if (dirs[i] == TRUE)
            number++;
    return number;
}

DIR getRandomDirect(const bool dirs[]){
    int number = numberOfPerspectiveDirs(dirs);
    assert(number != 0);
    int activeDirs[number];
    int j = 0;
    for (int i = 0; i < 4; i++){
        if (dirs[i] == TRUE){
            activeDirs[j] = i;
            j++;
        }
    }
    int randInt = rand() % number;
    return activeDirs[randInt];
}


Coordinate getCoordinateOfDir(DIR dir, const ShotBoard* const boardData, Coordinate prevShot){
    if (dir == UP){
        if (prevShot.y != 0) // не край карты
            if (boardData->board[prevShot.y-1][prevShot.x] == EMPTY) // и там не прострелено
                return (Coordinate){prevShot.x, prevShot.y-1};
    } else
    if (dir == DOWN){
        if (prevShot.y != boardData->Height-1) // не край карты
            if (boardData->board[prevShot.y+1][prevShot.x] == EMPTY) // и там не прострелено
                return (Coordinate){prevShot.x, prevShot.y+1};
    } else
    if (dir == LEFT){
        if (prevShot.x != 0) // не край карты
            if (boardData->board[prevShot.y][prevShot.x-1] == EMPTY) // и там не прострелено
                return (Coordinate){prevShot.x-1, prevShot.y};
    } else
    if (dir == RIGHT){
        if (prevShot.x != boardData->Width-1) // не край карты
            if (boardData->board[prevShot.y][prevShot.x+1] == EMPTY) // и там не прострелено
                return (Coordinate){prevShot.x+1, prevShot.y};
    }
    else assert(dir != NO_DIR);
    return (Coordinate) {-1, -1}; // Иначе, это направление не перспективно.
}

