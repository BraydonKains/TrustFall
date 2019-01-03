#include "line.h"

using std::map;
using std::string;

Line::Line(int _start_x, int _start_y, map<string, ALLEGRO_BITMAP*> _sprites) {
	start_x = _start_x;
	start_y = _start_y;
	
	sprites = _sprites;

	queued = false;
	fall = false;

	for (int i = 0; i < 3; i++) {
		catchers[i] = 0;
	}
	for (int i = 0; i < 5; i++) {
		employees[i] = 0;
	}
}

void Line::move() {
	if (employees[0]) {
		if (catchers[0]) {

			caught = true;
			catchers[0] = catchers[1];
			catchers[1] = catchers[0];
			catchers[2] = 0;
		}
		else {
			fall = true;
		}
	}
	for (int i = 0; i < 4; i++) {
		employees[i] = employees[i + 1];
	}
	employees[4] = (queued) ? 1 : 0;
	queued = false;
}

void Line::add_employee() {
	queued = true;
}

void Line::add_catcher() {
	for (int i = 2; i > -1; i--) {
		if (!catchers[i]) {
			catchers[i] = 1;
			break;
		}
	}
}

void Line::draw() {
	al_draw_bitmap(sprites["Conveyor"], start_x, start_y, NULL);
	
	for (int i = 0; i < 5; i++) {
		if (employees[i]) {
			al_draw_bitmap(sprites["Employee"], start_x + (40 * i), start_y, NULL);
		}
	}

	for (int i = 2; i > -1; i--) {
		if (i = 0) {
			if (caught) {
				al_draw_bitmap(sprites["EmployeeHappy"], start_x - 40, start_y, NULL);
				caught = false;
			}
			else if (fall) {
					al_draw_bitmap(sprites["EmployeeSad"], start_x - 40, start_y - 55, ALLEGRO_FLIP_VERTICAL);
			}
			else {
				if (catchers[i]) {
					al_draw_bitmap(sprites["Mr. Man"], start_x - 40, start_y - 40, NULL);
				}
			}
		}
		else {
			if (catchers[i]) {
				al_draw_bitmap(sprites["Mr. Man"], start_x - 40 - (50*(i)), start_y + 40, NULL);
			}
		}
	}
}