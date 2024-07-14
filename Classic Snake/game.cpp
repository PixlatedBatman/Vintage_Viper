#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float arena_half_size_x = 85, arena_half_size_y = 45;			// When changing these values, change Database array size as well
int square_x = 20; int square_y = (arena_half_size_y / arena_half_size_x) * square_x;
float square_length = (2.f * arena_half_size_x) / square_x;
float snake_half_size = (92.f / 100.f) * (square_length / 2.f);			// Percentage smaller the square is can be changed here
float fruit_half_size = (100.f / 100.f) * (square_length / 2.f);
float eye_half_size = square_length / 10.f;
float cycle_time = 0, cycle_period = 0.45f;

// Database
int coordinates[20*10], directions[20*10], unoccupied[20*10];
int fruit_coordinate, score = 0;

// Colors
u32 col_background = 0x000000;
u32 col_snake = 0x00ff00;					// These can ve viewed in Paint tries folder in the project directory
u32 col_fruit = 0xffff00;
u32 col_score = 0x00ffff;
u32 col_eye = 0xff0000;


enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
	GM_OVER,
	GM_WON,
};

Gamemode current_gamemode;


internal void
draw_snake_head(float x, float y, int direction, u32 color_body, u32 color_eye) {
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
randomizer(int start, int end) {

	float rand_percent = static_cast<float>(rand()) / RAND_MAX;

	int rand_ind = start + static_cast<int>(rand_percent * end);

	return rand_ind;
}

internal float
x_coordinate(int coordinate) {

	int x_sq = coordinate % square_x;
	if (!x_sq) x_sq = square_x;

	float x = (static_cast<float>(2 * x_sq - square_x) / 2.f - 0.5f) * square_length;
	return x;
}

internal float
y_coordinate(int coordinate) {

	int y_sq = ((coordinate - 1) / square_x) + 1;

	float y = (y_sq - static_cast<float>(square_y) / 2 - 0.5f) * square_length;
	return y;
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

			srand(time(0));
			
			coordinates[0] = randomizer(1, square_x * square_y);
			directions[0] = randomizer(0, 3);

			fruit_coordinate = randomizer(1, square_x * square_y);
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

		//cycle_time += dt;

		if (cycle_time >= cycle_period) {
			move_snake();
			cycle_time = 0;
		}

		draw_snake_head(x_coordinate(coordinates[0]), y_coordinate(coordinates[0]), directions[0], col_snake, col_eye);
		draw_rect(x_coordinate(fruit_coordinate), y_coordinate(fruit_coordinate), fruit_half_size, fruit_half_size, col_fruit);
		
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