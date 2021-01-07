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
    bool gameIsGoing = true;
    while (gameIsGoing) {
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

        // Процесс перестрелки игрока и компьютера
        GameResults gameResults = shootingGameLoop(&ShipsPlayer, &ShipsComputer, &BoardPlayer, &BoardComputer);
        freeDataAfterShootingLoop(&ShipsPlayer, &ShipsComputer, &BoardPlayer, &BoardComputer);


        // Окно заднего фона.
        WINDOW* win_bg = newwin(LINES, COLS, 0, 0);
        wbkgdset(win_bg, COLOR_PAIR(200));
        wclear(win_bg);
        wrefresh(win_bg);

        Stopif(gameResults.playerStatus == UNKONOWN, "Error: Game result has unknown status.");
        if (gameResults.playerStatus == PLAYER_WINS) {
            drawEndGameScreen(win_bg, 30, 20, PLAYER_WINS);
        } else {
            drawEndGameScreen(win_bg, 30, 20, PLAYER_LOSE);
        }

        wattron(win_bg, COLOR_PAIR(2));
        mvwprintw(win_bg, 0, 0, "Press ENTER to restart. Press ESC to end.");
        wrefresh(win_bg);

        int key;
        key = getch();
        gameIsGoing = false;
        switch (key) {
            case 27: // ESC
                gameIsGoing = false;
                break;
            case '\n':
                gameIsGoing = true;
                break;
        }
    }


    fclose(db_out);
    resetty();
	endwin();
	return 0;
}