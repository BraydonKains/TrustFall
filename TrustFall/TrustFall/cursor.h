#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <vector>

//Default object used for menues
template <class T>
class Cursor {
public:
	std::vector<T> items;
	unsigned int selected;
	bool active;

	Cursor();

	void activate(std::vector<T> _items);
	void deactivate();
	void draw(float _x_start, float _y_start, float _y_offset, ALLEGRO_FONT* _font);
	void up();
	void down();
	T get_selected();

private:
	float x_start;
	float y_start;
	float y_offset;
	ALLEGRO_FONT* font;

	std::vector<std::string> get_item_strings();
	void update_selector();
	void redraw();
};