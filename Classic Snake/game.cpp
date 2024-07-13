#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float arena_half_size_x = 85, arena_half_size_y = 45;			// When changing these values, change Database array size as well
int square_x = 20, square_y = (arena_half_size_y / arena_half_size_x) * square_x;
float square_length = arena_half_size_x / square_x;
float snake_half_size = (92 / 100) * (square_length / 2);			// Percentage smaller the square is can be changed here
float fruit_half_size = (100 / 100) * (square_length / 2);
float eye_half_size = square_length / 10;
float cycle_time = 0, cycle_period = 0.6f;

// Database
int coordinates[20*10], directions[20*10], unoccupied[20*10];
int fruit_coordinate, score = 0;

// Colors
u32 col_background = 0x000000;
u32 col_snake = 0x00ff00;					// These can ve viewed in Paint tries folder in the project directory
u32 col_fruit = 0xffff00;
u32 col_score = 0x00ffff;


enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
	GM_OVER,
	GM_WON,
};

Gamemode current_gamemode;

internal void
draw_snake_body(float x, float y, u32 color) {
	draw_rect(x, y, snake_half_size, snake_half_size, color);
}

internal void
draw_snake_body(float x, float y, int direction, u32 color_body, u32 color_eye) {
	draw_rect(x, y, snake_half_size, snake_half_size, color_body);

	if (direction == 0 || direction == 1) {
		draw_rect(x + 2 * eye_half_size, y + 2 * eye_half_size, eye_half_size, eye_half_size, color_eye);
	}
	if (direction == 1 || direction == 2) {
		draw_rect(x + 2 * eye_half_size, y - 2 * eye_half_size, eye_half_size, eye_half_size, color_eye);
	}
	if (direction == 2 || direction == 3) {
		draw_rect(x - 2 * eye_half_size, y - 2 * eye_half_size, eye_half_size, eye_half_size, color_eye);
	}
	if (direction == 3 || direction == 0) {
		draw_rect(x - 2 * eye_half_size, y + 2 * eye_half_size, eye_half_size, eye_half_size, color_eye);
	}
}

#include <time.h>

internal int
randomizer() {
	srand(time(0));
	float rand_percent = static_cast<float>(rand()) / RAND_MAX;

	int rand_ind = static_cast<int>(rand_percent * (square_x * square_y - score - 2));

	return unoccupied[rand_ind];
}

internal void
move_snake() {

}


internal void
simulate_game(Input* input, float dt) {
	//render_background();

	clear_screen(0x000000);					// 0x800080 for purple box
	//draw_rect(0, 0, 85, 45, 0x000000);

	// Menu
	if (current_gamemode == GM_MENU) {
		draw_text("VINTAGE VIPER", -75, 30, 2, 0x00ff00);
		draw_text("PLAY", -14, 0, 1.5, 0xffff00);

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
		}
	}

	// Gameplay
	else if (current_gamemode == GM_GAMEPLAY) {
		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}
		else if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_WON;
		}

		cycle_time += dt;

		if (cycle_time >= cycle_period) {
			move_snake();
			cycle_time = 0;
		}
	}

	// Game Over Screen
	else if (current_gamemode == GM_OVER) {
		draw_text("GAME OVER", -45, 20, 1.7, 0x00ff00);

		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}
	}

	// Won the game
	else {

		// Make the text Congo switch colors in duty cycles
		draw_text("CONGRATULATIONS", -78, 30, 1.8, 0xffff00);
		draw_text("YOU HAVE WON", -42, 0, 1.2, 0x00ffff);
		draw_text("VINTAGE VIPER", -55, -25, 1.5, 0x00ff00);

		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}
	}
}