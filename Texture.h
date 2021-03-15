#ifndef _Texture_h
#define _Texture_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
using namespace std;

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
	//render texture to screen with given coordination
	void render(SDL_Renderer* renderer, const int x, const int y, SDL_Rect* sourceRect) {
		SDL_Rect renderRect = {x, y, mWidth, mHeight};
		if (mTexture != NULL) {
			SDL_RenderCopy(renderer, mTexture, sourceRect, &renderRect);
		}
	}
	//render part of a texture (in the type of a rectangle) to the screen with given rectangle
	void rectRender(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect) {
		if (sourceRect == NULL) {
			SDL_Rect render = { 0, 0, mWidth, mHeight };
			sourceRect = &render;
		}
		if (mTexture != NULL) {
			SDL_RenderCopy(renderer, mTexture, sourceRect, renderRect);
		}
	}
	class Arrow;
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

struct GameTexture {
	LTexture arrow,
		blankArrow,
		pressedArrow,
		muscleDoge,
		cheems,
		smashedCheems,
		background;

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
		if (!background.loadFromFile(renderer, path + "/background.png")) {
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
		background.free();
	}
};


struct GameRect {
	int arrowRangeCol = 4;
	SDL_Rect* backgroundSrcRect = new SDL_Rect;
	SDL_Rect* backgroundDstRect = new SDL_Rect;
	SDL_Rect* blankArrowSrcRect = new SDL_Rect;
	SDL_Rect* blankArrowDstRect = new SDL_Rect;
	SDL_Rect* arrowSrcRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* pressedArrowDstRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* muscleDogeRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* cheemsSrcRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* cheemsDstRect = new SDL_Rect[arrowRangeCol];

	void createSourceRect(const int& screenUnit, GameTexture &texture);

	void free() {
		delete backgroundSrcRect;
		delete backgroundDstRect;
		delete blankArrowSrcRect;
		delete blankArrowDstRect;
		delete[] arrowSrcRect;
		delete[] pressedArrowDstRect;
		delete[] muscleDogeRect;
		delete[] cheemsSrcRect;
		delete[] cheemsDstRect;
		backgroundSrcRect = NULL;
		backgroundDstRect = NULL;
		blankArrowSrcRect = NULL;
		blankArrowDstRect = NULL;
		arrowSrcRect = NULL;
		pressedArrowDstRect = NULL;
		muscleDogeRect = NULL;
		cheemsSrcRect = NULL;
		cheemsDstRect = NULL;
	}

};


//still developing class
class Arrow {
private:
	SDL_Texture* mTexture;
	int velocity;
	SDL_Rect sourceRect = { 0, 0, 80, 80};
	int xCoordinate;
	int yCoordinate;
public:
	Arrow (int _velocity, int col, int screenUnit) {
		mTexture = NULL;
		velocity = _velocity;
		xCoordinate = screenUnit * (col*2 + 1);
		yCoordinate = screenUnit * 9;
	}
	bool loadFromFile(SDL_Renderer* renderer, string path) {
		bool success = true;
		SDL_Texture* newTexture = NULL;
		SDL_Surface* loadedImage = IMG_Load(path.c_str());
		if (loadedImage == NULL) {
			loadErrorLog();
			success = false;
		}
		else {
			SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0xFF, 0xFF, 0xFF));
			newTexture = SDL_CreateTextureFromSurface(renderer, loadedImage);
			if (newTexture == NULL) {
				loadErrorLog();
				success = false;
			}
			SDL_FreeSurface(loadedImage);
		}
		mTexture = newTexture;
		return success;
	}
	void render(SDL_Renderer* renderer, const int &screenUnit) {
		SDL_Rect renderRect = { xCoordinate, yCoordinate, screenUnit, screenUnit };
		if (mTexture != NULL) {
			SDL_RenderCopy(renderer, mTexture, &sourceRect, &renderRect);
		}
	}
	void move() {
		yCoordinate -= velocity;
	}
	
	void free() {
		if (mTexture != NULL) {
			SDL_DestroyTexture(mTexture);
		}
		mTexture = NULL;
		velocity = 0;
		xCoordinate = 0;
		yCoordinate = 0;
	}
	int getX() {
		return xCoordinate;
	}
	int getY() {
		return yCoordinate;
	}
};

#endif