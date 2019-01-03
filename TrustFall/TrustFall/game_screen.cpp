#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <string>

#include "game_screen.h"

#define FPS 60

using std::vector;
using std::string;

GameScreen::GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, int _lines) {
	sprites = _sprites;

	for (int i = 0; i < _lines; i++) {
		Line new_line(SCREEN_W/2, SCREEN_H/_lines * i, sprites);
		lines.push_back(new_line);
	}

	selected = 0;
}

void GameScreen::run(ALLEGRO_FONT * font) {
	bool music = true;

	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(1.5);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	redraw(font);
	al_flip_display();

	bool exit_screen = false;
	al_start_timer(timer);
	while (!exit_screen) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			int add_to = rand() % (lines.size()+1);
			for (int i = 0; i < lines.size(); i++) {
				if (i == add_to) {
					lines.at(i).add_employee();
				}
				lines.at(i).move();
			}
			al_clear_to_color(al_map_rgb(0, 0, 0));
			redraw(font);
			al_flip_display();
		}
	    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_DOWN:
				if (selected < 2) selected++;
				break;
			case ALLEGRO_KEY_UP:
				if (selected > 0) selected--;
				break;
			case ALLEGRO_KEY_SPACE:
				lines.at(selected).add_catcher();
				break;
			case ALLEGRO_KEY_M:
				if (ev.keyboard.modifiers == ALLEGRO_KEYMOD_CTRL) {
					music = false;
				}
				break;
			case ALLEGRO_KEY_H:
				if (ev.keyboard.modifiers == ALLEGRO_KEYMOD_CTRL) {
					music = music; //INCOMPLETE
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				next_state = Exit;
				exit_screen = true;
				break;
			}

			al_clear_to_color(al_map_rgb(0, 0, 0));
			redraw(font);
			al_flip_display();
		}
	}
}

void GameScreen::redraw(ALLEGRO_FONT* font) {
	al_draw_bitmap(sprites["Mr. Manager"], 10, 0 + (SCREEN_H / lines.size() * selected), NULL);

	for (int i = 0; i < lines.size(); i++) {
		lines.at(i).draw();
	}
}

void GameScreen::back() {
	next_state = Exit;
}

void GameScreen::cont() {
	next_state = End;
}