#pragma once
#include "line.h"
#include "screen.h"

//Main screen for gameplay
class GameScreen : public Screen {
public:
	std::map<std::string, ALLEGRO_BITMAP*> sprites;
	std::map<std::string, ALLEGRO_SAMPLE*> samples;
	std::vector<Line> lines;
	unsigned int selected;
	int max_catchers;
	int catchers;
	int score;
	int difficulty;
	bool music;

	GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, std::map<std::string, ALLEGRO_SAMPLE*> _samples);

	void reset(int _lines, int _max_catchers, int _difficulty);
	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};