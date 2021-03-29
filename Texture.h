#ifndef _Texture_h
#define _Texture_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
using namespace std;

enum backgroundNumber {
	MENU,
	CHOOSE_MUSIC,
	GAME,
	TOTAL_BACKGROUND,
};

enum colorStripNumber {
	ORANGE,
	GREEN,
	PURPLE,
	TOTAL_COLOR_STRIP,
};

//error loading function
void loadErrorLog()
{
	printf("Error: %s\n", SDL_GetError());
	printf("Error: %s\n", IMG_GetError());
	printf("Error: %s\n", Mix_GetError());
	printf("Error: %s\n", TTF_GetError());
}

//this file contains functions to use textures more easily

//coordinate used by the arrowRange
class Coordinate {
	int x;
	int y;

public:
	Coordinate(int _x, int _y) {
		x = _x;
		y = _y;
	}

	//create arrow rectangle to render
	SDL_Rect getRect(const int& width, const int& height) {
		SDL_Rect rect = { x, y, width, height };
		return rect;
	}

	//arrow moving 
	void arrowMove(const int& velocity) {
			y -= velocity;
	}

	//get the height coordinate of the arrow
	int getY() {
		return y;
	}

	//get arrow position 
	int getArrowCol(const int& screenUnit) {
		int arrowCol = (x / screenUnit - 1) / 2;
		return arrowCol;
	}
};

//texture class handler
class LTexture {

	//texture and the related parameters
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

public:

	//create a texture
	LTexture() {
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}

	//free the texture
	void free() {
		if (mTexture != NULL) {
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	//load resource to texture
	bool loadFromFile(SDL_Renderer* renderer, string path) {
		free();
		bool success = true;
		SDL_Texture* newTexture = NULL;
		SDL_Surface* loadedImage = IMG_Load(path.c_str());
		if (loadedImage == NULL) {
			loadErrorLog();
			success = false;
		}
		else {
			newTexture = SDL_CreateTextureFromSurface(renderer, loadedImage);
			if (newTexture == NULL) {
				loadErrorLog();
				success = false;
			}
			else {
				mWidth = loadedImage->w;
				mHeight = loadedImage->h;
			}
			SDL_FreeSurface(loadedImage);
		}
		mTexture = newTexture;
		return success;
	}

	//render part of a texture (in the type of a rectangle) to the screen with given rectangle
	void render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect) {
		if (sourceRect == NULL) {
			SDL_Rect render = { 0, 0, mWidth, mHeight };
			sourceRect = &render;
		}
		if (mTexture != NULL) {
			SDL_RenderCopy(renderer, mTexture, sourceRect, renderRect);
		}
	}

	//Destructor
	~LTexture() {
		free();
	}

	//get the width of the loaded picture
	int getWidth() {
		return mWidth;
	}

	//get the height of the loaded picture
	int getHeight() {
		return mHeight;
	}
};


//renderer and background class handler
class Screen {

	//renderer and the related parameters
	SDL_Renderer* renderer = NULL;
	int screenUnit = 0,
		screenWidth = 0,
		screenHeight = 0;

	//background and the related parameters
	LTexture background;
	int backgroundUnit = 0;
	SDL_Rect* backgroundSrcRect = NULL;
	SDL_Rect* backgroundDstRect = NULL;

	//color strips and the related parameters
	LTexture colorStrip[TOTAL_COLOR_STRIP];
	SDL_Rect* colorStripSrcRect = NULL;
	SDL_Rect* colorStripDstRect = NULL;
	int colorStripVelo = screenUnit / 20;

public:
	//set the renderer parameters
	void setScreenUnit(int number) {
		screenUnit = number;
		screenWidth = screenUnit * 16;
		screenHeight = screenUnit * 9;
	}

	//get renderer pointer
	SDL_Renderer* getRenderer() {
		return renderer;
	}

	//get the renderer parameters
	int getScreenUnit() {
		return screenUnit;
	}

	int getScreenWidth() {
		return screenWidth;
	}

	int getScreenHeight() {
		return screenHeight;
	}

	//create renderer
	bool createRenderer(SDL_Window*& window) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL) {
			return false;
		}
		return true;
	}

	//create background and its parameters
	void createBackgroundRect();

	//load background
	bool loadBackground(const string& path) {
		bool success = true;
		if (!background.loadFromFile(renderer, path + "/Picture/background.png")) {
			success = false;
		}
		if (!colorStrip[ORANGE].loadFromFile(renderer, path + "/Picture/ColorStripOrange.png")) {
			success = false;
		}
		if (!colorStrip[GREEN].loadFromFile(renderer, path + "/Picture/ColorStripGreen.png")) {
			success = false;
		}
		if (!colorStrip[PURPLE].loadFromFile(renderer, path + "/Picture/ColorStripPurple.png")) {
			success = false;
		}
		createBackgroundRect();
		return success;
	}

	//color strip moving function
	void moveColorStrip() {
		colorStripDstRect[MENU].y = (colorStripDstRect[MENU].y + colorStripVelo) % screenHeight;
		colorStripDstRect[CHOOSE_MUSIC].y = (colorStripDstRect[CHOOSE_MUSIC].y + colorStripVelo) % screenHeight;
		colorStripDstRect[GAME].y = (colorStripDstRect[GAME].y + colorStripVelo) % screenHeight;
	}

	//color strip render function
	void renderColorStrip(int backgroundType);

	//background picture moving function
	void moveBackground(int original, int move) {
		SDL_Rect transition = backgroundSrcRect[original];
		int xVelo = (backgroundSrcRect[move].x - backgroundSrcRect[original].x) * 8 / screenUnit / 5,
			yVelo = (backgroundSrcRect[move].y - backgroundSrcRect[original].y) * 8 / screenUnit / 5,
			wVelo = (backgroundSrcRect[move].w - backgroundSrcRect[original].w) * 8 / screenUnit / 5,
			hVelo = (backgroundSrcRect[move].h - backgroundSrcRect[original].h) * 8 / screenUnit / 5;
		while (!SDL_RectEquals(&transition, &backgroundSrcRect[move])) {
			SDL_RenderClear(renderer);
			transition.x += xVelo;
			transition.y += yVelo;
			transition.w += wVelo;
			transition.h += hVelo;
			if (wVelo < 0 && transition.w < backgroundSrcRect[move].w) {
				transition = backgroundSrcRect[move];
			}
			if (wVelo > 0 && transition.w > backgroundSrcRect[move].w) {
				transition = backgroundSrcRect[move];
			}
			if (hVelo < 0 && transition.h < backgroundSrcRect[move].h) {
				transition = backgroundSrcRect[move];
			}
			if (hVelo > 0 && transition.h > backgroundSrcRect[move].h) {
				transition = backgroundSrcRect[move];
			}
			background.render(renderer, &backgroundDstRect[GAME], &transition);
			renderColorStrip(move);
			SDL_RenderPresent(renderer);
		}
	}

	//background render function
	void renderBackground(int backgroundType) {
		background.render(renderer, &backgroundDstRect[backgroundType], &backgroundSrcRect[backgroundType]);
	}
	
	//free the screen
	void freeScreen() {
		background.free();
		for (int i = 0; i < TOTAL_BACKGROUND; i++) {
			colorStrip[i].free();
		}
		delete[] backgroundSrcRect;
		delete[] backgroundDstRect;
		delete[] colorStripSrcRect;
		delete[] colorStripDstRect;
		backgroundUnit = 0;
		backgroundSrcRect = NULL;
		backgroundDstRect = NULL;
		colorStripSrcRect = NULL;
		colorStripDstRect = NULL;
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	//screen destructor
	~Screen() {
		freeScreen();
	}
};



extern enum backgroundNumber;

#endif