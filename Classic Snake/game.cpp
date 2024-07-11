#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;

internal void
simulate_game(Input* input, float dt) {
	//render_background();

	clear_screen(0x000000);

	if (current_gamemode == GM_MENU) {
		draw_text("VINTAGE VIPER", -75, 30, 2, 0x00ff00);
		draw_text("PLAY", -14, 0, 1.5, 0xffff00);

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
		}
	}
	else {
		if (pressed(BUTTON_ESC)) {
			current_gamemode = GM_MENU;
		}
	}
}