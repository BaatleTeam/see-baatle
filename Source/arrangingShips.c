#include "arrange.h"

// подключён через arrange.h

extern FILE* db_out;

// -------------- AI -------------------------------
static void chooseFilling(Border borders[4], int width, int height); // выбирает стратегию порядка проверки
static bool tryToStandShip(ship* ship, Board* board, int start_x, int end_x, int start_y, int end_y);
static void swapBorders(Pair* a, Pair* b);

// 
static void DrawActiveShip_InArrangeWindow(WindowParametres *warr, int number, int size);
static void DrawMessage_InArrangeWindow(WINDOW* WIN, const char* msg);
static void EraseErrorMessage_InArrangeWindow(WINDOW*); // clear hardcoded line 12
static void colorizeCurrShip(WINDOW* WIN, ship Ship);


void arrangingShips_player(ShipsInfo *ShipsPlayer, Board *BoardPlayer){
    // Объявление параметров создаваемых окон.
    WindowParametres WMain;
    WindowParametres WArrange;
    WindowParametres WShip;
    WindowParametres WHelp;
    initWindowsParametres(ShipsPlayer, BoardPlayer, &WMain, &WArrange, &WShip, &WHelp);

    DrawMainWindow(&WMain);
    DrawTableWindow(&WShip);
    DrawDefaultArrangeWindow(&WArrange, ShipsPlayer);

    // Координаты перемещения курсора в WArrange->ptrWin для выбора корабля.
    int currShipSize = 0; 
    int currShipNumber = 0; // 1..number
    initCurrActiveShip_Arrange(ShipsPlayer, &currShipNumber, &currShipSize); // ?
    DrawActiveShip_InArrangeWindow(&WArrange, currShipNumber, currShipSize);

    enum actWind { ARRANGE = 1, SHIP = 2 };
    enum actWind active_window = ARRANGE; // Номер активного окна.
    int index; // Индекс выбранного корабля в массиве кораблей

    // Вспомогательный корабль для работы с полем ship.
    ship* TmpShip = malloc(sizeof(ship));
    clearTmpShip(TmpShip);

    bool isAllShipsStanding = FALSE;
    int key;

    while((key = getch()) != KEY_F(2) && isAllShipsStanding != TRUE){
		EraseErrorMessage_InArrangeWindow(WArrange.ptrWin);
        switch(key){
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
	            switch (active_window){
	                case ARRANGE:
                        DrawShips_InArangeWindow(&WArrange, ShipsPlayer);
						changeActiveShip(ShipsPlayer, &currShipNumber, &currShipSize, key);
						DrawActiveShip_InArrangeWindow(&WArrange, currShipNumber, currShipSize);
                        reDrawStandingShips(WShip.ptrWin, BoardPlayer);
                        colorizeCurrShip(WShip.ptrWin, ShipsPlayer->Ships[getIndex(ShipsPlayer, currShipNumber, currShipSize)]);
	                    break;
	                case SHIP:
	                    changeShipCoordinates(TmpShip, BoardPlayer, key);
	                    reDrawStandingShips(WShip.ptrWin, BoardPlayer);
	                    DrawTmpShip(WShip.ptrWin, TmpShip, BoardPlayer);
	                	break;            
	            }
	            break;
	        case 9: // tab
	            switch(active_window){
	                case SHIP:
	                    changeTypeOfShip(TmpShip, BoardPlayer);
	                    reDrawStandingShips(WShip.ptrWin, BoardPlayer);	
	                    DrawTmpShip(WShip.ptrWin, TmpShip, BoardPlayer);
	                    break;
	                case ARRANGE:
	                	break;
	        	}
    		    break;
	        case '\n':
	            switch (active_window){
	                case ARRANGE:
	                    active_window = SHIP;
	                    index = getIndex(ShipsPlayer, currShipNumber, currShipSize);
                    	clearTmpShip(TmpShip);
	                    if (ShipsPlayer->Ships[index].stand == FALSE){
	                        InitPrimaryCoordinates(currShipSize, TmpShip, BoardPlayer);
                        }
	                    else {
	                    	deleteShipFromField(&ShipsPlayer->Ships[index], BoardPlayer);
	                    	makeShipTmp(&ShipsPlayer->Ships[index], TmpShip);
	                    }
                        reDrawStandingShips(WShip.ptrWin, BoardPlayer);
                        DrawTmpShip(WShip.ptrWin, TmpShip, BoardPlayer);
	                    break;

	                case SHIP:
	                    index = getIndex(ShipsPlayer, currShipNumber, currShipSize);
	                    if (checkShipBorders(TmpShip, BoardPlayer) == FALSE){
	                        DrawMessage_InArrangeWindow(WArrange.ptrWin, "Ships can`t stand near which other!");
						}
	                    else {
	                    	addShip(&ShipsPlayer->Ships[index], TmpShip);
	                    	refresh_ship_player_array(ShipsPlayer, BoardPlayer);
	                    	reDrawStandingShips(WShip.ptrWin, BoardPlayer);

							isAllShipsStanding = checkAllShipsStanding(ShipsPlayer, BoardPlayer);
							if (isAllShipsStanding == TRUE)
								DrawMessage_InArrangeWindow(WArrange.ptrWin,
								"All ships arranged!\n Press any key to start the baatle!\n");
	                    	active_window = ARRANGE;
	                    }
	                    break;
        		}
                break;
            case 27: // Esc
                switch (active_window){
                    case SHIP:
                        reDrawStandingShips(WShip.ptrWin, BoardPlayer);
	                    active_window = ARRANGE;
                        break;
                    case ARRANGE:
                        // /No any reaction/
                        break;
                }
                break;
    	}
	}
    free(TmpShip);

	// Закончили расстановку кораблей. Очищаем ненужные окна.
    delwin(WArrange.ptrWin);
    delwin(WMain.ptrWin);
    delwin(WShip.ptrWin);
    // delwin(WHelp->ptrWin); // WHelp не используется, поэтому и не удаляем

	fprintf(db_out, "exit from arraging!\n");
}


void arrangingShips_computer(ShipsInfo *ShipsComputer, Board *BoardComputer){
	int index = 0;
	Border borders[4]; // вляиет на очередь проверки участков на поле
	// всего делится на 4 участка, разбивается одной точкой
	while (index < getShipsNumber(ShipsComputer)){
		chooseFilling(borders, BoardComputer->Width, BoardComputer->Height);

		if (ShipsComputer->Ships[index].stand == TRUE) // если предыдущий не был поставлен, то текущий ставим заново
			deleteShipFromField(&ShipsComputer->Ships[index], BoardComputer);
		for (int i = 0; i < 4; i++){
			ship tmpShip;
			makeShipTmp(&ShipsComputer->Ships[index], &tmpShip);
			tmpShip.type = rand() % 2;
			if (tryToStandShip(&tmpShip, BoardComputer,
								borders[i].pair_x.first, borders[i].pair_x.second,
								borders[i].pair_y.first, borders[i].pair_y.second))
			{
				addShip(&ShipsComputer->Ships[index], &tmpShip);
				break;
			}
		}

		if (ShipsComputer->Ships[index].stand) // если смогли поставить
			index++;
		else
			index--;
	}
	// TODO delete this before release
	FILE* f = fopen("compBoard.txt", "w");
	for (int y = 0; y < BoardComputer->Height; y++){
		for (int x = 0; x < BoardComputer->Width; x++)
			fprintf(f, "%d ", BoardComputer->field[y][x]);
		fprintf(f, "\n");
	}
	fflush(f);
	fclose(f);
}


bool tryToStandShip(ship* thisShip, Board* board, int start_x, int end_x, int start_y, int end_y){
	for (int y = start_y; y < end_y; y++){
		for (int x = start_x; x < end_x; x++){
			if ((thisShip->type == HORIZONTAL) && (x + thisShip->size > board->Width))
				break;
			if ((thisShip->type == VERTICAL) && (y + thisShip->size > board->Height))
				break;
			thisShip->x = x;
			thisShip->y = y;
			if (checkShipBorders(thisShip, board)){
				standing_ship(thisShip, board);
				thisShip->stand = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void chooseFilling(Border borders[4], int width, int height){
	int start_x = rand() % width;
	int start_y = rand() % height;
	bool dir_x = rand() % 2;
	bool dir_y = rand() % 2;

	borders[0].pair_x.first = borders[2].pair_x.first = 0;
	borders[0].pair_x.second = borders[2].pair_x.second = start_x;
	borders[1].pair_x.first = borders[3].pair_x.first = start_x;
	borders[1].pair_x.second = borders[3].pair_x.second = width;

	borders[0].pair_y.first = borders[2].pair_y.first = 0;
	borders[0].pair_y.second = borders[2].pair_y.second = start_y;
	borders[1].pair_y.first = borders[3].pair_y.first = start_y;
	borders[1].pair_y.second = borders[3].pair_y.second = height;

	if (dir_x){
		swapBorders(&borders[0].pair_x, &borders[1].pair_x);
	}
	if (dir_y){
		swapBorders(&borders[2].pair_y, &borders[3].pair_y);
	}
}

void swapBorders(Pair* a, Pair* b){
	Pair tmp = *a;
	*a = *b;
	*b = tmp;
}

void DrawActiveShip_InArrangeWindow(WindowParametres* warr, int number, int size) {
    char ch = 219;
    int y = 6 + number*2;
    int x = 0;
    switch (size){
        case 4: x =  3; break;
        case 3: x = 12; break;
        case 2: x = 21; break;
        case 1: x = 30; break;
        default: x = 0;
    }
    wattron(warr->ptrWin, COLOR_PAIR(100));
    for (int i = 0; i < size+1; i++)
        mvwprintw(warr->ptrWin, y, x+i,"%c", ch);
    wrefresh(warr->ptrWin);
}

void DrawMessage_InArrangeWindow(WINDOW* WIN, const char* msg) {
    wattron(WIN, COLOR_PAIR(100));
    mvwprintw(WIN, 12 , 1, msg); // TODO from size of board
    wattron(WIN, COLOR_PAIR(2));
    box(WIN, 0, 0);
    wrefresh(WIN);
}

void EraseErrorMessage_InArrangeWindow(WINDOW* WIN) {
    wattron(WIN, COLOR_PAIR(100));
    for (int i = 0; i < WIN->_maxx; i++)
        mvwprintw(WIN, 12 , i, " ");
    wattron(WIN, COLOR_PAIR(2));
    box(WIN, 0, 0);
    wrefresh(WIN);
}

void colorizeCurrShip(WINDOW* WIN, ship Ship) {
    if (Ship.stand == TRUE) {
        wattron(WIN, COLOR_PAIR(2));
        char rect = 254;
        int i = Ship.y;
        int j = Ship.x;

        switch(Ship.type) {
            case VERTICAL:
                for (; i < Ship.size + Ship.y; i++) {
                    mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
                }
                break;
            case HORIZONTAL:
                for (; j < Ship.size + Ship.x; j++) {
                    mvwprintw(WIN, i*2+3, j*2+4, "%c", rect);
                }
                break;
            default:
                Stopif(true, "colorizeCurrShip(): swithc-case unexpected value");
        }
        wrefresh(WIN);
    }
}
