#include "shoot.h"

void choosing_comp_strategy(bool ship_comp_field[10][15]){
    int key = rand() % 2;
    switch (key){
        case 0:
            ship_comp_field[6][1] = TRUE;
            ship_comp_field[7][1] = TRUE;
            ship_comp_field[8][1] = TRUE;
            ship_comp_field[9][1] = TRUE;

            ship_comp_field[1][11] = TRUE;
            ship_comp_field[1][12] = TRUE;
            ship_comp_field[1][13] = TRUE;
            ship_comp_field[1][14] = TRUE;

            ship_comp_field[0][4] = TRUE;
            ship_comp_field[1][4] = TRUE;
            ship_comp_field[2][4] = TRUE;

            ship_comp_field[4][7] = TRUE;
            ship_comp_field[5][7] = TRUE;
            ship_comp_field[6][7] = TRUE;

            ship_comp_field[7][11] = TRUE;
            ship_comp_field[7][12] = TRUE;
            ship_comp_field[7][13] = TRUE;

            ship_comp_field[1][0] = TRUE;
            ship_comp_field[2][0] = TRUE;

            ship_comp_field[4][3] = TRUE;
            ship_comp_field[5][3] = TRUE;

            ship_comp_field[0][8] = TRUE;
            ship_comp_field[1][8] = TRUE;

            ship_comp_field[8][7] = TRUE;
            ship_comp_field[9][7] = TRUE;

            ship_comp_field[1][2] = TRUE;
            ship_comp_field[1][6] = TRUE;
            ship_comp_field[3][10] = TRUE;
            ship_comp_field[7][4] = TRUE;
            ship_comp_field[4][13] = TRUE;
            ship_comp_field[8][9] = TRUE;
            break;
        case 1:
            ship_comp_field[7][2] = TRUE;
            ship_comp_field[7][3] = TRUE;
            ship_comp_field[7][4] = TRUE;
            ship_comp_field[7][5] = TRUE;

            ship_comp_field[6][11] = TRUE;
            ship_comp_field[7][11] = TRUE;
            ship_comp_field[8][11] = TRUE;
            ship_comp_field[9][11] = TRUE;

            ship_comp_field[2][10] = TRUE;
            ship_comp_field[3][10] = TRUE;
            ship_comp_field[4][10] = TRUE;

            ship_comp_field[1][0] = TRUE;
            ship_comp_field[2][0] = TRUE;
            ship_comp_field[6][0] = TRUE;

            ship_comp_field[0][12] = TRUE;
            ship_comp_field[0][13] = TRUE;
            ship_comp_field[0][14] = TRUE;

            ship_comp_field[4][1] = TRUE;
            ship_comp_field[5][1] = TRUE;

            ship_comp_field[0][5] = TRUE;
            ship_comp_field[1][5] = TRUE;

            ship_comp_field[3][5] = TRUE;
            ship_comp_field[4][5] = TRUE;

            ship_comp_field[1][7] = TRUE;
            ship_comp_field[2][7] = TRUE;

            ship_comp_field[1][2] = TRUE;
            ship_comp_field[4][3] = TRUE;
            ship_comp_field[5][8] = TRUE;
            ship_comp_field[8][7] = TRUE;
            ship_comp_field[2][12] = TRUE;
            ship_comp_field[4][11] = TRUE;
            break;
    }
}

void change_x_leftkey(int* x){
    *x -= 1;
}

void change_x_rightkey(int* x){
    *x += 1;
}

void change_y_botkey(int* y){
    *y += 1;
}

void change_y_topkey(int* y){
    *y -= 1;
}

bool check_border_left_shoot(int y, int x, int field[10][15]){
    if (x == 0 /*|| field[y][x-1] == 0 || field[y][x-1] == 1*/)
        return FALSE;
    else
        return TRUE;
}


bool check_border_right_shoot(int y, int x, int field[10][15]){
    if (x == 14 /*|| field[y][x+1] == 0 || field[y][x+1] == 1*/)
        return FALSE;
    else
        return TRUE;
}

bool check_border_top_shoot(int y, int x, int field[10][15]){
    if (y == 0 /*|| field[y-1][x] == 0 || field[y-1][x] == 1*/)
        return FALSE;
    else
        return TRUE;
}

bool check_border_bot_shoot(int y, int x, int field[10][15]){
    if (y == 9 /*|| field[y+1][x] == 0 || field[y+1][x] == 1*/)
        return FALSE;
    else
        return TRUE;
}