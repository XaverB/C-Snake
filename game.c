// file: src/game.c
#include "game.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "resources.h"
#include "utils.h"
#include "snake.h"
#include "board.h"
#include "logger.h"
#include "exit_codes.h"

snake_ptr snake;

#define START_COLUMN 2
#define START_ROW 4
#define START_SCREEN_SHOWTIME_SECONDS 3

time_t start_screen_start_time;

/*
* Prints a start screen. When calling again, after START_SCREEN_SHOWTIME_SECONDS the state will be set to game_state_play.
*/
void print_start() {
	time_t start_screen_end_time;
	time(&start_screen_end_time);
	double time_dif = difftime(start_screen_end_time, start_screen_start_time);
	al_draw_textf(font, color_random(0.5), WIDTH / 2.0 - 50, HEIGHT / 2.0, 0, "Get ready %1.0f..", (START_SCREEN_SHOWTIME_SECONDS - time_dif));
	if (time_dif >= START_SCREEN_SHOWTIME_SECONDS)
		state = game_state_play;
}

/*
* Prints a game over screen and initializes the board.
*/
static void print_game_over()
{
	board_init();
	al_draw_textf(font, color_game_over, WIDTH / 2.0 - 50, HEIGHT / 2.0 - 25, 0, "Game Over");
	al_draw_textf(font, color_game_over, WIDTH / 2.0 - 90, HEIGHT / 2.0, 0, "You reached %d points", snake_get_points(snake));
	al_draw_textf(font, color_game_over, WIDTH / 2.0 - 100, HEIGHT / 2.0 + 25, 0, "Press ENTER to restart");
	return;
}

/*
* Initializes the game and sets start_screen_start_time to now
*/
void game_init(void) {
	srand(time(NULL));
	snake_init();
	snake = snake_create(START_COLUMN, START_ROW, snake_direction_right);
	board_init();
	state = game_state_start;
	time(&start_screen_start_time);
}

void game_update(ALLEGRO_TIMER_EVENT event) {
	if (state != game_state_play) {
		return;
	}

	snake_spawn_credit();
	snake_move(snake);
}

void game_redraw(void) {
	al_clear_to_color(color_background);

	switch (state)
	{
	case game_state_start:
		print_start();
		break;
	case game_state_game_over:
		print_game_over();
		break;
	case game_state_play:
		board_draw();
		break;
	default: exit(EXIT_CODE_INVALID_STATE);
	}

	al_flip_display();
}

void mouse_move(ALLEGRO_MOUSE_EVENT event) {
}

void mouse_down(ALLEGRO_MOUSE_EVENT event) {
}

void mouse_up(ALLEGRO_MOUSE_EVENT event) {
}

void key_down(ALLEGRO_KEYBOARD_EVENT event) {
}

void key_up(ALLEGRO_KEYBOARD_EVENT event) {
}

void key_char(ALLEGRO_KEYBOARD_EVENT event) {
	switch (event.keycode) {
	case ALLEGRO_KEY_UP:
		snake_set_direction(snake, snake_direction_up);
		break;
	case ALLEGRO_KEY_DOWN:
		snake_set_direction(snake, snake_direction_down);
		break;
	case ALLEGRO_KEY_LEFT:
		snake_set_direction(snake, snake_direction_left);
		break;
	case ALLEGRO_KEY_RIGHT:
		snake_set_direction(snake, snake_direction_right);
		break;
	case ALLEGRO_KEY_SPACE:
		snake_print_position(snake);
		break;
	case ALLEGRO_KEY_ENTER:
		game_init();
		break;
	}
}