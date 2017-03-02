typedef struct ship_player
{
    int x;
    int y;
    bool type;
    bool stand;
} ship;

void standing_ship(int act_y, ship ship, bool field[10][15]){
    int size_of_ship = convert_size(act_y);
    switch (ship.type){
        case FALSE:
            for (int i = 0; i < size_of_ship-1; i++)
                field[ship.y][i+ship.x] = TRUE;
            break;
        case TRUE:
            for (int i = 0; i < size_of_ship-1; i++)
                field[i+ship.y][ship.x] = TRUE;
            break;
    }
}

void deleting_ship(int act_y, ship ship, bool field[10][15]){
    int size_of_ship = convert_size(act_y);
    switch (ship.type){
        case FALSE:
            for (int i = 0; i < size_of_ship-1; i++)
                field[ship.y][i+ship.x] = FALSE;
            break;
        case TRUE:
            for (int i = 0; i < size_of_ship-1; i++)
                field[i+ship.y][ship.x] = FALSE;
            break;
    }
}

void refresh_ship_player_gpaphics(WINDOW *WIN, bool field[10][15]){
    int rect = 254;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 15; j++){
            if (field[i][j] == TRUE){
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

bool check_border_left(ship ship){
    if (ship.x == 0)
        return FALSE;
    else 
        return TRUE;
}

bool check_border_right(int act_y, ship ship){
    switch (ship.type){
        case FALSE:
            if (ship.x + convert_size(act_y) == 16)
                return FALSE;
            else 
                return TRUE;
        case TRUE:
            if (ship.x == 14)
                return FALSE;
            else 
                return TRUE;
    }

}

bool check_border_top(ship ship){
    if (ship.y == 0)
        return FALSE;
    else 
        return TRUE;
}

bool check_border_bot(int act_y, ship ship){
    switch (ship.type){
        case TRUE:
            if (ship.y + convert_size(act_y) == 11)
                return FALSE;
            else 
                return TRUE;
        case FALSE:
            if (ship.y == 9)
                return FALSE;
            else 
                return TRUE;
    }

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

bool check_ship_right(int act_y, ship ship, bool field[10][15]){
    switch (ship.type){
        case TRUE: ;
            for (int i = 0; i < convert_size(act_y)-1; i++){
                if (field[ship.y+i][ship.x+1] == TRUE)
                    return FALSE;
            }
            return TRUE;

        case FALSE:
            if (field[ship.y][ship.x+convert_size(act_y)-1] == FALSE)
                return TRUE;
            else
                return FALSE;
    }     
}

bool check_ship_top(int act_y, ship ship, bool field[10][15]){
    switch (ship.type){
        case FALSE: ;
            for (int i = 0; i < convert_size(act_y)-1; i++){
                if (field[ship.y-1][ship.x+i] == TRUE)
                    return FALSE;
            }
            return TRUE;

        case TRUE:
            if (field[ship.y-1][ship.x] == FALSE)
                return TRUE;
            else
                return FALSE;
    }     
}

bool check_ship_bot(int act_y, ship ship, bool field[10][15]){
    switch (ship.type){
        case FALSE: ;
            for (int i = 0; i < convert_size(act_y)-1; i++){
                if (field[ship.y+1][ship.x+i] == TRUE)
                    return FALSE;
            }
            return TRUE;

        case TRUE:
            if (field[ship.y+convert_size(act_y)-1][ship.x] == FALSE)
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

void refresh_number_arrange(WINDOW *WIN, ship* ship, int *number_stand_ships){
    int number_4 = 0;
    int number_3 = 0;
    int number_2 = 0;
    int number_1 = 0;

    for (int i = 0; i < 2; i++)
        if (ship[i].stand == TRUE)
            number_4++;
    for (int i = 2; i < 5; i++)
        if (ship[i].stand == TRUE)
            number_3++;
    for (int i = 5; i < 9; i++)
        if (ship[i].stand == TRUE)
            number_2++;
    for (int i = 9; i < 15; i++)
        if (ship[i].stand == TRUE)
            number_1++;
   
    mvwprintw(WIN, 5 , 25, "%d / 2", number_4);
    mvwprintw(WIN, 7 , 25, "%d / 3", number_3);
    mvwprintw(WIN, 9 , 25, "%d / 4", number_2);
    mvwprintw(WIN, 11 , 25, "%d / 6", number_1);
    *number_stand_ships = number_1 + number_2 + number_3 + number_4;
    //*number_stand_ships = 15;
    wattron(WIN, COLOR_PAIR(2));
    mvwprintw(WIN, 13 , 6, "You have %d ships standing", *number_stand_ships);
    if (*number_stand_ships == 15){
        wattron(WIN, COLOR_PAIR(100));
        mvwprintw(WIN, 15 , 4, "You have arranged all your ships");
        mvwprintw(WIN, 17 , 7, "YOU CAN START THE BATTLE");
        mvwprintw(WIN, 19 , 9, "Press F2 to continue!");
    }
    wrefresh(WIN);
}
