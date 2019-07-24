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
    init_pair (49, COLOR_BLACK, COLOR_GREEN+8); // Для окна shot board
    init_pair (51, COLOR_BLACK+8, COLOR_GREEN+8); // ??
	savetty();

    // Создание и инициализация возможных вариантов игры.
    GameDataCase* GDCases;
    GDCases = malloc(GAME_CASES_NUMBER * sizeof(GameDataCase));
    initGameDataCases(GDCases);

    int caseShips; // тип кол-ва кораблйей
    int caseBoard; // тип размера поля
    choosingGDCase(GDCases, &caseShips, &caseBoard);

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
    
    // Закончили выбор режима игры, освобождаем данные, начинаем отрисовку окна расстановки.
    free(GDCases);
    arrangingShips(&ShipsPlayer, &BoardPlayer);

    WindowParametres *WBackGround = malloc(sizeof(WindowParametres));
    WindowParametres *WInfoPlayer = malloc(sizeof(WindowParametres));
    WindowParametres *WInfoComputer = malloc(sizeof(WindowParametres));
    WindowParametres *WBoardPlayer = malloc(sizeof(WindowParametres));
    WindowParametres *WBoardComputer = malloc(sizeof(WindowParametres));
    // WindowParametres *WHelp = malloc(sizeof(WindowParametres));
    initWindowsShooting(&ShipsPlayer, &BoardPlayer, WBackGround, WInfoPlayer, WInfoComputer, WBoardPlayer, WBoardComputer);


    wbkgdset(WBackGround->ptrWin, COLOR_PAIR(200));
    wclear(WBackGround->ptrWin);
    wrefresh(WBackGround->ptrWin);
    // mvprintw(0, 0, "Lines: %d, cols: %d", LINES, COLS);
    wrefresh(WBackGround->ptrWin);

    PlayerStats statisticsPlayer = {.shipCount[3][1] = ShipsPlayer.Number_4_Size, .shipCount[3][0] = ShipsPlayer.Number_4_Size,
                                    .shipCount[2][1] = ShipsPlayer.Number_3_Size, .shipCount[2][0] = ShipsPlayer.Number_3_Size,
                                    .shipCount[1][1] = ShipsPlayer.Number_2_Size, .shipCount[1][0] = ShipsPlayer.Number_2_Size,
                                    .shipCount[0][1] = ShipsPlayer.Number_1_Size, .shipCount[0][0] = ShipsPlayer.Number_1_Size,};
    PlayerStats statisticsComputer = {.shipCount[3][1] = ShipsComputer.Number_4_Size, .shipCount[3][0] = ShipsComputer.Number_4_Size,
                                      .shipCount[2][1] = ShipsComputer.Number_3_Size, .shipCount[2][0] = ShipsComputer.Number_3_Size,
                                      .shipCount[1][1] = ShipsComputer.Number_2_Size, .shipCount[1][0] = ShipsComputer.Number_2_Size,
                                      .shipCount[0][1] = ShipsComputer.Number_1_Size, .shipCount[0][0] = ShipsComputer.Number_1_Size,};
    enum ShootBoardState **boardArray = malloc(BoardComputer.Height * sizeof(enum ShootBoardState*));
    for (int i = 0; i < BoardComputer.Height; i++)
        boardArray[i] = calloc(BoardComputer.Width, sizeof(enum ShootBoardState));
    PlayerShotBoard shotBoard = {.Height = BoardComputer.Height,
                                 .Width = BoardComputer.Width,
                                 .board = boardArray };

    int key;
    int cursor_x_pos = 0;
    int cursor_y_pos = 0;
    bool isShotAvailable = TRUE;

    DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
    DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);
    DrawWBoard_Shoting_Default(*WBoardPlayer);
    DrawWBoard_Shoting(*WBoardComputer, shotBoard);
    DrawCursor_Shoting(*WBoardComputer, cursor_x_pos, cursor_y_pos, isShotAvailable);

    while((key = getch()) != KEY_F(5)){
        switch(key){
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
                moveCursor_Shooting(BoardPlayer, &cursor_x_pos, &cursor_y_pos, key);
                updateGraphics_Shoting(*WBoardComputer, shotBoard, cursor_x_pos, cursor_y_pos);
	            break;
	        case '\n': ;
                ShotResult shotResult = {0};
                if (isShotAvailable) // do shot
                    shotResult = makeShot(ShipsPlayer, shotBoard, cursor_x_pos, cursor_y_pos);
                    // ShipsPlayer arg for debug here !!!
                updateGraphics_Shoting(*WBoardComputer, shotBoard, cursor_x_pos, cursor_y_pos);
                
                updateStats(&statisticsPlayer, &statisticsComputer, shotResult);
                DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
                DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);
                break;
    	}
	}
    
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
    
    free(WBackGround);
    free(WInfoComputer);
    free(WInfoPlayer);
    free(WBoardComputer);
    free(WBoardPlayer);
    // free(WHelp);

    free(ShipsPlayer.Ships);
    free(ShipsComputer.Ships);

    for (int i = 0; i < BoardPlayer.Height; i++)
        free(BoardPlayer.field[i]);
    free(BoardPlayer.field);
    for (int i = 0; i < BoardComputer.Height; i++)
        free(BoardComputer.field[i]);
    free(BoardComputer.field);

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