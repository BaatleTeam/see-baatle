#include "ships.h"

void deleteShipFromField(ship* ship, struct Board Board){
    switch (ship->type){
        case FALSE:
            for (int i = 0; i < ship->size; i++)
                Board.field[ship->y][i+ship->x] = FALSE;
            break;
        case TRUE:
            for (int i = 0; i < ship->size; i++)
                Board.field[i+ship->y][ship->x] = FALSE;
            break;
    }
}

void refresh_ship_player_gpaphics(WINDOW *WIN, struct Board Board){
    int rect = 254;
    for (int i = 0; i < Board.Height; i++)
        for (int j = 0; j < Board.Width; j++){
            if (Board.field[i][j] == TRUE){
                wattron(WIN, COLOR_PAIR(10));
                mvwprintw(WIN,i*2+3,j*2+3, "%c", rect);
            }
            else {
                wattron(WIN, COLOR_PAIR(3));
                mvwprintw(WIN,i*2+3,j*2+3, " ");
            }
        }
    wrefresh(WIN);
}

bool check_ship_left(int act_y, ship ship, bool field[10][15]){
    switch (ship.type){
        case TRUE: ;
            for (int i = 0; i < convert_size(act_y)-1; i++){
                if (field[ship.y+i][ship.x-1] == TRUE)
                    return FALSE;
            }
            return TRUE;

        case FALSE:
            if (field[ship.y][ship.x-1] == FALSE)
                return TRUE;
            else
                return FALSE;
    }     
}

int convert_index_to_active_y(int index){
    if (index == 0 || index == 1)
        return 5;
    if (index == 2 || index == 3 || index == 4 )
        return 7;
    if (index == 5 || index == 6 || index == 7 || index == 8)
        return 9;
    if (index == 9 || index == 10 || index == 11 || index == 12 || index == 13 || index == 14 )
        return 11;
}

int convert_ship_index(int y, int x){
    switch (y)
    {
    case 5:
        if (x == 13)
            return 0;
        else 
            return 1;
        break;
    case 7:
        if (x == 13)
            return 2;
        if (x == 17)
            return 3;
        if (x == 21)
            return 4;
        break;
    case 9:
        if (x == 13)
            return 5;
        if (x == 16)
            return 6;
        if (x == 19)
            return 7;
        if (x == 22)
            return 8;
    case 11:
        if (x == 13)
            return 9;
        if (x == 15)
            return 10;
        if (x == 17)
            return 11;
        if (x == 19)
            return 12;
        if (x == 21)
            return 13;
        if (x == 23)
            return 14;
    default:
        break;
    }
}

// -------------------------------------------------------------------------------

bool checkShipBorders(ship* ship, struct Board Board){
    switch (ship->type){
        case FALSE: {
                if (checkShipBorders_top_bottom_horizontal(ship, Board) == FALSE ||
                    checkShipBorders_left_right_horizontal(ship, Board) == FALSE ||
                    checkItself(ship, Board) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
        }
        case TRUE: {
                if (checkShipBorders_top_bottom_vertical(ship, Board) == FALSE ||
                    checkShipBorders_left_right_vertical(ship, Board) == FALSE ||
                    checkItself(ship, Board) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
        }
    }     
}

bool checkShipBorders_top_bottom_horizontal(ship* ship, struct Board Board){
    if (ship->y == 0){ 
        for (int i = 0; i < ship->size; i++)
            if (Board.field[ship->y+1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    } else 
    if (ship->y == Board.Height-1){
        for (int i = 0; i < ship->size; i++)
            if (Board.field[ship->y-1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    } else {
        for (int i = 0; i < ship->size; i++)
            if (Board.field[ship->y-1][ship->x+i] == TRUE)
                return FALSE;
        for (int i = 0; i < ship->size; i++)
            if (Board.field[ship->y+1][ship->x+i] == TRUE)
                return FALSE;
        return TRUE;
    }
}

bool checkShipBorders_left_right_horizontal(ship* ship, struct Board Board){
    if (ship->x + ship->size == Board.Width-1){
        if (ship->y == 0)
            if (Board.field[0][ship->x-1] == TRUE || Board.field[1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->y == Board.Height-1)
            if (Board.field[Board.Height-1][ship->x-1] == TRUE || Board.field[Board.Height-1-1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->y != 0 && ship->y != Board.Height-1)
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (Board.field[i][ship->x-1] == TRUE)
                    return FALSE;
        return TRUE;
    };
    if (ship->x == 0){
        if (ship->y == 0)
            if (Board.field[0][ship->size] == TRUE || Board.field[1][ship->size] == TRUE)
                return FALSE;
        if (ship->y == Board.Height-1)
            if (Board.field[Board.Height-1][ship->size] == TRUE || Board.field[Board.Height-1-1][ship->size] == TRUE)
                return FALSE;
        if (ship->y != 0 && ship->y != Board.Height-1)
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (Board.field[i][ship->x+ship->size] == TRUE)
                    return FALSE;
        return TRUE;
    };

        if (ship->y == 0){
            if (Board.field[0][ship->size] == TRUE || Board.field[1][ship->size] == TRUE)
                return FALSE;
            if (Board.field[0][ship->x-1] == TRUE || Board.field[1][ship->x-1] == TRUE)
                return FALSE;
        } 
        if (ship->y == Board.Height-1){
            if (Board.field[Board.Height-1][ship->x-1] == TRUE || Board.field[Board.Height-1-1][ship->x-1] == TRUE)
                return FALSE;
            if (Board.field[Board.Height-1][ship->size] == TRUE || Board.field[Board.Height-1-1][ship->size] == TRUE)
                return FALSE;
        }
        if (ship->y != 0 && ship->y != Board.Height-1){
            for (int i = ship->y-1; i <= ship->y+1; i++)
                if (Board.field[i][ship->x+ship->size] == TRUE || Board.field[i][ship->x-1] == TRUE)
                    return FALSE;
        }
        return TRUE;
}

bool checkShipBorders_left_right_vertical(ship* ship, struct Board Board){
    switch (ship->x){
        case 0: { 
            for (int i = 0; i < ship->size; i++)
                if (Board.field[ship->y+i][ship->x+1] == TRUE)
                    return FALSE;
            return TRUE;
        }
        case 14: {
            for (int i = 0; i < ship->size; i++)
                if (Board.field[ship->y+i][ship->x-1] == TRUE)
                    return FALSE;
            return TRUE;
        }
        default:{
            for (int i = 0; i < ship->size; i++)
                if (Board.field[ship->y+i][ship->x+1] == TRUE)
                    return FALSE;
            for (int i = 0; i < ship->size; i++)
                if (Board.field[ship->y+i][ship->x-1] == TRUE)
                    return FALSE;
            return TRUE;
        }
    }
}
bool checkShipBorders_top_bottom_vertical(ship* ship, struct Board Board){
    if (ship->y + ship->size == Board.Height){
        if (ship->x == 0)
            if (Board.field[ship->y-1][ship->x] == TRUE || Board.field[ship->y-1][ship->x+1] == TRUE)
                return FALSE;
        if (ship->x == Board.Width)
            if (Board.field[ship->y-1][ship->x] == TRUE || Board.field[ship->y-1][ship->x-1] == TRUE)
                return FALSE;
        if (ship->x != 0 && ship->x != Board.Width)
            for (int i = ship->x-1; i <= ship->x+1; i++)
                if (Board.field[ship->y-1][i] == TRUE)
                    return FALSE;
    } else 
    if (ship->y == 0){
        if (ship->x == 0)
            if (Board.field[ship->size][ship->x] == TRUE || Board.field[ship->size][ship->x+1] == TRUE)
                return FALSE;
        if (ship->x == Board.Width)
            if (Board.field[ship->size][ship->x] == TRUE || Board.field[ship->size][ship->x-1] == TRUE)
                return FALSE;
        if (ship->x != 0 && ship->x != Board.Width)
            for (int i = ship->x-1; i <= ship->x+1; i++)
                if (Board.field[ship->size][i] == TRUE)
                    return FALSE;
    } else 
    for (int i = ship->x-1; i <= ship->x+1; i++)
        if (Board.field[ship->y-1][i] == TRUE || Board.field[ship->y+ship->size][i] == TRUE)
            return FALSE;
    return TRUE;
}

bool checkItself(ship* ship, struct Board Board){
    switch(ship->type){
        case FALSE:
            for (int i = 0; i < ship->size; i++)
                if (Board.field[ship->y][ship->x+i] == TRUE)
                    return FALSE;
            break;
        case TRUE:
            for (int i = 0; i < ship->size; i++)
                if (Board.field[ship->y+i][ship->x] == TRUE)
                    return FALSE;
            break;
    }
}

// -------------------------------------------------------------------------------

void clearTmpShip(ship* ship){
    ship->x = -1;
    ship->y = -1;
    ship->size = 0;
    ship->type = TRUE;
    ship->stand = FALSE;
}

void DrawTmpShip(WINDOW* WIN, ship* TmpShip, struct Board Board){
    if (checkShipBorders(TmpShip, Board) == FALSE)
        wattron(WIN, COLOR_PAIR(50));
    else 
        wattron(WIN, COLOR_PAIR(10));

    char rect = 254;
    int i = TmpShip->y;
    int j = TmpShip->x;
    switch(TmpShip->type){
        case TRUE:
            for (; i < TmpShip->size + TmpShip->y; i++){
                mvwprintw(WIN,i*2+3,j*2+3, "%c", rect);
            }
            break;
        case FALSE:
            for (; j < TmpShip->size + TmpShip->x; j++){
                mvwprintw(WIN,i*2+3,j*2+3, "%c", rect);
            }
            break;
    }
    wrefresh(WIN);
}

// -------------------------------------------------------------------------------

int InitPrimaryCoordinates(int act_y, ship* ship, struct Board Board){
    int x = 0;
    int y = 0;
    ship->size = convert_size(act_y)-1;
    while (checkPlace(x, y, ship->size, Board) != TRUE){
        if (y+1 > Board.Height-1){
            y = 0;
            x++;
        }
        else
            y++;
    }
    ship->x = x;
    ship->y = y ;
    ship->stand = TRUE;
    ship->type = FALSE;
}

bool checkPlace(int x, int y, int size, struct Board Board){
    for (int i = 0; i < size ; i++){
            if (Board.field[y][x+i] == TRUE)
                return FALSE;
    }
    return TRUE;       
}

// -------------------------------------------------------------------------------

void changeShipCoordinates(ship* TmpShip, struct Board Board, const int key){
    switch (key) {
        case KEY_LEFT:
            if (checkBorderLeft(TmpShip, Board))
                TmpShip->x -= 1;
            else 
                if (TmpShip->type == FALSE) 
                    TmpShip->x = Board.Width - TmpShip->size;
                else 
                    TmpShip->x = Board.Width-1;
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
                if (TmpShip->type == TRUE)
                    TmpShip->y = Board.Height - TmpShip->size;
                else
                    TmpShip->y = Board.Height-1;
            break;
        case KEY_DOWN:
            if (checkBorderBot(TmpShip, Board))
                TmpShip->y += 1;
            else
                TmpShip->y = 0;;
            break;
    }
}

bool checkBorderLeft(ship* ship, struct Board Board){
    if (ship->x == 0)
        return FALSE;
    else 
        return TRUE;
}

bool checkBorderRight(ship* ship, struct Board Board){
    switch (ship->type){
        case FALSE:
            if (ship->x + ship->size == Board.Width)
                return FALSE;
            else 
                return TRUE;
        case TRUE:
            if (ship->x == Board.Width-1)
                return FALSE;
            else 
                return TRUE;
    }
}

bool checkBorderTop(ship* ship, struct Board Board){
    if (ship->y == 0)
        return FALSE;
    else 
        return TRUE;
}

bool checkBorderBot(ship* ship, struct Board Board){
    switch (ship->type){
        case TRUE:
            if (ship->y + ship->size == Board.Height)
                return FALSE;
            else 
                return TRUE;
        case FALSE:
            if (ship->y == Board.Height-1)
                return FALSE;
            else 
                return TRUE;
    }
}

// -------------------------------------------------------------------------------

void addShip(ship* newShip, ship* TmpShip){
    newShip->x = TmpShip->x;
    newShip->y = TmpShip->y;
    newShip->size = TmpShip->size;
    newShip->type = TmpShip->type;
    newShip->stand = TmpShip->stand;
}

void makeShipTmp(ship* oldShip, ship* TmpShip){
    TmpShip->x = oldShip->x;
    TmpShip->y = oldShip->y;
    TmpShip->size = oldShip->size;
    TmpShip->type = oldShip->type;
    oldShip->stand = FALSE;
    TmpShip->stand = TRUE;
}

void changeTypeOfShip(ship* ship, struct Board Board){
    switch (ship->type){
        case FALSE:
            if (ship->y + ship->size > Board.Height)
                ship->y = Board.Height - ship->size;
            ship->type = TRUE;
            break;
        case TRUE:
            if (ship->x + ship->size > Board.Width)
                ship->x = Board.Width - ship->size;
            ship->type = FALSE;
            break;
    }
}