#pragma once
#include "cursor.h"
#include "screen.h"

//Screen to display results upon losing
class ResultScreen : public Screen {
public:
	int score;
	int difficulty;
	Cursor<std::string> menu;
	std::map<std::string, ALLEGRO_BITMAP*> sprites;
	State prev_state;

	ResultScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, int _score, int _difficulty, State _prev_state);

	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};