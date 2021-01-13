#include "header.h"
#include "menu.h"
#include "endGame.h"
#include "mainMenu.h"
#include "loginMenu.h"

FILE* db_out = NULL;

int main() {
    // FOR DEBUG 
    db_out = fopen("debug.txt", "w");
    // 
    srand(time(NULL));
    initscr();
	cbreak();
    noecho(); // Не отображает символы.
    curs_set(FALSE);

    resize_term(38,89); // Beta
    clear();
    refresh();
    
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
    GameDataCase *GDCases = NULL;
    CoreGameData *coreGameData = NULL;
    bool gameIsGoing = true;
    
    while (gameIsGoing) {
        GameNetworkType gameType = mainMenuWindowLoop();
        if (gameType == GAME_N_TYPE_UNKNOWN) {
            break;
        }

        if (gameType == SINGLE_ONLINE) {
            loginMenuWindowLoop();
        }
        return 0;

        GDCases = initGameDataCases(GDCases);
        GameDataCaseChoice choice;
        gameIsGoing = choosingGDCase(GDCases, &choice);
        if (!gameIsGoing) {
            break;
        }

        // Создание и инициализация данных о кораблях игрока и компьютера.
        coreGameData = createCoreGameData(GDCases, choice);
        clearGamaDataCasesArray(&GDCases);
        
        // Закончили выбор режима игры, освободили данные, начинаем отрисовку окна расстановки.
        arrangingShips_ByComputer(&coreGameData->gdArray[PLAYER_1]);
        gameIsGoing = arrangingShips_ByPlayer(&coreGameData->gdArray[PLAYER_0]);
        if (!gameIsGoing) {
            break;
        }

        // Процесс перестрелки игрока и компьютера
        GameResults gameResults = shootingGameLoop(coreGameData);
        clearCoreGameData(&coreGameData);

        // Выбор игрока продолжать или нет
        endGameWindowLoop(gameResults, &gameIsGoing);
    }

    clearGamaDataCasesArray(&GDCases);
    clearCoreGameData(&coreGameData);

    fclose(db_out);
    resetty();
	endwin();
	return 0;
}