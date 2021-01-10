#include "endGame.h"

static const char* enterText = "Press ENTER to restart.";
static const char* anykeyText = "Press ANY KEY to end.";

void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued) {
    WindowParametres win_bg, win_title;
    WindowString winEnter, winAnyKey;
    iniEndGameWindows(&win_bg, &win_title, &winEnter, &winAnyKey, gameResults.playerStatus);

    drawEndGameBgWindows(&win_bg, &win_title, gameResults.playerStatus);
    drawEndGameDynamicWindows(&winEnter, &winAnyKey, CONTINUE_GAME);
    
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


void iniEndGameWindows(WindowParametres *win_bg, WindowParametres *win_title, WindowString *win_enter, WindowString *win_anykey, enum playerEndGameStatus status) {
    *win_bg = (WindowParametres){.Begin_x = 0, .Begin_y = 0, .Width = COLS, .Height = LINES};
    createWindowWithParameters(win_bg);
    
    *win_title = (WindowParametres){.Width = 0, .Height = 9, .Begin_y = 5, .Begin_x = 30};
    int charsNum = 0;
    switch (status) {
        case PLAYER_WINS:
            charsNum = 5;
            win_title->Width = 6*charsNum+5 + 4 + 2*2;
            createWindowWithParameters(win_title);
            break;
        case PLAYER_LOSE:
            charsNum = 6;
            win_title->Width = 6*charsNum+5 + 4 + 2*2;
            createWindowWithParameters(win_title);
            break;
        default:
            Stopif(true, "drawStaticEndGameWindow(): Error: Game result has unknown status.");
    }
    
    *win_enter = createWindowString((WindowParametres){.Begin_y = 20, .Begin_x = 30, .Width = 0, .Height = 0}, enterText, -1, -1);


    *win_anykey = createWindowString((WindowParametres){.Begin_y = 20, .Begin_x = 60, .Width = 0, .Height = 0}, anykeyText, -1, -1);
}

void drawEndGameBgWindows(const WindowParametres *win_bg, const WindowParametres *win_title, enum playerEndGameStatus status) {
    wbkgdset(win_bg->ptrWin, COLOR_PAIR(200));
    wclear(win_bg->ptrWin);
    wrefresh(win_bg->ptrWin);

    switch (status) {
        case PLAYER_WINS:
            drawTitle_YouWin(win_title->ptrWin, 176);
            break;
        case PLAYER_LOSE:
            drawTitle_YouLose(win_title->ptrWin, 176);
            break;
        default:
            Stopif(true, "drawStaticEndGameWindow(): Error: Game result has unknown status.");
    }
    doSingleLineBorder(win_title->ptrWin);
    wrefresh(win_title->ptrWin);
}

void drawEndGameDynamicWindows(const WindowString *win_enter, const WindowString *win_anykey, userActiveChoice_EndGame choice) {
    switch (choice) {
        case CONTINUE_GAME:
            drawWindowString(win_anykey, 2);
            drawWindowString(win_enter, 3);
            break;
        case END_GAME:
            drawWindowString(win_anykey, 3);
            drawWindowString(win_enter, 2);
            break;
        default:
            Stopif(true, "drawEndGameDynamicWindows(): switch enexpected case.")
    }
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

void cleanWindowString(WindowString* win) {
    delwin(win->wp.ptrWin); 
    *win = (WindowString){};
    // text is static, should't be deleted
}


void drawWindowString(const WindowString *win, short colorPair) {
    WINDOW* curWinPtr = win->wp.ptrWin;

    wattron(curWinPtr, COLOR_PAIR(colorPair));
    wbkgdset(curWinPtr, COLOR_PAIR(colorPair));
    wclear(curWinPtr);
    doDoubleLineBorder(curWinPtr);
    mvwprintw(curWinPtr, win->string_begin_y, win->string_begin_x, win->string);
    wrefresh(curWinPtr);
}