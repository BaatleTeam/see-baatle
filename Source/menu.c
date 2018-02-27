#include "menu.h"

void initCaseWindowData(WindowParametres* array){
	array[0].Begin_x = array[2].Begin_x = 7;
	array[1].Begin_x = array[3].Begin_x = 47;
	array[0].Begin_y = array[1].Begin_y = 13;
	array[2].Begin_y = array[3].Begin_y = 26;

	for (int i = 0; i < 4; i++){
		array[i].Width = 35;
		array[i].Height = 10;
		array[i].ptrWin = newwin(array[i].Height,
								 array[i].Width,
								 array[i].Begin_y,
								 array[i].Begin_x);
	}
}

void initGameDataCases(GameDataCase *array){
	array[0].NumberOfShips[0] = 2;
	array[0].NumberOfShips[1] = 1;
	array[0].NumberOfShips[2] = 1;
	array[0].NumberOfShips[3] = 1;
	array[0].BoardWidth [0] = 10;
	array[0].BoardHeight[0] = 10;
	array[0].BoardWidth [1] = 11;
	array[0].BoardHeight[1] = 11;
	array[0].BoardWidth [2] = 12;
	array[0].BoardHeight[2] = 12;

	array[1].NumberOfShips[0] = 1;
	array[1].NumberOfShips[1] = 2;
	array[1].NumberOfShips[2] = 1;
	array[1].NumberOfShips[3] = 1;
	array[1].BoardWidth [0] = 11;
	array[1].BoardHeight[0] = 11;
	array[1].BoardWidth [1] = 12;
	array[1].BoardHeight[1] = 12;
	array[1].BoardWidth [2] = 13;
	array[1].BoardHeight[2] = 13;

	array[2].NumberOfShips[0] = 1;
	array[2].NumberOfShips[1] = 1;
	array[2].NumberOfShips[2] = 2;
	array[2].NumberOfShips[3] = 1;
	array[2].BoardWidth [0] = 12;
	array[2].BoardHeight[0] = 12;
	array[2].BoardWidth [1] = 13;
	array[2].BoardHeight[1] = 13;
	array[2].BoardWidth [2] = 14;
	array[2].BoardHeight[2] = 14;

	array[3].NumberOfShips[0] = 1;
	array[3].NumberOfShips[1] = 1;
	array[3].NumberOfShips[2] = 1;
	array[3].NumberOfShips[3] = 2;
	array[3].BoardWidth [0] = 13;
	array[3].BoardHeight[0] = 13;
	array[3].BoardWidth [1] = 14;
	array[3].BoardHeight[1] = 14;
	array[3].BoardWidth [2] = 15;
	array[3].BoardHeight[2] = 15;

}

void DrawCaseWindow(WindowParametres* wp, GameDataCase* gdc, int number, int color){
	wbkgdset(wp->ptrWin, COLOR_PAIR(color));
    wclear(wp->ptrWin);
    box(wp->ptrWin, 0, 0);
    // mvwprintw(wp->ptrWin, 0, 14, "Case 1");
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

    mvwprintw(wp->ptrWin, 4, 23, "%dx%d", gdc[number].BoardWidth[0], gdc[number].BoardHeight[0]);
    mvwprintw(wp->ptrWin, 6, 23, "%dx%d", gdc[number].BoardWidth[1], gdc[number].BoardHeight[1]);
    mvwprintw(wp->ptrWin, 8, 23, "%dx%d", gdc[number].BoardWidth[2], gdc[number].BoardHeight[2]);
    DrawGameDataCasesShips(wp->ptrWin, gdc[number].NumberOfShips);
    wrefresh(wp->ptrWin);
    napms(100);
}

void DrawGameDataCasesShips(WINDOW* WIN, int* shipsNumber){
	int y;
	int index;
	for (y = 5, index = 0; index < 4; index++, y++){
		mvwprintw(WIN, y, 3, "%d", index+1);
		mvwprintw(WIN, y, 11, "%d", shipsNumber[index]);
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

void testAnimation(WINDOW* win_hello){
	napms(500);
	for (int i = 0; i < 3; i++){
		drawSeeBattle(win_hello, 176+(i%3));
		wrefresh(win_hello);
		napms(100);
	}
}

void drawSeeBattle(WINDOW* win_hello, int smbl){
	int indent = 1;
	int word_width = 5;
	int begin_x = 2;
	int begin_y = 2;
	wrefresh(win_hello);
	drawSeeBattle_S(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_E(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_E(win_hello, smbl, word_width, begin_y, begin_x, 200);

	begin_x += (word_width+5);

	drawSeeBattle_B(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_A(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_T(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_T(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_L(win_hello, smbl, word_width, begin_y, begin_x, 200);
	begin_x += indent + word_width;
	drawSeeBattle_E(win_hello, smbl, word_width, begin_y, begin_x, 200);
}

void drawSeeBattle_S(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++){
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+2, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
	}
    mvwprintw(WIN, begin_y+1, begin_x, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x+word_width-1, "%c", smbl);
}

void drawSeeBattle_E(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++){
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+2, begin_x+i, "%c", smbl);
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
	}
    mvwprintw(WIN, begin_y+1, begin_x, "%c", smbl);
    mvwprintw(WIN, begin_y+3, begin_x, "%c", smbl);
}

void drawSeeBattle_B(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
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

void drawSeeBattle_A(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
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



	 //        mvwprintw(WIN, begin_y+0, begin_x+word_width/2, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+1, begin_x+word_width/2+1, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+1, begin_x+word_width/2-1, "%c", smbl);

	 //    mvwprintw(WIN, begin_y+2, begin_x+word_width/2-2, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+2, begin_x+word_width/2-1, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+2, begin_x+word_width/2, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+2, begin_x+word_width/2+1, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+2, begin_x+word_width/2+2, "%c", smbl);

	 //    mvwprintw(WIN, begin_y+3, begin_x+word_width/2-2, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+3, begin_x+word_width/2+2, "%c", smbl);

		// mvwprintw(WIN, begin_y+4, begin_x+word_width/2-2, "%c", smbl);
	 //    mvwprintw(WIN, begin_y+4, begin_x+word_width/2+2, "%c", smbl);
}

void drawSeeBattle_T(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < word_width; i++)
	    mvwprintw(WIN, begin_y+0, begin_x+i, "%c", smbl);
	for (int i = 0; i < 5; i++)
	    mvwprintw(WIN, begin_y+i, begin_x+word_width/2, "%c", smbl);

}

void drawSeeBattle_L(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color){
	wattron(WIN, COLOR_PAIR(color));
	for (int i = 0; i < 5; i++)
	    mvwprintw(WIN, begin_y+i, begin_x+0, "%c", smbl);
	for (int i = 1; i < word_width; i++)
	    mvwprintw(WIN, begin_y+4, begin_x+i, "%c", smbl);
}