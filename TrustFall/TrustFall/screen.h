#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <string>
#include <map>

#include "enums.h"

class Screen {
public:
	State next_state;
	std::map<std::string, ALLEGRO_BITMAP*> sprites;

	virtual void run(ALLEGRO_FONT* font) = 0;
	virtual void redraw(ALLEGRO_FONT* font) = 0;
	virtual void back() = 0;
	virtual void cont() = 0;
};