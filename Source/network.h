#pragma once
#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <time.h>


void initNetowrkContext();
char* checkServerConnection(char* msg);
void closeNetwork();
char* getCurrentServerAddres();