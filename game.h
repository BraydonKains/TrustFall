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

//Game state machine, contains data global to game
class Game {
public:
	//Allegro elements
	ALLEGRO_FONT* font;

	//Game elements
	State state;

	int score;
	
	std::map<std::string, ALLEGRO_BITMAP*> sprites;
	std::map<std::string, ALLEGRO_SAMPLE*> samples;

	Game();

	void init();
	void run();

private:
	void reset();
};
