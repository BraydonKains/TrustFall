#include "game.h"
#include "start_screen.h"
#include "game_screen.h"
#include "result_screen.h"

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
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Mr. ManagerSad", al_load_bitmap("mrmanagersad.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Mr. ManagerHappy", al_load_bitmap("mrmanagerhappy.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyUp", al_load_bitmap("small_key_up.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Spacebar", al_load_bitmap("spacebar.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Employee", al_load_bitmap("employee.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("EmployeeHappy", al_load_bitmap("employee_happy.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("EmployeeSad", al_load_bitmap("employee_sad.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("Conveyor", al_load_bitmap("conveyor.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyCtrl", al_load_bitmap("small_key_ctrl.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyH", al_load_bitmap("small_key_h.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyM", al_load_bitmap("small_key_m.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("KeyEsc", al_load_bitmap("small_key_esc.bmp")));
	
	font = al_create_builtin_font();
}

void Game::reset() {
	score = 0;
	state = Start;
}

void Game::run() {
	GameScreen game_screen(sprites);
	StartScreen start_screen(sprites);
	ResultScreen result_screen(sprites, 0, 0);
	while (state != Exit) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		switch (state) {
		case Start:
			start_screen.run(font);
			state = start_screen.next_state;
			break;
		case Gameplay:
			game_screen.reset(3, 5, 3);
			game_screen.run(font);
			state = game_screen.next_state;
			break;
		case End:
			result_screen.score = game_screen.score;
			result_screen.difficulty = game_screen.difficulty;
			result_screen.run(font);
			state = result_screen.next_state;
			break;
		}
	}
}