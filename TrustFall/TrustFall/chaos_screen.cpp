#define HAVE_STRUCT_TIMESPEC
#include <pthread.h> //#%#@ VC++ with their bulls*** linker stuff I'm never using Visual Studio for this language again
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>
#include <map>
#include <ctime>

#include "chaos_screen.h"

#define FPS 60

using std::vector;
using std::string;
using std::ostringstream;
using std::map;
using std::pair;

pthread_mutex_t go_line_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t game_over_mutex = PTHREAD_MUTEX_INITIALIZER;
int go_line[3] = { false, false, false };
bool game_over = false;
int ticks = 0;

void* run_line(void *line_arg) {
	int* line = (int*)line_arg;
	bool exit_thread = false;
	while (!exit_thread) {
		//Figure out how many seconds to wait this time
		int tenth_secs_wait = rand() % 15 + 1;
		Sleep(tenth_secs_wait * 100);
		//Check for gameover
		while (!pthread_mutex_trylock(&game_over_mutex)) {
		}
		pthread_mutex_lock(&game_over_mutex);
		if (game_over) {
			exit_thread = true;
		}
		else {
			while (!pthread_mutex_trylock(&go_line_mutex)) {
			}
			pthread_mutex_lock(&go_line_mutex);
			go_line[*line] = true;
			pthread_mutex_unlock(&go_line_mutex);
		}
		pthread_mutex_unlock(&game_over_mutex);
	}

	return 0;
}


ChaosScreen::ChaosScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, std::map<std::string, ALLEGRO_SAMPLE*> _samples) {
	sprites = _sprites;
	samples = _samples;
	srand(time(NULL)); //Seed random number generator with current time so sequence is unique each run
}

//Resets game to default values, and uses new given values
void ChaosScreen::reset(int _lines, int _max_catchers, int _difficulty) {
	lines.clear();
	for (int i = 0; i < _lines; i++) {
		Line new_line(SCREEN_W / 2, SCREEN_H / _lines * i, sprites);
		lines.push_back(new_line);
	}

	selected = 0;
	max_catchers = _max_catchers;
	catchers = 0;
	score = 0;
	difficulty = _difficulty;
	music = true;
}

//If a sample needs to be played while it is still being played, it will be stopped first.
//Admittedly this was a hack solution to the fact that my sound effect samples are too long. 
//In future projects I will use a better program for sound effect creation that does not have a large minimum length for audio export.
void play(ALLEGRO_SAMPLE_INSTANCE* x) {
	if (al_get_sample_instance_playing(x)) {
		al_stop_sample_instance(x);
	}
	al_play_sample_instance(x);
}

void ChaosScreen::run(ALLEGRO_FONT* font) {
	//Map that represents difficulty scale
	map<int, double> difficulty_map;
	difficulty_map.insert(pair<int, int>(1, 15));
	difficulty_map.insert(pair<int, int>(2, 12));
	difficulty_map.insert(pair<int, int>(3, 10));
	difficulty_map.insert(pair<int, int>(4, 8));
	difficulty_map.insert(pair<int, int>(5, 5));
	difficulty_map.insert(pair<int, int>(6, 3));
	difficulty_map.insert(pair<int, int>(7, 2));

	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(0.1); //Universally a "tick" in the context of the game is 1/10 of a second.
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	redraw(font);
	al_flip_display();

	//Create map of sample instances. This is more memory efficient than play samples over and over again individually, 
	//and allows the anti-overlap functionality implemented above.
	map<string, ALLEGRO_SAMPLE_INSTANCE*> instances;
	map<string, ALLEGRO_SAMPLE*>::iterator it;
	for (it = samples.begin(); it != samples.end(); it++) {
		instances.insert(pair<string, ALLEGRO_SAMPLE_INSTANCE*>(it->first, al_create_sample_instance(it->second)));
		al_attach_sample_instance_to_mixer(instances[it->first], al_get_default_mixer());
	}

	al_set_sample_instance_playmode(instances["Theme"], ALLEGRO_PLAYMODE_LOOP);
	play(instances["Theme"]);

	pthread_t line_ts[3];
	for (int i = 0; i < 3; i++) {
		pthread_create(&line_ts[i], NULL, run_line, (void*)i);
	}

	bool exit_screen = false;
	bool ctrl = false; //whether ctrl key is currently held
	bool help = false; //whether help screen is currently displayed

	al_start_timer(timer);
	while (!exit_screen) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			for (unsigned int i = 0; i < 3; i++) {
				if (go_line[i]) {
					int add_to = rand() % (lines.size() + 1); //Randomly decide which line to add to, with a 1/lines.size() chance of not adding
					if (i == add_to) {
						lines.at(i).add_employee();
					}
					lines.at(i).move(); //Every line will move regardless of any factors
					if (lines.at(i).fall) { //If an employee has fallen, the game is over
						map<string, ALLEGRO_SAMPLE_INSTANCE*>::iterator in_it;
						for (in_it = instances.begin(); in_it != instances.end(); in_it++) {
							al_stop_sample_instance(in_it->second);
						}
						al_play_sample_instance(instances["Fall"]);
						exit_screen = true;
						game_over = true;
						al_clear_to_color(al_map_rgb(0, 0, 0));
						redraw(font);
						al_flip_display();
						al_rest(2.0);
					}
					else if (lines.at(i).caught) { //If an employee is caught, add to the score and then check if difficulty should be adjusted
						play(instances["Catch"]);
						catchers--;
						score++;
						if ((score == 10 || score == 20 || score == 30 || score == 40 || score == 50 || score == 75 || score == 100) && difficulty < 7) {
							play(instances["LevelUp"]);
							difficulty++;
						}
					}

					go_line[i] = false;
				}
			}
				
		}
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_DOWN:
				play(instances["Move"]);
				if (selected < lines.size() - 1) selected++;
				break;
			case ALLEGRO_KEY_UP:
				play(instances["Move"]);
				if (selected > 0) selected--;
				break;
			case ALLEGRO_KEY_SPACE:
				if (catchers < max_catchers) {
					if (!lines.at(selected).catchers[2]) {
						play(instances["Place"]);
						lines.at(selected).add_catcher();
						catchers++;
					}
				}
				break;
			case ALLEGRO_KEY_M:
				//Toggle music
				if (ctrl) {
					if (music) {
						al_stop_sample_instance(instances["Theme"]);
					}
					else {
						al_play_sample_instance(instances["Theme"]);
					}
					music = !music;
				}
				break;
			case ALLEGRO_KEY_H:
				if (ctrl) {
					help = true; //help screen is up

					//Draw the help screen
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 20, ALLEGRO_ALIGN_CENTER, "Place catchers at the ends of the conveyors");
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 30, ALLEGRO_ALIGN_CENTER, "You can place 3 catchers per conveyor");
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 40, ALLEGRO_ALIGN_CENTER, "Make sure to keep track of how many catchers you have left");

					al_draw_bitmap(sprites["KeyUp"], SCREEN_W / 8, SCREEN_H / 2 + 60, NULL);
					al_draw_bitmap(sprites["KeyUp"], SCREEN_W / 8 + 34, SCREEN_H / 2 + 60, ALLEGRO_FLIP_VERTICAL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 68, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "Up and down - Move cursor");

					al_draw_bitmap(sprites["Spacebar"], SCREEN_W / 8 + 20, SCREEN_H / 2 + 100, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 68, SCREEN_H / 1.3 - 20, ALLEGRO_ALIGN_LEFT, "Spacebar - ");
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 150, SCREEN_H / 1.3 - 15, ALLEGRO_ALIGN_LEFT, "Place Employee");
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 150, SCREEN_H / 1.3 - 25, ALLEGRO_ALIGN_LEFT, "Select Menu Item");

					al_draw_bitmap(sprites["KeyEsc"], SCREEN_W / 2 + 58, SCREEN_H / 2 + 60, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 113, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "Close menu");

					al_draw_bitmap(sprites["KeyCtrl"], SCREEN_W / 2 + 40, SCREEN_H / 2 + 100, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 70, SCREEN_H / 2 + 110, ALLEGRO_ALIGN_LEFT, "+");
					al_draw_bitmap(sprites["KeyM"], SCREEN_W / 2 + 78, SCREEN_H / 2 + 100, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 113, SCREEN_H / 1.3 - 20, ALLEGRO_ALIGN_LEFT, "Toggle Music");

					al_flip_display();

					while (help) { //While the help screen is still down
						ALLEGRO_EVENT unhelp;
						al_wait_for_event(event_queue, &unhelp);

						if (unhelp.type == ALLEGRO_EVENT_KEY_DOWN) {
							if (unhelp.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
								help = false; //Take the help screen down
							}
						}
					}
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				//Force quit game
				back();
				exit_screen = true;
				break;
			case ALLEGRO_KEY_LCTRL:
			case ALLEGRO_KEY_RCTRL:
				ctrl = true; //ctrl is held
				break;
			}
			if (ev.type == ALLEGRO_EVENT_KEY_UP) {
				if (ev.keyboard.keycode == ALLEGRO_KEY_LCTRL || ev.keyboard.keycode == ALLEGRO_KEY_RCTRL) {
					ctrl = false; //ctrl is released
				}
			}
			//Global refresh
			al_clear_to_color(al_map_rgb(0, 0, 0));
			redraw(font);
			al_flip_display();
		}
	}
	if (next_state != Exit) { //If the game loop was exited naturally rather than forced by Esc
		cont();
	}

	//Garbage collection
	for (int i = 0; i < 3; i++) {
		pthread_join(line_ts[i], NULL);
	}
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	map<string, ALLEGRO_SAMPLE_INSTANCE*>::iterator it2;
	for (it2 = instances.begin(); it2 != instances.end(); it2++) {
		al_destroy_sample_instance(it2->second);
	}
}

//Redraw all elements of the screen
void ChaosScreen::redraw(ALLEGRO_FONT* font) {
	al_draw_bitmap(sprites["Mr. Manager"], 80, 0 + (SCREEN_H / lines.size() * selected), NULL); //Draw the manager based on which line is selected

	for (unsigned int i = 0; i < lines.size(); i++) { //Draw each line
		lines.at(i).draw();
	}

	//Display score, number of catchers, current difficulty level, and whether the music is on or off
	ostringstream score_msg;
	score_msg << "Score:  " << score;
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W - 90, 0, ALLEGRO_ALIGN_LEFT, score_msg.str().c_str());

	al_draw_bitmap(sprites["Mr. Man"], SCREEN_W - 90, 20, NULL);
	ostringstream catcher_msg;
	catcher_msg << " x " << max_catchers - catchers;
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W - 50, 30, ALLEGRO_ALIGN_LEFT, catcher_msg.str().c_str());

	ostringstream difficulty_msg;
	difficulty_msg << "Level:  " << difficulty;
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W - 90, 75, ALLEGRO_ALIGN_LEFT, difficulty_msg.str().c_str());

	string music_img = (music) ? "MusicOn" : "MusicOff";
	al_draw_bitmap(sprites[music_img], SCREEN_W - 60, 90, NULL);
}

void ChaosScreen::back() {
	next_state = Exit;
}

void ChaosScreen::cont() {
	next_state = End;
}