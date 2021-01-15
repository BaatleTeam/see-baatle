#include "ships.h"

// TODO move this func to window-magaer module ----------------------------------
void initWindowWithParameters(WindowParametres *wp) {
    Stopif(wp->ptrWin != NULL, "initWindowWithParameters(): winptr should be NULL");
    Stopif(wp->Height == 0, "initWindowWithParameters(): height of new window is 0!");
    Stopif(wp->Height > LINES, "initWindowWithParameters(): height of new window more than LINES!");
    Stopif(wp->Width == 0, "initWindowWithParameters(): width of new window is 0!");
    Stopif(wp->Width > COLS, "initWindowWithParameters(): width of new window more than COLS!");
    Stopif(wp->Begin_x >= COLS, "initWindowWithParameters(): Begin_x of new window more than COLS!");
    Stopif(wp->Begin_y >= LINES, "initWindowWithParameters(): Begin_y of new window more than LINES!");

    wp->ptrWin = newwin(wp->Height, wp->Width, wp->Begin_y, wp->Begin_x);
}

void initDerWindowWithParameters(WINDOW *parent, WindowParametres *wp) {
    Stopif(wp->ptrWin != NULL, "initWindowWithParameters(): winptr should be NULL");
    Stopif(wp->Height == 0, "initWindowWithParameters(): height of new window is 0!");
    Stopif(wp->Height > LINES, "initWindowWithParameters(): height of new window more than LINES!");
    Stopif(wp->Width == 0, "initWindowWithParameters(): width of new window is 0!");
    Stopif(wp->Width > COLS, "initWindowWithParameters(): width of new window more than COLS!");
    Stopif(wp->Begin_x >= COLS, "initWindowWithParameters(): Begin_x of new window more than COLS!");
    Stopif(wp->Begin_y >= LINES, "initWindowWithParameters(): Begin_y of new window more than LINES!");

    wp->ptrWin = derwin(parent, wp->Height, wp->Width, wp->Begin_y, wp->Begin_x);
}

void clearWindowParametres(WindowParametres *wp) {
    delwin(wp->ptrWin);
    *wp = (WindowParametres){0};
}


// -------------------------------------------------


bool isAllShipsStanding(ShipsInfo ships){
    int shipsNum = ships.Number_1_Size + ships.Number_2_Size + ships.Number_3_Size + ships.Number_4_Size;
    for (int i = 0; i < shipsNum; i++)
        if (ships.Ships[i].stand == FALSE)
            return FALSE;
    return TRUE;
}

void deleteShipFromField(Ship* ship, const Board *board){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                board->field[ship->y][i+ship->x] = FALSE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                board->field[i+ship->y][ship->x] = FALSE;
            break;
        default:
            Stopif(true, "deleteShipFromField(): unexpeced switch value.")
    }
}

void reDrawStandingShips(WINDOW *WIN, const Board *board){
    int rect = 254;
    for (int i = 0; i < board->Height; i++)
        for (int j = 0; j < board->Width; j++){
            if (board->field[i][j] == TRUE){
                wattron(WIN, COLOR_PAIR(10));
                mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
            }
            else {
                wattron(WIN, COLOR_PAIR(3));
                mvwprintw(WIN, i*2+3, j*2+4, " ");
            }
        }
    wrefresh(WIN);
}

void refresh_ship_player_array(const ShipsInfo *Ships, const Board *board){
    for (int i = 0; i < Ships->Number_4_Size + Ships->Number_3_Size + Ships->Number_2_Size + Ships->Number_1_Size; i++)
        if (Ships->Ships[i].stand == TRUE)
            standing_ship(&Ships->Ships[i], board);
}

void standing_ship(Ship* ship, const Board *board){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                board->field[ship->y][i+ship->x] = TRUE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                board->field[i+ship->y][ship->x] = TRUE;
            break;
        default:
            Stopif(true, "standing_ship(): unexpeced switch value.")
    }
}

int getShipsNumber(const ShipsInfo* info){
	return info->Number_4_Size + info->Number_3_Size + info->Number_2_Size + info->Number_1_Size;
}

// -------------------------------------------------------------------------------
// Секция функций проверки свободы границ корабля
bool checkShipBorders(const Ship* ship, const Board *board){
    switch (ship->type) {
        case HORIZONTAL:
                if (checkShipBorders_top_bottom_horizontal(ship, board) == FALSE ||
                    checkShipBorders_left_right_horizontal(ship, board) == FALSE ||
                    checkItself(ship, board) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
                break;
        case VERTICAL:
                if (checkShipBorders_top_bottom_vertical(ship, board) == FALSE ||
                    checkShipBorders_left_right_vertical(ship, board) == FALSE ||
                    checkItself(ship, board) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
                break;
    }
    return TRUE; // should be not reached - for warning suspend
}

bool checkShipBorders_top_bottom_horizontal(const Ship* ship, const Board *board){
    if (ship->y == 0){ 
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y+1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    } else 
    if (ship->y == board->Height-1){
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y-1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    } else {
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y-1][ship->x+i] == TRUE)
                return FALSE;
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y+1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    }
}

bool checkShipBorders_left_right_horizontal(const Ship* ship, const Board *board){
    if (ship->x + ship->size == board->Width){ // Здесь было board->Width-1
        if (ship->y == 0)
            if (board->field[0][ship->x-1] == TRUE || board->field[1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->y == board->Height-1)
            if (board->field[board->Height-1][ship->x-1] == TRUE || board->field[board->Height-1-1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->y != 0 && ship->y != board->Height-1)
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (board->field[i][ship->x-1] == TRUE)
                    return FALSE;
        return TRUE;
    };

    if (ship->x == 0){
        if (ship->y == 0)
            if (board->field[0][ship->size] == TRUE || board->field[1][ship->size] == TRUE)
                return FALSE;
        if (ship->y == board->Height-1)
            if (board->field[board->Height-1][ship->size] == TRUE || board->field[board->Height-1-1][ship->size] == TRUE)
                return FALSE;
        if (ship->y != 0 && ship->y != board->Height-1)
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (board->field[i][ship->x+ship->size] == TRUE)
                    return FALSE;
        return TRUE;
    };

    if (ship->y == 0){
        if (board->field[0][ship->x+ship->size] == TRUE || board->field[1][ship->x+ship->size] == TRUE)
            return FALSE;
        if (board->field[0][ship->x-1] == TRUE || board->field[1][ship->x-1] == TRUE)
            return FALSE;
    } 
    if (ship->y == board->Height-1){
        if (board->field[board->Height-1][ship->x-1] == TRUE || board->field[board->Height-1-1][ship->x-1] == TRUE)
            return FALSE;
        if (board->field[board->Height-1][ship->x+ship->size] == TRUE || board->field[board->Height-1-1][ship->x+ship->size] == TRUE)
            return FALSE;
    }
    if (ship->y != 0 && ship->y != board->Height-1){
        for (int i = ship->y-1; i <= ship->y+1; i++)
            if (board->field[i][ship->x+ship->size] == TRUE || board->field[i][ship->x-1] == TRUE)
                return FALSE;
    }
    return TRUE;
}

bool checkShipBorders_left_right_vertical(const Ship* ship, const Board *board){
    if (ship->x == 0){ 
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y+i][ship->x+1] == TRUE)
                return FALSE;
        return TRUE;
    } else
    if (ship->x == board->Width-1) {
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y+i][ship->x-1] == TRUE)
                return FALSE;
        return TRUE;
    } else {
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y+i][ship->x+1] == TRUE)
                return FALSE;
        for (int i = 0; i < ship->size; i++)
            if (board->field[ship->y+i][ship->x-1] == TRUE)
                return FALSE;
        return TRUE;
    }
}

bool checkShipBorders_top_bottom_vertical(const Ship* ship, const Board *board){
    if (ship->y + ship->size == board->Height){ // Упёрлись в нижний край.
        if (ship->x == 0)
            if (board->field[ship->y-1][ship->x] == TRUE || board->field[ship->y-1][ship->x+1] == TRUE)
                return FALSE;
        if (ship->x == board->Width)
            if (board->field[ship->y-1][ship->x] == TRUE || board->field[ship->y-1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->x != 0 && ship->x != board->Width)
            for (int i = ship->x-1; i <= ship->x+1; i++)
                if (board->field[ship->y-1][i] == TRUE)
                    return FALSE;
        return TRUE;
    }

    if (ship->y == 0){ // Упёрлись в верхний край.
        if (ship->x == 0)
            if (board->field[ship->size][ship->x] == TRUE || board->field[ship->size][ship->x+1] == TRUE)
                return FALSE;
        if (ship->x == board->Width)
            if (board->field[ship->size][ship->x] == TRUE || board->field[ship->size][ship->x-1] == TRUE)
                return FALSE;
        if (ship->x != 0 && ship->x != board->Width)
            for (int i = ship->x-1; i <= ship->x+1; i++)
                if (board->field[ship->size][i] == TRUE)
                    return FALSE;
        return TRUE;
    } 

    if (ship->x == 0){ // Уперлись в левый край.
        if (board->field[ship->y-1][ship->x] == TRUE || board->field[ship->y-1][ship->x+1] == TRUE)
            return FALSE;
        if (board->field[ship->y+ship->size][ship->x] == TRUE || board->field[ship->y+ship->size][ship->x+1] == TRUE)
            return FALSE;
    } else
    if (ship->x == board->Width-1){ // Уперлись в правый край.
        if (board->field[ship->y-1][ship->x] == TRUE || board->field[ship->y-1][ship->x-1] == TRUE)
            return FALSE;
        if (board->field[ship->y+ship->size][ship->x] == TRUE || board->field[ship->y+ship->size][ship->x-1] == TRUE)
            return FALSE;
    } else // Посередине
    if (ship->x != 0 && ship->x != board->Width-1)
        for (int i = ship->x-1; i <= ship->x+1; i++)
            if (board->field[ship->y-1][i] == TRUE || board->field[ship->y+ship->size][i] == TRUE)
                return FALSE;
    return TRUE;
}

bool checkItself(const Ship* ship, const Board *board){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                if (board->field[ship->y][ship->x+i] == TRUE)
                    return FALSE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                if (board->field[ship->y+i][ship->x] == TRUE)
                    return FALSE;
            break;
        default:
            Stopif(true, "checkItself(): unexpected switch case.");
    }
    return TRUE;
}

// -------------------------------------------------------------------------------

void clearTmpShip(Ship* ship){
    ship->x = -1;
    ship->y = -1;
    ship->size = 0;
    ship->type = VERTICAL;
    ship->stand = FALSE;
}

void DrawTmpShip(WINDOW* WIN, Ship* tmpShip, const Board *board){
    if (checkShipBorders(tmpShip, board) == FALSE)
        wattron(WIN, COLOR_PAIR(50));
    else 
        wattron(WIN, COLOR_PAIR(10));

    int rect = 254;
    int i = tmpShip->y;
    int j = tmpShip->x;
    switch(tmpShip->type) {
        case VERTICAL:
            for (; i < tmpShip->size + tmpShip->y; i++) {
                mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
            }
            break;
        case HORIZONTAL:
            for (; j < tmpShip->size + tmpShip->x; j++) {
                mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
            }
            break;
        default:
            Stopif(true, "DrawTmpShip(): unexpected switch case.");
    }
    wrefresh(WIN);
}

// -------------------------------------------------------------------------------

bool checkAllShipsStanding(const ShipsInfo *shipsPlayer){
    for (int i = 0; i < getShipsNumber(shipsPlayer); i++){
        if (shipsPlayer->Ships[i].stand == FALSE)
            return FALSE;
    }
    return TRUE;
}

void InitPrimaryCoordinates(int ShipSize, Ship* ship, const Board* const board){
    int x = 0;
    int y = 0;
    while (checkPlace(x, y, ShipSize, board) != TRUE){
        if (y+1 > board->Height-1){
            y = 0;
            x++;
        }
        else
            y++;
    }
    ship->x = x;
    ship->y = y;
    ship->size = ShipSize;
    ship->stand = FALSE;
    ship->type = HORIZONTAL;
}

bool checkPlace(int x, int y, int size, const Board *board){
    for (int i = 0; i < size ; i++){
            if (board->field[y][x+i] == TRUE)
                return FALSE;
    }
    return TRUE;       
}

// -------------------------------------------------------------------------------

void changeShipCoordinates(Ship* tmpShip, const Board *board, const int key){
    switch (key) {
        case KEY_LEFT:
            if (checkBorderLeft(tmpShip, board))
                tmpShip->x -= 1;
            else 
                if (tmpShip->type == HORIZONTAL) 
                    tmpShip->x = board->Width - tmpShip->size;
                else 
                    tmpShip->x = board->Width-1;
            break;
        case KEY_RIGHT:
            if (checkBorderRight(tmpShip, board))
                tmpShip->x += 1;
            else
                tmpShip->x = 0;
            break;
        case KEY_UP:
            if (checkBorderTop(tmpShip, board))
                tmpShip->y -= 1;
            else
                if (tmpShip->type == VERTICAL)
                    tmpShip->y = board->Height - tmpShip->size;
                else
                    tmpShip->y = board->Height-1;
            break;
        case KEY_DOWN:
            if (checkBorderBot(tmpShip, board))
                tmpShip->y += 1;
            else
                tmpShip->y = 0;;
            break;
        default: // it is ok
            return;
    }
}

bool checkBorderLeft(Ship* ship, const Board *board) {
    Stopif(ship->x >= board->Width, "checkBorderLeft(): Ship Coordinate=x more than board width.");
    if (ship->x == 0)
        return FALSE;
    else 
        return TRUE;
}

bool checkBorderRight(Ship* ship, const Board *board) {
    Stopif(ship->x >= board->Width, "checkBorderRight(): Ship Coordinate=x more than board width.");
    switch (ship->type) {
        case HORIZONTAL:
            if (ship->x + ship->size == board->Width)
                return FALSE;
            else 
                return TRUE;
        case VERTICAL:
            if (ship->x == board->Width-1)
                return FALSE;
            else 
                return TRUE;
        default:
            Stopif(true, "checkBorderRight(): switch case unexpeted.");
    }
    return TRUE; // control should not be reached
}

bool checkBorderTop(Ship* ship, const Board *board) {
    Stopif(ship->y >= board->Height, "checkBorderTop(): Ship Coordinate=y more than board height.");
    if (ship->y == 0)
        return FALSE;
    else 
        return TRUE;
}

bool checkBorderBot(Ship* ship, const Board *board) {
    Stopif(ship->y >= board->Height, "checkBorderTop(): Ship Coordinate=y more than board height.");
    switch (ship->type) {
        case VERTICAL:
            if (ship->y + ship->size == board->Height)
                return FALSE;
            else 
                return TRUE;
        case HORIZONTAL:
            if (ship->y == board->Height-1)
                return FALSE;
            else 
                return TRUE;
        default:
            Stopif(true, "checkBorderRight(): switch case unexpeted.");
    }
    return TRUE; // control should not be reached
}

// -------------------------------------------------------------------------------

void addShip(Ship* newShip, Ship* tmpShip){
    newShip->x = tmpShip->x;
    newShip->y = tmpShip->y;
    newShip->size = tmpShip->size;
    newShip->type = tmpShip->type;
    newShip->stand = TRUE;
}

void makeShipTmp(Ship* oldShip, Ship* tmpShip){
    tmpShip->x = oldShip->x;
    tmpShip->y = oldShip->y;
    tmpShip->size = oldShip->size;
    tmpShip->type = oldShip->type;
    oldShip->stand = HORIZONTAL;
    tmpShip->stand = TRUE;
}

void changeTypeOfShip(Ship* ship, const Board *board){
    switch (ship->type) {
        case HORIZONTAL:
            if (ship->y + ship->size > board->Height)
                ship->y = board->Height - ship->size;
            ship->type = TRUE;
            break;
        case VERTICAL:
            if (ship->x + ship->size > board->Width)
                ship->x = board->Width - ship->size;
            ship->type = FALSE;
            break;
        default:
            Stopif(true, "changeTypeOfShip(): unexpected case value.");
    }
}

// -------------------------------------------------------------------------------


bool isShipHit(const Ship* ship, int coord_x, int coord_y){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                if (ship->y == coord_y && ship->x+i == coord_x)
                    return TRUE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                if (ship->x == coord_x && ship->y+i == coord_y)
                    return TRUE;
            break;
        default:
            Stopif(true, "isShipHit(): unexpected case value.");
    }
    return FALSE;
}


// -------------------------------------------------------------------------------

void outputShip(FILE* out, const Ship* const ship){
    fprintf(out,\
        "Info about ship:\n\t\
        x: %d\n\t\
        y: %d\n\t\
        size: %d\n\t\
        type: %d\n\t\
        stand: %d\n",\
        ship->x, ship->y, ship->size, ship->type, ship->stand);
}

void outputBoard(FILE* out, const Board* const board){
    for (int i = 0; i < board->Height; i++){
        for (int j = 0; j < board->Width; j++){
            fprintf(out, "%d ", board->field[i][j]);
        }
        fprintf(out, "\n");
    }
}