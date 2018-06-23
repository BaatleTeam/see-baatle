# File for finding pdcurses Library
# This file should define:
# PDCURSES_FOUND - TRUE if everything is okay
# PDCURSES_INCLUDE_DIRS - directories that may contain source files of pdcurses
# PDCURSES_LIBRARIES - list of a libraries

# set(PDCURSES_FOUND TRUE)

# Searching header file
find_path(
	PDCURSES_INCLUDE_DIR curses.h
)
list(APPEND PDCURSES_INCLUDE_DIRS "${PDCURSES_INCLUDE_DIR}")

# Searching for a library
find_library(
	PDCURSES_LIBRARY pdcurses libpdcurses
)
list(APPEND PDCURSES_LIBRARIES "${PDCURSES_LIBRARY}")
