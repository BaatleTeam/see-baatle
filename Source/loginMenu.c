#include "loginMenu.h"

#define BUFFSIZE 32
static char bufferLogin[BUFFSIZE] = {'\0'};
static char bufferPassword[BUFFSIZE] = {'\0'};

static void initLoginMenuWindows(WindowParametres *win_bg, WindowParametres *win_menu, WindowParametres *win_login, WindowParametres *win_passwd);
static void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_menu);
static void readStringInBufferFromWindow(WindowParametres *win, char *buffer);
static bool isLoginCorrect();
static bool isPasswordCorrect();

void loginMenuWindowLoop() {
    WindowParametres win_bg, win_menu, win_login, win_passwd;
    initLoginMenuWindows(&win_bg, &win_menu, &win_login, &win_passwd);
    drawStaticBgLoginMenu(&win_bg, &win_menu);
    
    do {
        readStringInBufferFromWindow(&win_login, bufferLogin);
    }
    while (!isLoginCorrect());
    do {
        readStringInBufferFromWindow(&win_passwd, bufferPassword);
    }
    while (!isPasswordCorrect());
    noecho();
}

static void initLoginMenuWindows(WindowParametres *win_bg, WindowParametres *win_menu, WindowParametres *win_login, WindowParametres *win_passwd) {
    *win_bg = (WindowParametres){.Begin_y = 0, .Begin_x = 0, .Width = COLS, .Height = LINES};
    initWindowWithParameters(win_bg);
    *win_menu = (WindowParametres){.Begin_x = 20, .Begin_y = 5, .Width = 45, .Height = 9};
    initWindowWithParameters(win_menu);

    *win_login = (WindowParametres){.Begin_x = 12, .Begin_y = 3, .Width = BUFFSIZE-1, .Height = 1};
    win_login->ptrWin = derwin(win_menu->ptrWin, win_login->Height, win_login->Width, win_login->Begin_y, win_login->Begin_x);
    *win_passwd = (WindowParametres){.Begin_x = 12, .Begin_y = 5, .Width = BUFFSIZE-1, .Height = 1};
    win_passwd->ptrWin = derwin(win_menu->ptrWin, win_passwd->Height, win_passwd->Width, win_passwd->Begin_y, win_passwd->Begin_x);
}

void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_menu) {
    wbkgdset(win_bg->ptrWin, COLOR_PAIR(200));
    wclear(win_bg->ptrWin);
    wrefresh(win_bg->ptrWin);

    wbkgdset(win_menu->ptrWin, COLOR_PAIR(2));
    wclear(win_menu->ptrWin);
    doDoubleLineBorder(win_menu->ptrWin);
    mvwprintw(win_menu->ptrWin, 1, 19, "LogIn");
    mvwprintw(win_menu->ptrWin, 3, 2, "   login:");
    mvwprintw(win_menu->ptrWin, 5, 2, "password:");
    wattron(win_menu->ptrWin, COLOR_PAIR(3));
    for (int i = 0; i < BUFFSIZE-1; i++) {    
        mvwprintw(win_menu->ptrWin, 3, 12+i, " ");
        mvwprintw(win_menu->ptrWin, 5, 12+i, " ");
    }
    wrefresh(win_menu->ptrWin);
}

void readStringInBufferFromWindow(WindowParametres *win, char* buffer) {
    wbkgdset(win->ptrWin, COLOR_PAIR(3));
    wclear(win->ptrWin);
    echo();
    curs_set(1);
    wmove(win->ptrWin, 0, 0);
    wgetnstr(win->ptrWin, buffer, BUFFSIZE-1);
    curs_set(0);
    noecho();
}

bool isLoginCorrect() {
    return true; // TODO
}

bool isPasswordCorrect() {
    return true; //TODO
}

