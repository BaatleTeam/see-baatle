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

typedef struct ship_player
{
    int x;
    int y;
    int size;
    shipOrientationType type;
    bool stand;
} ship;

typedef struct ShipsInfo {
	int Number_4_Size;
	int Number_3_Size;
	int Number_2_Size;
	int Number_1_Size;
	ship* Ships;
} ShipsInfo;

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

typedef struct Indents {
    size_t LeftIndent;
    size_t BetweenIndent;
    size_t RightIndent;
    size_t TopIndent;
    size_t BottomIndent;
} Indents;

// void calculateAndRezizeWindow(Indents Indents, struct Board BoardPlayer);

// Инициализирует окно в соответствии с параметрами.
#define initWindow(wp){ \
    wp->ptrWin = newwin(wp->Height, wp->Width, wp->Begin_y, wp->Begin_x); \
}

// Функции для работы с полями-массивами и их отрисовкой
bool isAllShipsStanding(ShipsInfo ships);
extern int getSize(int);
void deleteShipFromField(ship* ship, const Board *Board);
void reDrawStandingShips(WINDOW *WIN, const Board *Board);
void refresh_ship_player_array(const ShipsInfo *Ships, const Board *Board);
void standing_ship(ship* ship, const Board *Board);

// Проверка границ при постановке корабля в ship.
bool checkShipBorders(const ship* ship, const Board *Board);
bool checkShipBorders_top_bottom_horizontal(const ship* ship, const Board *Board);
bool checkShipBorders_left_right_horizontal(const ship* ship, const Board *Board);
bool checkShipBorders_left_right_vertical(const ship* ship, const Board *Board);
bool checkShipBorders_top_bottom_vertical(const ship* ship, const Board *Board);
bool checkItself(const ship* ship, const Board *Board);

bool checkAllShipsStanding(const ShipsInfo *ShipsPlayer, const Board *BoardPlayer); // Проверка постановки всех кораблей
int getShipsNumber(const ShipsInfo* info); // Возврат общего количества кораблей игрока
void InitPrimaryCoordinates(int curr_y, ship* ship, const Board* const); // Установка начальных координат при первой постановке
bool checkPlace(int x, int y, int size, const Board *Board); // Проверка места под корабль

void DrawTmpShip(WINDOW* WIN, ship* TmpShip, const Board *Board); // Рисует tmpShip в окне SHIP
void addShip(ship*, ship* TmpShip); // Добавляет tmp-корабль как элемент массива кораблей
void makeShipTmp(ship* oldShipOnBoard, ship* TmpShip); // Копирует данные корабля с доски в tmpShip, меняет поле oldShip->stane на false
void changeTypeOfShip(ship* ship, const Board *Board); // Проверяет и меняет ориентацию корабля.
void clearTmpShip(ship*);

// Изменение координат корабля при нажатие стрелок
// Функции типа changeBorder возвращают true при возможности подвинуть корбаль.
void changeShipCoordinates(ship* TmpShip, const Board *Board, const int key);
bool checkBorderLeft(ship* ship, const Board*);
bool checkBorderRight(ship* ship, const Board*);
bool checkBorderTop(ship* ship, const Board*);
bool checkBorderBot(ship* ship, const Board*);


// Функции для обработки стрельбы
bool isShipHit(const ship* ship, int coord_x, int coord_y);


// Функции для дебажного вывода состояния
void outputShip(FILE* out, const ship* const ship);
void outputBoard(FILE* out, const Board* const ship);