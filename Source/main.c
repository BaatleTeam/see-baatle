#include "header.h"
#include "table.h"
#include "menu.h"

int main(){	
    srand(time(NULL));
    initscr();
	cbreak();
    noecho(); // Не отображает символы.
    curs_set(FALSE);
	keypad(stdscr, TRUE);
    start_color();
    init_pair (2, COLOR_BLUE+8, COLOR_YELLOW+8); // Для окна aarange.
    init_pair (55, COLOR_GREEN+8, COLOR_YELLOW+8); // Для окна arrange.
    init_pair (3, COLOR_BLUE+8, COLOR_GREEN+8); // Для окна ship, выбора окна case.
    init_pair (33, COLOR_RED+8, COLOR_GREEN+8); // Для окна ship, выбора размера доски.
    init_pair (50, COLOR_BLACK+8, COLOR_CYAN+8); // Для неправильных корабликов на поле ship.
    init_pair (10, COLOR_RED+8, COLOR_YELLOW+8); // Для корабликов в окне ship.
    init_pair (100, COLOR_RED+8, COLOR_YELLOW+8); // Выбранный корабль в окне arrange.
    init_pair (1, COLOR_RED + 8, COLOR_BLUE); // ??
    init_pair (66, COLOR_BLACK+8, COLOR_BLACK+8); // ??
    init_pair (200, COLOR_BLUE+8, COLOR_WHITE+8); // ??
	savetty();

    // Создание и инициализация возможных вариантов игры.
    GameDataCase* GDCases;
    GDCases = malloc(GAME_CASES_NUMBER * sizeof(GameDataCase));
    initGameDataCases(GDCases);

    int caseShips; // тип кол-ва кораблйей
    int caseBoard; // тип размера поля
    choosingGDCase(GDCases, &caseShips,&caseBoard);

    // Создание и инициализация данных о кораблях игрока и компьютера.
    ShipsInfo ShipsPlayer;
    ShipsInfo ShipsComputer;
    initShipsInfo(&GDCases[caseShips], &ShipsPlayer);
    initShipsInfo(&GDCases[caseShips], &ShipsComputer);
    
    // Создание и инициализация размеров игорового поля.
    Board BoardPlayer;
    Board BoardComputer;
    initBoard(&BoardPlayer, GDCases[caseShips].BoardHeight[caseBoard], GDCases[caseShips].BoardWidth[caseBoard]);
    initBoard(&BoardComputer, GDCases[caseShips].BoardHeight[caseBoard], GDCases[caseShips].BoardWidth[caseBoard]);

    free(GDCases);
    // Закончили выбор режима игры, освобождаем данные, начинаем отрисовку окна расстановки.

    arrangingShips(&ShipsPlayer, &BoardPlayer);

    WindowParametres *WBackGround = malloc(sizeof(WindowParametres));
    WindowParametres *WInfoPlayer = malloc(sizeof(WindowParametres));
    WindowParametres *WInfoComputer = malloc(sizeof(WindowParametres));
    WindowParametres *WBoardPlayer = malloc(sizeof(WindowParametres));
    WindowParametres *WBoardComputer = malloc(sizeof(WindowParametres));
    WindowParametres *WHelp = malloc(sizeof(WindowParametres));
    initWindowsShooting(&ShipsPlayer, &BoardPlayer, WBackGround, WInfoPlayer, WInfoComputer, WBoardPlayer, WBoardComputer);



    wbkgdset(WBackGround->ptrWin, COLOR_PAIR(200));
    wclear(WBackGround->ptrWin);
    wrefresh(WBackGround->ptrWin);
    // mvprintw(0, 0, "Lines: %d, cols: %d", LINES, COLS);
    wrefresh(WBackGround->ptrWin);

    DrawWInfo_Default(WInfoPlayer);
    DrawWInfo_Default(WInfoComputer);
    DrawWBoard_Default(WBoardPlayer);
    DrawWBoard_Default(WBoardComputer);


    // WindowParametres *WTest = malloc(sizeof(WindowParametres));
    // WTest->Height = calculateArrangeHeight(&ShipsPlayer);
    // WTest->Width = 38;
    // WTest->Begin_x = 1;
    // WTest->Begin_y = 1;
    // initWindow(WTest);
    // DrawDefaltArrangeWindow(WTest, &ShipsPlayer);


    

    // Окно заднего фона.
    // drawShootWindows(WMain, WShip, WShoot, &Indents);
    int key;
    while((key = getch()) != KEY_F(5));


    // // choosing_comp_strategy(ship_comp_field);
    // // podchet_ships(ship_player_field, ship_comp_field);
    
    // mvprintw(7, 76, "X - hit");
    // mvprintw(9, 76, "O - miss");
    // mvprintw(11, 74, "The game will last");
    // mvprintw(12, 74, "as long as the number");
    // mvprintw(13, 74, "of ships of any of the");
    // mvprintw(14, 74, "players will");
    // mvprintw(15, 74, "not fall lo zero.");
    // attron(COLOR_PAIR(50));
    // mvprintw(18, 80, "GOOD LUCK!");
    // attroff(COLOR_PAIR(50));
    // refresh();

    // int x_shoot = 0;
    // int y_shoot = 0;
    // int ran_x;
    // int ran_y;
    // char k = 88;

    //ship_comp_field[2][2] = TRUE;

    /*
    move(29,0);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 15; j++)
            printw("%d ", shoot_comp_field[i][j]);
        printw("\n");
    }
    */
    // refresh();
    // wattron(WShip->ptrWin, COLOR_PAIR(200));

    // refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    // while((key = getch()) != KEY_F(1)){
    //     switch(key){	
    //         case KEY_LEFT:
    //             if (check_border_left_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){ 
    //                 change_x_leftkey(&x_shoot);
    //                 refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //             }/*
    //             else {
    //                 int var_x = x_shoot - 1;
    //                 for (var_x; var_x > 0; var_x++){
    //                     if (check_border_left_shoot(y_shoot, var_x, shoot_player_field) == TRUE){
    //                         x_shoot = var_x;
    //                         change_x_leftkey(&x_shoot);
    //                         refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //                         continue;
    //                     } 
    //                 break;
    //                 }
    //             }*/
    //             break;
    //         case KEY_RIGHT:
    //             if (check_border_right_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
    //                 change_x_rightkey(&x_shoot);
    //                 refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //             }/*
    //             else {
    //                 int var_x = x_shoot + 1;
    //                 for (var_x; var_x < 15; var_x--){
    //                     if (check_border_right_shoot(y_shoot, var_x, shoot_player_field) == TRUE){
    //                         x_shoot = var_x;
    //                         change_x_rightkey(&x_shoot);
    //                         refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //                         break;
    //                     } 
    //                 break;
    //                 }
    //             }*/
    //             break;
    //         case KEY_UP:
    //             if (check_border_top_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
    //                 change_y_topkey(&y_shoot);
    //                 refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //             }/*
    //             else {
    //                 int var_y = y_shoot - 1;
    //                 for (var_y; var_y > 0; var_y--){
    //                     if (check_border_top_shoot(y_shoot, var_y, shoot_player_field) == TRUE){
    //                         y_shoot = var_y;
    //                         change_y_topkey(&y_shoot);
    //                         refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //                         break;
    //                     } 
    //                 break;
    //                 }
    //             }*/
    //             break;
    //         case KEY_DOWN:
    //             if (check_border_bot_shoot(y_shoot, x_shoot, shoot_player_field) == TRUE){
    //                 change_y_botkey(&y_shoot);
    //                 refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //             }
    //             else {
    //                 int var_y = y_shoot + 1;
    //                 for (var_y; var_y < 10; var_y++){
    //                     if (check_border_bot_shoot(y_shoot, var_y, shoot_player_field) == TRUE){
    //                         y_shoot = var_y;
    //                         change_y_botkey(&y_shoot);
    //                         refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
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
    //             refresh_shoot_player_gpaphics(WShoot->ptrWin, shoot_player_field, y_shoot, x_shoot);
    //             podchet_ships(ship_player_field, ship_comp_field);
    //             do {
    //                 ran_y = rand() % 10;
    //                 ran_x = rand() % 15;
    //             }
    //             while (shoot_comp_field[ran_y][ran_x] != -1);
    //             if (ship_player_field[ran_y][ran_x] == TRUE){
    //                 ship_player_field[ran_y][ran_x] = FALSE;
    //                 shoot_comp_field[ran_y][ran_x] = 1;
    //                 mvwprintw(WShip->ptrWin, ran_y*2+3, ran_x*2+3, "%c", k);
    //             }
    //             else{
    //                 shoot_comp_field[ran_y][ran_x] = 0;
    //                 mvwprintw(WShip->ptrWin, ran_y*2+3, ran_x*2+3, "%c", n);
    //             }
    //             wrefresh(WShip->ptrWin);
    //             podchet_ships(ship_player_field, ship_comp_field);
    //             reDrawStandingShips(WShip->ptrWin, Board);
    
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

// todo убрать везде struct у ShipsInfo
bool isAllShipsStanding(ShipsInfo ships){
    int shipsNum = ships.Number_1_Size + ships.Number_2_Size + ships.Number_3_Size + ships.Number_4_Size;
    for (int i = 0; i < shipsNum; i++)
        if (ships.Ships[i].stand == FALSE)
            return FALSE;
    return TRUE;
}

void drawShootWindows(WindowParametres *WMain, WindowParametres *WShip, WindowParametres *WShoot, const Indents *Indents){
    delwin(WMain->ptrWin);
    WMain->Width = Indents->LeftIndent + 2*WShip->Width + Indents->BetweenIndent + Indents->RightIndent;
    WMain->Height = Indents->TopIndent + WShip->Height + Indents->BottomIndent;
    WMain->ptrWin = newwin(WMain->Height, WMain->Width, WMain->Begin_y, WMain->Begin_x);

    resize_term(WMain->Height, WMain->Width);
    DrawMainWindow(WMain);

    delwin(WShip->ptrWin);
    WShip->ptrWin = newwin(WShip->Height, WShip->Width, WShip->Begin_y, WShip->Begin_x);
    DrawTableWindow(WShip);

    delwin(WShoot->ptrWin);
    WShoot->ptrWin = newwin(WShoot->Height, WShoot->Width, WShoot->Begin_y, WShoot->Begin_x);
    DrawTableWindow(WShoot);
}

// void podchet_ships(bool ship_player_field[10][15], bool ship_comp_field[10][15]){
//     int number_ships_comp = 0;
//     int number_ships_player = 0;
//     for (int i = 0; i < 10; i++)
//        for (int j = 0; j < 15; j++){
//            if (ship_player_field[i][j] == TRUE)
//                 number_ships_player++;
//             if (ship_comp_field[i][j] == TRUE)
//                 number_ships_comp++; 
//        }
//     if (number_ships_player == 0 || number_ships_comp == 0){
//         if (number_ships_player == 0){
//             attron(COLOR_PAIR(66));
//             mvprintw(4, 12, "                 ");
//             mvprintw(4, 47, "                 ");
//             attroff(COLOR_PAIR(50));
//             mvprintw(4, 16, "You lose!");
//             mvprintw(4, 47, "Computer wins!");
//         }
//         if (number_ships_comp == 0){
//             attron(COLOR_PAIR(66));
//             mvprintw(4, 12, "                 ");
//             mvprintw(4, 47, "                 ");
//             attroff(COLOR_PAIR(50));
//             mvprintw(4, 16, "You win!");
//             mvprintw(4, 47, "Computer lose!");
//         }
//     }
//     else {
//     mvprintw(4, 12, "Number of ships: %d ", number_ships_player);
//     mvprintw(4, 47, "Number of ships: %d ", number_ships_comp);
//     }
// }

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