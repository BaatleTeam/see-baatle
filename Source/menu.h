#pragma once
#include "includeCurses.h"
#include <stdlib.h>
#include <string.h>
#include "ships.h"

enum playerEndGameStatus {
        UNKONOWN = -1,
        PLAYER_WINS = 0,
        PLAYER_LOSE = 1
};


typedef struct GameResults {
    enum playerEndGameStatus playerStatus;
} GameResults;

// from table.h
extern void doDoubleLineBorder(WINDOW* win_ptr);
extern void doSingleLineBorder(WINDOW* win_ptr);

enum chooseMode { choosingShips, choosingSize };
enum actCase { CASE_1 = 0, CASE_2, CASE_3, CASE_4 };

#define GAME_CASES_NUMBER 4

bool choosingGDCase(const GameDataCase *GDCases, int *caseShips, int *caseBoard);
void initShipsInfo(const GameDataCase *GDCases, ShipsInfo *info);
void initBoard(Board *board, int height, int width);

void DrawHelloWindow(WINDOW* win_hello, int h, int w);
void drawTitle_SeeBattle(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouWin(WINDOW* win_hello, int smbl); // Рисует приветствие.
void drawTitle_YouLose(WINDOW* win_hello, int smbl); // Рисует приветствие.

void drawHugeWord_A(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_B(WINDOW*, int smbl, int  word_width, int begin_y, int begin_x);
void drawHugeWord_E(WINDOW*, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_L(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_S(WINDOW*, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_T(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);

void drawHugeWord_Y(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_O(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_U(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_W(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_I(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void drawHugeWord_N(WINDOW* WIN, int smbl, int word_width, int begin_y, int begin_x);
void testAnimation(WINDOW* win_hello); // Обеспечивает моргание надписи.

GameDataCase* initGameDataCases(GameDataCase *array); // Инициализирует 4 варианта параметров игры.
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
