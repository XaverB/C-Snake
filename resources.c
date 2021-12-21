// file: src/resources.c
#include "resources.h"

#include "utils.h"

bool game_over = false;
bool done = false;
bool redraw = true;

ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp; 
ALLEGRO_FONT *font;
ALLEGRO_TEXTLOG* log;

ALLEGRO_COLOR color_background = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
ALLEGRO_COLOR color_snake = { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
ALLEGRO_COLOR color_game_over = {.r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f }; 
ALLEGRO_COLOR color_credit = { .r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };

ALLEGRO_SAMPLE* sound_mlem;
ALLEGRO_SAMPLE* sound_game_over;

enum game_state state = game_state_start;

void init_assets(void) {
  timer = assert_not_null(al_create_timer(REFRESH_RATE), "timer");
  queue = assert_not_null(al_create_event_queue(), "event queue");
  disp = assert_not_null(al_create_display(WIDTH, HEIGHT), "display");
  font = assert_not_null(al_create_builtin_font(), "built-in font");
  log = assert_not_null(al_open_native_text_log("Snake Log", ALLEGRO_TEXTLOG_NO_CLOSE), "log");
  al_reserve_samples(2);
  sound_game_over = assert_not_null(al_load_sample("game-over.mp3"), "game over");
  sound_mlem = assert_not_null(al_load_sample("mlem.mp3"), "mlem");
}

void destroy_assets(void) {
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_event_queue(queue);
  al_destroy_timer(timer);
  al_close_native_text_log(log);
  al_destroy_sample(sound_mlem);
  al_destroy_sample(sound_game_over);
}

void register_events(void) {
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
}

void unregister_events(void) {
  al_unregister_event_source(queue, al_get_display_event_source(disp));
  al_unregister_event_source(queue, al_get_mouse_event_source());
  al_unregister_event_source(queue, al_get_keyboard_event_source());
  al_unregister_event_source(queue, al_get_timer_event_source(timer));
}