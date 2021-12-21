// file: src/resources.h

#pragma once

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define WIDTH 512
#define HEIGHT 512

#define BLOCK_WIDTH 30
#define BLOCK_HEIGHT 30
#define ROWS (HEIGHT/BLOCK_HEIGHT)
#define COLS (WIDTH/BLOCK_WIDTH)

#define REFRESH_RATE (1.0 / 10)

extern ALLEGRO_COLOR color_background;
extern ALLEGRO_COLOR color_snake;
extern ALLEGRO_COLOR color_game_over;
extern ALLEGRO_COLOR color_credit;

extern ALLEGRO_SAMPLE* sound_mlem;
extern ALLEGRO_SAMPLE* sound_game_over;

typedef enum game_state {
	game_state_start,
	game_state_play,
	game_state_game_over,
	game_state_done

} game_state;
extern enum game_state state;

extern bool redraw;

extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_FONT *font;
extern ALLEGRO_TEXTLOG* log;

void init_assets(void);
void destroy_assets(void);

void register_events(void);
void unregister_events(void);