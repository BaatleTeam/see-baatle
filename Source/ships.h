#pragma once
#include "includeCurses.h"
#include <assert.h>

typedef enum shipOrientationType {
	HORIZONTAL = false,
	VERTICAL = true
} shipOrientationType;

typedef struct Board {
	int Height;
	int Width;
	bool** field;
} Board;

typedef struct Ship {
    int x;
    int y;
    int size;
    shipOrientationType type;
    bool stand;
} Ship;

typedef struct ShipsInfo {
	int Number_4_Size;
	int Number_3_Size;
	int Number_2_Size;
	int Number_1_Size;
	Ship* Ships;
} ShipsInfo;

typedef enum PlayerIndex {
	PLAYER_0 = 0,
	PLAYER_1 = 1
} PlayerIndex;

typedef struct CorePlayerGameData {
	ShipsInfo ships;
	Board board;
} CorePlayerGameData;

typedef struct CoreGameData {
	CorePlayerGameData gdArray[2];
} CoreGameData;


typedef struct WindowParametres {
	int Begin_x;
	int Begin_y;
	int Width;
	int Height;
	WINDOW* ptrWin;
} WindowParametres;


typedef struct GameDataCase {
	int NumberOfShips[4];
	int BoardWidth[3];
	int BoardHeight[3];
} GameDataCase;

typedef struct GameDataCaseChoice {
	int caseShipsNumber;
	int caseBoardSize;
} GameDataCaseChoice;

typedef struct Indents {
    size_t LeftIndent;
    size_t BetweenIndent;
    size_t RightIndent;
    size_t TopIndent;
    size_t BottomIndent;
} Indents;

// void calculateAndRezizeWindow(Indents Indents, struct Board BoardPlayer);

// Инициализирует окно в соответствии с параметрами.
// TODO change for function createWindowWithParameters
#define initWindow(wp){ \
    wp->ptrWin = newwin(wp->Height, wp->Width, wp->Begin_y, wp->Begin_x); \
}

void initWindowWithParameters(WindowParametres *wp);
void initDerWindowWithParameters(WINDOW *parent, WindowParametres *wp);
void clearWindowParametres(WindowParametres *wp);

// Функции для работы с полями-массивами и их отрисовкой
bool isAllShipsStanding(ShipsInfo ships);
extern int getSize(int);
void deleteShipFromField(Ship* ship, const Board *Board);
void reDrawStandingShips(WINDOW *WIN, const Board *Board);
void refresh_ship_player_array(const ShipsInfo *Ships, const Board *Board);
void standing_ship(Ship* ship, const Board *Board);

// Проверка границ при постановке корабля в ship.
bool checkShipBorders(const Ship* ship, const Board *Board);
bool checkShipBorders_top_bottom_horizontal(const Ship* ship, const Board *Board);
bool checkShipBorders_left_right_horizontal(const Ship* ship, const Board *Board);
bool checkShipBorders_left_right_vertical(const Ship* ship, const Board *Board);
bool checkShipBorders_top_bottom_vertical(const Ship* ship, const Board *Board);
bool checkItself(const Ship* ship, const Board *Board);

bool checkAllShipsStanding(const ShipsInfo *ShipsPlayer); // Проверка постановки всех кораблей
int getShipsNumber(const ShipsInfo* info); // Возврат общего количества кораблей игрока
void InitPrimaryCoordinates(int curr_y, Ship* ship, const Board* const); // Установка начальных координат при первой постановке
bool checkPlace(int x, int y, int size, const Board *Board); // Проверка места под корабль

void DrawTmpShip(WINDOW* WIN, Ship* TmpShip, const Board *Board); // Рисует tmpShip в окне SHIP
void addShip(Ship*, Ship* TmpShip); // Добавляет tmp-корабль как элемент массива кораблей
void makeShipTmp(Ship* oldShipOnBoard, Ship* TmpShip); // Копирует данные корабля с доски в tmpShip, меняет поле oldShip->stane на false
void changeTypeOfShip(Ship* ship, const Board *Board); // Проверяет и меняет ориентацию корабля.
void clearTmpShip(Ship*);

// Изменение координат корабля при нажатие стрелок
// Функции типа changeBorder возвращают true при возможности подвинуть корбаль.
void changeShipCoordinates(Ship* TmpShip, const Board *Board, const int key);
bool checkBorderLeft(Ship* ship, const Board*);
bool checkBorderRight(Ship* ship, const Board*);
bool checkBorderTop(Ship* ship, const Board*);
bool checkBorderBot(Ship* ship, const Board*);


// Функции для обработки стрельбы
bool isShipHit(const Ship* ship, int coord_x, int coord_y);


// Функции для дебажного вывода состояния
void outputShip(FILE* out, const Ship* const ship);
void outputBoard(FILE* out, const Board* const ship);