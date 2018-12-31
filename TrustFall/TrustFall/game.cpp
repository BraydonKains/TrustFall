#include "game.h"
#include "start_screen.h"

using std::string;
using std::map;
using std::pair;

Game::Game() {
}

void Game::init() {
	score = 0;
	state = Start;
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Title", al_load_bitmap("logo.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Mr. Man", al_load_bitmap("MrMan.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Mr. Manager", al_load_bitmap("MrManager.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyUp", al_load_bitmap("small_key_up.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Spacebar", al_load_bitmap("spacebar.bmp")));
	
	font = al_create_builtin_font();
}

void Game::reset() {
	score = 0;
	state = Start;
}

void Game::run() {
	while (state != Exit) {
		switch (state) {
		case Start:
			StartScreen start_screen(sprites);
			start_screen.run(font);
			state = start_screen.next_state;
			break;
		}
	}
}