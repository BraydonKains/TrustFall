#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "game_screen.h"

#define FPS 60

using std::vector;
using std::string;
using std::ostringstream;
using std::map;
using std::pair;

GameScreen::GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites) {
	sprites = _sprites;
}

void GameScreen::reset(int _lines, int _max_catchers, int _difficulty) {
	if (!lines.empty()) {
		lines.erase(lines.begin(), lines.begin() + _lines);
	}
	for (int i = 0; i < _lines; i++) {
		Line new_line(SCREEN_W / 2, SCREEN_H / _lines * i, sprites);
		lines.push_back(new_line);
	}

	selected = 0;
	max_catchers = _max_catchers;
	catchers = 0;
	score = 0;
	difficulty = _difficulty;
}

void GameScreen::run(ALLEGRO_FONT* font) {
	bool music = true;
	bool help = false;

	map<int, double> difficulty_map;
	difficulty_map.insert(pair<int, int>(1, 15));
	difficulty_map.insert(pair<int, int>(2, 12));
	difficulty_map.insert(pair<int, int>(3, 10));
	difficulty_map.insert(pair<int, int>(4, 8));
	difficulty_map.insert(pair<int, int>(5, 5));
	
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(0.1);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
	redraw(font);
	al_flip_display();

	int ticks = 0;
	bool exit_screen = false;
	bool ctrl = false;
	al_start_timer(timer);
	while (!exit_screen) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (ticks == difficulty_map[difficulty]) {
				int add_to = rand() % (lines.size() + 1);
				for (unsigned int i = 0; i < lines.size(); i++) {
					if (i == add_to) {
						lines.at(i).add_employee();
					}
					lines.at(i).move();
					if (lines.at(i).fall) {
						exit_screen = true;
					}
					else if (lines.at(i).caught) {
						catchers--;
						score++;
						if ((score == 10 || score == 20 || score == 30 || score == 40 || score == 50) && difficulty < 5) {
							difficulty++;
						}
					}
				}
				al_clear_to_color(al_map_rgb(0, 0, 0));
				redraw(font);
				al_flip_display();
				ticks = 0;
			}
			else {
				ticks++;
			}
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
				if (catchers < max_catchers) {
					lines.at(selected).add_catcher();
					catchers++;
				}
				break;
			case ALLEGRO_KEY_M:
				if (ev.keyboard.modifiers == ALLEGRO_KEYMOD_CTRL) {
					music = !music;
				}
				break;
			case ALLEGRO_KEY_H:
				if (ctrl) {
					help = true;

					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 20, ALLEGRO_ALIGN_CENTER, "Place catchers at the ends of the conveyors");
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 30, ALLEGRO_ALIGN_CENTER, "You can place 3 catchers per conveyor");
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 40, ALLEGRO_ALIGN_CENTER, "Make sure to keep track of how many catchers you have left");
					
					al_draw_bitmap(sprites["KeyUp"], SCREEN_W / 8, SCREEN_H / 2 + 60, NULL);
					al_draw_bitmap(sprites["KeyUp"], SCREEN_W / 8 + 34, SCREEN_H / 2 + 60, ALLEGRO_FLIP_VERTICAL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 68, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "Up and down - Move cursor");

					al_draw_bitmap(sprites["Spacebar"], SCREEN_W / 8 + 20, SCREEN_H / 2 + 100, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 8 + 68, SCREEN_H / 1.3 - 20, ALLEGRO_ALIGN_LEFT, "Spacebar - Place employee");
										
					al_draw_bitmap(sprites["KeyEsc"], SCREEN_W / 2 + 58, SCREEN_H / 2 + 60, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 113, SCREEN_H / 1.5 - 10, ALLEGRO_ALIGN_LEFT, "Close menu");

					al_draw_bitmap(sprites["KeyCtrl"], SCREEN_W / 2 + 40, SCREEN_H / 2 + 100, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 70, SCREEN_H / 2 + 110, ALLEGRO_ALIGN_LEFT, "+");
					al_draw_bitmap(sprites["KeyM"], SCREEN_W / 2 + 78, SCREEN_H / 2 + 100, NULL);
					al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 113, SCREEN_H / 1.3 - 20, ALLEGRO_ALIGN_LEFT, "Toggle Music");

					al_flip_display();

					while (help) {
						ALLEGRO_EVENT unhelp;
						al_wait_for_event(event_queue, &unhelp);

						if (unhelp.type == ALLEGRO_EVENT_KEY_DOWN) {
							if (unhelp.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
								help = false;
							}
						}
					}
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				back();
				exit_screen = true;
				break;
			case ALLEGRO_KEY_LCTRL:
			case ALLEGRO_KEY_RCTRL:
				ctrl = true;
				break;
			}
			if (ev.type == ALLEGRO_EVENT_KEY_UP) {
				if (ev.keyboard.keycode == ALLEGRO_KEY_LCTRL || ev.keyboard.keycode == ALLEGRO_KEY_RCTRL) {
					ctrl = false;
				}
			}
			al_clear_to_color(al_map_rgb(0, 0, 0));
			redraw(font);
			al_flip_display();
		}
	}
	if (next_state != Exit) {
		cont();
	}
	al_rest(2.0);
}

void GameScreen::redraw(ALLEGRO_FONT* font) {
	al_draw_bitmap(sprites["Mr. Manager"], 80, 0 + (SCREEN_H / lines.size() * selected), NULL);

	for (unsigned int i = 0; i < lines.size(); i++) {
		lines.at(i).draw();
	}

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
}

void GameScreen::back() {
	next_state = Exit;
}

void GameScreen::cont() {
	next_state = End;
}