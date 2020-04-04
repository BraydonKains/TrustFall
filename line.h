#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <map>
#include <string>

//Represents a conveyor belt line, contains data for the employees and catchers attached as well as some data about its current state
class Line {
public:
	float start_x;
	float start_y;

	std::map<std::string, ALLEGRO_BITMAP*> sprites;

	int catchers[3];
	int employees[6];

	bool queued;
	bool fall;
	bool caught;

	Line(int _start_x, int _start_y, std::map<std::string, ALLEGRO_BITMAP*> _sprites);

	void move();
	void draw();
	void add_employee();
	void add_catcher();
};