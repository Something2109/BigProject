#ifndef _Texture_h
#define _Texture_h
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
using namespace std;

class LTexture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
public:
	LTexture() {
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
	void free() {
		if (mTexture != NULL) {
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}
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
			SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0xFF, 0xFF, 0xFF));
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
	void render(SDL_Renderer* renderer, const int& x, const int& y, SDL_Rect* sourceRect) {
		SDL_Rect renderRect = { x, y, mWidth, mHeight };
		if (mTexture != NULL) {
			SDL_RenderCopy(renderer, mTexture, sourceRect, &renderRect);
		}
	}
	void clipRender(SDL_Renderer* renderer, const int& x, const int& y, SDL_Rect* sourceRect) {
		SDL_Rect renderRect = { x, y, mWidth / 2, mHeight / 2 };
		if (mTexture != NULL) {
			SDL_RenderCopy (renderer, mTexture, sourceRect, &renderRect);
		}
	}
	void rectRender(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect) {
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

class Arrow {
private:
	SDL_Texture* mTexture;
	int velocity;
	SDL_Rect* sourceRect;
	int xCoordinate;
	int yCoordinate;
public:
	Arrow(int _velocity, int col, int screenUnit) {
		mTexture = NULL;
		velocity = _velocity;
		xCoordinate = screenUnit *(col + 1)* 2;
		yCoordinate = screenUnit * 9;
	}
	void render(SDL_Renderer* renderer, const int &screenUnit) {
		SDL_Rect renderRect = { xCoordinate, yCoordinate, screenUnit, screenUnit };
		if (mTexture != NULL) {
			SDL_RenderCopy(renderer, mTexture, sourceRect, &renderRect);
		}
	}
	void move() {
		yCoordinate -= velocity;
	}
	bool onScreen(const int& screenUnit) {
		if (yCoordinate >= -screenUnit) {
			return true;
		}
		return false;
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
};

#endif