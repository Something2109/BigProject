#ifndef _Texture_h
#define _Texture_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

enum backgroundNumber {
	MENU,
	CHOOSE_MUSIC,
	GAME,
};

//error loading function
void loadErrorLog()
{
	printf("Error: %s\n", SDL_GetError());
	printf("Error: %s\n", IMG_GetError());
	printf("Error: %s\n", Mix_GetError());
}


//this file contains functions to use textures more easily

class Coordinate {
private:
	int x;
	int y;
public:
	Coordinate(int _x, int _y) {
		x = _x;
		y = _y;
	}
	SDL_Rect getRect(const int& width, const int& height) {
		SDL_Rect rect = { x, y, width, height };
		return rect;
	}
	void arrowMove(const int& velocity) {
			y -= velocity;
	}
	int getY() {
		return y;
	}
	int getArrowCol(const int& screenUnit) {
		int arrowCol = (x / screenUnit - 1) / 2;
		return arrowCol;
	}
};

class LTexture {
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
	class Background;
	~LTexture() {
		free();
	}
	int getWidth() {
		return mWidth;
	}
	int getHeight() {
		return mHeight;
	}
};

class Screen  {
	SDL_Renderer* renderer = NULL;
	int screenUnit = 80;

	LTexture background;
	int backgroundUnit = 0;
	int screenBackgroundNumber = 3;
	SDL_Rect* backgroundSrcRect = new SDL_Rect[screenBackgroundNumber];
	SDL_Rect* backgroundDstRect = new SDL_Rect;
public:
	SDL_Renderer* getRenderer() {
		return renderer;
	}

	int getScreenUnit() {
		return screenUnit;
	}

	bool createRenderer(SDL_Window* &window) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL) {
			return false;
		}
		return true;
	}

	void loadBackground(const string &path) {
		background.loadFromFile(renderer, path + "/background.png");
		backgroundUnit = background.getHeight() / 9;
		*backgroundDstRect = { 0, 0, screenUnit * 16, screenUnit * 9 };
		backgroundSrcRect[MENU] = { 0, 0, background.getWidth(), background.getHeight() };
		backgroundSrcRect[CHOOSE_MUSIC] = { 0, 0, static_cast<int>(backgroundUnit * 9.5), static_cast<int>(backgroundUnit * 6) };
		backgroundSrcRect[GAME] = { 0, backgroundUnit * 2, static_cast<int>(backgroundUnit * 9.5), static_cast<int>(backgroundUnit * 6) };
	}

	void renderBackground(int backgroundType) {
		background.render(renderer, backgroundDstRect, &backgroundSrcRect[backgroundType]);
	}

	void moveBackground(int& original, int& move) {
		SDL_Rect transition = backgroundSrcRect[original];
		int xVelo = backgroundSrcRect[move].x - backgroundSrcRect[original].x,
			yVelo = backgroundSrcRect[move].y - backgroundSrcRect[original].y,
			wVelo = backgroundSrcRect[move].w - backgroundSrcRect[original].w,
			hVelo = backgroundSrcRect[move].h - backgroundSrcRect[original].h;
		while (transition.x != backgroundSrcRect[move].x) {
			SDL_RenderClear(renderer);
			if (abs(backgroundSrcRect[move].x - transition.x) < abs(xVelo)) {
				transition = backgroundSrcRect[move];
			}
			else {
				transition.x += xVelo;
				transition.y += yVelo;
				transition.w += wVelo;
				transition.h += hVelo;
			}
			background.render(renderer, backgroundDstRect, &transition);
		}
	}

	void freeScreen() {
		background.free();
		delete[] backgroundSrcRect;
		backgroundSrcRect = NULL;
		backgroundUnit = 0;
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	void move(const int src, const int dst) {

	}
};

struct GameTexture {
	LTexture arrow,
		blankArrow,
		pressedArrow,
		muscleDoge,
		cheems,
		smashedCheems;

	bool loadTexture(SDL_Renderer* renderer, const string& path) {
		bool success = true;
		if (!blankArrow.loadFromFile(renderer, path + "/blankArrow.png")) {
			loadErrorLog();
			success = false;
		}
		if (!arrow.loadFromFile(renderer, path + "/arrow.png")) {
			loadErrorLog();
			success = false;
		}
		if (!pressedArrow.loadFromFile(renderer, path + "/pressedArrow.png")) {
			loadErrorLog();
			success = false;
		}
		if (!muscleDoge.loadFromFile(renderer, path + "/muscleDoge.png")) {
			loadErrorLog();
			success = false;
		}
		if (!cheems.loadFromFile(renderer, path + "/cheems.png")) {
			loadErrorLog();
			success = false;
		}
		if (!smashedCheems.loadFromFile(renderer, path + "/smashedCheems.png")) {
			loadErrorLog();
			success = false;
		}
		return success;
	}
	void free() {
		arrow.free();
		blankArrow.free();
		pressedArrow.free();
		muscleDoge.free();
		cheems.free();
		smashedCheems.free();
	}
};

struct GameRect {
	int arrowRangeCol = 4;
	SDL_Rect* blankArrowSrcRect = new SDL_Rect;
	SDL_Rect* blankArrowDstRect = new SDL_Rect;
	SDL_Rect* arrowSrcRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* pressedArrowDstRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* muscleDogeRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* cheemsSrcRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* cheemsDstRect = new SDL_Rect[arrowRangeCol];

	void createSourceRect(const int& screenUnit, GameTexture &texture);

	void free() {
		delete blankArrowSrcRect;
		delete blankArrowDstRect;
		delete[] arrowSrcRect;
		delete[] pressedArrowDstRect;
		delete[] muscleDogeRect;
		delete[] cheemsSrcRect;
		delete[] cheemsDstRect;
		blankArrowSrcRect = NULL;
		blankArrowDstRect = NULL;
		arrowSrcRect = NULL;
		pressedArrowDstRect = NULL;
		muscleDogeRect = NULL;
		cheemsSrcRect = NULL;
		cheemsDstRect = NULL;
	}

};

struct MenuTexture {

};

extern enum backgroundNumber;

#endif