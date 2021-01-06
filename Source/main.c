#include "header.h"
#include "menu.h"

FILE* db_out = NULL;

void initDataForTestEndGame(ShotBoard *shotBoardPlayer, ShotBoard* shotBoardComputer); // TODO remove

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


    shootingGameLoop(&ShipsPlayer, &ShipsComputer, &BoardPlayer, &BoardComputer);
    

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


void initDataForTestEndGame(ShotBoard *shotBoardPlayer, ShotBoard* shotBoardComputer) {
    for (int i = 0; i < shotBoardPlayer->Height; i++) {
        for (int j = 0; j < shotBoardPlayer->Width; j++) {
            shotBoardPlayer->board[i][j] = KILLED;
        }
    }

}
