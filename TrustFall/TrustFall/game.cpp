#include "game.h"
#include "start_screen.h"
#include "game_screen.h"

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
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Mr. Manager", al_load_bitmap("BigMrManager.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyUp", al_load_bitmap("small_key_up.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Spacebar", al_load_bitmap("spacebar.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Employee", al_load_bitmap("employee.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("EmployeeHappy", al_load_bitmap("employee_happy.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("EmployeeSad", al_load_bitmap("employee_sad.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Conveyor", al_load_bitmap("conveyor.bmp")));
		
	font = al_create_builtin_font();
}

void Game::reset() {
	score = 0;
	state = Start;
}

void Game::run() {
	StartScreen start_screen(sprites);
	GameScreen game_screen(sprites);
	while (state != Exit) {
		switch (state) {
		case Start:
			start_screen.run(font);
			state = start_screen.next_state;
			break;
		case Gameplay:
			game_screen.run(font);
			state = game_screen.next_state;
			break;
		}
	}
}