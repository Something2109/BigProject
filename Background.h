#ifndef _Structs_h
#define _Structs_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "ErrorAndEnum.h"
#include "Utility.h"
using namespace std;

//this file contain all the texture struct related to all the game frame type

//menu texture struct

class Background : private Screen {
	//background and the related parameters
	LTexture background;
	SDL_Rect* bgSourceRect = NULL;
	SDL_Rect* bgDestinationRect = NULL;

	//color strips and the related parameters
	LTexture colorStrip[static_cast<int>(COLORSTRIP::TOTAL)];
	SDL_Rect* colorStripDstRect = NULL;
	int yColorStripVelo = 0;

public:

	Background(Screen& screen);

	//basic background functions

	bool load(const string& path);

	void createRect();

	void render(BACKGROUND type);

	void free();
	
	//background moving function

	void moveColorStrip();

	void moveColorStrip(SDL_Rect& movingRect);

	void moveBackground(BACKGROUND origin, BACKGROUND moving);

	void moveComplete(SDL_Rect& movingRect, SDL_Rect& original, SDL_Rect& move);

	//color strip render function

	void renderColorStrip(SDL_Rect &rectangle);

	void renderColorStrip(BACKGROUND type);

	void renderBackground(BACKGROUND type);

};

#endif
