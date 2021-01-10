#include "endGame.h"

static const char* enterText = "Press ENTER to restart.";
static const char* anykeyText = "Press ANY KEY to end.";

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
        case PLAYER_WINS:
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

    
    
    WindowString enterWindow = createWindowString((WindowParametres){.Begin_y = 20, .Begin_x = 30, .Width = 0, .Height = 0}, enterText, -1, -1);
    drawWindowString(enterWindow, 2);

    WindowString anykeyWindow = createWindowString((WindowParametres){.Begin_y = 20, .Begin_x = 60, .Width = 0, .Height = 0}, anykeyText, -1, -1);
    drawWindowString(anykeyWindow, 2);

	// delwin(trParams.ptrWin);
    // delwin(win_bg);
}

WindowString createWindowString(WindowParametres wp, const char* text, int begin_x, int begin_y) {
    int textLength = strlen(text);
    int textHeight = 1;
    int indent_x = 2;
    int indent_y = 2;

    if (wp.Width == 0) {
        wp.Width = textLength + indent_x*2;
    }
    if (wp.Height == 0) {
        wp.Height = textHeight + indent_y*2;
    }
    if (begin_x == -1) {
        begin_x = (wp.Width - textLength) / 2;
    }
    if (begin_y == -1) {
        begin_y = (wp.Height - textHeight) / 2;
    }
    createWindowWithParameters(&wp);

    WindowString newWindow = {.wp = wp, .string_begin_x = begin_x, .string_begin_y = begin_y, .string = text};
    return newWindow;
}


void drawWindowString(WindowString win, short colorPair) {
    WINDOW* curWinPtr = win.wp.ptrWin;

    wattron(curWinPtr, COLOR_PAIR(colorPair));
    wbkgdset(curWinPtr, COLOR_PAIR(colorPair));
    wclear(curWinPtr);
    doDoubleLineBorder(curWinPtr);
    mvwprintw(curWinPtr, win.string_begin_y, win.string_begin_x, win.string);
    wrefresh(curWinPtr);
}