#include "ships.h"

// TODO move this func to window-magaer module ----------------------------------
void createWindowWithParameters(WindowParametres *wp) {
    Stopif(wp->Height == 0, "createWindowWithParameters(): height of new window is 0!");
    Stopif(wp->Height > LINES, "createWindowWithParameters(): height of new window more than LINES!");
    Stopif(wp->Width == 0, "createWindowWithParameters(): width of new window is 0!");
    Stopif(wp->Width > COLS, "createWindowWithParameters(): width of new window more than COLS!");
    Stopif(wp->Begin_x >= COLS, "createWindowWithParameters(): Begin_x of new window more than COLS!");
    Stopif(wp->Begin_y >= LINES, "createWindowWithParameters(): Begin_y of new window more than LINES!");

    wp->ptrWin = newwin(wp->Height, wp->Width, wp->Begin_y, wp->Begin_x);
}


// -------------------------------------------------


bool isAllShipsStanding(ShipsInfo ships){
    int shipsNum = ships.Number_1_Size + ships.Number_2_Size + ships.Number_3_Size + ships.Number_4_Size;
    for (int i = 0; i < shipsNum; i++)
        if (ships.Ships[i].stand == FALSE)
            return FALSE;
    return TRUE;
}

void deleteShipFromField(ship* ship, const Board *Board){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                Board->field[ship->y][i+ship->x] = FALSE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                Board->field[i+ship->y][ship->x] = FALSE;
            break;
        default:
            Stopif(true, "deleteShipFromField(): unexpeced switch value.")
    }
}

void reDrawStandingShips(WINDOW *WIN, const Board *Board){
    int rect = 254;
    for (int i = 0; i < Board->Height; i++)
        for (int j = 0; j < Board->Width; j++){
            if (Board->field[i][j] == TRUE){
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

void refresh_ship_player_array(const ShipsInfo *Ships, const Board *Board){
    for (int i = 0; i < Ships->Number_4_Size + Ships->Number_3_Size + Ships->Number_2_Size + Ships->Number_1_Size; i++)
        if (Ships->Ships[i].stand == TRUE)
            standing_ship(&Ships->Ships[i], Board);
}

void standing_ship(ship* ship, const Board *Board){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                Board->field[ship->y][i+ship->x] = TRUE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                Board->field[i+ship->y][ship->x] = TRUE;
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
bool checkShipBorders(const ship* ship, const Board *Board){
    switch (ship->type) {
        case HORIZONTAL:
                if (checkShipBorders_top_bottom_horizontal(ship, Board) == FALSE ||
                    checkShipBorders_left_right_horizontal(ship, Board) == FALSE ||
                    checkItself(ship, Board) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
                break;
        case VERTICAL:
                if (checkShipBorders_top_bottom_vertical(ship, Board) == FALSE ||
                    checkShipBorders_left_right_vertical(ship, Board) == FALSE ||
                    checkItself(ship, Board) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
                break;
    }
    return TRUE; // should be not reached - for warning suspend
}

bool checkShipBorders_top_bottom_horizontal(const ship* ship, const Board *Board){
    if (ship->y == 0){ 
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y+1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    } else 
    if (ship->y == Board->Height-1){
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y-1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    } else {
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y-1][ship->x+i] == TRUE)
                return FALSE;
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y+1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    }
}

bool checkShipBorders_left_right_horizontal(const ship* ship, const Board *Board){
    if (ship->x + ship->size == Board->Width){ // Здесь было Board->Width-1
        if (ship->y == 0)
            if (Board->field[0][ship->x-1] == TRUE || Board->field[1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->y == Board->Height-1)
            if (Board->field[Board->Height-1][ship->x-1] == TRUE || Board->field[Board->Height-1-1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->y != 0 && ship->y != Board->Height-1)
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (Board->field[i][ship->x-1] == TRUE)
                    return FALSE;
        return TRUE;
    };

    if (ship->x == 0){
        if (ship->y == 0)
            if (Board->field[0][ship->size] == TRUE || Board->field[1][ship->size] == TRUE)
                return FALSE;
        if (ship->y == Board->Height-1)
            if (Board->field[Board->Height-1][ship->size] == TRUE || Board->field[Board->Height-1-1][ship->size] == TRUE)
                return FALSE;
        if (ship->y != 0 && ship->y != Board->Height-1)
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (Board->field[i][ship->x+ship->size] == TRUE)
                    return FALSE;
        return TRUE;
    };

    if (ship->y == 0){
        if (Board->field[0][ship->x+ship->size] == TRUE || Board->field[1][ship->x+ship->size] == TRUE)
            return FALSE;
        if (Board->field[0][ship->x-1] == TRUE || Board->field[1][ship->x-1] == TRUE)
            return FALSE;
    } 
    if (ship->y == Board->Height-1){
        if (Board->field[Board->Height-1][ship->x-1] == TRUE || Board->field[Board->Height-1-1][ship->x-1] == TRUE)
            return FALSE;
        if (Board->field[Board->Height-1][ship->x+ship->size] == TRUE || Board->field[Board->Height-1-1][ship->x+ship->size] == TRUE)
            return FALSE;
    }
    if (ship->y != 0 && ship->y != Board->Height-1){
        for (int i = ship->y-1; i <= ship->y+1; i++)
            if (Board->field[i][ship->x+ship->size] == TRUE || Board->field[i][ship->x-1] == TRUE)
                return FALSE;
    }
    return TRUE;
}

bool checkShipBorders_left_right_vertical(const ship* ship, const Board *Board){
    if (ship->x == 0){ 
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y+i][ship->x+1] == TRUE)
                return FALSE;
        return TRUE;
    } else
    if (ship->x == Board->Width-1) {
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y+i][ship->x-1] == TRUE)
                return FALSE;
        return TRUE;
    } else {
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y+i][ship->x+1] == TRUE)
                return FALSE;
        for (int i = 0; i < ship->size; i++)
            if (Board->field[ship->y+i][ship->x-1] == TRUE)
                return FALSE;
        return TRUE;
    }
}

bool checkShipBorders_top_bottom_vertical(const ship* ship, const Board *Board){
    if (ship->y + ship->size == Board->Height){ // Упёрлись в нижний край.
        if (ship->x == 0)
            if (Board->field[ship->y-1][ship->x] == TRUE || Board->field[ship->y-1][ship->x+1] == TRUE)
                return FALSE;
        if (ship->x == Board->Width)
            if (Board->field[ship->y-1][ship->x] == TRUE || Board->field[ship->y-1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->x != 0 && ship->x != Board->Width)
            for (int i = ship->x-1; i <= ship->x+1; i++)
                if (Board->field[ship->y-1][i] == TRUE)
                    return FALSE;
        return TRUE;
    }

    if (ship->y == 0){ // Упёрлись в верхний край.
        if (ship->x == 0)
            if (Board->field[ship->size][ship->x] == TRUE || Board->field[ship->size][ship->x+1] == TRUE)
                return FALSE;
        if (ship->x == Board->Width)
            if (Board->field[ship->size][ship->x] == TRUE || Board->field[ship->size][ship->x-1] == TRUE)
                return FALSE;
        if (ship->x != 0 && ship->x != Board->Width)
            for (int i = ship->x-1; i <= ship->x+1; i++)
                if (Board->field[ship->size][i] == TRUE)
                    return FALSE;
        return TRUE;
    } 

    if (ship->x == 0){ // Уперлись в левый край.
        if (Board->field[ship->y-1][ship->x] == TRUE || Board->field[ship->y-1][ship->x+1] == TRUE)
            return FALSE;
        if (Board->field[ship->y+ship->size][ship->x] == TRUE || Board->field[ship->y+ship->size][ship->x+1] == TRUE)
            return FALSE;
    } else
    if (ship->x == Board->Width-1){ // Уперлись в правый край.
        if (Board->field[ship->y-1][ship->x] == TRUE || Board->field[ship->y-1][ship->x-1] == TRUE)
            return FALSE;
        if (Board->field[ship->y+ship->size][ship->x] == TRUE || Board->field[ship->y+ship->size][ship->x-1] == TRUE)
            return FALSE;
    } else // Посередине
    if (ship->x != 0 && ship->x != Board->Width-1)
        for (int i = ship->x-1; i <= ship->x+1; i++)
            if (Board->field[ship->y-1][i] == TRUE || Board->field[ship->y+ship->size][i] == TRUE)
                return FALSE;
    return TRUE;
}

bool checkItself(const ship* ship, const Board *Board){
    switch (ship->type) {
        case HORIZONTAL:
            for (int i = 0; i < ship->size; i++)
                if (Board->field[ship->y][ship->x+i] == TRUE)
                    return FALSE;
            break;
        case VERTICAL:
            for (int i = 0; i < ship->size; i++)
                if (Board->field[ship->y+i][ship->x] == TRUE)
                    return FALSE;
            break;
        default:
            Stopif(true, "checkItself(): unexpected switch case.");
    }
    return TRUE;
}

// -------------------------------------------------------------------------------

void clearTmpShip(ship* ship){
    ship->x = -1;
    ship->y = -1;
    ship->size = 0;
    ship->type = VERTICAL;
    ship->stand = FALSE;
}

void DrawTmpShip(WINDOW* WIN, ship* TmpShip, const Board *Board){
    if (checkShipBorders(TmpShip, Board) == FALSE)
        wattron(WIN, COLOR_PAIR(50));
    else 
        wattron(WIN, COLOR_PAIR(10));

    char rect = 254;
    int i = TmpShip->y;
    int j = TmpShip->x;
    switch(TmpShip->type) {
        case VERTICAL:
            for (; i < TmpShip->size + TmpShip->y; i++) {
                mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
            }
            break;
        case HORIZONTAL:
            for (; j < TmpShip->size + TmpShip->x; j++) {
                mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
            }
            break;
        default:
            Stopif(true, "DrawTmpShip(): unexpected switch case.");
    }
    wrefresh(WIN);
}

// -------------------------------------------------------------------------------

bool checkAllShipsStanding(const ShipsInfo *ShipsPlayer, const Board *BoardPlayer){
    for (int i = 0; i < getShipsNumber(ShipsPlayer); i++){
        if (ShipsPlayer->Ships[i].stand == FALSE)
            return FALSE;
    }
    return TRUE;
}

void InitPrimaryCoordinates(int ShipSize, ship* ship, const Board* const Board){
    int x = 0;
    int y = 0;
    while (checkPlace(x, y, ShipSize, Board) != TRUE){
        if (y+1 > Board->Height-1){
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

bool checkPlace(int x, int y, int size, const Board *Board){
    for (int i = 0; i < size ; i++){
            if (Board->field[y][x+i] == TRUE)
                return FALSE;
    }
    return TRUE;       
}

// -------------------------------------------------------------------------------

void changeShipCoordinates(ship* TmpShip, const Board *Board, const int key){
    switch (key) {
        case KEY_LEFT:
            if (checkBorderLeft(TmpShip, Board))
                TmpShip->x -= 1;
            else 
                if (TmpShip->type == HORIZONTAL) 
                    TmpShip->x = Board->Width - TmpShip->size;
                else 
                    TmpShip->x = Board->Width-1;
            break;
        case KEY_RIGHT:
            if (checkBorderRight(TmpShip, Board))
                TmpShip->x += 1;
            else
                TmpShip->x = 0;
            break;
        case KEY_UP:
            if (checkBorderTop(TmpShip, Board))
                TmpShip->y -= 1;
            else
                if (TmpShip->type == VERTICAL)
                    TmpShip->y = Board->Height - TmpShip->size;
                else
                    TmpShip->y = Board->Height-1;
            break;
        case KEY_DOWN:
            if (checkBorderBot(TmpShip, Board))
                TmpShip->y += 1;
            else
                TmpShip->y = 0;;
            break;
        default: // it is ok
            return;
    }
}

bool checkBorderLeft(ship* ship, const Board *Board){
    if (ship->x == 0)
        return FALSE;
    else 
        return TRUE;
}

bool checkBorderRight(ship* ship, const Board *Board){
    switch (ship->type) {
        case HORIZONTAL:
            if (ship->x + ship->size == Board->Width)
                return FALSE;
            else 
                return TRUE;
        case VERTICAL:
            if (ship->x == Board->Width-1)
                return FALSE;
            else 
                return TRUE;
    }
    return TRUE; // control should not be reached
}

bool checkBorderTop(ship* ship, const Board *Board){
    if (ship->y == 0)
        return FALSE;
    else 
        return TRUE;
}

bool checkBorderBot(ship* ship, const Board *Board){
    switch (ship->type) {
        case VERTICAL:
            if (ship->y + ship->size == Board->Height)
                return FALSE;
            else 
                return TRUE;
        case HORIZONTAL:
            if (ship->y == Board->Height-1)
                return FALSE;
            else 
                return TRUE;
        default:
            return TRUE; // control should not be reached
    }
}

// -------------------------------------------------------------------------------

void addShip(ship* newShip, ship* TmpShip){
    newShip->x = TmpShip->x;
    newShip->y = TmpShip->y;
    newShip->size = TmpShip->size;
    newShip->type = TmpShip->type;
    newShip->stand = TRUE;
}

void makeShipTmp(ship* oldShip, ship* TmpShip){
    TmpShip->x = oldShip->x;
    TmpShip->y = oldShip->y;
    TmpShip->size = oldShip->size;
    TmpShip->type = oldShip->type;
    oldShip->stand = HORIZONTAL;
    TmpShip->stand = TRUE;
}

void changeTypeOfShip(ship* ship, const Board *Board){
    switch (ship->type) {
        case HORIZONTAL:
            if (ship->y + ship->size > Board->Height)
                ship->y = Board->Height - ship->size;
            ship->type = TRUE;
            break;
        case VERTICAL:
            if (ship->x + ship->size > Board->Width)
                ship->x = Board->Width - ship->size;
            ship->type = FALSE;
            break;
        default:
            Stopif(true, "changeTypeOfShip(): unexpected case value.");
    }
}

// -------------------------------------------------------------------------------


bool isShipHit(const ship* ship, int coord_x, int coord_y){
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

void outputShip(FILE* out, const ship* const ship){
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
    for (size_t i = 0; i < board->Height; i++){
        for (size_t j = 0; j < board->Width; j++){
            fprintf(out, "%d ", board->field[i][j]);
        }
        fprintf(out, "\n");
    }
}