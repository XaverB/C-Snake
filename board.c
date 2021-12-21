// file: src/board.c
#include "board.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "resources.h"
#include "snake.h"
#include "utils.h"

ALLEGRO_COLOR board[ROWS][COLS];

static void block_draw(int col, int row, ALLEGRO_COLOR color) {
    float x1 = col * BLOCK_WIDTH + 1;
    float y1 = row * BLOCK_HEIGHT + 1;
    float x2 = (col + 1) * BLOCK_WIDTH - 1;
    float y2 = (row + 1) * BLOCK_HEIGHT - 1;

    al_draw_filled_rectangle(x1, y1, x2, y2, color);
}

void board_init(void) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col] = color_background;
        }
    }
}

void board_draw(void) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            block_draw(row, col, board[row][col]);
        }
    }
}

bool board_is_empty(int row, int col) {
    return color_equal(board[row][col], color_background);
}