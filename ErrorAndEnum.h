#ifndef _ErrorAndEnum_h
#define _ErrorAndEnum_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
using namespace std;

//error loading function
void loadErrorLog();

//enumerations

enum backgroundNumber {
	MENU = 0,
	CHOOSE_MUSIC = 1,
	GAME = 2,
	TOTAL_BACKGROUND = 3,
};

enum colorStripNumber {
	ORANGE = 0,
	GREEN = 1,
	PURPLE = 2,
	TOTAL_COLOR_STRIP = 3,
};

enum arrowKeyPress {
	LEFT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
	TOTAL_ARROW,
	CHOOSE_BUTTON,
	ESCAPE,
};

enum menuChoose {
	NOTCHOOSE = -1,
	PLAY = 0,
	SETTING = 1,
	EXIT = 2,
	TOTAL_BUTTON,
};

enum muscleDoge {
	SOURCE_LEFT,
	SOURCE_IDLE,
	SOURCE_RIGHT,
	DESTINATION,
	TOTAL_DOGE_RECT
};

extern menuChoose;
extern backgroundNumber;
extern colorStripNumber;
extern arrowKeyPress;
extern muscleDoge;

#endif
