#pragma once
#include <curses.h>
#include "ships.h"

// enum actCase { CASE_1 = 0, CASE_2, CASE_3, CASE_4 } ;

void DrawHelloWindow(WINDOW* win_hello, int h, int w);
void drawSeeBattle(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawSeeBattle_S(WINDOW*, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawSeeBattle_E(WINDOW*, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawSeeBattle_B(WINDOW*, int smbl, int  word_width, int begin_y, int begin_x, int color);
void drawSeeBattle_A(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawSeeBattle_T(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawSeeBattle_L(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void testAnimation(WINDOW* win_hello); // Обеспечивает моргание надписи.

void initGameDataCases(GameDataCase *array); // Инициализирует 4 варианта параметров игры.
void initCaseWindowData(WindowParametres*); // Инициализирует параметры для окон выбора режим игры [1..4]
void DrawCaseWindow(WindowParametres*, GameDataCase*, int number, int color); // Рисует окно выбора согласно переданным парметрам, иниц-нным раннее.
void DrawGameDataCasesShips(WINDOW* WIN, GameDataCase gdc);
void DrawGameDataCasesSize(WINDOW* WIN, GameDataCase gdc, int index, int color);
void DrawLegendDelay(WINDOW* win_menu);
void changeActiveSize(int *active_size, int key);
void printPhraseChoose(WINDOW* WIN, int color);
void deletePhraseChoose(WINDOW* WIN, int color);

bool CheckChangingOfCaseWindow(enum actCase activeCase, int key);
void DrawNonActiveCaseWindow(WindowParametres*, GameDataCase*, int number, int color);
void changeActiveCase(enum actCase *activeCase, int key);
void DrawActiveCaseWindow(WindowParametres*, GameDataCase*, int number, int color);
