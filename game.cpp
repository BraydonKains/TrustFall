#include "game.h"
#include "start_screen.h"
#include "game_screen.h"
#include "result_screen.h"

using std::string;
using std::map;
using std::pair;

Game::Game() {
}

//Load all global data for the game: sprites, audio, and font.
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
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("MusicOn", al_load_bitmap("music_on.bmp")));
	sprites.insert(pair<string, ALLEGRO_BITMAP*>("MusicOff", al_load_bitmap("music_off.bmp")));
	
	al_reserve_samples(4);
	samples.insert(pair<string, ALLEGRO_SAMPLE*>("Theme", al_load_sample("rasputin.wav")));
	samples.insert(pair<string, ALLEGRO_SAMPLE*>("Move", al_load_sample("move.wav")));
	samples.insert(pair<string, ALLEGRO_SAMPLE*>("Catch", al_load_sample("catch.wav")));
	samples.insert(pair<string, ALLEGRO_SAMPLE*>("Fall", al_load_sample("fall.wav")));
	samples.insert(pair<string, ALLEGRO_SAMPLE*>("Place", al_load_sample("place.wav")));
	samples.insert(pair<string, ALLEGRO_SAMPLE*>("LevelUp", al_load_sample("level.wav")));

	font = al_create_builtin_font();
}

//Was not used in this project
void Game::reset() {
}

//Run the game state machine
void Game::run() {
	//Load screens
	GameScreen game_screen(sprites, samples);
	StartScreen start_screen(sprites);
	ResultScreen result_screen(sprites, 0, 0, Easy);
	int difficulty = 1; //Difficulty to run
	bool EXTREME_ON = false; //Flag for EXTREME easter egg
	while (state != Exit) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		switch (state) {
		case Start:
			start_screen.run(font);
			if (start_screen.next_state == EXTREME) {
				EXTREME_ON = true;
				state = Hard;
			}
			else {
				state = start_screen.next_state;
			}
			break;
		//The same set of code will be run for any difficulty state
		case Easy:
		case Medium:
		case Hard:
			//But we need to check once here whether the difficulty value needs to be adjusted
			switch (state) {
			case Medium:
				difficulty = 3;
				result_screen.prev_state = Medium; 
				break;
			case Hard:
				difficulty = 5;
				result_screen.prev_state = Hard;
				break;
			}
			if (EXTREME_ON) {
				game_screen.reset(5, 5, difficulty); //If the EXTREME easter egg is enabled, the game will be run with 5 lines on hard mode
			}
			else {
				game_screen.reset(3, 5, difficulty);
			}
			EXTREME_ON = false;
			game_screen.run(font);
			state = game_screen.next_state;
			break;
		case End:
			//Send proper finishing values for this game to the result screen
			result_screen.score = game_screen.score;
			result_screen.difficulty = game_screen.difficulty;
			result_screen.run(font);
			state = result_screen.next_state;
			break;
		}
	}

	//Garbage collection
	map<string, ALLEGRO_BITMAP*>::iterator it;
	for (it = sprites.begin(); it != sprites.end(); it++) {
		al_destroy_bitmap(it->second);
	}
	map<string, ALLEGRO_SAMPLE*>::iterator it2;
	for (it2 = samples.begin(); it2 != samples.end(); it2++) {
		al_destroy_sample(it2->second);
	}
}
