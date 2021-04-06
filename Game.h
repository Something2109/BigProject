#ifndef _Game_h_
#define _Game_h_
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Background.h"
using namespace std;

//this file contains the game running related functions

class ArrowTexture: private Screen{
	LTexture arrow,
		blankArrow,
		pressedArrow;

	vector <Coordinate> arrowRange;
	Uint32 startTime = 0;
	Uint32 gameTime = 0;
	Uint32 endTime = 0;
	unsigned int arrowCount = 0;
	int point = 0;

	SDL_Rect* blankArrowDstRect = NULL;
	SDL_Rect* arrowSrcRect = NULL;
	SDL_Rect* pressedArrowDstRect = NULL;

public:

	void createArrowRect();

	//load all the texture from source
	bool loadTexture(Screen& background, const string& path);

	void setTimeline(Music& song);

	void addNewArrow(Music &song);

	bool continueGame(Event& event);

	void scoreCheck(int& keyType);

	void updateArrowRange(const int& velocity);

	void renderPressedArrow(Event& event);

	void renderArrowRange();

	//free all the texture
	void freeArrowTexture();

};

class DogeTexture : private Screen {
	LTexture muscleDoge,
		cheems,
		smashedCheems;
	Mix_Chunk* bonk = NULL;

	SDL_Rect* muscleDogeRect = NULL;
	SDL_Rect* cheemsSrcRect = NULL;
	SDL_Rect* cheemsDstRect = NULL;

public:
	void createDogeRect();

	bool loadTexture(Screen& background, const string& path);
	
	void playBonkSound();

	void renderDogeScreen(Event& event);

	//free all the texture
	void freeDogeTexture();
};

//main game function
void game(Screen& screen, Background& background, Event& event, Music& song);

//loading resources
bool loadingGameResource(Screen& screen, ArrowTexture& texture, DogeTexture& dogeTexture, Music& song);

void freeGamesTexture(ArrowTexture& texture, DogeTexture& dogeTexture, Music& song);

#endif