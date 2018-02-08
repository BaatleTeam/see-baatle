#include "header.h"
#include "table.h"

void clearTmpShip(ship*);
void DrawTmpShip(WINDOW*, ship* , bool field[10][15], int);
void addShip(ship* , ship* TmpShip);
void tmp_otladchik_tmp_ship(ship* TmpShip);
void changeShipCoordinates(ship* TmpShip, int key);
void makeShipTmp(ship* oldShip, ship* TmpShip);
void changeTypeOfShip(ship* TmpShip, bool field[10][15]);

int main(){	
    srand(time(NULL));
    // Отступы между окнами и краями главного окна.
    int LeftIndent, RightIndent, BetweenIndent, TopIndent, BottomIndent;
    LeftIndent = 4;
    BetweenIndent = 3;
    RightIndent = 4;
    TopIndent = 3;
    BottomIndent = 4;

    // Далее параметры создаваемых окон.
	int start_x_ship, start_y_ship, width_win_ship, height_win_ship;
    int start_x_arrange, start_y_arrange, width_win_arrange, height_win_arrange;
    int start_x_shoot, start_y_shoot, width_win_shoot, height_win_shoot;

    // Основным окном считается окно расстановки кораблей ship,
    // Поэтому размеры окна arrange соответсвуют окну ship. 
    // Новые зависимости от размера отступов.
    height_win_ship = 23;
	width_win_ship = 33; 
	start_y_ship = TopIndent;
	start_x_ship = LeftIndent;

    height_win_arrange = height_win_ship;
    width_win_arrange = 38;
    start_y_arrange = TopIndent;
    start_x_arrange = LeftIndent + width_win_ship + BetweenIndent;

    // Размеры окна под стрельбу должны соотвествовать окну ship.
    width_win_shoot = width_win_ship;
    height_win_shoot = height_win_ship;
    start_x_shoot = start_x_arrange;
    start_y_shoot = start_y_arrange;

    int widthOfMainWindow = start_x_arrange + width_win_arrange + RightIndent;
    int heightOfMainWindow = TopIndent + height_win_ship + BottomIndent;

    system ("mode con cols=90 lines=40"); // Cols = 82;

    WINDOW *win_ship;
    WINDOW *win_arrange;
    WINDOW *win_shoot;

    bool ship_player_field[10][15]; 
    bool ship_comp_field[10][15];
    int shoot_player_field[10][15];
    int shoot_comp_field[10][15];

    int active_window = 1; // Номер активного окна. Неплохо бы реализовать через enum.
    int number_stand_ships = 0; 
	int ch; // ??
    int key;
    int kr = 197; // ??
    
    ship ships_player[15]; // Инициализация кораблей для игрока.
    for (int i = 0; i < 15; i++){
        ships_player[i].x = 0;
        ships_player[i].y = 0;
        ships_player[i].type = FALSE;
        ships_player[i].stand = FALSE;
    }

    for (int i = 0; i < 10; i++) //Инициализация всех игровых полей начальными условиями.
       for (int j = 0; j < 15; j++){
           ship_player_field[i][j] = FALSE;
           ship_comp_field[i][j] = FALSE;
           shoot_player_field[i][j] = -1;
           shoot_comp_field[i][j] = -1;
       }

	initscr();
	cbreak();
    noecho(); // Не отображает символы.
    curs_set(FALSE);
	keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE);
    init_pair (2, COLOR_BLUE, COLOR_YELLOW);
    init_pair (3, COLOR_BLUE, COLOR_GREEN);
    init_pair (10, COLOR_RED, COLOR_WHITE);
    init_pair (100, COLOR_RED, COLOR_YELLOW);
    init_pair (66, COLOR_BLACK, COLOR_BLACK);
    init_pair (50, COLOR_RED, COLOR_BLACK);
    init_pair (200, COLOR_RED, COLOR_GREEN);
    init_pair (55, COLOR_GREEN, COLOR_YELLOW);

    refresh();

	win_ship = newwin(height_win_ship, width_win_ship, start_y_ship, start_x_ship);
    win_arrange = newwin(height_win_arrange, width_win_arrange, start_y_arrange, start_x_arrange);
    win_shoot = newwin(height_win_shoot, width_win_shoot, start_y_shoot, start_x_shoot);

    // ------------------------->

    ch = 219;
    attron(COLOR_PAIR(2));
    for (int i = 0; i < widthOfMainWindow; i++){
    	mvprintw(0, i, "%c", ch);
    	mvprintw(heightOfMainWindow-1, i, "%c", ch);
    }
   	for (int i = 0; i < heightOfMainWindow; i++){
    	mvprintw(i, 0, "%c", ch);
    	// mvprintw(i, widthOfMainWindow, "%c", ch);
   	}

    // ------------------------->

    DrawTableWindow(win_ship, width_win_ship, height_win_ship);
    DrawDefaltArrangeWindow(win_arrange);
    DrawNewNumberOfStandingShips(win_arrange, ships_player, &number_stand_ships);
    wrefresh(win_ship);

    // Координаты перемещения курсора в win_arrange для выбора корабля.
    int active_x = 13;
    int active_y = 5;
    ch = 219;
    int n  = 79; // ??

    ship* TmpShip = malloc(sizeof(ship));
    clearTmpShip(TmpShip);

    int index = convert_ship_index(active_y, active_x);
    DrawActiveShip(win_arrange, active_y, active_x);
    wrefresh(win_arrange);

    enum actWind {ARRANGE = 1, SHIP = 2};

    refresh_ship_player_gpaphics(win_ship, ship_player_field);

    while((key = getch()) != KEY_F(2)){
        for (int  i = 1; i < width_win_arrange-1; i++)
            mvwprintw(win_arrange, 15, i, " ");
        wrefresh(win_arrange);
        switch(key){
        case KEY_LEFT:
            switch (active_window){
                case ARRANGE:
                    check_left(&active_x, active_y);
                    DrawStandingShips(win_arrange, ships_player);
                    DrawActiveShip(win_arrange, active_y, active_x);
                    break;
                case SHIP:
                    changeShipCoordinates(TmpShip, key);
                    refresh_ship_player_gpaphics(win_ship, ship_player_field);
                	DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                    tmp_otladchik(ship_player_field);
                    tmp_otladchik_tmp_ship(TmpShip);
                    // index = convert_ship_index(active_y, active_x);
                    // if (check_border_left(ships_player[index]) == TRUE /*&& check_ship_left(active_y, ships_player[index], ship_player_field) == TRUE*/){
                    //     deleting_ship(active_y, ships_player[index], ship_player_field);
                    //     ships_player[index].x -= 1;
                    //     standing_ship(active_y, &ships_player[index], ship_player_field);
                    //     refresh_ship_player_gpaphics(win_ship, ship_player_field);
                    //     tmp_otladchik(ship_player_field);
                    // }
                    break;
            }
            break;
        case KEY_RIGHT:
            switch (active_window){
                case ARRANGE:
                    check_right(&active_x, active_y);
                    DrawStandingShips(win_arrange, ships_player);
                    DrawActiveShip(win_arrange, active_y, active_x);
                    break;
                case SHIP:
                    changeShipCoordinates(TmpShip, key);
                    refresh_ship_player_gpaphics(win_ship, ship_player_field);
                	DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                    tmp_otladchik(ship_player_field);
                    tmp_otladchik_tmp_ship(TmpShip);
	                //     // index = convert_ship_index(active_y, active_x);
	                //     if (check_border_right(active_y, ships_player[index]) == TRUE /*&& check_ship_right(active_y, ships_player[index], ship_player_field) == TRUE*/){
	                //         deleting_ship(active_y, ships_player[index], ship_player_field);
	                //         ships_player[index].x+=1;
	                //         // standing_ship(active_y, &ships_player[index], ship_player_field);
	                //         // refresh_ship_player_gpaphics(win_ship, ship_player_field);
	                //         // tmp_otladchik(ship_player_field);          
	                // }
                break;
            }
            break;
        case KEY_UP:
            switch (active_window){
                case ARRANGE:
                    wattron(win_arrange, COLOR_PAIR(2));
                    if (active_y-2 < 5)
                        active_y = 11;
                    else active_y-=2;
                    active_x = 13;
                    DrawStandingShips(win_arrange, ships_player);
                    DrawActiveShip(win_arrange, active_y, active_x);
                    break;
                case SHIP:
					changeShipCoordinates(TmpShip, key);
                    refresh_ship_player_gpaphics(win_ship, ship_player_field);
                	DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                    tmp_otladchik(ship_player_field);
                    tmp_otladchik_tmp_ship(TmpShip);
                 //    index = convert_ship_index(active_y, active_x);
                 //    if (check_border_top(ships_player[index]) == TRUE /*&& check_ship_top(active_y, ships_player[index], ship_player_field) == TRUE*/){
                 //        deleting_ship(active_y, ships_player[index], ship_player_field);
                 //        ships_player[index].y-=1;
                 //        standing_ship(active_y, &ships_player[index], ship_player_field);
                 //        refresh_ship_player_gpaphics(win_ship, ship_player_field);
                 //        tmp_otladchik(ship_player_field);
                	// }
                break;            
            }
            break;
        case KEY_DOWN:
            switch (active_window){
                case ARRANGE:
                    wattron(win_arrange, COLOR_PAIR(2));
                    if (active_y+2 > 11)
                    	active_y = 5;
                    else 
                    	active_y+=2;
                    active_x = 13;
                    DrawStandingShips(win_arrange, ships_player);
                    DrawActiveShip(win_arrange, active_y, active_x);
                    break;
                case SHIP:
               		changeShipCoordinates(TmpShip, key);
                    refresh_ship_player_gpaphics(win_ship, ship_player_field);
                	DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                    tmp_otladchik(ship_player_field);
                    tmp_otladchik_tmp_ship(TmpShip);	
                 //    index = convert_ship_index(active_y, active_x);
                 //    if (check_border_bot(active_y, ships_player[index]) == TRUE /*&& check_ship_bot(active_y, ships_player[index], ship_player_field) == TRUE*/){
                 //        deleting_ship(active_y, ships_player[index], ship_player_field);
                 //        ships_player[index].y+=1;
                 //        standing_ship(active_y, &ships_player[index], ship_player_field);
                 //        refresh_ship_player_gpaphics(win_ship, ship_player_field);
                 //        tmp_otladchik(ship_player_field);
                	// }	
                break;
            }
            break;
        case 9: {
            switch(active_window){
                case SHIP:
                    // index = convert_ship_index(active_y, active_x);
                    changeTypeOfShip(TmpShip, ship_player_field);
                    refresh_ship_player_gpaphics(win_ship, ship_player_field);	
                    DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                    // if (ships_player[index].type == FALSE && 
                    //     ships_player[index].y + convert_size(active_y) - 1 > 10 ||
                    //     ships_player[index].type == TRUE &&
                    //     ships_player[index].x + convert_size(active_y) -1 > 15)
                    //     break;
                    // else {
                    //     deleteShipFromField(&ships_player[index], ship_player_field);
                    //     if (ships_player[index].type == FALSE)
                    //         ships_player[index].type = TRUE;
                    //     else 
                    //         ships_player[index].type = FALSE;
                    //     standing_ship(active_y, &ships_player[index], ship_player_field);
                    //     refresh_ship_player_gpaphics(win_ship, ship_player_field);
                    // }
                    break;
                case ARRANGE:
                	break;
        }
    }
    break;
        case '\n':
            switch (active_window){
                case ARRANGE:
                    active_window = SHIP;
                    index = convert_ship_index(active_y, active_x);
                    if (ships_player[index].stand == FALSE){
                    	clearTmpShip(TmpShip);
                        begin_coord(active_y, TmpShip, ship_player_field);
                        refresh_ship_player_gpaphics(win_ship, ship_player_field);
                        DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                        tmp_otladchik(ship_player_field);
                    } 
                    else {
                    	clearTmpShip(TmpShip);
                    	deleteShipFromField(&ships_player[index], ship_player_field);
                    	makeShipTmp(&ships_player[index], TmpShip);
                    	refresh_ship_player_gpaphics(win_ship, ship_player_field);
                    	DrawTmpShip(win_ship, TmpShip, ship_player_field, active_y);
                    	tmp_otladchik(ship_player_field);
                    }
                    break;
                case SHIP:
                    index = convert_ship_index(active_y, active_x);
                    if (check_ship_borders(active_y, TmpShip, ship_player_field) == FALSE){
                        DrawErrorMessage(win_arrange);
                        break;
                    }
                    else {
                    	addShip(&ships_player[index], TmpShip);
                        refresh_ship_player_gpaphics(win_ship, ship_player_field);
                    	DrawNewNumberOfStandingShips(win_arrange, ships_player, &number_stand_ships);
                    	DrawStandingShips(win_arrange, ships_player);
                    	active_window = ARRANGE;

                    	refresh_ship_player_array(ships_player, ship_player_field);
                    	refresh_ship_player_gpaphics(win_ship, ship_player_field);
                    	tmp_otladchik(ship_player_field);
                    /*
                    // !!! КОСТЫЛЬ !!!
                    for (int i = 0; i < 15; i++){
                        if (ships_player[i].stand == TRUE)
                            standing_ship(convert_in&dex_to_active_y(i), ships_player[i], ship_player_field);
                        refresh_ship_player_gpaphics(win_ship, ship_player_field);
                        tmp_otladchik(ship_player_field); 
                    }
                    // !!! КОСТЫЛЬ !!!
                    */
                    break;
                    }
        }
    }
}
    wbkgdset(win_arrange, COLOR_PAIR(66));
    wclear(win_arrange);
	wrefresh(win_arrange);

    wbkgdset(win_shoot, COLOR_PAIR(2));
    wclear(win_shoot);
	wrefresh(win_shoot);

    str_top(win_shoot, width_win_shoot);
    wattron(win_shoot,COLOR_PAIR(3));
    for (int i = 2; i < height_win_shoot-1; i+=2){
        str_line(win_shoot, width_win_shoot, i);
        str_numb(win_shoot, width_win_shoot, i+1);
    }
    str_bottom(win_shoot, width_win_shoot, height_win_shoot);
    wrefresh(win_shoot);

    choosing_comp_strategy(ship_comp_field);
    podchet_ships(ship_player_field, ship_comp_field);
    
    mvprintw(7, 76, "X - hit");
    mvprintw(9, 76, "O - miss");
    mvprintw(11, 74, "The game will last");
    mvprintw(12, 74, "as long as the number");
    mvprintw(13, 74, "of ships of any of the");
    mvprintw(14, 74, "players will");
    mvprintw(15, 74, "not fall lo zero.");
    attron(COLOR_PAIR(50));
    mvprintw(18, 80, "GOOD LUCK!");
    attroff(COLOR_PAIR(50));
    refresh();

    int x_shoot = 0;
    int y_shoot = 0;
    int ran_x;
    int ran_y;
    char k = 88;

    //ship_comp_field[2][2] = TRUE;

    /*
    move(29,0);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 15; j++)
            printw("%d ", shoot_comp_field[i][j]);
        printw("\n");
    }
    */
    refresh();
    wattron(win_ship, COLOR_PAIR(200));

    refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    while((key = getch()) != KEY_F(1)){
        switch(key){	
            case KEY_LEFT:
                if (check_border_left_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){ 
                    change_x_leftkey(&x_shoot);
                    refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                }/*
                else {
                    int var_x = x_shoot - 1;
                    for (var_x; var_x > 0; var_x++){
                        if (check_border_left_shoot(y_shoot, var_x, shoot_player_field) == TRUE){
                            x_shoot = var_x;
                            change_x_leftkey(&x_shoot);
                            refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                            continue;
                        } 
                    break;
                    }
                }*/
                break;
            case KEY_RIGHT:
                if (check_border_right_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
                    change_x_rightkey(&x_shoot);
                    refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                }/*
                else {
                    int var_x = x_shoot + 1;
                    for (var_x; var_x < 15; var_x--){
                        if (check_border_right_shoot(y_shoot, var_x, shoot_player_field) == TRUE){
                            x_shoot = var_x;
                            change_x_rightkey(&x_shoot);
                            refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                            break;
                        } 
                    break;
                    }
                }*/
                break;
            case KEY_UP:
                if (check_border_top_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
                    change_y_topkey(&y_shoot);
                    refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                }/*
                else {
                    int var_y = y_shoot - 1;
                    for (var_y; var_y > 0; var_y--){
                        if (check_border_top_shoot(y_shoot, var_y, shoot_player_field) == TRUE){
                            y_shoot = var_y;
                            change_y_topkey(&y_shoot);
                            refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                            break;
                        } 
                    break;
                    }
                }*/
                break;
            case KEY_DOWN:
                if (check_border_bot_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
                    change_y_botkey(&y_shoot);
                    refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                }/*
                else {
                    int var_y = y_shoot + 1;
                    for (var_y; var_y < 10; var_y++){
                        if (check_border_bot_shoot(y_shoot, var_y, shoot_player_field) == TRUE){
                            y_shoot = var_y;
                            change_y_botkey(&y_shoot);
                            refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                            break;
                        } 
                    break;
                    }
                }*/
                break;
            case '\n':
                if (ship_comp_field[y_shoot][x_shoot] == TRUE){
                    ship_comp_field[y_shoot][x_shoot] = FALSE;
                    shoot_player_field[y_shoot][x_shoot] = 1;
                    if (
                        ship_comp_field[y_shoot-1][x_shoot-1] == FALSE &&
                        ship_comp_field[y_shoot-1][x_shoot] == FALSE &&
                        ship_comp_field[y_shoot-1][x_shoot+1] == FALSE &&
                        ship_comp_field[y_shoot][x_shoot-1] == FALSE &&
                        ship_comp_field[y_shoot][x_shoot+1] == FALSE &&
                        ship_comp_field[y_shoot+1][x_shoot-1] == FALSE &&
                        ship_comp_field[y_shoot+1][x_shoot] == FALSE &&
                        ship_comp_field[y_shoot+1][x_shoot+1] == FALSE
                        ){
                            shoot_player_field[y_shoot-1][x_shoot] = 0;
                            shoot_player_field[y_shoot+1][x_shoot] = 0;
                            shoot_player_field[y_shoot][x_shoot+1] = 0;
                            shoot_player_field[y_shoot][x_shoot-1] = 0;
                            shoot_player_field[y_shoot-1][x_shoot+1] = 0;
                            shoot_player_field[y_shoot+1][x_shoot+1] = 0;
                            shoot_player_field[y_shoot-1][x_shoot-1] = 0;
                            shoot_player_field[y_shoot+1][x_shoot-1] = 0;
                        }
                }
                else 
                    shoot_player_field[y_shoot][x_shoot] = 0;
                //x_shoot = ;
                //y_shoot = ;
                refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
                podchet_ships(ship_player_field, ship_comp_field);
                do {
                    ran_y = rand() % 10;
                    ran_x = rand() % 15;
                }
                while (shoot_comp_field[ran_y][ran_x] != -1);
                if (ship_player_field[ran_y][ran_x] == TRUE){
                    ship_player_field[ran_y][ran_x] = FALSE;
                    shoot_comp_field[ran_y][ran_x] = 1;
                    mvwprintw(win_ship, ran_y*2+3, ran_x*2+3, "%c", k);
                }
                else{
                    shoot_comp_field[ran_y][ran_x] = 0;
                    mvwprintw(win_ship, ran_y*2+3, ran_x*2+3, "%c", n);
                }
                wrefresh(win_ship);
                podchet_ships(ship_player_field, ship_comp_field);
                //refresh_ship_player_gpaphics(win_ship, ship_player_field);
    
    /*
    move(29,0);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 15; j++)
            printw("%d ", shoot_comp_field[i][j]);
        printw("\n");
    }
    */
    refresh();
                break;
        }
    }
	endwin();
	return 0;
}

void podchet_ships(bool ship_player_field[10][15], bool ship_comp_field[10][15]){
    int number_ships_comp = 0;
    int number_ships_player = 0;
    for (int i = 0; i < 10; i++)
       for (int j = 0; j < 15; j++){
           if (ship_player_field[i][j] == TRUE)
                number_ships_player++;
            if (ship_comp_field[i][j] == TRUE)
                number_ships_comp++; 
       }
    if (number_ships_player == 0 || number_ships_comp == 0){
        if (number_ships_player == 0){
            attron(COLOR_PAIR(66));
            mvprintw(4, 12, "                 ");
            mvprintw(4, 47, "                 ");
            attroff(COLOR_PAIR(50));
            mvprintw(4, 16, "You lose!");
            mvprintw(4, 47, "Computer wins!");
        }
        if (number_ships_comp == 0){
            attron(COLOR_PAIR(66));
            mvprintw(4, 12, "                 ");
            mvprintw(4, 47, "                 ");
            attroff(COLOR_PAIR(50));
            mvprintw(4, 16, "You win!");
            mvprintw(4, 47, "Computer lose!");
        }
    }
    else {
    mvprintw(4, 12, "Number of ships: %d ", number_ships_player);
    mvprintw(4, 47, "Number of ships: %d ", number_ships_comp);
    }
}

bool check_ship_borders(int act_y, ship* ship, bool field[10][15]){
    int size_of_ship = convert_size(act_y)-1;
    switch (ship->type){
        case FALSE: {
                if (check_ship_borders_top_bottom_horizontal(ship, field) == FALSE ||
                    check_ship_borders_left_right_horizontal(ship, field) == FALSE ||
                    checkItself(ship, field) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
        }
        case TRUE: {
                if (check_ship_borders_top_bottom_vertical(ship, field) == FALSE ||
                    check_ship_borders_left_right_vertical(ship, field) == FALSE)
                    return FALSE;
                else 
                    return TRUE;
        }
    }     
}





int begin_coord(int act_y, ship* ship, bool field[10][15]){
    int x = 0;
    int y = 0;
    ship->size = convert_size(act_y)-1;
    move(2,0);
    while (dop_begin(x, y, ship->size, field) != TRUE){
        if (y+1 > 9){
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

bool dop_begin(int x, int y, int size, bool field[10][15]){
    for (int i = 0; i < size ; i++){
            if(field[y][x+i] == TRUE)
                return FALSE;
    }
    return TRUE;       
}

void refresh_shoot_player_gpaphics(WINDOW *WIN, int field[10][15], int y, int x){
    int mimo = 79;
    int popal = 88;
    int rect = 254;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 15; j++){
            switch(field[i][j]){
                case -1:
                    wattron(WIN,COLOR_PAIR(3));
                    mvwprintw(WIN,i*2+3,j*2+3, " ");
                    break;
                case 0:
                    wattron(WIN,COLOR_PAIR(3));
                    mvwprintw(WIN,i*2+3,j*2+3, "%c", mimo);
                    break;
                case 1:
                    wattron(WIN,COLOR_PAIR(3));
                    mvwprintw(WIN,i*2+3,j*2+3, "%c", popal);
                    break;
            }
        }
    wattron(WIN,COLOR_PAIR(100));
    mvwprintw(WIN, y*2+3, x*2+3, "%c", rect);
    wrefresh(WIN);
}



void tmp_otladchik(bool field[10][15]){
    move(29,0);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 15; j++)
            printw("%d ", field[i][j]);
        printw("\n");
    }
    refresh();
}

void tmp_otladchik_tmp_ship(ship* TmpShip){
	move(28,0);
	printw("X: %d, Y: %d, type: %d, stand: %d, size: %d", TmpShip->x, TmpShip->y, TmpShip->type, TmpShip->stand, TmpShip->size);
}

void refresh_ship_player_array(ship* ship, bool field[10][15]){
    // !!! КОСТЫЛЬ !!!
    for (int i = 0; i < 15; i++){
        if (ship[i].stand == TRUE)
            standing_ship(convert_index_to_active_y(i), &ship[i], field);
        tmp_otladchik(field); 
    }
    // !!! КОСТЫЛЬ !!!
}

void clearTmpShip(ship* ship){
	ship->x = -1;
	ship->y = -1;
	ship->size = 0;
	ship->type = TRUE;
	ship->stand = FALSE;
}

void DrawTmpShip(WINDOW* WIN, ship* TmpShip, bool field[10][15], int act_y){
	if (check_ship_borders(act_y, TmpShip, field) == FALSE)
		wattron(WIN, COLOR_PAIR(50));
	else 
		wattron(WIN, COLOR_PAIR(10));

	char rect = 254;
	int i = TmpShip->y;
	int j = TmpShip->x;
	switch(TmpShip->type){
		case TRUE:
			for(; i < TmpShip->size + TmpShip->y; i++){
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

void addShip(ship* newShip, ship* TmpShip){
	newShip->x = TmpShip->x;
	newShip->y = TmpShip->y;
	newShip->size = TmpShip->size;
	newShip->type = TmpShip->type;
	newShip->stand = TmpShip->stand;
}

void changeShipCoordinates(ship* TmpShip, int key){
	switch (key) {
		case KEY_LEFT:
			if (checkBorderLeft(TmpShip))
				TmpShip->x -= 1;
			break;
		case KEY_RIGHT:
			if (checkBorderRight(TmpShip))
				TmpShip->x += 1;
			break;
		case KEY_UP:
			if (checkBorderTop(TmpShip))
				TmpShip->y -= 1;
			break;
		case KEY_DOWN:
			if (checkBorderBot(TmpShip))
				TmpShip->y += 1;
			break;
	}
}

// Функции типа changeBorder возвращают true при возможности подвинуть корбаль.


void makeShipTmp(ship* oldShip, ship* TmpShip){
	TmpShip->x = oldShip->x;
	TmpShip->y = oldShip->y;
	TmpShip->size = oldShip->size;
	TmpShip->type = oldShip->type;
	oldShip->stand = FALSE;
	TmpShip->stand = TRUE;
}

void changeTypeOfShip(ship* ship, bool field[10][15]){
	switch(ship->type){
		case FALSE:
			if (ship->y + ship->size <= 10)
				ship->type = TRUE;
			break;
		case TRUE:
			if (ship->x + ship->size <= 15)
				ship->type = FALSE;
			break;
	}
}