#include <curses.h>
#include "ships.h"
#include "arrange.h"
#include "header.h"

// подключён через header.h

void arrangingShips(ShipsInfo *ShipsPlayer, Board *BoardPlayer){
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
	                        DrawErrorMessage(WArrange->ptrWin);
	                    else {
	                    	addShip(&ShipsPlayer->Ships[index], TmpShip);
	                    	refresh_ship_player_array(ShipsPlayer, BoardPlayer);
	                    	reDrawStandingShips(WShip->ptrWin, BoardPlayer);

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