// file: src/board.h
#pragma once
#include "resources.h"
#include "snake.h"

#define ROWS (HEIGHT/BLOCK_HEIGHT)
#define COLS (WIDTH/BLOCK_WIDTH)

extern ALLEGRO_COLOR board[ROWS][COLS];

void board_init(void);
void board_draw(void);
bool board_is_empty(int row, int col);