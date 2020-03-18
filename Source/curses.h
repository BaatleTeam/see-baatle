#pragma once 

// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
#if defined(_WIN32)
    #include <curses.h>
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #include <ncurses.h>
#endif