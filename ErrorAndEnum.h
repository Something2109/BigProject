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

//enumerators

enum class BACKGROUND {
	MENU = 0,
	CHOOSE_MUSIC = 1,
	GAME = 2,
	TOTAL = 3,
};

enum class COLORSTRIP {
	ORANGE = 0,
	GREEN = 1,
	PURPLE = 2,
	TOTAL = 3,
};

enum class CONTROL {
	LEFT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
	CHOOSE,
	ESCAPE,
	TOTAL,
	TOTAL_ARROW = 4,
};

enum class MENU {
	NOT_CHOOSE = -1,
	PLAY = 0,
	SETTING = 1,
	EXIT = 2,
	TOTAL,
};

enum class DOGE {
	SOURCE_LEFT,
	SOURCE_IDLE,
	SOURCE_RIGHT,
	DESTINATION,
	TOTAL,
};

enum class POINT {
	POINT,
	TOTAL_ARROW,
	ACCURACY,
	PRESSED_ARROW,
	WRONG_PRESSED,
	TOTAL,
};

template <class T> int toInt(T enumerate) {
	return static_cast<int> (enumerate);
}

#endif
