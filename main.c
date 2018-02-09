#include "header.h"
#include "table.h"

void showDebugFieid(struct Board Board);

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
 
    // Новые зависимости от размера отступов.
    height_win_ship = 23;
	width_win_ship = 33; 
	start_y_ship = TopIndent;
	start_x_ship = LeftIndent;

    height_win_arrange = 21; // min
    width_win_arrange = 38; // min
    start_y_arrange = TopIndent;
    start_x_arrange = LeftIndent + width_win_ship + BetweenIndent;

    // Размеры окна под стрельбу должны соотвествовать окну ship.
    width_win_shoot = width_win_ship;
    height_win_shoot = height_win_ship;
    start_x_shoot = start_x_arrange;
    start_y_shoot = start_y_arrange;

    int widthOfMainWindow = start_x_arrange + width_win_arrange + RightIndent;
    int heightOfMainWindow = TopIndent + height_win_ship + BottomIndent;

    WINDOW *win_ship;
    WINDOW *win_arrange;
    WINDOW *win_shoot;

    bool ship_player_field[10][15]; // Массив-доска с кораблями игрока.	
    bool ship_comp_field[10][15];
    int shoot_player_field[10][15];
    int shoot_comp_field[10][15];

    int active_window = 1; // Номер активного окна.
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

    for (int i = 0; i < 10; i++) // Инициализация всех игровых полей начальными условиями.
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

	savetty();
    // resize_term(heightOfMainWindow, widthOfMainWindow);
    resize_term(40,90); // Beta
    clear();
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

    // DrawTableWindow(win_ship, width_win_ship, height_win_ship);
    struct Board Board = {10, 10, NULL};
    Board.field = malloc(Board.Width * sizeof(bool*));
    for (int i = 0; i < Board.Width; i++)
    	Board.field[i] = calloc(Board.Height, sizeof(bool));
    showDebugFieid(Board);
    refresh();

    DrawTableWindow(win_ship, 3+Board.Width*2, 3+Board.Height*2);
    DrawDefaltArrangeWindow(win_arrange);
    DrawNewNumberOfStandingShips(win_arrange, ships_player, &number_stand_ships);

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


    while((key = getch()) != KEY_F(2)){
        for (int  i = 1; i < width_win_arrange-1; i++)
            mvwprintw(win_arrange, 15, i, " ");
        wrefresh(win_arrange);
        switch(key){
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
	            switch (active_window){
	                case ARRANGE:
	                	changeActiveShip(&active_x, &active_y, key);
	                    DrawStandingShips(win_arrange, ships_player);
	                    DrawActiveShip(win_arrange, active_y, active_x);
	                    break;
	                case SHIP:
	                    changeShipCoordinates(TmpShip, Board, key);
	                    refresh_ship_player_gpaphics(win_ship, Board);
	                    DrawTmpShip(win_ship, TmpShip, Board);
	                    showDebugFieid(Board);
	                    tmp_otladchik_tmp_ship(TmpShip);
	                break;            
	            }
	            break;
	        case 9: {
	            switch(active_window){
	                case SHIP:
	                    changeTypeOfShip(TmpShip, Board);
	                    refresh_ship_player_gpaphics(win_ship, Board);	
	                    DrawTmpShip(win_ship, TmpShip, Board);
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
                    	clearTmpShip(TmpShip);
	                    if (ships_player[index].stand == FALSE)
	                        InitPrimaryCoordinates(active_y, TmpShip, Board);
	                    else {
	                    	deleteShipFromField(&ships_player[index], Board);
	                    	makeShipTmp(&ships_player[index], TmpShip);
	                    }
                        refresh_ship_player_gpaphics(win_ship, Board);
                        DrawTmpShip(win_ship, TmpShip, Board);
	                    break;
	                case SHIP:
	                    index = convert_ship_index(active_y, active_x);
	                    if (checkShipBorders(TmpShip, Board) == FALSE)
	                        DrawErrorMessage(win_arrange);
	                    else {
	                    	active_window = ARRANGE;
	                    	addShip(&ships_player[index], TmpShip);
	                    	DrawNewNumberOfStandingShips(win_arrange, ships_player, &number_stand_ships);
	                    	DrawStandingShips(win_arrange, ships_player);

	                    	refresh_ship_player_array(ships_player, Board);
	                    	refresh_ship_player_gpaphics(win_ship, Board);
	                    }
	                    break;
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
                refresh_ship_player_gpaphics(win_ship, Board);
    
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
    resetty();
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


void showDebugFieid(struct Board Board){
	move(29,0);
    for (int i = 0; i < Board.Width; i++){
        for (int j = 0; j < Board.Height; j++)
            printw("%d ", Board.field[i][j]);
        printw("\n");
    }
    refresh();
}

void tmp_otladchik_tmp_ship(ship* TmpShip){
	move(28,0);
	printw("X: %d, Y: %d, type: %d, stand: %d, size: %d", TmpShip->x, TmpShip->y, TmpShip->type, TmpShip->stand, TmpShip->size);
}

void refresh_ship_player_array(ship* ship, struct Board Board){
    // !!! КОСТЫЛЬ !!!
    for (int i = 0; i < 15; i++){
        if (ship[i].stand == TRUE)
            standing_ship(convert_index_to_active_y(i), &ship[i], Board);
        showDebugFieid(Board);
    }
    // !!! КОСТЫЛЬ !!!
}

void standing_ship(int act_y, ship* ship, struct Board Board){
    int size_of_ship = convert_size(act_y);
    switch (ship->type){
        case FALSE:
            for (int i = 0; i < size_of_ship-1; i++)
                Board.field[ship->y][i+ship->x] = TRUE;
            break;
        case TRUE:
            for (int i = 0; i < size_of_ship-1; i++)
                Board.field[i+ship->y][ship->x] = TRUE;
            break;
    }
}