#pragma once
#include "line.h"
#include "screen.h"
#include "cursor.h"

class GameScreen : public Screen {
public:
	std::map<std::string, ALLEGRO_BITMAP*> sprites;
	std::vector<Line> lines;
	int selected;

	GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites, int _lines);

	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};