#pragma once
#include "cursor.h"
#include "screen.h"

class ResultScreen : public Screen {
public:
	int score;
	int difficulty;
	Cursor<std::string> menu;
	std::map<std::string, ALLEGRO_BITMAP*> sprites;

	ResultScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, int _score, int _difficulty);

	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};