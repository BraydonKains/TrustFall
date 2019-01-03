#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <vector>
#include <string>
#include <map>

#include "enums.h"

class Game {
public:
	//Allegro elements
	ALLEGRO_FONT* font;

	//Game elements
	State state;

	int score;
	
	std::map<std::string, ALLEGRO_BITMAP*> sprites;

	Game();

	void init();
	void run();

private:
	void reset();
};