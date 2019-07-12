cmake_minimum_required(VERSION 3.8)

project(see-baatle) # Name of a project

file(GLOB src
    Source/*.c
	) # Adding source files here


set(CMAKE_C_STANDARD 11)
# include_directories("C:/msys64/mingw32/include/ncurses")
include_directories("C:/MinGW/include")
include_directories("C:/MinGW/lib")

# Tried to link pdcurses like it's done in big projects:

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules") # Search path for "Findpdcurses.cmake"
# find_package(pdcurses REQUIRED) # Uses "Findpdcurses.cmake" in CMAKE_MODULE_PATH directory to find package
# include_directories(${PDCURSES_INCLUDE_DIRS})
# link_libraries(${PDCURSES_LIBRARIES})

add_executable(see_baatle ${src})

# Static linking for standard C++ libraries and pdcurses linking(probably doing this wrong)
target_link_libraries(see_baatle -static -lpdcurses)
