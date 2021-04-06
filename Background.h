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
#include "Texture.h"
using namespace std;

//this file contain all the texture struct related to all the game frame type

//menu texture struct

class Background : private Screen {
	//background and the related parameters
	LTexture background;
	SDL_Rect* bgSourceRect = NULL;
	SDL_Rect* bgDestinationRect = NULL;

	//color strips and the related parameters
	LTexture colorStrip[TOTAL_COLOR_STRIP];
	SDL_Rect* colorStripDstRect = NULL;
	int yColorStripVelo = 0;

public:
	
	//create background and its parameters
	void createBackgroundRect();

	//load background
	bool loadBackground(Screen& screen, const string& path);
	
	//color strip moving function
	void moveColorStrip();

	void moveColorStrip(SDL_Rect& movingRect);

	void moveComplete(SDL_Rect& movingRect, SDL_Rect& original, SDL_Rect& move);

	//background picture moving function
	void moveBackground(int original, int move);

	//color strip render function
	void renderColorStrip(SDL_Rect &rectangle);

	void renderColorStrip(int backgroundType);

	//background render function
	void renderBackground(int backgroundType);

	void freeBackground();

};


//game texture struct


#endif
