// file: src/snake.h

#pragma once

#include <allegro5/allegro5.h>

/*
* Enum for the current direction of the snake head
*/
typedef enum snake_direction {
    snake_direction_left  = 0,
    snake_direction_right = 1,
    snake_direction_down = 2,
    snake_direction_up = 3
} snake_direction;

typedef struct snake_t *snake_ptr;

/*
* Needs to be before any other functions of this module are used.
*/
void snake_init(void);

/*
* Creates a new snake.
* @Param start_x: start position x for the snake.
* @Param start_y: start position y for the snake.
* @Param start_direction: the start moving direction of the newly created snake.
* Returns: snake
*/
snake_ptr snake_create(int start_column, int start_row, snake_direction start_direction);

/*
* Updates the snakes position according to the set direction. Updates the board according to the new positions.
* Snake will grow, if the head hit a credit.
* Checks and sets game_over if the snake is out of map or hit itself.
* @Param snake: snake
*/
void snake_move(snake_ptr snake);

/*
* Set the current moving direction.
* @Param snake: snake
* @Param snake: direction
*/
void snake_set_direction(snake_ptr snake, snake_direction direction);

/*
* Spawns a credit at a random position.
* There can only be MAX_CREDITS_ON_MAP credits
*/
void snake_spawn_credit(void);

/*
* Prints the snakes position on the log window.
* @Param snake: snake
*/
void snake_print_position(snake_ptr snake);

/*
* Determines the current players points.
* @Param snake: snake
* @Return points (snake body nodes * POINT_MULTIPLICATOR
*/
int snake_get_points(snake_ptr snake);