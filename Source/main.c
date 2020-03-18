#include "header.h"
#include "menu.h"

FILE* db_out = NULL;

int main(){	
    // FOR DEBUG 
    db_out = fopen("debug.txt", "w");
    // 
    srand(time(NULL));
    initscr();
	cbreak();
    noecho(); // Не отображает символы.
    curs_set(FALSE);
	keypad(stdscr, TRUE);
    start_color();
    init_pair (2,   COLOR_BLUE+8,  COLOR_YELLOW+8); // Для окна aarange.
    init_pair (55,  COLOR_GREEN+8, COLOR_YELLOW+8); // Для окна arrange.
    init_pair (3,   COLOR_BLUE+8,  COLOR_GREEN+8 ); // Для окна ship, выбора окна case.
    init_pair (33,  COLOR_RED+8,   COLOR_GREEN+8 ); // Для окна ship, выбора размера доски.
    init_pair (50,  COLOR_BLACK+8, COLOR_CYAN+8  ); // Для неправильных корабликов на поле ship.
    init_pair (10,  COLOR_RED+8,   COLOR_YELLOW+8); // Для корабликов в окне ship.
    init_pair (100, COLOR_RED+8,   COLOR_YELLOW+8); // Выбранный корабль в окне arrange.
    init_pair (1,   COLOR_RED + 8, COLOR_BLUE    ); // ??
    init_pair (66,  COLOR_BLACK+8, COLOR_BLACK+8 ); // ??
    init_pair (200, COLOR_BLUE+8,  COLOR_WHITE+8 ); // ??
    init_pair (49,  COLOR_BLACK,   COLOR_GREEN+8 ); // Для окна shot board
    init_pair (51,  COLOR_BLACK+8, COLOR_GREEN+8 ); // ??
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
    arrangingShips_player(&ShipsPlayer, &BoardPlayer);
    arrangingShips_computer(&ShipsComputer, &BoardComputer);


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
    // поле для стрельбы человека по компьюеру                                      
    enum ShootBoardState **boardArrayPlayer = malloc(BoardComputer.Height * sizeof(enum ShootBoardState*));
    for (int i = 0; i < BoardComputer.Height; i++)
        boardArrayPlayer[i] = calloc(BoardComputer.Width, sizeof(enum ShootBoardState));
    ShotBoard shotBoardPlayer = {.Height = BoardComputer.Height,
                                 .Width = BoardComputer.Width,
                                 .board = boardArrayPlayer };
    // поле для стрельбы компьютера по человеку
    enum ShootBoardState **boardArrayComputer = malloc(BoardPlayer.Height * sizeof(enum ShootBoardState*));
    for (int i = 0; i < BoardPlayer.Height; i++)
        boardArrayComputer[i] = calloc(BoardPlayer.Width, sizeof(enum ShootBoardState));
    ShotBoard shotBoardComputer = {.Height = BoardPlayer.Height,
                                 .Width = BoardPlayer.Width,
                                 .board = boardArrayComputer };

    int key;
    Coordinate cursorPostion = {0};
    Coordinate computerShot = {-1};

    DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
    DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);
    DrawWBoard_Shoting_Default(*WBoardPlayer);
    updateGraphics_Shoting(*WBoardComputer, shotBoardPlayer, cursorPostion);

    while((key = getch()) != KEY_F(5)){
        switch(key){
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
                moveCursor_Shooting(BoardPlayer, &cursorPostion, key);
                updateGraphics_Shoting(*WBoardComputer, shotBoardPlayer, cursorPostion);
	            break;
	        case '\n': ;
                ShotResult shotResultPlayer = {0};
                if (checkShotPos(shotBoardPlayer, cursorPostion)){
                    shotResultPlayer = makeShot(ShipsComputer, shotBoardPlayer, cursorPostion);
                    updateStats(&statisticsPlayer, &statisticsComputer, shotResultPlayer);
                }
                else {
                    // Some msg to player
                    break;
                }
                updateGraphics_Shoting(*WBoardComputer, shotBoardPlayer, cursorPostion);
                DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
                DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);

                if (shotResultPlayer.isHit)
                    break; // Если игрок попал, то он ходит снова.
                ShotResult shotResultComputer = {0};
                do {
                    computerShot = generateShotCoordinate(&shotBoardComputer, &computerShot, &statisticsComputer);
                    shotResultComputer = makeShot(ShipsPlayer, shotBoardComputer, computerShot);
                    DrawWBoard_Shoting(*WBoardPlayer, shotBoardComputer);
                    updateStats(&statisticsComputer, &statisticsPlayer, shotResultComputer);
                    DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
                    DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);
                } while (shotResultComputer.isHit); // Если попал компьютер, то он ходит снова.
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

    fclose(db_out);
    resetty();
	endwin();
	return 0;
}