#include "loginMenu.h"

typedef enum ActiveMenuElement {
    ADDRESS = 0, LOGIN, PASSWD, BUTTON_OK, BUTTON_BACK      
} ActiveMenuElement;

typedef enum ServerConnectionStatus {
    CONNECTION_OK, CONNECTION_NO, CONNECTION_WAIT
} ServerConnectionStatus;

#define BUFFSIZE 32
static char bufferLogin[BUFFSIZE] = {'\0'};
static char bufferPassword[BUFFSIZE] = {'\0'};
static char bufferAddres[BUFFSIZE] = {'\0'};

static char *arrow = {"<---"};
static char *btnLoginTitle = {"Login"};
static char *btnBackTitle = {"Back"};

static void initLoginMenuWindows(WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*, WindowParametres*, WindowString*, WindowString*);
static void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_menu, char* serverAddr);
static void updateConnectionStatus(WindowParametres* win, ServerConnectionStatus isConnected);
static void updateDynamicMainMenu(WindowParametres *,  WindowParametres *, WindowParametres *, WindowParametres *, WindowString *, WindowString *, ActiveMenuElement);
static void changeActiveMenuElement(ActiveMenuElement *activeMenuElement, int key);

static void readStringInBufferFromWindow(WindowParametres *win, char *buffer);
static void showWindowError(char *msg); // TODO
static bool isLoginCorrect();
static bool isPasswordCorrect();

#define CLRS_MENU COLOR_PAIR(2)
#define CLRS_INPUT_INACTIVE COLOR_PAIR(3)
#define CLRS_INPUT_ACTIVE COLOR_PAIR(101)

void loginMenuWindowLoop() {
    ActiveMenuElement activeMenuElement = ADDRESS;
    bool isCheckNeeded = true;
    initNetowrkContext();
    WindowParametres win_bg, win_menu, win_status, win_login, win_passwd, win_addres;
    WindowString win_btn_login, win_btn_back;
    initLoginMenuWindows(&win_bg, &win_menu, &win_status, &win_addres, &win_login, &win_passwd, &win_btn_login, &win_btn_back);
    
    char* currentServerAddres = getCurrentServerAddres();
    strncpy(bufferAddres, currentServerAddres, BUFFSIZE);
    drawStaticBgLoginMenu(&win_bg, &win_menu, currentServerAddres);

    ServerConnectionStatus isConnected = CONNECTION_WAIT;
    updateConnectionStatus(&win_status, isConnected);
   
    bool isExit = false;
    int key;
    do {
        updateDynamicMainMenu(&win_menu, &win_addres, &win_login, &win_passwd, &win_btn_login, &win_btn_back, activeMenuElement);
        if (isCheckNeeded) {
            // blocks for 1 sec if no connect
            bool yes = checkServerConnection(); // TODO async
            if (yes) {
                isConnected = CONNECTION_OK;
                isCheckNeeded = false;
            }
            else {
                isConnected = CONNECTION_NO;
                isCheckNeeded = false;
            }
            updateConnectionStatus(&win_status, isConnected);
        }

        key = getch();
        switch (key) {
            case KEY_F(5):
                isCheckNeeded = true;
                isConnected = CONNECTION_WAIT;
                updateConnectionStatus(&win_status, isConnected);
                break;
            case '\n':
                switch (activeMenuElement) {
                    case ADDRESS:
                        readStringInBufferFromWindow(&win_addres, bufferAddres);
                        isCheckNeeded = true;
                        isConnected = CONNECTION_WAIT;
                        break;
                    case LOGIN:
                        readStringInBufferFromWindow(&win_login, bufferLogin);
                        if (!isLoginCorrect()) {
                            showWindowError("Login is not correct");
                        }
                        break;
                    case PASSWD:
                        readStringInBufferFromWindow(&win_passwd, bufferPassword);
                        if (!isPasswordCorrect()) {
                            showWindowError("Password is not correct");
                        }
                        break;
                    case BUTTON_OK:
                        // TODO
                        break;
                    case BUTTON_BACK:
                        // TODO
                        break;
                    default:
                        Stopif(true, "loginMenuWindowLoop(): switch case enexpected.");
                }
                changeActiveMenuElement(&activeMenuElement, KEY_DOWN);
                break;
            case KEY_DOWN:
            case KEY_UP:
            case KEY_LEFT:
            case KEY_RIGHT:
                changeActiveMenuElement(&activeMenuElement, key);
                break;
            default: 
                // nothing
                break;
        }   
    }
    while (!isExit && key != KEY_F(2));

    noecho();
    void closeNetwork();
}

static void initLoginMenuWindows(WindowParametres *win_bg, 
                                 WindowParametres *win_menu, 
                                 WindowParametres *win_status, 
                                 WindowParametres *win_addres, 
                                 WindowParametres *win_login, 
                                 WindowParametres *win_passwd,
                                 WindowString *win_btn_login,
                                 WindowString *win_btn_back
) {
    *win_bg = (WindowParametres){.Begin_y = 0, .Begin_x = 0, .Width = COLS, .Height = LINES};
    initWindowWithParameters(win_bg);
    *win_menu = (WindowParametres){.Begin_x = 20, .Begin_y = 5, .Width = 51, .Height = 13};
    initWindowWithParameters(win_menu);

    *win_status = (WindowParametres){.Begin_x = 12, .Begin_y = 1, .Width = BUFFSIZE-1, .Height = 1};
    win_status->ptrWin = derwin(win_menu->ptrWin, win_status->Height, win_status->Width, win_status->Begin_y, win_status->Begin_x);
    *win_addres = (WindowParametres){.Begin_x = 12, .Begin_y = 3, .Width = BUFFSIZE-1, .Height = 1};
    win_addres->ptrWin = derwin(win_menu->ptrWin, win_addres->Height, win_addres->Width, win_addres->Begin_y, win_addres->Begin_x);
    *win_passwd = (WindowParametres){.Begin_x = 12, .Begin_y = 7, .Width = BUFFSIZE-1, .Height = 1};
    win_passwd->ptrWin = derwin(win_menu->ptrWin, win_passwd->Height, win_passwd->Width, win_passwd->Begin_y, win_passwd->Begin_x);
    *win_login = (WindowParametres){.Begin_x = 12, .Begin_y = 5, .Width = BUFFSIZE-1, .Height = 1};
    win_login->ptrWin = derwin(win_menu->ptrWin, win_login->Height, win_login->Width, win_login->Begin_y, win_login->Begin_x);

    *win_btn_login = createDerWindowString(win_menu->ptrWin, (WindowParametres){.Width = 15, .Height=3, .Begin_x = 8, .Begin_y = 9}, btnLoginTitle, -1, -1);
    *win_btn_back = createDerWindowString(win_menu->ptrWin, (WindowParametres){.Width = 15, .Height=3, .Begin_x = 30, .Begin_y = 9}, btnBackTitle, -1, -1);
    
}

void drawStaticBgLoginMenu(WindowParametres *win_bg, WindowParametres *win_menu, char* serverAddr) {
    wbkgdset(win_bg->ptrWin, COLOR_PAIR(200));
    wclear(win_bg->ptrWin);
    wrefresh(win_bg->ptrWin);

    wbkgdset(win_menu->ptrWin, COLOR_PAIR(2));
    wclear(win_menu->ptrWin);
    doDoubleLineBorder(win_menu->ptrWin);
    mvwprintw(win_menu->ptrWin, 1, 2, "  status:");
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

void updateConnectionStatus(WindowParametres *win, ServerConnectionStatus conStatus) {
    wattron(win->ptrWin, CLRS_MENU);
    wbkgdset(win->ptrWin, CLRS_MENU);
    wclear(win->ptrWin);
    if (conStatus == CONNECTION_OK) {
        mvwprintw(win->ptrWin, 0, 0, "Connected!");
    } else if (conStatus == CONNECTION_NO) {
        wattron(win->ptrWin, COLOR_PAIR(10));
        mvwprintw(win->ptrWin, 0, 0, "No connection :(");
    } else {
        mvwprintw(win->ptrWin, 0, 0, "Waiting...");
    }
    wrefresh(win->ptrWin);
}

void updateDynamicMainMenu(WindowParametres *win_menu,
                           WindowParametres *win_addr,
                           WindowParametres *win_login,
                           WindowParametres *win_passwd, 
                           WindowString *win_btn_login,
                           WindowString *win_btn_back, 
                           ActiveMenuElement activeMenuElement)
{
    wattron(win_menu->ptrWin, COLOR_PAIR(2));
    mvwprintw(win_menu->ptrWin, 3, 44, "    ");
    mvwprintw(win_menu->ptrWin, 5, 44, "    ");
    mvwprintw(win_menu->ptrWin, 7, 44, "    ");

    wattron(win_addr->ptrWin, CLRS_INPUT_INACTIVE);
    wbkgdset(win_addr->ptrWin, CLRS_INPUT_INACTIVE);
    wclear(win_addr->ptrWin);
    mvwprintw(win_addr->ptrWin, 0, 0, bufferAddres);

    wattron(win_login->ptrWin, CLRS_INPUT_INACTIVE);
    wbkgdset(win_login->ptrWin, CLRS_INPUT_INACTIVE);
    wclear(win_login->ptrWin);
    mvwprintw(win_login->ptrWin, 0, 0, bufferLogin);
    
    wattron(win_passwd->ptrWin, CLRS_INPUT_INACTIVE);
    wbkgdset(win_passwd->ptrWin, CLRS_INPUT_INACTIVE);
    wclear(win_passwd->ptrWin);
    mvwprintw(win_passwd->ptrWin, 0, 0, bufferPassword);

    drawWindowString(win_btn_login, 3);
    drawWindowString(win_btn_back, 3);

    switch (activeMenuElement) {
        case ADDRESS:
            mvwprintw(win_menu->ptrWin, 3, 44, arrow);
            wattron(win_addr->ptrWin, CLRS_INPUT_ACTIVE);
            wbkgdset(win_addr->ptrWin, CLRS_INPUT_ACTIVE);
            wclear(win_addr->ptrWin);
            mvwprintw(win_addr->ptrWin, 0, 0, bufferAddres);
            break;
        case LOGIN:
            mvwprintw(win_menu->ptrWin, 5, 44, arrow);
            wattron(win_login->ptrWin, CLRS_INPUT_ACTIVE);
            wbkgdset(win_login->ptrWin, CLRS_INPUT_ACTIVE);
            wclear(win_login->ptrWin);
            mvwprintw(win_login->ptrWin, 0, 0, bufferLogin);
            break;
        case PASSWD:
            mvwprintw(win_menu->ptrWin, 7, 44, arrow);
            wattron(win_passwd->ptrWin, CLRS_INPUT_ACTIVE);
            wbkgdset(win_passwd->ptrWin, CLRS_INPUT_ACTIVE);
            wclear(win_passwd->ptrWin);
            mvwprintw(win_passwd->ptrWin, 0, 0, bufferPassword);
            break;
        case BUTTON_OK:
            drawWindowString(win_btn_login, 101);
            break;
        case BUTTON_BACK:
            drawWindowString(win_btn_back, 101);
            break;
        default:
            Stopif(true, "updateDynamicMainMenu(): unexpected switch case.");
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
                *activeMenuElement = BUTTON_OK;
            break;
        case BUTTON_OK:
            if (key == KEY_UP)
                *activeMenuElement = PASSWD;
            if (key == KEY_DOWN)
                *activeMenuElement = ADDRESS;
            if (key == KEY_RIGHT)
                *activeMenuElement = BUTTON_BACK;
            break;
        case BUTTON_BACK:
            if (key == KEY_UP)
                *activeMenuElement = PASSWD;
            if (key == KEY_DOWN)
                *activeMenuElement = ADDRESS;
            if (key == KEY_LEFT)
                *activeMenuElement = BUTTON_OK;
            break;
        default:
            Stopif(true, "changeActiveMenuElement(): unexpected switch case.")
    }
}


void readStringInBufferFromWindow(WindowParametres *win, char* buffer) {
    wbkgdset(win->ptrWin, CLRS_INPUT_ACTIVE);
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

void showWindowError(char *msg) {

}