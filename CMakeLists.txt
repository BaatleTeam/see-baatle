cmake_minimum_required(VERSION 3.5)

project(see-baatle) # Name of a project

file(GLOB src
    ./Source/*.c
	) # Adding source files here

set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -finstrument-functions -ftrapv -Wundef -Wshadow -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -O0 -g")
set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wundef -Wcast-qual -O0 -Wswitch-enum -Wunreachable-code -g")

add_executable(see_baatle ${src})
if(WIN32)
    # find_package(pdcurses REQUIRED)
    target_link_libraries(see_baatle PRIVATE -lpdcurses)
else()
    find_package(Curses REQUIRED)
    # Uncomment to show these variables:
    # message(STATUS "Include dirs:" ${CURSES_INCLUDE_DIRS})
    # message(STATUS "Libs:" ${CURSES_LIBRARIES})
    target_include_directories(see_baatle PRIVATE ${CURSES_INCLUDE_DIRS})
    target_link_libraries(see_baatle PRIVATE ${CURSES_LIBRARIES})
endif()

message(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
message(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
message(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
find_package(ZeroMQ REQUIRED)
message(ZeroMQ_LIBRARY ${ZeroMQ_LIBRARY})

target_link_libraries(see_baatle PRIVATE ${ZeroMQ_LIBRARY})