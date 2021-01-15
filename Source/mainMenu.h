#pragma once
#include "includeCurses.h"
#include "menu.h"

typedef enum GameNetworkType {
    SINGLE_OFFLINE,
    SINGLE_ONLINE, 
    // ...
    GAME_N_TYPE_UNKNOWN // also Exit from mainMenu
} GameNetworkType;


GameNetworkType mainMenuWindowLoop();


