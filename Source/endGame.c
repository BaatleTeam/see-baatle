#include "endGame.h"

void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued) {
    drawStaticEndGameWindow(gameResults.playerStatus);


    int key;
    key = getch();
    switch (key) {
        case 27: // ESC
            *isGameWillBeContinued = false;
            break;
        case '\n':
            *isGameWillBeContinued = true;
            break;
        default:
            *isGameWillBeContinued = false;
    }
    
}


void drawStaticEndGameWindow(enum playerEndGameStatus status) {
    // Expect: other windows have already been deleted.
    WINDOW* win_bg = newwin(LINES, COLS, 0, 0);
    wbkgdset(win_bg, COLOR_PAIR(200));
    wclear(win_bg);
    wrefresh(win_bg);

    // Рисуем заставочку большими буквами
    WindowParametres trParams = {.Width = 0, .Height = 9, .Begin_y = 5, .Begin_x = 30};
    int charsNum = 0;
    switch (status) {
        case PLAYER_WINS: ;
            charsNum = 5;
            trParams.Width = 6*charsNum+5 + 4 + 2*2;
            createWindowWithParameters(&trParams);
            drawTitle_YouWin(trParams.ptrWin, 176);
            break;
        case PLAYER_LOSE:
            charsNum = 6;
            trParams.Width = 6*charsNum+5 + 4 + 2*2;
            createWindowWithParameters(&trParams);
            drawTitle_YouLose(trParams.ptrWin, 176);
            break;
        default:
            Stopif(true, "drawStaticEndGameWindow(): Error: Game result has unknown status.");
    }
    doSingleLineBorder(trParams.ptrWin);
    wrefresh(trParams.ptrWin);

    
    WindowParametres enterWindow = {.Begin_y = 20, .Begin_x = 30, .Width = 0, .Height = 0};
    const char* enterText = "Press ENTER to restart.";
    createWindowWithText(&enterWindow, enterText, 2);

    WindowParametres anykeyWindow = {.Begin_y = 20, .Begin_x = 60, .Width = 0, .Height = 0};
    const char* anykeyText = "Press ANY KEY to end.";
    createWindowWithText(&anykeyWindow, anykeyText, 2);

	// delwin(trParams.ptrWin);
    // delwin(win_bg);
}

void createWindowWithText(WindowParametres *wp, const char* text, short colorPair) {
    Stopif(wp->ptrWin != NULL, "drawWindowWithText(): win should be empty");
    int textLength = strlen(text);
    int indent = 2;

    wp->Width = textLength + indent*2;
    wp->Height = 1 + indent*2;
    createWindowWithParameters(wp);

    wattron(wp->ptrWin, COLOR_PAIR(colorPair));
    wbkgdset(wp->ptrWin, COLOR_PAIR(colorPair));
    wclear(wp->ptrWin);
    doDoubleLineBorder(wp->ptrWin);
    mvwprintw(wp->ptrWin, indent, indent, text);
    wrefresh(wp->ptrWin);
}