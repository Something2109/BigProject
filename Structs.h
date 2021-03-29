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
#include "Texture.h"
using namespace std;

//this file contain all the texture struct related to all the game frame type

//menu texture struct

struct MenuTexture {

};

//game texture struct

struct GameTexture {
	LTexture arrow,
		blankArrow,
		pressedArrow,
		muscleDoge,
		cheems,
		smashedCheems;

	SDL_Rect* blankArrowSrcRect = NULL;
	SDL_Rect* blankArrowDstRect = NULL;
	SDL_Rect* arrowSrcRect = NULL;
	SDL_Rect* pressedArrowDstRect = NULL;
	SDL_Rect* muscleDogeRect = NULL;
	SDL_Rect* cheemsSrcRect = NULL;
	SDL_Rect* cheemsDstRect = NULL;

	void createGameRect(const int& screenUnit);

	//load all the texture from source
	bool loadTexture(Screen &background, const string& path) {
		bool success = true;
		SDL_Renderer* renderer = background.getRenderer();
		int screenUnit = background.getScreenUnit();

		if (!blankArrow.loadFromFile(renderer, path + "/Picture/blankArrow.png")) {
			loadErrorLog();
			success = false;
		}
		if (!arrow.loadFromFile(renderer, path + "/Picture/arrow.png")) {
			loadErrorLog();
			success = false;
		}
		if (!pressedArrow.loadFromFile(renderer, path + "/Picture/pressedArrow.png")) {
			loadErrorLog();
			success = false;
		}
		if (!muscleDoge.loadFromFile(renderer, path + "/Picture/muscleDoge.png")) {
			loadErrorLog();
			success = false;
		}
		if (!cheems.loadFromFile(renderer, path + "/Picture/cheems.png")) {
			loadErrorLog();
			success = false;
		}
		if (!smashedCheems.loadFromFile(renderer, path + "/Picture/smashedCheems.png")) {
			loadErrorLog();
			success = false;
		}
		createGameRect(screenUnit);
		return success;
	}

	//free all the texture
	void free() {
		arrow.free();
		blankArrow.free();
		pressedArrow.free();
		muscleDoge.free();
		cheems.free();
		smashedCheems.free();

		delete blankArrowSrcRect;
		blankArrowSrcRect = NULL;
		delete blankArrowDstRect;
		blankArrowDstRect = NULL;
		delete[] arrowSrcRect;
		arrowSrcRect = NULL;
		delete[] pressedArrowDstRect;
		pressedArrowDstRect = NULL;
		delete[] muscleDogeRect;
		muscleDogeRect = NULL;
		delete[] cheemsSrcRect;
		cheemsSrcRect = NULL;
		delete[] cheemsDstRect;
		cheemsDstRect = NULL;
	}
};

#endif
