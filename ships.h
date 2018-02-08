#pragma once
#include <curses.h>

typedef struct ship_player
{
    int x;
    int y;
    int size;
    bool type;
    bool stand;
} ship;

extern int convert_size(int);
void deleteShipFromField(ship* ship, bool field[10][15]);
void refresh_ship_player_gpaphics(WINDOW *WIN, bool field[10][15]);

int convert_ship_index(int, int);
int convert_index_to_active_y(int index);

bool checkShipBorders(ship* ship, bool field[10][15]);
bool checkShipBorders_top_bottom_horizontal(ship* ship, bool field[10][15]);
bool checkShipBorders_left_right_horizontal(ship* ship, bool field[10][15]);
bool checkShipBorders_left_right_vertical(ship* ship, bool field[10][15]);
bool checkShipBorders_top_bottom_vertical(ship* ship, bool field[10][15]);
bool checkItself(ship* ship, bool field[10][15]);

// Установка начальных коориданат при первой постановке
int InitPrimaryCoordinates(int curr_y, ship* ship, bool field[10][15]); 
bool checkPlace(int x, int y, int size, bool field[10][15]); // Проверка места под корабль

void clearTmpShip(ship*);
void DrawTmpShip(WINDOW*, ship* , bool field[10][15]); // Рисует tmpShip в окне SHIP
void addShip(ship*, ship* TmpShip); // Добавляет tmp-корабль как элемент массива кораблей
void makeShipTmp(ship* oldShipOnBoard, ship* TmpShip); // Копирует данные корабля с доски в tmpShip, меняет поле oldShip->stane на false
void changeTypeOfShip(ship* TmpShip, bool field[10][15]); // Проверяет и меняет ориентацию корабля.

// Изменение координат корабля при нажатие стрелок
// Функции типа changeBorder возвращают true при возможности подвинуть корбаль.
void changeShipCoordinates(ship* TmpShip, const int key);
bool checkBorderLeft(ship* ship);
bool checkBorderRight(ship* ship);
bool checkBorderTop(ship* ship);
bool checkBorderBot(ship* ship);