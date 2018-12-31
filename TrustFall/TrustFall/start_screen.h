#pragma once
#include "screen.h"
#include "cursor.h"

class StartScreen : public Screen {
public:
	Cursor<std::string> menu;
	std::map<std::string, ALLEGRO_BITMAP*> sprites;
	
	StartScreen(std::map<std::string, ALLEGRO_BITMAP*> _sprites);

	void run(ALLEGRO_FONT* font);
	void redraw(ALLEGRO_FONT* font);
	void back();
	void cont();
};