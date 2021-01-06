#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include "ships.h"

enum chooseMode { choosingShips, choosingSize };
enum actCase { CASE_1 = 0, CASE_2, CASE_3, CASE_4 };
enum playerEndGameStatus {PLAYER_WINS = 0, PLAYER_LOSE = 1};

#define GAME_CASES_NUMBER 4

void choosingGDCase(const GameDataCase *GDCases, int *caseShips, int *caseBoard);
void initShipsInfo(const GameDataCase *GDCases, ShipsInfo *info);
void initBoard(Board *board, int height, int width);

void DrawHelloWindow(WINDOW* win_hello, int h, int w);
void drawTitle_SeeBattle(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouWin(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouLose(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawEndGameScreen(WINDOW* WIN, int height, int width, enum playerEndGameStatus status); // Рисует концовку.

void drawHugeWord_A(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_B(WINDOW*, int smbl, int  word_width, int begin_y, int begin_x, int color);
void drawHugeWord_E(WINDOW*, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_L(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_S(WINDOW*, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_T(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);

void drawHugeWord_Y(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_O(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_U(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_W(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_I(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void drawHugeWord_N(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x, int color);
void testAnimation(WINDOW* win_hello); // Обеспечивает моргание надписи.

void initGameDataCases(GameDataCase *array); // Инициализирует 4 варианта параметров игры.
void initCaseWindowData(WindowParametres*); // Инициализирует параметры для окон выбора режим игры [1..4]
void DrawCaseWindow(WindowParametres*, const GameDataCase*, int number, int color); // Рисует окно выбора согласно переданным парметрам, иниц-нным раннее.
void DrawGameDataCasesShips(WINDOW* WIN, const GameDataCase *gdc);
void DrawGameDataCasesSize(WINDOW* WIN, const GameDataCase *gdc, int index, int color);
void DrawLegendDelay(WINDOW* win_menu);
void changeActiveSize(int *active_size, int key);
void printPhraseChoose(WINDOW* WIN, int color);
void deletePhraseChoose(WINDOW* WIN, int color);

bool CheckChangingOfCaseWindow(enum actCase activeCase, int key);
void DrawNonActiveCaseWindow(WindowParametres*, const GameDataCase*, int number, int color);
void changeActiveCase(enum actCase *activeCase, int key);
void DrawActiveCaseWindow(WindowParametres*, const GameDataCase*, int number, int color);
