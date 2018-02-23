#pragma once
#include <curses.h>

struct Board {
	int Height;
	int Width;
	bool** field;
};

typedef struct ship_player
{
    int x;
    int y;
    int size;
    bool type;
    bool stand;
} ship;

struct ShipsInfo {
	int Number_4_Size;
	int Number_3_Size;
	int Number_2_Size;
	int Number_1_Size;
	ship* Ships;
};


extern int getSize(int);
void deleteShipFromField(ship* ship, struct Board Board);
void refresh_ship_player_gpaphics(WINDOW *WIN, struct Board Board);

bool checkShipBorders(const ship* const ship, const struct Board Board);
bool checkShipBorders_top_bottom_horizontal(const ship* const ship, const struct Board Board);
bool checkShipBorders_left_right_horizontal(const ship* const ship, const struct Board Board);
bool checkShipBorders_left_right_vertical(const ship* const ship, const struct Board Board);
bool checkShipBorders_top_bottom_vertical(const ship* const ship, const struct Board Board);
bool checkItself(const ship* const ship, const struct Board Board);

// Установка начальных коориданат при первой постановке
int InitPrimaryCoordinates(int curr_y, ship* ship, struct Board); 
bool checkPlace(int x, int y, int size, struct Board Board); // Проверка места под корабль

void clearTmpShip(ship*);
void DrawTmpShip(WINDOW* WIN, ship* TmpShip, struct Board Board); // Рисует tmpShip в окне SHIP
void addShip(ship*, ship* TmpShip); // Добавляет tmp-корабль как элемент массива кораблей
void makeShipTmp(ship* oldShipOnBoard, ship* TmpShip); // Копирует данные корабля с доски в tmpShip, меняет поле oldShip->stane на false
void changeTypeOfShip(ship* ship, struct Board Board); // Проверяет и меняет ориентацию корабля.

// Изменение координат корабля при нажатие стрелок
// Функции типа changeBorder возвращают true при возможности подвинуть корбаль.
void changeShipCoordinates(ship* TmpShip, struct Board Board, const int key);
bool checkBorderLeft(ship* ship, struct Board);
bool checkBorderRight(ship* ship, struct Board);
bool checkBorderTop(ship* ship, struct Board);
bool checkBorderBot(ship* ship, struct Board);