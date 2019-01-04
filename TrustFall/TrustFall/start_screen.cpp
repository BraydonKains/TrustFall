#include "start_screen.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

StartScreen::StartScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites) {
	vector<string> menu_options;
	menu_options.push_back("Start");
	menu_options.push_back("Quit");
	menu.activate(menu_options);

	sprites = _sprites;
}

void StartScreen::run(ALLEGRO_FONT * font) {
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	redraw(font);
	menu.draw(300.0, 400.0, 20.0, font);
	al_flip_display();
	
	bool exit_screen = false;
	while (!exit_screen) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				menu.up();
				break;
			case ALLEGRO_KEY_DOWN:
				menu.down();
				break;
			case ALLEGRO_KEY_ENTER:
				cont();
				exit_screen = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				back();
				exit_screen = true;
				break;
			}
			redraw(font);
			al_flip_display();
		}
	}
}

void StartScreen::redraw(ALLEGRO_FONT* font) {
	al_draw_bitmap(sprites["Title"], 0, 20, NULL);
	al_draw_bitmap(sprites["Mr. Manager"], SCREEN_W / 2 - 40, 120, NULL);
	for (int i = 1; i < 7; i++) {
		al_draw_bitmap(sprites["Mr. Man"], SCREEN_W / 8 * i + 14, SCREEN_H / 2 - 30, NULL);
	}

	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 20, ALLEGRO_ALIGN_CENTER, "Mr. Manager wants to run a teambuilding exercise for his employees.");
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 30, ALLEGRO_ALIGN_CENTER, "Help Mr.Manager place his employees at the end of his");
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 40, ALLEGRO_ALIGN_CENTER, "conveyor belts to make sure no other employees fall off.");

	al_draw_bitmap(sprites["KeyUp"], SCREEN_W / 8, SCREEN_H / 2 + 60, NULL);
	al_draw_bitmap(sprites["KeyUp"], SCREEN_W / 8 + 34, SCREEN_H / 2 + 60, ALLEGRO_FLIP_VERTICAL);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 68, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "Up and down - Move cursor");

	al_draw_bitmap(sprites["Spacebar"], SCREEN_W / 8 + 20, SCREEN_H / 2 + 100, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 68, SCREEN_H / 1.3 - 20, ALLEGRO_ALIGN_LEFT, "Spacebar - Place employee");

	al_draw_bitmap(sprites["KeyCtrl"], SCREEN_W / 2 + 40, SCREEN_H / 2 + 60, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 70, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "+");
	al_draw_bitmap(sprites["KeyH"], SCREEN_W / 2 + 78, SCREEN_H / 2 + 60, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 113, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "Help menu");

	al_draw_bitmap(sprites["KeyCtrl"], SCREEN_W / 2 + 40, SCREEN_H / 2 + 100, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 70, SCREEN_H / 2 + 110, ALLEGRO_ALIGN_LEFT, "+");
	al_draw_bitmap(sprites["KeyM"], SCREEN_W / 2 + 78, SCREEN_H / 2 + 100, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 113, SCREEN_H / 1.3 - 20, ALLEGRO_ALIGN_LEFT, "Toggle Music");
}

void StartScreen::back() {
	next_state = Exit;
}

void StartScreen::cont() {
	if (menu.get_selected() == "Start") {
		next_state = Gameplay;
	}
	else {
		next_state = Exit;
	}
}