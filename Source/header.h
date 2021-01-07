#pragma once
// #include <ncurses.h>
#include <stdio.h>
// #include "includeCurses.h"
#include <stdlib.h>
#include <time.h>
#include "arrange.h"
#include "shootingGameLoop.h"
#include "shoot.h"

#include "table.h"
#include "ships.h"
#include "shootWindows.h"

/** To where should I write errors? If this is \c NULL, write to \c stderr. */
FILE *error_log;

#define Stopif(assertion, ...) {                    \
        if (assertion) {                                           \
            fprintf(error_log ? error_log : stderr, __VA_ARGS__); \
            fprintf(error_log ? error_log : stderr, "\n");        \
            abort();                         \
        } }
