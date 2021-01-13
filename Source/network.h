#pragma once
#include <stdlib.h>
#include <string.h>
#include <zmq.h>


void initNetowrkContext();
char* checkServerConnection(char* msg);
void closeNetwork();