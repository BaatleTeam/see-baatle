#pragma once
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "includeCurses.h"
#include <zmq.h>


void initNetowrkContext();
bool checkServerConnection();
void closeNetwork();
char* getCurrentServerAddres();