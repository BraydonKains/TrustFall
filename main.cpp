#include <stdio.h>
#include <string>

#include "game.h"

using std::string;

int main()
{
	//Allegro setup
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	if (!al_init_font_addon()) {
		fprintf(stderr, "failed to initialize font!\n");
		return -1;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives!\n");
		return -1;
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		return -1;
	}
	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}
	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize keyboard!\n");
		return -1;
	}

	//Create display
	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	//Create game, initialize and run it
	Game main_game;
	main_game.init();
	main_game.run();

	//Garbage collection
	al_uninstall_audio();
	al_uninstall_keyboard();
	al_destroy_display(display);
	
	return 0;
}