#include "game_screen.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

GameScreen::GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, int _lines) {
	sprites = _sprites;

	for (int i = 0; i < _lines; i++) {
		Line new_line(SCREEN_W-220, SCREEN_H/_lines * i, sprites);
	}

	selected = 0;
}

void GameScreen::run(ALLEGRO_FONT * font) {
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	redraw(font);
	al_flip_display();

	bool exit_screen = false;
	while (!exit_screen) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			
		}
	}
}

void GameScreen::redraw(ALLEGRO_FONT* font) {

}

void GameScreen::back() {
	next_state = Exit;
}

void GameScreen::cont() {
	next_state = End;
}