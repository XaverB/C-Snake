// file: src/snake.c

#include "snake.h"

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>

#include "exit_codes.h"
#include "resources.h"
#include "utils.h"
#include "board.h"
#include "logger.h"

#define COORDINATES_UNSET INT_MIN
#define MAX_CREDITS_ON_MAP 4
#define MAX_CREDIT_PLACEMENT_TRIES 10
#define POINT_MULTIPLICATOR 10

// ----------------------------------
// ---- internal struct typedefs ----
// ----------------------------------

typedef struct position
{
	int column;
	int row;
} position;

/*
* Single node for the snake body.
* A snake consists of multiple nodes.
*/
typedef struct snake_body_node* snake_body_node_ptr;
typedef struct snake_body_node
{
	position position;
	snake_body_node_ptr next;

} snake_body_node;
typedef snake_body_node_ptr snake_head_node_ptr;

/*
* Snake struct. Defines direction and body
*/
typedef struct snake_t
{
	snake_direction direction;
	snake_head_node_ptr head;

} snake_t;

// ----------------------------------
// ------ member variables ----------
// ----------------------------------

/* We need to remember the amount of credits on the board, because we want to spawn only MAX_CREDITS_ON_MAP */
static int credits_on_map_counter = 0;


// ----------------------------------
// -------- internal Util -----------
// ----------------------------------

/*
* Places all nodes on the board
*/
static void snake_node_to_board(snake_head_node_ptr node) {
	while (node != NULL) {
		int column = node->position.column;
		int row = node->position.row;
		board[column][row] = color_snake;
		node = node->next;
	}
}

/*
* Removes the last body node from the board and overwrites it's place with color_background
*/
static void remove_last_snake_node_from_board(position position) {
	int column = position.column;
	int row = position.row;
	board[column][row] = color_background;

}

static bool is_out_of_map(position head_position) {
	bool is_out = (head_position.column < 0 || head_position.column >= COLS || head_position.row < 0 || head_position.row >= ROWS);
	if (is_out)
		Logger_Info("snake is out of map. head_position column (%d), row (%d)", head_position.column, head_position.row);

	return is_out;
}

static bool is_inside_itself(position head_position) {
	bool is_inside = color_equal(board[head_position.column][head_position.row], color_snake);
	if (is_inside)
		Logger_Info("snake hit itself. head_position column (%d), row (%d)", head_position.column, head_position.row);

	return is_inside;
}

static game_state is_game_over(position head_position) {
	if (is_out_of_map(head_position))
		return game_state_game_over;
	if (is_inside_itself(head_position))
		return game_state_game_over;
	return game_state_play;
}

static void snake_pre_grow()
{
	al_play_sample(sound_mlem, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	credits_on_map_counter--;
}

/*
* Creates a new nody node
*/
static snake_body_node_ptr snake_create_body_node(void) {
	snake_body_node_ptr node = (snake_body_node_ptr)malloc(sizeof(struct snake_body_node));
	if (node == NULL) exit(EXIT_CODE_NO_MEMORY_AVAILABLE);

	node->next = NULL;
	node->position.column = COORDINATES_UNSET;
	node->position.row = COORDINATES_UNSET;

	return node;
}

static void snake_grow(snake_ptr snake, position last_position) {
	snake_body_node_ptr node = snake->head;
	while (node->next != NULL) {
		node = node->next;
	}

	node->next = snake_create_body_node();
	memcpy(&node->next->position, &last_position, sizeof(struct position));
}

static bool should_grow(snake_head_node_ptr head) {
	ALLEGRO_COLOR current_color = board[head->position.column][head->position.row];
	bool grow = color_equal(current_color, color_credit);
	if (grow) {
		Logger_Debug("we need to grow, because we found credits at column (%d), row (%d)", head->position.column, head->position.row);
	}
	return grow;
}

static position get_last_position(snake_ptr snake) {
	snake_body_node_ptr node = snake->head;
	while (node->next != NULL) {
		node = node->next;
	}
	position p;
	memcpy(&p, &node->position, sizeof(struct position));
	return node->position;
}


/*
* Recursively move the body
*/
static void snake_move_body(snake_body_node_ptr node, position* new_position) {
	// body moved
	if (node == NULL)
		return;

	if (node->next == NULL) {
		board[node->position.column][node->position.row] = color_background;
	}

	// call for next body node
	snake_move_body(node->next, &node->position);
	// next node should get the current node position
	memcpy(&node->position, new_position, sizeof(struct position));

}

static void update_head_position(snake_ptr snake) {
	switch (snake->direction)
	{
	case snake_direction_right:
		snake->head->position.column += 1;
		break;
	case snake_direction_left:
		snake->head->position.column -= 1;
		break;
	case snake_direction_down:
		snake->head->position.row += 1;
		break;
	case snake_direction_up:
		snake->head->position.row -= 1;
		break;
	}
}

// ----------------------------------
// ------ visible functions----------
// ----------------------------------

/*
* Needs to be before any other functions of this module are used.
*/
void snake_init(void) {
	credits_on_map_counter = 0;
}

/*
* Creates a new snake.
* @Param start_x: start position x for the snake.
* @Param start_y: start position y for the snake.
* @Param start_direction: the start moving direction of the newly created snake.
* Returns: snake
*/
snake_ptr snake_create(int start_column, int start_row, snake_direction start_direction) {
	snake_ptr snake = (snake_ptr)malloc(sizeof(struct snake_t));
	if (snake == NULL) exit(EXIT_CODE_NO_MEMORY_AVAILABLE);
	snake->direction = start_direction;
	snake->head = (snake_head_node_ptr)snake_create_body_node();
	snake->head->position.column = start_column;
	snake->head->position.row = start_row;
	return snake;
}

/*
* Updates the snakes position according to the set direction. Updates the board according to the new positions.
* Snake will grow, if the head hit a credit.
* Checks and sets game_over if the snake is out of map or hit itself.
* @Param snake: snake
*/
void snake_move(snake_ptr snake) {
	// 1. remember position of the last body node and move the snake ´to its new position
	position last_position = get_last_position(snake);
	snake_move_body(snake->head, &snake->head->position);
	update_head_position(snake);


	// 2. determine if the snake ate some tasty credits
	if (should_grow(snake->head))
	{
		//2a. play some sound and make the snake grow!
		snake_pre_grow();
		snake_grow(snake, last_position);
	}
	else {
		// 2b. we only need to remove the last node from the board if the snake did not grow
		remove_last_snake_node_from_board(last_position);
	}

	// 3. check if it's game over (snake outside of map or hit itself
	state = is_game_over(snake->head->position);
	if (state == game_state_game_over) {
		Logger_Info("game over :(");
		al_play_sample(sound_game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		return;
	}

	// 4. update the board with the new snakes position
	snake_node_to_board(snake->head);
}

/*
* Set the current moving direction.
* @Param snake: snake
* @Param snake: direction
*/
void snake_set_direction(snake_ptr snake, snake_direction direction) {
	snake->direction = direction;
}

/*
* Spawns a credit at a random position.
* There can only be MAX_CREDITS_ON_MAP credits
*/
void snake_spawn_credit(void) {
	if (credits_on_map_counter >= MAX_CREDITS_ON_MAP)
		return;

	bool new_credit_placed = false;
	int tries = 0;
	while (!new_credit_placed && MAX_CREDIT_PLACEMENT_TRIES <= 10) {
		int column = rand() % COLS;
		int row = rand() % ROWS;

		if (color_equal(board[column][row], color_background)) {
			board[column][row] = color_credit;
			credits_on_map_counter++;
			new_credit_placed = true;
		}
		tries++;
	}
}

/*
* Prints the snakes position on the log window.
* @Param snake: snake
*/
void snake_print_position(snake_ptr snake) {
	snake_body_node_ptr node = snake->head;
	int counter = 1;
	while (node != NULL) {
		Logger_Info("snake node (%d) position is at column (%d), row (%d)", counter++, node->position.column, node->position.row);
		node = node->next;
	}
}

/*
* Determines the current players points.
* @Param snake: snake
* @Return points (snake body nodes * POINT_MULTIPLICATOR
*/
int snake_get_points(snake_ptr snake) {
	int node_count = 0;
	snake_body_node_ptr node = snake->head->next;
	while (node != NULL) {
		node = node->next;
		node_count++;
	}
	return node_count * POINT_MULTIPLICATOR;
}