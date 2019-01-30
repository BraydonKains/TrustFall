#pragma once

#define SCREEN_W 640
#define SCREEN_H 480

//Collection of states run by state machine
enum State {
	Start,
	Easy,
	Medium,
	Hard,
	EXTREME,
	Chaos,
	Help,
	End,
	Exit
};