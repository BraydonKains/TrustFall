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

//Move the line forward
void Line::move() {
	//If a catch was previously made, move catchers forward and reset catch state
	if (caught) {
		caught = false;
		catchers[0] = catchers[1];
		catchers[1] = catchers[2];
		catchers[2] = 0;
	}
	
	//move employees forward
	for (int i = 0; i < 5; i++) {
		employees[i] = employees[i + 1];
	}
	//check if an employee is set to be added to this line
	employees[5] = (queued) ? 1 : 0;
	queued = false;
	
	//check if a catch was made
	if (employees[0]) {
		if (catchers[0]) {
			caught = true;
		}
		else {
			fall = true;
		}
	}
}

//Add employee to this line
void Line::add_employee() {
	queued = true;
}

//Add a catched to this line
void Line::add_catcher() {
	for (int i = 0; i < 3; i++) {
		if (!catchers[i]) {
			catchers[i] = 1;
			break;
		}
	}
}

//Draw this line
void Line::draw() {
	al_draw_bitmap(sprites["Conveyor"], start_x, start_y, NULL); //Sprite
	
	for (int i = 1; i < 6; i++) {
		if (employees[i]) { //If the array says there is a catcher at the current position
			al_draw_bitmap(sprites["Employee"], start_x + (40 * (i-1)), start_y, NULL);
		}
	}

	for (int i = 2; i > -1; i--) {
		if (i == 0) { //If this is the position closes to the line
			if (caught) { //If there was a catch
				al_draw_bitmap(sprites["EmployeeHappy"], start_x - 40, start_y, NULL);
			}
			else if (fall) { //If there was not a catch
				al_draw_bitmap(sprites["EmployeeSad"], start_x - 40, start_y + 55, ALLEGRO_FLIP_VERTICAL);
			}
		}
		if (catchers[i]) { //If there is a catcher at this position
			al_draw_bitmap(sprites["Mr. Man"], start_x - 40 - (50*(i)), start_y + 40, NULL);
		}
	}
}