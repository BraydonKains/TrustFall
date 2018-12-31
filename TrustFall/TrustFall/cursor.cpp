#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>

#include "cursor.h"

using std::string;
using std::vector;

template<class T>
Cursor<T>::Cursor()
{
	active = false;
}

template<class T>
void Cursor<T>::activate(std::vector<T> _items)
{
	items = _items;
	selected = 0;
	active = true;
}

template<class T>
void Cursor<T>::deactivate()
{
	active = false;
}

template<class T>
void Cursor<T>::draw(float _x_start, float _y_start, float _y_offset, ALLEGRO_FONT* _font) {
	vector<string> items_text = get_item_strings();

	x_start = _x_start;
	y_start = _y_start;
	y_offset = _y_offset;
	font = _font;

	for (int i = 0; i < items_text.size(); i++) {
		al_draw_text(font, al_map_rgb(255, 255, 255), x_start, y_start + (y_offset*i), ALLEGRO_ALIGN_LEFT, items_text.at(i).c_str());
	}

	update_selector();
}

template<class T>
void Cursor<T>::down()
{
	if (selected < items.size() - 1) {
		selected++;
		redraw();
	}
}

template<class T>
void Cursor<T>::up()
{
	if (selected > 0) {
		selected--;
		redraw();
	}
}

template<class T>
T Cursor<T>::get_selected() {
	return items.at(selected);
}

template<class T>
void Cursor<T>::redraw()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	draw(x_start, y_start, y_offset, font);
}

template<class T>
void Cursor<T>::update_selector() {
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	float offset = y_offset * selected;
	float x1 = x_start - 5;
	float y1 = y_start + offset;
	float x2 = x_start - 5;
	float y2 = y_start + 5 + offset;
	float x3 = x_start - 1;
	float y3 = y_start + 2.5 + offset;

	al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, white);
}

template Cursor<string>::Cursor();
template void Cursor<string>::activate(std::vector<string> _options);
template void Cursor<string>::deactivate();
template void Cursor<string>::draw(float x_start, float y_start, float _y_offset, ALLEGRO_FONT* font);
template void Cursor<string>::redraw();
template void Cursor<string>::up();
template void Cursor<string>::down();
template string Cursor<string>::get_selected();
template void Cursor<string>::update_selector();
vector<string> Cursor<string>::get_item_strings() {
	return items;
}
