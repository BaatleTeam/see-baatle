#include "loginMenu.h"

#define BUFFSIZE 32
static const char bufferLogin[BUFFSIZE] = {'\0'};

static void initLoginMenuWindows(WindowParametres *win_bg, WindowParametres *win_login);
static void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_login);

void loginMenuWindowLoop() {
    WindowParametres win_bg, win_login;
    initLoginMenuWindows(&win_bg, &win_login);
    drawStaticBgLoginMenu(&win_bg, &win_login);
    echo();
    char buffer[32];
    wgetnstr(win_login.ptrWin, buffer, 32-1);
    noecho();
}

void initLoginMenuWindows(WindowParametres *win_bg, WindowParametres *win_login) {
    *win_bg = (WindowParametres){.Begin_y = 0, .Begin_x = 0, .Width = COLS, .Height = LINES};
    initWindowWithParameters(win_bg);

    *win_login = (WindowParametres){.Begin_x = 20, .Begin_y = 5, .Width = 45, .Height = 9};
    initWindowWithParameters(win_login);
}

void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_login) {
    wbkgdset(win_bg->ptrWin, COLOR_PAIR(200));
    wclear(win_bg->ptrWin);
    wrefresh(win_bg->ptrWin);

    wbkgdset(win_login->ptrWin, COLOR_PAIR(2));
    wclear(win_login->ptrWin);
    doDoubleLineBorder(win_login->ptrWin);
    mvwprintw(win_login->ptrWin, 1, 19, "LogIn");
    mvwprintw(win_login->ptrWin, 3, 2, "   login:");
    mvwprintw(win_login->ptrWin, 5, 2, "password:");
    wattron(win_login->ptrWin, COLOR_PAIR(3));
    for (int i = 0; i < BUFFSIZE-1; i++) {    
        mvwprintw(win_login->ptrWin, 3, 12+i, " ");
        mvwprintw(win_login->ptrWin, 5, 12+i, " ");
    }
    wmove(win_login->ptrWin, 3, 12);
    wrefresh(win_login->ptrWin);
}

