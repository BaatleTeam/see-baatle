#pragma once
#include <curses.h>

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
    bool type;
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

void calculateAndRezizeWindow(Indents Indents, struct Board BoardPlayer);


enum actCase { CASE_1 = 0, CASE_2, CASE_3, CASE_4 };
enum shipOrientation { HORIZONTAL = FALSE, VERTICAL = TRUE } currOrientation; // На будущее

// Функции для работы с полями-массивами и их отрисовкой
extern int getSize(int);
void deleteShipFromField(ship* ship, struct Board Board);
void reDrawStandingShips(WINDOW *WIN, struct Board Board);
void refresh_ship_player_array(struct ShipsInfo Ships, struct Board Board);
void standing_ship(ship* ship, struct Board Board);

// Проверка границ при постановке корабля в ship.
bool checkShipBorders(const ship* const ship, const struct Board Board);
bool checkShipBorders_top_bottom_horizontal(const ship* const ship, const struct Board Board);
bool checkShipBorders_left_right_horizontal(const ship* const ship, const struct Board Board);
bool checkShipBorders_left_right_vertical(const ship* const ship, const struct Board Board);
bool checkShipBorders_top_bottom_vertical(const ship* const ship, const struct Board Board);
bool checkItself(const ship* const ship, const struct Board Board);

// Установка начальных коориданат при первой постановке
int InitPrimaryCoordinates(int curr_y, ship* ship, struct Board); 
bool checkPlace(int x, int y, int size, struct Board Board); // Проверка места под корабль

void DrawTmpShip(WINDOW* WIN, ship* TmpShip, struct Board Board); // Рисует tmpShip в окне SHIP
void addShip(ship*, ship* TmpShip); // Добавляет tmp-корабль как элемент массива кораблей
void makeShipTmp(ship* oldShipOnBoard, ship* TmpShip); // Копирует данные корабля с доски в tmpShip, меняет поле oldShip->stane на false
void changeTypeOfShip(ship* ship, struct Board Board); // Проверяет и меняет ориентацию корабля.
void clearTmpShip(ship*);

// Изменение координат корабля при нажатие стрелок
// Функции типа changeBorder возвращают true при возможности подвинуть корбаль.
void changeShipCoordinates(ship* TmpShip, struct Board Board, const int key);
bool checkBorderLeft(ship* ship, struct Board);
bool checkBorderRight(ship* ship, struct Board);
bool checkBorderTop(ship* ship, struct Board);
bool checkBorderBot(ship* ship, struct Board);