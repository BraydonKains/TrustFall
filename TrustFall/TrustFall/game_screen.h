#pragma once
#include "screen.h"
#include "cursor.h"

class GameScreen : public Screen {
public:
	std::map<std::string, ALLEGRO_BITMAP*> sprites;

	GameScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites);

	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};