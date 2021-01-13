#include "loginMenu.h"

typedef enum ActiveMenuElement {
    ADDRESS = 0, LOGIN, PASSWD
} ActiveMenuElement;

#define BUFFSIZE 32
static char bufferLogin[BUFFSIZE] = {'\0'};
static char bufferPassword[BUFFSIZE] = {'\0'};
static char bufferAddres[BUFFSIZE] = {'\0'};
static char *arraow = {"<--"};

static void initLoginMenuWindows(WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*);
static void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_menu, char* serverAddr);
static void updateDynamicMainMenu(WindowParametres *win_menu,  WindowParametres *win_addr, WindowParametres *win_login, WindowParametres *win_passwd, ActiveMenuElement activeMenuElement);
static void changeActiveMenuElement(ActiveMenuElement *activeMenuElement, int key);

static void readStringInBufferFromWindow(WindowParametres *win, char *buffer);
static bool isLoginCorrect();
static bool isPasswordCorrect();

#define CLRS_MENU COLOR_PAIR(2)
#define CLRS_INPUT_INACTIVE COLOR_PAIR(3)
#define CLRS_INPUT_ACTIVE COLOR_PAIR(101)

void loginMenuWindowLoop() {
    ActiveMenuElement activeMenuElement = ADDRESS;
    initNetowrkContext();
    WindowParametres win_bg, win_menu, win_login, win_passwd, win_addres;
    initLoginMenuWindows(&win_bg, &win_menu, &win_login, &win_passwd, &win_addres);
    char* currentServerAddres = getCurrentServerAddres();
    strncpy(bufferAddres, currentServerAddres, BUFFSIZE);
    drawStaticBgLoginMenu(&win_bg, &win_menu, currentServerAddres);

    
    bool isExit = false;
    int key;
    do {
        updateDynamicMainMenu(&win_menu, &win_addres, &win_login, &win_passwd, activeMenuElement);

        key = getch();
        switch (key) {
            case '\n':
                break;
            case KEY_DOWN:
                changeActiveMenuElement(&activeMenuElement, key);
                break;
            case KEY_UP:
                changeActiveMenuElement(&activeMenuElement, key);
                break;
            default: 
                // nothing
                break;

        }   
    }
    while (!isExit && key != KEY_F(2));
    
    wattron(win_menu.ptrWin, COLOR_PAIR(49));
    char *msg = {"Hello server! ..."};
    mvwprintw(win_menu.ptrWin, 9, 3, msg);
    napms(200);
    char* answer = checkServerConnection(msg); // TODO async
    if (answer != NULL)
        mvwprintw(win_menu.ptrWin, 9, 21, answer);
    else
        mvwprintw(win_menu.ptrWin, 9, 21, "No answer:(");
    wrefresh(win_menu.ptrWin);

    do {
        readStringInBufferFromWindow(&win_login, bufferLogin);
    }
    while (!isLoginCorrect());
    do {
        readStringInBufferFromWindow(&win_passwd, bufferPassword);
    }
    while (!isPasswordCorrect());
    noecho();
    void closeNetwork();
}

static void initLoginMenuWindows(WindowParametres *win_bg, WindowParametres *win_menu, WindowParametres *win_login, WindowParametres *win_passwd, WindowParametres *win_addres) {
    *win_bg = (WindowParametres){.Begin_y = 0, .Begin_x = 0, .Width = COLS, .Height = LINES};
    initWindowWithParameters(win_bg);
    *win_menu = (WindowParametres){.Begin_x = 20, .Begin_y = 5, .Width = 60, .Height = 14};
    initWindowWithParameters(win_menu);

    *win_addres = (WindowParametres){.Begin_x = 12, .Begin_y = 3, .Width = BUFFSIZE-1, .Height = 1};
    win_addres->ptrWin = derwin(win_menu->ptrWin, win_addres->Height, win_addres->Width, win_addres->Begin_y, win_addres->Begin_x);
    *win_passwd = (WindowParametres){.Begin_x = 12, .Begin_y = 7, .Width = BUFFSIZE-1, .Height = 1};
    win_passwd->ptrWin = derwin(win_menu->ptrWin, win_passwd->Height, win_passwd->Width, win_passwd->Begin_y, win_passwd->Begin_x);
    *win_login = (WindowParametres){.Begin_x = 12, .Begin_y = 5, .Width = BUFFSIZE-1, .Height = 1};
    win_login->ptrWin = derwin(win_menu->ptrWin, win_login->Height, win_login->Width, win_login->Begin_y, win_login->Begin_x);
}

void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_menu, char* serverAddr) {
    wbkgdset(win_bg->ptrWin, COLOR_PAIR(200));
    wclear(win_bg->ptrWin);
    wrefresh(win_bg->ptrWin);

    wbkgdset(win_menu->ptrWin, COLOR_PAIR(2));
    wclear(win_menu->ptrWin);
    doDoubleLineBorder(win_menu->ptrWin);
    mvwprintw(win_menu->ptrWin, 1, 19, "LogIn");
    mvwprintw(win_menu->ptrWin, 3, 2, " address:");
    mvwprintw(win_menu->ptrWin, 5, 2, "   login:");
    mvwprintw(win_menu->ptrWin, 7, 2, "password:");
    wattron(win_menu->ptrWin, COLOR_PAIR(3));
    for (int i = 0; i < BUFFSIZE-1; i++) {    
        mvwprintw(win_menu->ptrWin, 3, 12+i, " ");
        mvwprintw(win_menu->ptrWin, 5, 12+i, " ");
        mvwprintw(win_menu->ptrWin, 7, 12+i, " ");
    }
    mvwprintw(win_menu->ptrWin, 3, 12, serverAddr);
    wrefresh(win_menu->ptrWin);
}

void updateDynamicMainMenu(WindowParametres *win_menu, WindowParametres *win_addr, WindowParametres *win_login, WindowParametres *win_passwd, ActiveMenuElement activeMenuElement) {
    if (activeMenuElement == ADDRESS) {
        wattron(win_menu->ptrWin, COLOR_PAIR(2));
        mvwprintw(win_menu->ptrWin, 3, 40, arraow);
        wattron(win_addr->ptrWin, CLRS_INPUT_ACTIVE);
        wbkgdset(win_addr->ptrWin, CLRS_INPUT_ACTIVE);
        wclear(win_addr->ptrWin);
        mvwprintw(win_addr->ptrWin, 0, 0, bufferAddres);
    }
    wrefresh(win_menu->ptrWin);
    wrefresh(win_addr->ptrWin);
    wrefresh(win_login->ptrWin);
    wrefresh(win_passwd->ptrWin);
}

static void changeActiveMenuElement(ActiveMenuElement *activeMenuElement, int key) {
    switch (*activeMenuElement) {
        case ADDRESS:
            if (key == KEY_UP)
                *activeMenuElement = PASSWD;
            if (key == KEY_DOWN)
                *activeMenuElement = LOGIN;
            break;
        case LOGIN:
            if (key == KEY_UP)
                *activeMenuElement = ADDRESS;
            if (key == KEY_DOWN)
                *activeMenuElement = PASSWD;
            break;
        case PASSWD:
            if (key == KEY_UP)
                *activeMenuElement = LOGIN;
            if (key == KEY_DOWN)
                *activeMenuElement = ADDRESS;
            break;
        default:
            Stopif(true, "changeActiveMenuElement(): unexpected switch case.")
    }
}


void readStringInBufferFromWindow(WindowParametres *win, char* buffer) {
    wbkgdset(win->ptrWin, COLOR_PAIR(3));
    wclear(win->ptrWin);
    echo();
    curs_set(2);
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

