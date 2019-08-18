#include <curses.h>
#include "ships.h"
#include "arrange.h"
#include "header.h"

// подключён через header.h

void arrangingShips_player(ShipsInfo *ShipsPlayer, Board *BoardPlayer){
    // Объявление параметров создаваемых окон.
    WindowParametres *WMain = malloc(sizeof(WindowParametres));
    WindowParametres *WArrange = malloc(sizeof(WindowParametres));
    WindowParametres *WShip = malloc(sizeof(WindowParametres));
    WindowParametres *WHelp = malloc(sizeof(WindowParametres));
    initWindowsParametres(ShipsPlayer, BoardPlayer, WMain, WArrange, WShip, WHelp);

    DrawMainWindow(WMain);
    DrawTableWindow(WShip);
    DrawDefaltArrangeWindow(WArrange, ShipsPlayer);

    // Координаты перемещения курсора в WArrange->ptrWin для выбора корабля.
    int currShipSize = 0; 
    int currShipNumber = 0; // 1..number
    initCurrActiveShip_Arrange(ShipsPlayer, &currShipNumber, &currShipSize); // ?
    DrawActiveShip_InArrangeWindow(WArrange, currShipNumber, currShipSize);

    enum actWind { ARRANGE = 1, SHIP = 2 };
    enum actWind active_window = ARRANGE; // Номер активного окна.
    int index; // Индекс выбранного корабля в массиве кораблей

    // Вспомогательный корабль для работы с полем ship.
    ship* TmpShip = malloc(sizeof(ship));
    clearTmpShip(TmpShip);

    bool isAllShipsStanding = FALSE; // todo проверка постановки кораблей
    int key;
    while((key = getch()) != KEY_F(2) && isAllShipsStanding != TRUE){
		EraseErrorMessage_InArrangeWindow(WArrange->ptrWin);
        switch(key){
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
	            switch (active_window){
	                case ARRANGE:
                        DrawShips_InArangeWindow(WArrange, ShipsPlayer);
						changeActiveShip(ShipsPlayer, &currShipNumber, &currShipSize, key);
						DrawActiveShip_InArrangeWindow(WArrange, currShipNumber, currShipSize);
                        reDrawStandingShips(WShip->ptrWin, BoardPlayer);
                        colorizeCurrShip(WShip->ptrWin, ShipsPlayer->Ships[getIndex(ShipsPlayer, currShipNumber, currShipSize)]);
	                    break;
	                case SHIP:
	                    changeShipCoordinates(TmpShip, BoardPlayer, key);
	                    reDrawStandingShips(WShip->ptrWin, BoardPlayer);
	                    DrawTmpShip(WShip->ptrWin, TmpShip, BoardPlayer);
	                break;            
	            }
	            break;
	        case 9: // tab
	            switch(active_window){
	                case SHIP:
	                    changeTypeOfShip(TmpShip, BoardPlayer);
	                    reDrawStandingShips(WShip->ptrWin, BoardPlayer);	
	                    DrawTmpShip(WShip->ptrWin, TmpShip, BoardPlayer);
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
                        reDrawStandingShips(WShip->ptrWin, BoardPlayer);
                        DrawTmpShip(WShip->ptrWin, TmpShip, BoardPlayer);
	                    break;

	                case SHIP:
	                    index = getIndex(ShipsPlayer, currShipNumber, currShipSize);
	                    if (checkShipBorders(TmpShip, BoardPlayer) == FALSE)
	                        DrawMessage_InArrangeWindow(WArrange->ptrWin, "Ships can`t stand near which other!");
	                    else {
	                    	addShip(&ShipsPlayer->Ships[index], TmpShip);
	                    	refresh_ship_player_array(ShipsPlayer, BoardPlayer);
	                    	reDrawStandingShips(WShip->ptrWin, BoardPlayer);
							isAllShipsStanding = checkAllShipsStanding(ShipsPlayer, BoardPlayer);
							if (isAllShipsStanding == TRUE)
								DrawMessage_InArrangeWindow(WArrange->ptrWin,
								"All ships arranged!\n Press any key to start the baatle!\n");
	                    	active_window = ARRANGE;
	                    }
	                    break;
        		}
                break;
            case 27: // Esc
                switch (active_window){
                    case SHIP:
                        reDrawStandingShips(WShip->ptrWin, BoardPlayer);
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
    delwin(WArrange->ptrWin);
    delwin(WMain->ptrWin);
    delwin(WShip->ptrWin);
    // delwin(WHelp->ptrWin); // WHelp не используется, поэтому и не удаляем

    free(WArrange);
    free(WMain);
    free(WHelp);
    free(WShip);
}


void arrangingShips_computer(ShipsInfo *ShipsComputer, Board *BoardComputer){
	// цикл по всем кораблям, если ещё не поставлен, то пытаемся поставить
	// циклами проверяем и ставим при первом совпадении
	// если поставили -> идем дальше
	// если не поставили -> окатываемся и ставим предыдущий заново
	//  при постановке каждого корабля рандомим начальную точку x y и направление проверки

	int index = 0;
	#define isStand ShipsComputer->Ships[index].stand
	#define curShip ShipsComputer->Ships[index]
	f = fopen("tmp.txt", "w");
	for (int i = 0; i < getShipsNumber(ShipsComputer); i++)
		fprintf(f, "Stand_1: %d %d %d %d %d\n", ShipsComputer->Ships[i].x, ShipsComputer->Ships[i].y, ShipsComputer->Ships[i].size, ShipsComputer->Ships[i].stand, ShipsComputer->Ships[i].type);
	// fprintf(f, "%d\n", getShipsNumber(ShipsComputer));
	// for (int i = 0; i < getShipsNumber(ShipsComputer); i++)
	// 	fprintf(f, "%d %d %d  ", ShipsComputer->Ships->x, ShipsComputer->Ships->y, ShipsComputer->Ships->stand);

	Border borders[4];
	while (index < getShipsNumber(ShipsComputer)){
		chooseFilling(borders, BoardComputer->Width, BoardComputer->Height);
		// for (int i = 0; i < 4; i++)
		// 	fprintf(f, "X: %d %d Y: %d %d"	, borders[i].pair_x.first, borders[i].pair_x.second, borders[i].pair_y.first, borders[i].pair_y.second);

		for (int i = 0; i < 4; i++){
			ship tmpShip;
			makeShipTmp(&curShip, &tmpShip);
			if (tryToStandShip(&tmpShip, BoardComputer,
								borders[i].pair_x.first, borders[i].pair_x.second,
								borders[i].pair_y.first, borders[i].pair_y.second))
			{
				addShip(&curShip, &tmpShip);
				fprintf(f, "Stand_2: %d %d %d %d %d\n", curShip.x, curShip.y, curShip.size, curShip.stand, curShip.type);
				break;
			}
			else {
				//  to do delete ship from board
			}
		}

		if (isStand) // если смогли поставить
			index++;
		else { // иначе
			index--;
		}
	}

	for (int y = 0; y < BoardComputer->Height; y++){
		for (int x = 0; x < BoardComputer->Width; x++)
			fprintf(f, "%d ", BoardComputer->field[y][x]);
		fprintf(f, "\n");
	}
}


bool tryToStandShip(ship* thisShip, Board* board, int start_x, int end_x, int start_y, int end_y){
	for (int y = start_y; y < end_y; y++){
		for (int x = start_x; x < end_x; x++){
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