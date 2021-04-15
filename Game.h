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
#include "Utility.h"
#include "Background.h"
#include "Point.h"
using namespace std;

//this file contains the game running related functions

class ArrowTexture: private Screen{
	LTexture arrow,
		blankArrow,
		pressedArrow;

	vector <SDL_Point> arrowRange;
	Uint32 startGameTime = 0;
	Uint32 gameTime = 0;
	Uint32 spawnDuration = 0;
	unsigned int arrowCount = 0;

	SDL_Rect* blankArrowDstRect = NULL;
	SDL_Rect* arrowSrcRect = NULL;
	SDL_Rect* pressedArrowDstRect = NULL;

public:

	//constructor

	ArrowTexture(Screen& screen);

	//basic functions

	bool load(const string& path);

	void render(Event& event, Point& point);

	void free();

	//Arrow spawning function
	
	void setTimeline(Music& song);

	void addNewArrow(Music &song, Point &point);

	void scoreCheck(int& keyType, Point& point);

	void updateArrowRange(Music& song, Point& point);

	//render function

	void createRect();

	void renderPressedArrow(Event& event, Point& point);

	void renderArrowRange();

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
	
	//constructor

	DogeTexture(Screen& screen);
	
	//basic functions

	bool load(const string& path);

	void render(Event& event);

	void free();

	//play sound

	void playBonkSound();	

	//render function

	void createRect();

};

//main game function
void game(Screen& screen, Background& background, Event& event, Music& song);

//loading resources

bool loadingGameResource(Screen& screen, ArrowTexture& texture, DogeTexture& dogeTexture, Music& song, Point& point);

void freeGamesTexture(ArrowTexture& texture, DogeTexture& dogeTexture, Music& song, Point& point);

#endif