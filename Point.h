#ifndef _Point_h_
#define _Point_h_
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Utility.h"
using namespace std;

class Point : private Screen {
	LTexture pointText,
		totalArrowText,
		accuracyText,   
		pressedArrowText,
		wrongPressedText;

	string playerName;
	unsigned int point = 0,
		totalArrow = 0,
		accuracy = 0,
		pressedArrow = 0,
		wrongPressed = 0;

	SDL_Rect* pointRect = NULL;
	TTF_Font* pointFont = NULL;
	SDL_Color pointColor;

public:

	//constructor

	Point(Screen& screen);

	//basic function

	bool loadGameFont(string& path);

	bool loadPointScreen(string& path);

	void renderGamePoint();

	void renderPointScreen();

	void free();

	// point count

	void addPoint(int& accuracy);

	void arrowCount();

	void wrongPressCount();

	//compare score

	bool compare(Point& point);

	//render functions

	void createPointRect();

};

#endif