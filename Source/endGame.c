#include "endGame.h"

typedef enum userChoice_EndGame {
    END_GAME, CONTINUE_GAME, CHOICE_ENDGAME_UNKNOWN
} userChoice_EndGame;

static const char* enterText  = "  --> Play more! <--  ";
static const char* anykeyText = "     --> Quit:( <--   ";

static void initEndGameWindows(WindowParametres *win_bg, WindowParametres *win_title, WindowString *win_enter, WindowString *win_anykey, enum playerEndGameStatus status);
static void drawEndGameBgWindows(const WindowParametres *win_bg, const WindowParametres *win_title, enum playerEndGameStatus status);
static void drawEndGameDynamicWindows(const WindowString *win_enter, const WindowString *win_anykey, userChoice_EndGame choice);

void endGameWindowLoop(GameResults gameResults, bool *isGameWillBeContinued) {
    WindowParametres win_bg, win_title;
    WindowString winEnter, winAnyKey;
    initEndGameWindows(&win_bg, &win_title, &winEnter, &winAnyKey, gameResults.playerStatus);

    drawEndGameBgWindows(&win_bg, &win_title, gameResults.playerStatus);

    userChoice_EndGame finalChoice = CHOICE_ENDGAME_UNKNOWN;
    userChoice_EndGame currChoice = END_GAME;

    do {
        drawEndGameDynamicWindows(&winEnter, &winAnyKey, currChoice);
        int key = getch();
        switch (key) {
            case '\n':
                finalChoice = currChoice;
                if (finalChoice == CONTINUE_GAME) {
                    *isGameWillBeContinued = true;
                }
                if (finalChoice == END_GAME) {
                    *isGameWillBeContinued = false;
                }
                break;
            case KEY_LEFT:
                currChoice = END_GAME;
                break;
            case KEY_RIGHT:
                currChoice = CONTINUE_GAME;
                break;
            default: 
                // nothing
                break;

        }   
    }
    while (finalChoice == CHOICE_ENDGAME_UNKNOWN);

    cleanWindowString(&winEnter);
    cleanWindowString(&winAnyKey);
    clearWindowParametres(&win_bg);
    clearWindowParametres(&win_title);
}


void initEndGameWindows(WindowParametres *win_bg, WindowParametres *win_title, WindowString *win_enter, WindowString *win_anykey, enum playerEndGameStatus status) {
    *win_bg = (WindowParametres){.Begin_x = 0, .Begin_y = 0, .Width = COLS, .Height = LINES};
    initWindowWithParameters(win_bg);
    
    *win_title = (WindowParametres){.Width = 0, .Height = 9, .Begin_y = 5, .Begin_x = 30};
    int charsNum = 0;
    switch (status) {
        case PLAYER_WINS:
            charsNum = 5;
            win_title->Width = 6*charsNum+5 + 4 + 2*2;
            initWindowWithParameters(win_title);
            break;
        case PLAYER_LOSE:
            charsNum = 6;
            win_title->Width = 6*charsNum+5 + 4 + 2*2;
            initWindowWithParameters(win_title);
            break;
        case UNKONOWN:
            // fall through
        default:
            Stopif(true, "initEndGameWindows(): unexpected UNKONOWN switch case.");
    }
    
    *win_anykey = createWindowString((WindowParametres){.Begin_y = 20, .Begin_x = 30, .Width = 0, .Height = 0}, anykeyText, -1, -1);
    *win_enter = createWindowString((WindowParametres){.Begin_y = 20, .Begin_x = 60, .Width = 0, .Height = 0}, enterText, -1, -1);
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
        case UNKONOWN:
        default:
            Stopif(true, "drawStaticEndGameWindow(): Error: Game result has unknown status.");
    }
    doSingleLineBorder(win_title->ptrWin);
    wrefresh(win_title->ptrWin);
}

void drawEndGameDynamicWindows(const WindowString *win_enter, const WindowString *win_anykey, userChoice_EndGame choice) {
    switch (choice) {
        case CONTINUE_GAME:
            drawWindowString(win_anykey, 2);
            drawWindowString(win_enter, 3);
            break;
        case END_GAME:
            drawWindowString(win_anykey, 3);
            drawWindowString(win_enter, 2);
            break;
        case CHOICE_ENDGAME_UNKNOWN:
            // fall through
        default:
            Stopif(true, "drawEndGameDynamicWindows(): switch enexpected case.")
    }
}


// -------------------- Window String methods -----------------------------------

WindowString createWindowString(WindowParametres wp, const char *text, int begin_x, int begin_y) {
    tuneWindowString(&wp, text, &begin_x, &begin_y);
    initWindowWithParameters(&wp);

    WindowString newWindow = {.wp = wp, .string_begin_x = begin_x, .string_begin_y = begin_y, .string = text};
    return newWindow;
}

WindowString createDerWindowString(WINDOW *parent, WindowParametres wp, const char* text, int begin_x, int begin_y) {
    tuneWindowString(&wp, text, &begin_x, &begin_y);
    initDerWindowWithParameters(parent, &wp);

    WindowString newWindow = {.wp = wp, .string_begin_x = begin_x, .string_begin_y = begin_y, .string = text};
    return newWindow;
}

void tuneWindowString(WindowParametres *wp, const char *text, int *begin_x, int *begin_y) {
    int textLength = strlen(text);
    int textHeight = 1;

    if (wp->Width == 0) {
        int indent_x = 2;
        wp->Width = textLength + indent_x*2;
    }
    if (wp->Height == 0) {
        int indent_y = 2;
        wp->Height = textHeight + indent_y*2;
    }
    if (*begin_x == -1) {
        *begin_x = (wp->Width - textLength) / 2;
    }
    if (*begin_y == -1) {
        *begin_y = (wp->Height - textHeight) / 2;
    }
}

void cleanWindowString(WindowString* win) {
    delwin(win->wp.ptrWin); 
    *win = (WindowString){0};
    // text is static, should't be deleted
}


void drawWindowString(const WindowString *win, short colorPair) { // TODO Unify short
    WINDOW* curWinPtr = win->wp.ptrWin;

    wattron(curWinPtr, COLOR_PAIR(colorPair));
    wbkgdset(curWinPtr, COLOR_PAIR(colorPair));
    wclear(curWinPtr);
    doDoubleLineBorder(curWinPtr);
    mvwprintw(curWinPtr, win->string_begin_y, win->string_begin_x, win->string);
    wrefresh(curWinPtr);
}