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


//enumerators

enum class BACKGROUND {
	INTRO1,
	INTRO2,
	INTRO3,
	START,
	MENU,
	SETTING,
	SCORE,
	CHOOSE_MUSIC,
	GAME,
	EXIT,
	TOTAL,
};

enum class COLORSTRIP {
	ORANGE,
	GREEN,
	PURPLE,
	TOTAL,
};

enum class BUTTON {
	CHANGE_SONG_LEFT,
	CHANGE_SONG_RIGHT,
	PLAY,
	CONTINUE,
	SAVE,
	SETTING,
	SCORE,
	EXIT,
	PRESS_KEY,
	TOTAL,
	NOT_CHOOSE = -1,
};

enum class TITLE {
	INTRO,
	START,
	MENU,
	SETTING,
	SCORE,
	CHOOSE_MUSIC,
	GAME,
	FINISH,
	TOTAL,
	NO_TITLE,
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
	NOT_CHOOSE = -1,
};

enum class POINT {
	NAME,
	POINT,
	TOTAL_ARROW,
	ACCURACY,
	PRESSED_ARROW,
	MAX_COMBO,
	WRONG_PRESSED,
	TOTAL,
};

enum class SETTING {
	RESOLUTION,
	ARROW_COLOR,
	CONTROL,
};


template <class T> int toInt(T enumerate) {
	return static_cast<int> (enumerate);
}

#endif
