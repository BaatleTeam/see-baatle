#include "menu.h"

void choosingGDCase(const GameDataCase *GDCases, int *caseShips, int *caseBoard){
	resize_term(38,89); // Beta
    clear();
    refresh();

    // Окно заднего фона.
    WINDOW* win_menu = newwin(LINES, COLS, 0, 0);
    wbkgdset(win_menu, COLOR_PAIR(200));
    wclear(win_menu);
    wrefresh(win_menu);

    // Окно с надписью.
    WINDOW* win_hello = newwin(9, 61, 1, 14);
    DrawHelloWindow(win_hello, 9, 61);

	WindowParametres *WCaseParametres;
	WCaseParametres = malloc(GAME_CASES_NUMBER * sizeof(WindowParametres));
	initCaseWindowData(WCaseParametres);
	for (int i = 0; i < GAME_CASES_NUMBER; i++)
		DrawCaseWindow(&WCaseParametres[i], GDCases, i, 2);
	DrawLegendDelay(win_menu);

    enum chooseMode chooseMode = choosingShips;
    enum actCase active_case = CASE_1;
	DrawActiveCaseWindow(&WCaseParametres[active_case], GDCases, active_case, 3);

	int indexOfCurrSizeOfBoard = 0; // 0/1/2 (3 варианта)
    int key; // Хранение кода нажатой клавиши.
    while((key = getch()) != '\n' || chooseMode != choosingSize) {
    	switch(key){
    		case KEY_LEFT:
    		case KEY_RIGHT:
    		case KEY_UP:
    		case KEY_DOWN:
    			switch (chooseMode){
    				case choosingShips:
		    			if (CheckChangingOfCaseWindow(active_case, key))
			    			DrawNonActiveCaseWindow(&WCaseParametres[active_case], GDCases, active_case, 2);
			    		changeActiveCase(&active_case, key);
			    		DrawActiveCaseWindow(&WCaseParametres[active_case], GDCases, active_case, 3);
		    			break;
		    		case choosingSize:
		    			DrawGameDataCasesSize(WCaseParametres[active_case].ptrWin, &GDCases[active_case], indexOfCurrSizeOfBoard, 3);
		    			changeActiveSize(&indexOfCurrSizeOfBoard, key);
		    			DrawGameDataCasesSize(WCaseParametres[active_case].ptrWin, &GDCases[active_case], indexOfCurrSizeOfBoard, 33);
		    			break;
		    	}
		    	break;
		    case '\n':
			    switch (chooseMode){
			    	case choosingShips:
			    		chooseMode = choosingSize;
			    		printPhraseChoose(WCaseParametres[active_case].ptrWin, 33);
			    		napms(100);
			    		DrawGameDataCasesSize(WCaseParametres[active_case].ptrWin, &GDCases[active_case], indexOfCurrSizeOfBoard, 33);
			    		break;
			    	case choosingSize:
						chooseMode = choosingShips;
			    		break;
			    }
		    	break;
		    case 27: // ESC
		    	if (chooseMode == choosingSize){
		    		deletePhraseChoose(WCaseParametres[active_case].ptrWin, 3);
	    			DrawGameDataCasesSize(WCaseParametres[active_case].ptrWin, &GDCases[active_case], indexOfCurrSizeOfBoard, 3);
	    			indexOfCurrSizeOfBoard = 0;
		    		chooseMode = choosingShips;
		    	}
		    	break;
    	}
    }
	*caseShips = active_case;
	*caseBoard = indexOfCurrSizeOfBoard;

	for (int i = 0; i < GAME_CASES_NUMBER; i++) 
        delwin(WCaseParametres[i].ptrWin);
    delwin(win_menu);
    delwin(win_hello);
    free(WCaseParametres);
}

void initShipsInfo(const GameDataCase *GDCases, ShipsInfo *info){
    // Количество кораблей на основе выбора игрока.
    #define S_N_S(num) GDCases->NumberOfShips[num-1] // Ships_Number_Size 

    ShipsInfo tmp = { S_N_S(4), S_N_S(3), S_N_S(2), S_N_S(1), NULL };
	*info = tmp;
    info->Ships = calloc(S_N_S(1) + S_N_S(2) + S_N_S(3) + S_N_S(4), sizeof(ship));
	for (int i = 0; i < S_N_S(4); i++)
		info->Ships[i].size = 4;
	for (int i = S_N_S(4); i < S_N_S(4) + S_N_S(3); i++)
		info->Ships[i].size = 3;
	for (int i = S_N_S(4) + S_N_S(3); i < S_N_S(4) + S_N_S(3) + S_N_S(2); i++)
		info->Ships[i].size = 2;
	for (int i = S_N_S(4) + S_N_S(3) + S_N_S(2); i < S_N_S(4) + S_N_S(3) + S_N_S(2) + S_N_S(1); i++)
		info->Ships[i].size = 1;
}

void initBoard(Board *board, int height, int width){
	Board tmp = { height, width, NULL } ;
	*board = tmp;

	board->field = malloc(board->Height * sizeof(bool*));
    for (int i = 0; i < board->Height; i++)
    	board->field[i] = calloc(board->Width, sizeof(bool));
}

void initCaseWindowData(WindowParametres* array){
	array[0].Begin_x = array[2].Begin_x = 7;
	array[1].Begin_x = array[3].Begin_x = 47;
	array[0].Begin_y = array[1].Begin_y = 13;
	array[2].Begin_y = array[3].Begin_y = 26;

	for (int i = 0; i < GAME_CASES_NUMBER; i++){
		array[i].Width = 35;
		array[i].Height = 10;
		array[i].ptrWin = newwin(array[i].Height,
								 array[i].Width,
								 array[i].Begin_y,
								 array[i].Begin_x);
	}
}

void initGameDataCases(GameDataCase *array){
	array[0].NumberOfShips[0] = 3;
	array[0].NumberOfShips[1] = 3;
	array[0].NumberOfShips[2] = 3;
	array[0].NumberOfShips[3] = 0;
	array[0].BoardWidth [0] = 10;
	array[0].BoardHeight[0] = 6;
	array[0].BoardWidth [1] = 13;
	array[0].BoardHeight[1] = 9;
	array[0].BoardWidth [2] = 8;
	array[0].BoardHeight[2] = 10;

	array[1].NumberOfShips[0] = 5;
	array[1].NumberOfShips[1] = 4;
	array[1].NumberOfShips[2] = 3;
	array[1].NumberOfShips[3] = 2;
	array[1].BoardWidth [0] = 11;
	array[1].BoardHeight[0] = 11;
	array[1].BoardWidth [1] = 12;
	array[1].BoardHeight[1] = 12;
	array[1].BoardWidth [2] = 13;
	array[1].BoardHeight[2] = 13;

	array[2].NumberOfShips[0] = 2;
	array[2].NumberOfShips[1] = 2;
	array[2].NumberOfShips[2] = 5;
	array[2].NumberOfShips[3] = 5;
	array[2].BoardWidth [0] = 12;
	array[2].BoardHeight[0] = 12;
	array[2].BoardWidth [1] = 13;
	array[2].BoardHeight[1] = 13;
	array[2].BoardWidth [2] = 14;
	array[2].BoardHeight[2] = 14;

	array[3].NumberOfShips[0] = 9;
	array[3].NumberOfShips[1] = 6;
	array[3].NumberOfShips[2] = 0;
	array[3].NumberOfShips[3] = 0;
	array[3].BoardWidth [0] = 13;
	array[3].BoardHeight[0] = 13;
	array[3].BoardWidth [1] = 14;
	array[3].BoardHeight[1] = 14;
	array[3].BoardWidth [2] = 15;
	array[3].BoardHeight[2] = 15;

}

void DrawCaseWindow(WindowParametres* wp, const GameDataCase* gdc, int number, int color){
	wattron(wp->ptrWin, COLOR_PAIR(color));
	wbkgdset(wp->ptrWin, COLOR_PAIR(color));
    wclear(wp->ptrWin);
    box(wp->ptrWin, 0, 0);
    for (int i = 1; i < wp->Width-1; i++)
    	mvwprintw(wp->ptrWin, 2, i, "%c", 196);
    for (int i = 1; i < 16; i++)
    	mvwprintw(wp->ptrWin, 4, i, "%c", 196);
    for (int i = 0; i < wp->Height-1; i++)
    	mvwprintw(wp->ptrWin, i, 16, "%c", 179);
    for (int i = 2; i < wp->Height-1; i++)
    	mvwprintw(wp->ptrWin, i, 7, "%c", 179);
    mvwprintw(wp->ptrWin, 0, 16, "%c", 194);
    mvwprintw(wp->ptrWin, 2, 0, "%c", 195);
    mvwprintw(wp->ptrWin, 2, 7, "%c", 194);
    mvwprintw(wp->ptrWin, 2, 16, "%c", 197);
    mvwprintw(wp->ptrWin, 2, 34, "%c", 180);
    mvwprintw(wp->ptrWin, 4, 0, "%c", 195);
    mvwprintw(wp->ptrWin, 4, 7, "%c", 197);
    mvwprintw(wp->ptrWin, 4, 16, "%c", 180);
    mvwprintw(wp->ptrWin, 9, 7, "%c", 193);
    mvwprintw(wp->ptrWin, 9, 16, "%c", 193);

    mvwprintw(wp->ptrWin, 1, 6, "SHIPS");
    mvwprintw(wp->ptrWin, 1, 23, "BOARD");
    mvwprintw(wp->ptrWin, 3, 2, "Size");
    mvwprintw(wp->ptrWin, 3, 9, "Number");

    for (int index = 0; index < 3; index++)
    	DrawGameDataCasesSize(wp->ptrWin, &gdc[number], index, color);
    DrawGameDataCasesShips(wp->ptrWin, &gdc[number]);
    wrefresh(wp->ptrWin);
    napms(100);
}

void DrawGameDataCasesSize(WINDOW* WIN, const GameDataCase *gdc, int index, int color){
	wattron(WIN, COLOR_PAIR(color));
	mvwprintw(WIN, 4+index*2, 23, "%dx%d", gdc->BoardWidth[index], gdc->BoardHeight[index]);
    wrefresh(WIN);
}

void DrawGameDataCasesShips(WINDOW* WIN, const GameDataCase *gdc){
	int y;
	int index;
	for (y = 5, index = 0; index < 4; index++, y++){
		mvwprintw(WIN, y, 3, "%d", index+1);
		mvwprintw(WIN, y, 11, "%d", gdc->NumberOfShips[index]);
	}
}

void printPhraseChoose(WINDOW* WIN, int color){
	wattron(WIN, COLOR_PAIR(color));
	mvwprintw(WIN, 3, 20, "Choose one:");
	wrefresh(WIN);
}

void deletePhraseChoose(WINDOW* WIN, int color){
	wattron(WIN, COLOR_PAIR(color));
	mvwprintw(WIN, 3, 20, "           ");
	wrefresh(WIN);
}

void changeActiveSize(int *active_size, int key){
	switch (key){
		case KEY_UP:
			if (*active_size == 0)
				*active_size = 2;
			else 
				(*active_size)--;
			break;
		case KEY_DOWN:
			if (*active_size == 2)
				*active_size = 0;
			else
				(*active_size)++;	
			break;
	}
}

void DrawHelloWindow(WINDOW* WIN, int height, int width){
    wbkgdset(WIN, COLOR_PAIR(100));
    wclear(WIN);
    mvwprintw(WIN, 0, 0,"%c", 201);
    mvwprintw(WIN, 0, width-1,"%c", 187);
    mvwprintw(WIN, height-1, 0,"%c", 200);
    mvwprintw(WIN, height-1, width-1 ,"%c", 188);
    for (int i = 1; i < width-1; i++){
    	mvwprintw(WIN, 0, i, "%c", 205);
    	mvwprintw(WIN, height-1, i, "%c", 205);
    }
    for (int i = 1; i < height-1; i++){
    	mvwprintw(WIN, i, 0, "%c", 186);
    	mvwprintw(WIN, i, width-1, "%c", 186);
    }
    testAnimation(WIN);
    wrefresh(WIN);
}

void drawEndGameScreen(WINDOW* WIN, int height, int width, enum playerEndGameStatus status) {
	WINDOW* win_title_result = NULL;
    wbkgdset(WIN, COLOR_PAIR(100));
    wclear(WIN);

	if (status == PLAYER_WINS) { 
		int win_width = 6*5+5 + 4 + 2*2;
		win_title_result = newwin(9, win_width, 5, 30);
		drawTitle_YouWin(win_title_result, 176);
	}
	else {
		int win_width = 6*6+5 + 4 + 2*2;
		win_title_result = newwin(9, win_width, 5, 30);
		drawTitle_YouLose(win_title_result, 176);
	}
	
    // mvwprintw(win_title_result, 20, 0,"%c", '#');
    // mvwprintw(win_title_result, 0, width-1,"%c", 187);
    // mvwprintw(win_title_result, height-1, 0,"%c", 200);
    // mvwprintw(win_title_result, height-1, width-1 ,"%c", 188);
    // for (int i = 1; i < width-1; i++) {
    // 	mvwprintw(win_title_result, 0, i, "%c", 205);
    // 	mvwprintw(win_title_result, height-1, i, "%c", 205);
    // }
    // for (int i = 1; i < height-1; i++) {
    // 	mvwprintw(win_title_result, i, 0, "%c", 186);
    // 	mvwprintw(win_title_result, i, width-1, "%c", 186);
    // }
    // drawTitle_SeeBattle(win_title_result, 176);
    wrefresh(win_title_result);
	delwin(win_title_result);
}

void testAnimation(WINDOW* win_hello){
	napms(500);
	for (int i = 0; i < 3; i++){
		drawTitle_SeeBattle(win_hello, 176+(i%3));
		wrefresh(win_hello);
		napms(100);
	}
}

void DrawLegendDelay(WINDOW* win_menu){
	int delay = 100;
	napms(delay);
	mvwprintw(win_menu, 11, 39, "CHOOSE ONE:");
	wrefresh(win_menu);
	napms(delay);
	mvwprintw(win_menu, 12, 21, "SMALL");
	wrefresh(win_menu);
	napms(delay);
	mvwprintw(win_menu, 12, 60, "STANDART");
	wrefresh(win_menu);
	napms(delay);
	mvwprintw(win_menu, 25, 22, "HUGE");
	wrefresh(win_menu);
	napms(delay);
	mvwprintw(win_menu, 25, 62, "DUEL");
	wrefresh(win_menu);	
}

bool CheckChangingOfCaseWindow(enum actCase activeCase, int key){
	if (activeCase == CASE_1 && (key == KEY_RIGHT || key == KEY_DOWN)) return TRUE;
	if (activeCase == CASE_2 && (key == KEY_LEFT || key == KEY_DOWN)) return TRUE;
	if (activeCase == CASE_3 && (key == KEY_RIGHT || key == KEY_UP)) return TRUE;
	if (activeCase == CASE_4 && (key == KEY_LEFT || key == KEY_UP)) return TRUE;
	return FALSE;
}

void DrawNonActiveCaseWindow(WindowParametres* WIN, const GameDataCase* gdc, int number, int color){	
	DrawCaseWindow(WIN, gdc, number, color);
}

void changeActiveCase(enum actCase *activeCase, int key){
	switch (*activeCase){
		case CASE_1:	
			switch(key){
				case KEY_RIGHT:
					*activeCase = CASE_2;
					break;
				case KEY_DOWN:
					*activeCase = CASE_3;
					break;					
			}
			break;
		case CASE_2:	
			switch(key){
				case KEY_LEFT:
					*activeCase = CASE_1;
					break;
				case KEY_DOWN:
					*activeCase = CASE_4;
					break;					
			}
			break;
		case CASE_3:	
			switch(key){
				case KEY_RIGHT:
					*activeCase = CASE_4;
					break;
				case KEY_UP:
					*activeCase = CASE_1;
					break;					
			}
			break;	
		case CASE_4:	
			switch(key){
				case KEY_LEFT:
					*activeCase = CASE_3;
					break;
				case KEY_UP:
					*activeCase = CASE_2;
					break;					
			}
			break;		
	}
}

void DrawActiveCaseWindow(WindowParametres* WIN, const GameDataCase* gdc, int number, int color){
	DrawCaseWindow(WIN, gdc, number, color);
}





void drawTitle_SeeBattle(WINDOW* win_ptr, int smbl){
	int indent = 1;
	int word_width = 5;
	int begin_x = 2;
	int begin_y = 2;
	wrefresh(win_ptr);
	drawHugeWord_S(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_E(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_E(win_ptr, smbl, word_width, begin_y, begin_x, 200);

	begin_x += (word_width+5);

	drawHugeWord_B(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_A(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_T(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_T(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_L(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_E(win_ptr, smbl, word_width, begin_y, begin_x, 200);
}

void drawTitle_YouWin(WINDOW* win_ptr, int smbl) {
	wattron(win_ptr, COLOR_PAIR(2));
	int indent = 1;
	int word_width = 5;
	int begin_x = 2;
	int begin_y = 2;
	wrefresh(win_ptr);
	drawHugeWord_Y(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_O(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_U(win_ptr, smbl, word_width, begin_y, begin_x, 200);

	begin_x += (word_width+4);

	// wattron(win_ptr, COLOR_PAIR(200));
	drawHugeWord_W(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	// wattron(win_ptr, COLOR_PAIR(2));
	begin_x += indent + word_width;
	drawHugeWord_I(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_N(win_ptr, smbl, word_width, begin_y, begin_x, 200);
}

void drawTitle_YouLose(WINDOW* win_ptr, int smbl) {
	int indent = 1;
	int word_width = 5;
	int begin_x = 2;
	int begin_y = 2;
	wrefresh(win_ptr);
	drawHugeWord_Y(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_O(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_U(win_ptr, smbl, word_width, begin_y, begin_x, 200);

	begin_x += (word_width+4);

	drawHugeWord_L(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_O(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_S(win_ptr, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawHugeWord_E(win_ptr, smbl, word_width, begin_y, begin_x, 200);
}

void drawHugeWord_S(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++){
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+2, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
	}
    mvwprintw(WIN, begin_y+1, begin_x, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width-1, "%c", smbl);
}

void drawHugeWord_E(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++){
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+2, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
	}
    mvwprintw(WIN, begin_y+1, begin_x, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x, "%c", smbl);
}

void drawHugeWord_B(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++){
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+2, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
	}
    mvwprintw(WIN, begin_y+1, begin_x, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x, "%c", smbl);
    mvwprintw(WIN, begin_y+1, begin_x+word_width-1, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width-1, "%c", smbl);
}

void drawHugeWord_A(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
    mvwprintw(WIN, begin_y+0, begin_x+word_width/2, "%c", smbl);
    mvwprintw(WIN, begin_y+1, begin_x+word_width/2+1, "%c", smbl);
    mvwprintw(WIN, begin_y+1, begin_x+word_width/2-1, "%c", smbl);

    mvwprintw(WIN, begin_y+2, begin_x+word_width/2-2, "%c", smbl);
    mvwprintw(WIN, begin_y+2, begin_x+word_width/2+2, "%c", smbl);

    mvwprintw(WIN, begin_y+3, begin_x+word_width/2-2, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width/2-1, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width/2, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width/2+1, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width/2+2, "%c", smbl);

	mvwprintw(WIN, begin_y+4, begin_x+word_width/2-2, "%c", smbl);
    mvwprintw(WIN, begin_y+4, begin_x+word_width/2+2, "%c", smbl);
}

void drawHugeWord_T(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++)
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	for (int i = 0; i < 5; i++)
	    mvwprintw(WIN, begin_y+i, begin_x+word_width/2, "%c", smbl);

}

void drawHugeWord_L(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < 5; i++)
	    mvwprintw(WIN, begin_y+i, begin_x+0, "%c", smbl);
	for (int i = 1; i < word_width; i++)
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
}

void drawHugeWord_Y(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color) {
	wattron(WIN, COLOR_PAIR(color));
	mvwprintw(WIN, begin_y, begin_x, "%c", smbl);
	mvwprintw(WIN, begin_y+1, begin_x+1, "%c", smbl);
	mvwprintw(WIN, begin_y, begin_x+word_width-1, "%c", smbl);
	mvwprintw(WIN, begin_y+1, begin_x+word_width-2, "%c", smbl);
	for (int i = 2; i < 2+3; i++) {
		mvwprintw(WIN, begin_y+i, begin_x+2, "%c", smbl);
	}
}

void drawHugeWord_O(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color) {
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++) {
		mvwprintw(WIN, begin_y, begin_x+i, "%c", smbl);
		mvwprintw(WIN, begin_y+word_width-1, begin_x+i, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x+word_width-1, "%c", smbl);
	}
}

void drawHugeWord_U(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color) {
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++) {
		mvwprintw(WIN, begin_y+word_width-1, begin_x+i, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x+word_width-1, "%c", smbl);
	}
}

void drawHugeWord_W(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color) {
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width-1; i++) {
		mvwprintw(WIN, begin_y+i, begin_x, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x+word_width-1, "%c", smbl);
	}
	for (int i = 1; i < word_width-1; i++) {
		mvwprintw(WIN, begin_y+i, begin_x+2, "%c", smbl);
	}
	mvwprintw(WIN, begin_y+word_width-1, begin_x+1, "%c", smbl);
	mvwprintw(WIN, begin_y+word_width-1, begin_x+3, "%c", smbl);
}

void drawHugeWord_I(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color) {
	for (int i = 0; i < word_width; i++) {
		mvwprintw(WIN, begin_y, begin_x+i, "%c", smbl);
		mvwprintw(WIN, begin_y+word_width-1, begin_x+i, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x+2, "%c", smbl);
	}
}

void drawHugeWord_N(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color) {
	for (int i = 1; i < word_width; i++) {
		mvwprintw(WIN, begin_y+i, begin_x, "%c", smbl);
		mvwprintw(WIN, begin_y+i, begin_x+word_width, "%c", smbl);
	}
	for (int i = 1; i < word_width; i++) {
		mvwprintw(WIN, begin_y, begin_x+i, "%c", smbl);
	}

}
