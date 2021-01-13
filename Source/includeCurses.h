#pragma once 
#include <stdlib.h>
#include <curses.h>
#include <zmq.h>

/** To where should I write errors? If this is \c NULL, write to \c stderr. */
FILE *error_log;

#define Stopif(assertion, ...) {                    \
        if (assertion) {                                           \
            fprintf(error_log ? error_log : stderr, __VA_ARGS__); \
            fprintf(error_log ? error_log : stderr, "\n");        \
            abort();                         \
        } }
