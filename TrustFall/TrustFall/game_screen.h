#pragma once
#include "line.h"
#include "screen.h"

class GameScreen : public Screen {
public:
	std::map<std::string, ALLEGRO_BITMAP*> sprites;
	std::vector<Line> lines;
	int selected;
	int max_catchers;
	int catchers;
	int score;
	int difficulty;

	GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites);

	void reset(int _lines, int _max_catchers, int _difficulty);
	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};