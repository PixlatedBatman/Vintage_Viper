#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float arena_half_size_x = 85, arena_half_size_y = 45;			// When changing these values, change Database array sizes as well
float border_length = 1.4f;
int square_x = 20; int square_y = static_cast<int>((arena_half_size_y / arena_half_size_x) * square_x);
float square_length = (2.f * arena_half_size_x) / square_x;
float snake_half_size = (92.f / 100.f) * (square_length / 2.f);			// Percentage smaller the square is can be changed here
float fruit_half_size = (100.f / 100.f) * (square_length / 2.f);
float eye_half_size = square_length / 10.f;
float cycle_time = 0, cycle_period = 0.3f;

// Database
int coordinates[1000], unoccupied[1000];
bool availability[1000];
int fruit_coordinate, score = 0, current_direction, direction;
int last_coordinate;
bool is_collided = false;

// The shift in y axis, so that score can be displayed
float y_shift = -3.f;

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

// x_sq and y_sq denote the xth and yth square position with the bottom left most square reference as (1,1)
// x_coordinate and y_coordinate are coordinates of the centres of the squares and are wrt pixels in the window

internal int
x_sq(int coordinate) {

	int pos = (coordinate - 1) % square_x + 1;
	return pos;
}

internal float
x_coordinate(int coordinate) {

	float x = (static_cast<float>(2 * x_sq(coordinate) - square_x) / 2.f - 0.5f) * square_length;
	return x;
}

internal int
y_sq(int coordinate) {

	int pos = ((coordinate - 1) / square_x) + 1;
	return pos;
}

internal float
y_coordinate(int coordinate) {

	float y = (static_cast<float>(2 * y_sq(coordinate) - square_y) / 2.f - 0.5f) * square_length;
	return y;
}

internal void
move_snake(int current_direction) {

	for (int i = score + 1; i > 0; i--) {
		coordinates[i] = coordinates[i - 1];
	}

	if (abs(current_direction - direction) == 2) current_direction = direction;

	if (current_direction == 0) {
		if (y_sq(coordinates[0]) == square_y) coordinates[0] -= (square_y - 1) * square_x;
		else coordinates[0] += square_x;
	}
	else if (current_direction == 1) {
		if (x_sq(coordinates[0]) == square_x) coordinates[0] -= (square_x - 1);
		else coordinates[0]++;
	}
	else if (current_direction == 2) {
		if (y_sq(coordinates[0]) == 1) coordinates[0] += (square_y - 1) * square_x;
		else coordinates[0] -= square_x;
	}
	else if (current_direction == 3) {
		if (x_sq(coordinates[0]) == 1) coordinates[0] += (square_x - 1);
		else coordinates[0]--;
	}

	direction = current_direction;
}

// Appends availability array to check which spots are occupied and which arent
internal void
check_availability() {

	for (int i = 1; i <= 200; i++) {
		availability[i] = true;
	}

	for (int i = 0; i <= score; i++) {
		availability[coordinates[i]] = false;
	}
}

// Finds the unoccupied coordinates and appends them to unoccupied array for the generation of random fruit coordinates
internal void
set_unoccupied() {
	
	int index = 0;

	for (int i = 1; i <= 200; i++) {
		if (availability[i]) {
			unoccupied[index] = i;
			index++;
		}
		else {
			continue;
		}
	}
}

// Checks for collision between the head of the snake and any other body part of it 
internal bool
check_collision() {
	for (int i = 1; i <= score; i++) {
		if (coordinates[i] == coordinates[0]) {
			return true;
		}
		else continue;
	}
	return false;
}

// Resets the game statistics so that new game can start
internal void reset_stats() {
	score = 0;
	cycle_time = 0;

	coordinates[0] = randomizer(1, square_x * square_y);
	direction = randomizer(0, 3);
	current_direction = direction;

	fruit_coordinate = randomizer(1, square_x * square_y);
}

// Time is used for generating randomness for now, may be changed if better randomness algorithm found
#include <time.h>


internal void
simulate_game(Input* input, float dt) {
	//render_background();

	clear_screen(0x000000);

	// Menu
	if (current_gamemode == GM_MENU) {
		draw_text("VINTAGE VIPER", -75, 30, 2, 0x00ff00);
		draw_text("PLAY", -14, 0, 1.5, 0xffff00);

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;

			srand(static_cast<unsigned int>(time(nullptr)));

			reset_stats();
		}
	}

	// Gameplay
	else if (current_gamemode == GM_GAMEPLAY) {
		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}

		// Updating cycle time to check for cycle periods
		cycle_time += dt;

		if (pressed(BUTTON_UP) || pressed(BUTTON_W)) {
			current_direction = 0;
		}
		if (pressed(BUTTON_RIGHT) || pressed(BUTTON_D)) {
			current_direction = 1;
		}
		if (pressed(BUTTON_DOWN) || pressed(BUTTON_S)) {
			current_direction = 2;
		}
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_A)) {
			current_direction = 3;
		}

		// Cycle period reached
		if (cycle_time >= cycle_period) {

			last_coordinate = coordinates[score];

			// Moving the snake in the current_direction
			move_snake(current_direction);

			cycle_time = 0; // Reset cycle_time

			// Collision detection
			is_collided = check_collision();
			if (is_collided) {
				current_gamemode = GM_OVER;
				return;
			}

			// Head reaches fruit
			if (coordinates[0] == fruit_coordinate) {

				score++;
				coordinates[score] = last_coordinate;

				// When the maximum score is reached
				if (score == square_x * square_y - 1) {
					current_gamemode = GM_WON;
					return;
				}

				// Checking positions that are occupied
				check_availability();

				// Selecting the unoccupied positions
				set_unoccupied();

				// Finding new fruit coordinate
				fruit_coordinate = unoccupied[randomizer(0, square_x * square_y - score - 2)];

			}
		}

		// Rendering snake and fruit
		draw_snake_head(x_coordinate(coordinates[0]), y_coordinate(coordinates[0]) + y_shift, direction, col_snake, col_eye);

		for (int i = 1; i <= score; i++) {
			draw_rect(x_coordinate(coordinates[i]), y_coordinate(coordinates[i]) + y_shift, snake_half_size, snake_half_size, col_snake);
		}

		draw_rect(x_coordinate(fruit_coordinate), y_coordinate(fruit_coordinate) + y_shift, fruit_half_size, fruit_half_size, col_fruit);
		
		// Rendering border box
		draw_rect((square_x * square_length * 0.5f) + (border_length * 0.5f) + (square_length * 0.18f), y_shift, border_length, (square_y * square_length * 0.5f) + 1.3f * border_length, 0x1c1c1c);
		draw_rect(-((square_x * square_length * 0.5f) + (border_length * 0.5f) + (square_length * 0.18f)), y_shift, border_length, (square_y * square_length * 0.5f) + 1.3f * border_length, 0x1c1c1c);
		draw_rect(0, (square_y * square_length * 0.5f) + (border_length * 0.5f) + (square_length * 0.18f) + y_shift, (square_x * square_length * 0.5f) + 2.56f * border_length, border_length, 0x1c1c1c);
		draw_rect(0, -((square_y * square_length * 0.5f) + (border_length * 0.5f) + (square_length * 0.18f)) + y_shift, (square_x * square_length * 0.5f) + 2.56f * border_length, border_length, 0x1c1c1c);

		// Rendering live score
		draw_number(score, 0, arena_half_size_y + border_length, 1.3f, col_score);
	}

	// Game Over Screen
	else if (current_gamemode == GM_OVER) {

		draw_text("GAME OVER", -45, 20, 1.7f, 0x00ff00);
		draw_text("YOUR SCORE", -33, 0, 1.1f, col_fruit);

		draw_number(score, 2, -18, 1.4f, col_score);

		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}
		if (pressed(BUTTON_ENTER)) {
			reset_stats();
			current_gamemode = GM_GAMEPLAY;
		}
	}

	// Won the game
	else {

		// Make the text Congo switch colors in duty cycles
		draw_text("CONGRATULATIONS", -78, 30, 1.8f, 0xffff00);
		draw_text("YOU HAVE WON", -42, 0, 1.2f, 0x00ffff);
		draw_text("VINTAGE VIPER", -55, -25, 1.5f, 0x00ff00);

		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}
	}
}