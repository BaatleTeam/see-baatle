#include "mainMenu.h"

#define MAIN_MENU_ITEMS_COUNT 3

static const char *menuItems[MAIN_MENU_ITEMS_COUNT] = {
    "Singleplayer - Offline",
    "Singleplayer - Online",
    "Exit"
};
// static const char *choosedArrowLeft = "-->";


static void initMainMenuWindows(WindowParametres *win_bg, WindowParametres *win_menu);
static void drawStaticBgMainMenu(WindowParametres *win_bg);
static void updateDynamicMainMenu(WindowParametres *win_menu, const bool *item_state);
static void updateMenuItemChooseState(bool *item_state, int key_pressed);
static GameNetworkType convertActiveMenuItemToGameType(const bool *item_state);

GameNetworkType mainMenuWindowLoop() {
    WindowParametres win_bg, win_menu;

    initMainMenuWindows(&win_bg, &win_menu);
    drawStaticBgMainMenu(&win_bg);

    GameNetworkType gameType = GAME_N_TYPE_UNKNOWN;
    bool itemMenuState[MAIN_MENU_ITEMS_COUNT] = {true, false, false};
    bool isChoiceMade = false;

    do {
        updateDynamicMainMenu(&win_menu, itemMenuState);
        int key = getch();
        switch (key) {
            case '\n':
                gameType = convertActiveMenuItemToGameType(itemMenuState);
                isChoiceMade = true;
                break;
            case KEY_DOWN:
            case KEY_UP:
                updateMenuItemChooseState(itemMenuState, key);
                break;
            default: 
                // nothing
                break;

        }   
    }
    while (!isChoiceMade);

    clearWindowParametres(&win_bg);
    clearWindowParametres(&win_menu);

    return gameType;
}

void initMainMenuWindows(WindowParametres *win_bg, WindowParametres *win_menu) {
    *win_bg = (WindowParametres){.Begin_x = 0, .Begin_y = 0, .Width = COLS, .Height = LINES};
    initWindowWithParameters(win_bg);

    *win_menu = (WindowParametres){.Begin_x = 10, .Begin_y = 5, .Width = 30, .Height = 9};
    initWindowWithParameters(win_menu);
}

void drawStaticBgMainMenu(WindowParametres *win_bg) {
    wbkgdset(win_bg->ptrWin, COLOR_PAIR(200));
    wclear(win_bg->ptrWin);
    wrefresh(win_bg->ptrWin);
}

void updateDynamicMainMenu(WindowParametres *win_menu, const bool *item_state) {
    wbkgdset(win_menu->ptrWin, COLOR_PAIR(3));
    wclear(win_menu->ptrWin);
    // doSingleLineBorder(win_menu->ptrWin);
    wrefresh(win_menu->ptrWin);
    
    for (int i = 0; i < MAIN_MENU_ITEMS_COUNT; i++) {
        if (item_state[i] == false) {
            wattron(win_menu->ptrWin, COLOR_PAIR(3));
            mvwprintw(win_menu->ptrWin, 2+i*2, 3, menuItems[i]);
        } else {
            wattron(win_menu->ptrWin, COLOR_PAIR(33));
            mvwprintw(win_menu->ptrWin, 2+i*2, 3, menuItems[i]);
        }
    }
    wrefresh(win_menu->ptrWin);
}

void updateMenuItemChooseState(bool *item_state, int key_pressed) {
    int index = 0;
    bool isStateMoved = false;

    while (!isStateMoved) {
        Stopif(index >= MAIN_MENU_ITEMS_COUNT, "updateMenuItemChooseState(): Iterate over menu items: invalid index.");
        if (item_state[index] == true) {
            item_state[index] = false;
            switch (key_pressed) {
                case KEY_UP:
                    if (index == 0) {
                        item_state[MAIN_MENU_ITEMS_COUNT-1] = true;
                    } else {
                        item_state[index - 1] = true;
                    }
                    break;
                case KEY_DOWN:
                    if (index == MAIN_MENU_ITEMS_COUNT-1) {
                        item_state[0] = true;
                    } else {
                        item_state[index + 1] = true;
                    }
                    break;
                default:
                    Stopif(true, "updateMenuItemChooseState(): unexpected key value handle.");
            }
            isStateMoved = true;
        }
        index++;
    }
}


GameNetworkType convertActiveMenuItemToGameType(const bool *item_state) {
    for (int i = 0; i < MAIN_MENU_ITEMS_COUNT; i++) {
        if (item_state[i] == true) {
            if (i == 0) return SINGLE_OFFLINE;
            if (i == 1) return SINGLE_ONLINE;
            if (i == 2) return GAME_N_TYPE_UNKNOWN;
        }
    }
    Stopif(true, "convertActiveMenuItemToGameType(): Control shouldn't be reached.");
    // return GAME_N_TYPE_UNKNOWN;
}