#include "header.h"
#include "table.h"
#include "menu.h"

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
	start_y_ship = TopIndent;
	start_x_ship = LeftIndent;

 	// Размеры создаваемых окон.
    height_win_ship = 23;
	width_win_ship = 33; 
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

    int active_window = 1; // Номер активного окна.
    int number_stand_ships = 0; 
	int ch; // ??
    int key;
    int kr = 197; // ??

    // Количество кораблей, в дальнеёшем будет регулироваться.
    int N_4 = 2;
    int N_3 = 3;
    int N_2 = 4;
    int N_1 = 5;

    // Создание и инициализация данных о кораблях игрока и компьютера.
    struct ShipsInfo ShipsPlayer   = { N_4, N_3, N_2, N_1, NULL };
    struct ShipsInfo ShipsComputer = { N_4, N_3, N_2, N_1, NULL };
    ShipsPlayer.Ships   = malloc((N_1+N_2+N_3+N_4) * sizeof(ship));
    ShipsComputer.Ships = malloc((N_1+N_2+N_3+N_4) * sizeof(ship));
    for (int i = 0; i < N_1+N_2+N_3+N_4; i++){
    	ShipsPlayer.Ships[i].x = 0;
    	ShipsPlayer.Ships[i].y = 0;
    	ShipsPlayer.Ships[i].type = FALSE;
    	ShipsPlayer.Ships[i].stand = FALSE;
    	ShipsComputer.Ships[i].x = 0;
    	ShipsComputer.Ships[i].y = 0;
    	ShipsComputer.Ships[i].type = FALSE;
    	ShipsComputer.Ships[i].stand = FALSE;
    }

    //Размеры игорового поля, в дальнейшем в будут регулироваться.
    int board_width = 15;
    int board_height = 10;

 	// Создание и инициализация данных игровых досок игрока и компьютера.
    struct Board BoardPlayer   = { board_height, board_width, NULL };
    struct Board BoardComputer = { board_height, board_width, NULL };
    BoardPlayer.field   = malloc(BoardPlayer.Height * sizeof(bool*));
    BoardComputer.field = malloc(BoardComputer.Height * sizeof(bool*));
    for (int i = 0; i < BoardPlayer.Height; i++){
    	BoardPlayer.field[i]   = calloc(BoardPlayer.Width, sizeof(bool));
    	BoardComputer.field[i] = calloc(BoardComputer.Width, sizeof(bool));
    }
    showDebugFieid(BoardPlayer);

	initscr();
	cbreak();
    noecho(); // Не отображает символы.
    curs_set(FALSE);
	keypad(stdscr, TRUE);
    start_color();
    init_pair (2, COLOR_BLUE+8, COLOR_YELLOW+8); // Для окна aarange.
    init_pair (55, COLOR_GREEN+8, COLOR_YELLOW+8); // Для окна arrange.
    init_pair (3, COLOR_BLUE+8, COLOR_GREEN+8); // Для окна ship.
    init_pair (50, COLOR_BLACK+8, COLOR_CYAN+8); // Для неправильных корабликов на поле ship.
    init_pair (10, COLOR_RED+8, COLOR_YELLOW+8); // Для корабликов в окне ship.
    init_pair (100, COLOR_RED+8, COLOR_YELLOW+8); // Выбранный корабль в окне arrange.
    init_pair(1, COLOR_RED + 8, COLOR_BLUE); // ??
    init_pair (66, COLOR_BLACK+8, COLOR_BLACK+8); // ??
    init_pair (200, COLOR_BLUE+8, COLOR_WHITE+8); // ??

	savetty();
    // resize_term(heightOfMainWindow, widthOfMainWindow);
    resize_term(40,90); // Beta
    clear();
    refresh();

    WINDOW* win_menu = newwin(LINES, COLS, 0, 0);
    wbkgdset(win_menu, COLOR_PAIR(200));
    wclear(win_menu);
    // wattron(win_menu, COLOR_PAIR(2));
    for (int i = 0; i < LINES; i++)
    	mvwprintw(win_menu, i, 0, "%d", i);
    for (int i = 0; i < COLS; i++){
    	mvwprintw(win_menu, 0, i, "%d", i / 10);
    	mvwprintw(win_menu, 1, i, "%d", i % 10);
    }
    wrefresh(win_menu);

    GameDataCase* GDCases;
    GDCases = malloc(4 * sizeof(GameDataCase));
    initGameDataCases(GDCases);

    WINDOW* win_hello = newwin(9, 61, 2, 14);
    DrawHelloWindow(win_hello, 9, 61);

	WindowParametres *WParametres;
	WParametres = malloc(4 * sizeof(WindowParametres));
	initCaseWindowData(WParametres);
	for (int i = 0; i < 4; i++)
		DrawCaseWindow(&WParametres[i], GDCases, i, 2);



    while((key = getch()) != '\n') ;
    wbkgdset(win_hello, COLOR_PAIR(200));
    wclear(win_hello);
    wrefresh(win_hello);
    delwin(win_hello);



	win_ship = newwin(height_win_ship, width_win_ship, start_y_ship, start_x_ship);
    win_arrange = newwin(height_win_arrange, width_win_arrange, start_y_arrange, start_x_arrange);
    win_shoot = newwin(height_win_shoot, width_win_shoot, start_y_shoot, start_x_shoot);

    // // ------------------------->

    // ch = 219;
    // attron(COLOR_PAIR(2));
    // for (int i = 0; i < widthOfMainWindow; i++){
    // 	mvprintw(0, i, "%c", ch);
    // 	mvprintw(heightOfMainWindow-1, i, "%c", ch);
    // }
   	// for (int i = 0; i < heightOfMainWindow; i++){
    // 	mvprintw(i, 0, "%c", ch);
    // 	// mvprintw(i, widthOfMainWindow, "%c", ch);
   	// }

    // // ------------------------->

    // DrawTableWindow(win_ship, width_win_ship, height_win_ship);
    refresh();

    DrawTableWindow(win_ship, 3+BoardPlayer.Width*2, 3+BoardPlayer.Height*2);
    DrawDefaltArrangeWindow(win_arrange, ShipsPlayer);

    // Координаты перемещения курсора в win_arrange для выбора корабля.
    int active_x = 0;
    int currLine = 5;
    int currShip = 0;
    ch = 219;
    int n  = 79; // ??

    ship* TmpShip = malloc(sizeof(ship));
    clearTmpShip(TmpShip);

    // int index = convert_ship_index(currLine, active_x);
    int index;
    DrawActiveShip(win_arrange, currLine, currShip);
    wrefresh(win_arrange);

    enum actWind {ARRANGE = 1, SHIP = 2};


    while((key = getch()) != KEY_F(2)){
    	wattron(win_arrange, COLOR_PAIR(2));
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
						changeActiveShip(ShipsPlayer, &currShip, &currLine, key);
						DrawStandingShips(win_arrange, ShipsPlayer);
						DrawActiveShip(win_arrange, currLine, currShip);
	                    break;
	                case SHIP:
	                    changeShipCoordinates(TmpShip, BoardPlayer, key);
	                    refresh_ship_player_gpaphics(win_ship, BoardPlayer);
	                    DrawTmpShip(win_ship, TmpShip, BoardPlayer);
	                    showDebugFieid(BoardPlayer);
	                    tmp_otladchik_tmp_ship(TmpShip);
	                break;            
	            }
	            break;
	        case 9: {
	            switch(active_window){
	                case SHIP:
	                    changeTypeOfShip(TmpShip, BoardPlayer);
	                    refresh_ship_player_gpaphics(win_ship, BoardPlayer);	
	                    DrawTmpShip(win_ship, TmpShip, BoardPlayer);
	                    tmp_otladchik_tmp_ship(TmpShip);
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
	                    index = getIndex(currLine, currShip, ShipsPlayer);
                    	clearTmpShip(TmpShip);
	                    if (ShipsPlayer.Ships[index].stand == FALSE)
	                        InitPrimaryCoordinates(currLine, TmpShip, BoardPlayer);
	                    else {
	                    	deleteShipFromField(&ShipsPlayer.Ships[index], BoardPlayer);
	                    	makeShipTmp(&ShipsPlayer.Ships[index], TmpShip);
	                    }
                        refresh_ship_player_gpaphics(win_ship, BoardPlayer);
                        DrawTmpShip(win_ship, TmpShip, BoardPlayer);
	                    break;
	                case SHIP:
	                    index = getIndex(currLine, currShip, ShipsPlayer);
	                    if (checkShipBorders(TmpShip, BoardPlayer) == FALSE)
	                        DrawErrorMessage(win_arrange);
	                    else {
	                    	active_window = ARRANGE;
	                    	addShip(&ShipsPlayer.Ships[index], TmpShip);
	                    	DrawNewNumberOfStandingShips(win_arrange, ShipsPlayer.Ships, &number_stand_ships);
	                    	DrawStandingShips(win_arrange, ShipsPlayer);

	                    	refresh_ship_player_array(ShipsPlayer, BoardPlayer);
	                    	refresh_ship_player_gpaphics(win_ship, BoardPlayer);
	                    	// tmp_otladchik_tmp_ship(TmpShip);
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

    // choosing_comp_strategy(ship_comp_field);
    // podchet_ships(ship_player_field, ship_comp_field);
    
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

    // refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    // while((key = getch()) != KEY_F(1)){
    //     switch(key){	
    //         case KEY_LEFT:
    //             if (check_border_left_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){ 
    //                 change_x_leftkey(&x_shoot);
    //                 refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //             }/*
    //             else {
    //                 int var_x = x_shoot - 1;
    //                 for (var_x; var_x > 0; var_x++){
    //                     if (check_border_left_shoot(y_shoot, var_x, shoot_player_field) == TRUE){
    //                         x_shoot = var_x;
    //                         change_x_leftkey(&x_shoot);
    //                         refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //                         continue;
    //                     } 
    //                 break;
    //                 }
    //             }*/
    //             break;
    //         case KEY_RIGHT:
    //             if (check_border_right_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
    //                 change_x_rightkey(&x_shoot);
    //                 refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //             }/*
    //             else {
    //                 int var_x = x_shoot + 1;
    //                 for (var_x; var_x < 15; var_x--){
    //                     if (check_border_right_shoot(y_shoot, var_x, shoot_player_field) == TRUE){
    //                         x_shoot = var_x;
    //                         change_x_rightkey(&x_shoot);
    //                         refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //                         break;
    //                     } 
    //                 break;
    //                 }
    //             }*/
    //             break;
    //         case KEY_UP:
    //             if (check_border_top_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
    //                 change_y_topkey(&y_shoot);
    //                 refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //             }/*
    //             else {
    //                 int var_y = y_shoot - 1;
    //                 for (var_y; var_y > 0; var_y--){
    //                     if (check_border_top_shoot(y_shoot, var_y, shoot_player_field) == TRUE){
    //                         y_shoot = var_y;
    //                         change_y_topkey(&y_shoot);
    //                         refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //                         break;
    //                     } 
    //                 break;
    //                 }
    //             }*/
    //             break;
    //         case KEY_DOWN:
    //             if (check_border_bot_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
    //                 change_y_botkey(&y_shoot);
    //                 refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //             }
    //             else {
    //                 int var_y = y_shoot + 1;
    //                 for (var_y; var_y < 10; var_y++){
    //                     if (check_border_bot_shoot(y_shoot, var_y, shoot_player_field) == TRUE){
    //                         y_shoot = var_y;
    //                         change_y_botkey(&y_shoot);
    //                         refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //                         break;
    //                     } 
    //                 break;
    //                 }
    //             }
    //             break;
    //         case '\n':
    //             if (ship_comp_field[y_shoot][x_shoot] == TRUE){
    //                 ship_comp_field[y_shoot][x_shoot] = FALSE;
    //                 shoot_player_field[y_shoot][x_shoot] = 1;
    //                 if (
    //                     ship_comp_field[y_shoot-1][x_shoot-1] == FALSE &&
    //                     ship_comp_field[y_shoot-1][x_shoot] == FALSE &&
    //                     ship_comp_field[y_shoot-1][x_shoot+1] == FALSE &&
    //                     ship_comp_field[y_shoot][x_shoot-1] == FALSE &&
    //                     ship_comp_field[y_shoot][x_shoot+1] == FALSE &&
    //                     ship_comp_field[y_shoot+1][x_shoot-1] == FALSE &&
    //                     ship_comp_field[y_shoot+1][x_shoot] == FALSE &&
    //                     ship_comp_field[y_shoot+1][x_shoot+1] == FALSE
    //                     ){
    //                         shoot_player_field[y_shoot-1][x_shoot] = 0;
    //                         shoot_player_field[y_shoot+1][x_shoot] = 0;
    //                         shoot_player_field[y_shoot][x_shoot+1] = 0;
    //                         shoot_player_field[y_shoot][x_shoot-1] = 0;
    //                         shoot_player_field[y_shoot-1][x_shoot+1] = 0;
    //                         shoot_player_field[y_shoot+1][x_shoot+1] = 0;
    //                         shoot_player_field[y_shoot-1][x_shoot-1] = 0;
    //                         shoot_player_field[y_shoot+1][x_shoot-1] = 0;
    //                     }
    //             }
    //             else 
    //                 shoot_player_field[y_shoot][x_shoot] = 0;
    //             //x_shoot = ;
    //             //y_shoot = ;
    //             refresh_shoot_player_gpaphics(win_shoot, shoot_player_field, y_shoot, x_shoot);
    //             podchet_ships(ship_player_field, ship_comp_field);
    //             do {
    //                 ran_y = rand() % 10;
    //                 ran_x = rand() % 15;
    //             }
    //             while (shoot_comp_field[ran_y][ran_x] != -1);
    //             if (ship_player_field[ran_y][ran_x] == TRUE){
    //                 ship_player_field[ran_y][ran_x] = FALSE;
    //                 shoot_comp_field[ran_y][ran_x] = 1;
    //                 mvwprintw(win_ship, ran_y*2+3, ran_x*2+3, "%c", k);
    //             }
    //             else{
    //                 shoot_comp_field[ran_y][ran_x] = 0;
    //                 mvwprintw(win_ship, ran_y*2+3, ran_x*2+3, "%c", n);
    //             }
    //             wrefresh(win_ship);
    //             podchet_ships(ship_player_field, ship_comp_field);
    //             refresh_ship_player_gpaphics(win_ship, Board);
    
    // /*
    // move(29,0);
    // for (int i = 0; i < 10; i++){
    //     for (int j = 0; j < 15; j++)
    //         printw("%d ", shoot_comp_field[i][j]);
    //     printw("\n");
    // }
    // */
    // refresh();
    //             break;
    //     }
    // }
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
    for (int i = 0; i < Board.Height; i++){
        for (int j = 0; j < Board.Width; j++)
            printw("%d ", Board.field[i][j]);
        printw("\n");
    }
    refresh();
}

void tmp_otladchik_tmp_ship(ship* TmpShip){
	move(28,0);
	printw("X: %d, Y: %d, type: %d, stand: %d, size: %d", TmpShip->x, TmpShip->y, TmpShip->type, TmpShip->stand, TmpShip->size);
}

void refresh_ship_player_array(struct ShipsInfo Ships, struct Board Board){
    // !!! КОСТЫЛЬ !!!
    for (int i = 0; i < Ships.Number_4_Size+Ships.Number_3_Size+Ships.Number_2_Size+Ships.Number_1_Size; i++){
        if (Ships.Ships[i].stand == TRUE)
            standing_ship(&Ships.Ships[i], Board);
    }
        showDebugFieid(Board);
    // !!! КОСТЫЛЬ !!!
}

void standing_ship(ship* ship, struct Board Board){
    switch (ship->type){
        case FALSE:
            for (int i = 0; i < ship->size; i++)
                Board.field[ship->y][i+ship->x] = TRUE;
            break;
        case TRUE:
            for (int i = 0; i < ship->size; i++)
                Board.field[i+ship->y][ship->x] = TRUE;
            break;
    }
}

