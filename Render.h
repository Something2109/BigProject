#ifndef _Render_h
#define _Render_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Texture.h"
using namespace std;

//this file contains contents and functions to render

enum arrowKeyPress {
	LEFT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
};

enum muscleDoge {
	SOURCE_LEFT,
	SOURCE_IDLE,
	SOURCE_RIGHT,
	DESTINATION,
};

void GameRect::createSourceRect(const int& screenUnit, GameTexture &texture)
{
	//rectangle to render from image
	*blankArrowSrcRect = { 0, 0, texture.blankArrow.getWidth(), texture.blankArrow.getHeight() };
	arrowSrcRect[LEFT_ARROW] = { 0, 0, texture.arrow.getWidth() / 2, texture.arrow.getHeight() / 2 };
	arrowSrcRect[UP_ARROW] = { texture.arrow.getWidth() / 2, 0, texture.arrow.getWidth() / 2, texture.arrow.getHeight() / 2 };
	arrowSrcRect[DOWN_ARROW] = { 0, texture.arrow.getHeight() / 2, texture.arrow.getWidth() / 2, texture.arrow.getHeight() / 2 };
	arrowSrcRect[RIGHT_ARROW] = { texture.arrow.getWidth() / 2, texture.arrow.getHeight() / 2, texture.arrow.getWidth() / 2, texture.arrow.getHeight() / 2 };
	muscleDogeRect[SOURCE_LEFT] = { 0, 0, texture.muscleDoge.getWidth() / 3, texture.muscleDoge.getHeight() };
	muscleDogeRect[SOURCE_IDLE] = { texture.muscleDoge.getWidth() / 3, 0, texture.muscleDoge.getWidth() / 3, texture.muscleDoge.getHeight() };
	muscleDogeRect[SOURCE_RIGHT] = { texture.muscleDoge.getWidth() * 2 / 3, 0, texture.muscleDoge.getWidth() / 3, texture.muscleDoge.getHeight() };
	cheemsSrcRect[LEFT_ARROW] = { 0, 0, texture.cheems.getWidth() / 2, texture.cheems.getHeight() / 2 };
	cheemsSrcRect[UP_ARROW] = { 0, texture.cheems.getHeight() / 2, texture.cheems.getWidth() / 2, texture.cheems.getHeight() / 2 };
	cheemsSrcRect[DOWN_ARROW] = { texture.cheems.getWidth() / 2, texture.cheems.getHeight() / 2, texture.cheems.getWidth() / 2, texture.cheems.getHeight() / 2 };
	cheemsSrcRect[RIGHT_ARROW] = { texture.cheems.getWidth() / 2, 0, texture.cheems.getWidth() / 2, texture.cheems.getHeight() / 2 };
	//rectangle to render to screen
	*blankArrowDstRect = { screenUnit, screenUnit, screenUnit * 7, screenUnit };
	muscleDogeRect[DESTINATION] = { static_cast<int>(screenUnit * 10), screenUnit * 3, screenUnit * 4, screenUnit * 4 };
	pressedArrowDstRect[LEFT_ARROW] = { screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[UP_ARROW] = { 3 * screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[DOWN_ARROW] = { 5 * screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[RIGHT_ARROW] = { 7 * screenUnit, screenUnit, screenUnit, screenUnit };
	cheemsDstRect[LEFT_ARROW] = { static_cast<int>(screenUnit * 9.5), screenUnit * 5, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[UP_ARROW] = { static_cast<int>(screenUnit * 10.5), screenUnit * 6, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[DOWN_ARROW] = { static_cast<int>(screenUnit * 12), screenUnit * 6, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[RIGHT_ARROW] = { static_cast<int>(screenUnit * 13), screenUnit * 5, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
}

void renderPressedArrow(Screen& background, SDL_Event& e, vector <Coordinate>& arrowRange, GameTexture &texture, GameRect &rectangles) 
{
	SDL_Renderer* renderer = background.getRenderer();
	int screenUnit = background.getScreenUnit();
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_LEFT: {
			texture.pressedArrow.render(renderer, &rectangles.pressedArrowDstRect[LEFT_ARROW], &rectangles.arrowSrcRect[LEFT_ARROW]);
			int arrowCol = arrowRange[0].getArrowCol(screenUnit);
			if (arrowCol == LEFT_ARROW && arrowRange[0].getY() >= 0 && arrowRange[0].getY() <= 2 * screenUnit) {
				SDL_Rect renderRect = arrowRange[0].getRect(screenUnit, screenUnit);
				texture.arrow.render(renderer, &renderRect, &rectangles.arrowSrcRect[arrowCol]);
				arrowRange.erase(arrowRange.begin());
			}
			break;
		}
		case SDLK_UP: {
			texture.pressedArrow.render(renderer, &rectangles.pressedArrowDstRect[UP_ARROW], &rectangles.arrowSrcRect[UP_ARROW]);
			int arrowCol = arrowRange[0].getArrowCol(screenUnit);
			if (arrowCol == UP_ARROW && arrowRange[0].getY() >= 0 && arrowRange[0].getY() <= 2 * screenUnit) {
				SDL_Rect renderRect = arrowRange[0].getRect(screenUnit, screenUnit);
				texture.arrow.render(renderer, &renderRect, &rectangles.arrowSrcRect[arrowCol]);
				arrowRange.erase(arrowRange.begin());
			}
			break;
		}
		case SDLK_DOWN: {
			texture.pressedArrow.render(renderer, &rectangles.pressedArrowDstRect[DOWN_ARROW], &rectangles.arrowSrcRect[DOWN_ARROW]);
			int arrowCol = arrowRange[0].getArrowCol(screenUnit);
			if (arrowCol == DOWN_ARROW && arrowRange[0].getY() >= 0 && arrowRange[0].getY() <= 2 * screenUnit) {
				SDL_Rect renderRect = arrowRange[0].getRect(screenUnit, screenUnit);
				texture.arrow.render(renderer, &renderRect, &rectangles.arrowSrcRect[arrowCol]);
				arrowRange.erase(arrowRange.begin());
			}
			break;
		}
		case SDLK_RIGHT: {
			texture.pressedArrow.render(renderer, &rectangles.pressedArrowDstRect[RIGHT_ARROW], &rectangles.arrowSrcRect[RIGHT_ARROW]);
			int arrowCol = arrowRange[0].getArrowCol(screenUnit);
			if (arrowCol == RIGHT_ARROW && arrowRange[0].getY() >= 0 && arrowRange[0].getY() <= 2 * screenUnit) {
				SDL_Rect renderRect = arrowRange[0].getRect(screenUnit, screenUnit);
				texture.arrow.render(renderer, &renderRect, &rectangles.arrowSrcRect[arrowCol]);
				arrowRange.erase(arrowRange.begin());
			}
			break;
		}
		}
	}
}

void renderArrowRange(Screen& background, vector <Coordinate> &arrowRange, LTexture& arrow, SDL_Rect* arrowSrcRect) 
{
	SDL_Renderer* renderer = background.getRenderer();
	int screenUnit = background.getScreenUnit();
	for (int i = 0; i < arrowRange.size(); i++) {
		SDL_Rect renderRect = arrowRange[i].getRect(screenUnit, screenUnit);
		int arrowCol = arrowRange[i].getArrowCol(screenUnit);
		arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
	}
}

void renderDogeScreen(Screen& background, SDL_Event& e, GameTexture& texture, GameRect& rectangle, Mix_Chunk* bonk)
{
	SDL_Renderer* renderer = background.getRenderer();
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_LEFT: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			texture.pressedArrow.render(renderer, &rectangle.pressedArrowDstRect[LEFT_ARROW], &rectangle.arrowSrcRect[LEFT_ARROW]);
			texture.smashedCheems.render(renderer, &rectangle.cheemsDstRect[LEFT_ARROW], &rectangle.cheemsSrcRect[LEFT_ARROW]);
			texture.muscleDoge.render(renderer, &rectangle.muscleDogeRect[DESTINATION], &rectangle.muscleDogeRect[SOURCE_LEFT]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[RIGHT_ARROW], &rectangle.cheemsSrcRect[RIGHT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[UP_ARROW], &rectangle.cheemsSrcRect[UP_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[DOWN_ARROW], &rectangle.cheemsSrcRect[DOWN_ARROW]);
			break;
		}
		case SDLK_UP: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			texture.pressedArrow.render(renderer, &rectangle.pressedArrowDstRect[UP_ARROW], &rectangle.arrowSrcRect[UP_ARROW]);
			texture.muscleDoge.render(renderer, &rectangle.muscleDogeRect[DESTINATION], &rectangle.muscleDogeRect[SOURCE_IDLE]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[LEFT_ARROW], &rectangle.cheemsSrcRect[LEFT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[RIGHT_ARROW], &rectangle.cheemsSrcRect[RIGHT_ARROW]);
			texture.smashedCheems.render(renderer, &rectangle.cheemsDstRect[UP_ARROW], &rectangle.cheemsSrcRect[UP_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[DOWN_ARROW], &rectangle.cheemsSrcRect[DOWN_ARROW]);
			break;
		}
		case SDLK_DOWN: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			texture.pressedArrow.render(renderer, &rectangle.pressedArrowDstRect[DOWN_ARROW], &rectangle.arrowSrcRect[DOWN_ARROW]);
			texture.muscleDoge.render(renderer, &rectangle.muscleDogeRect[DESTINATION], &rectangle.muscleDogeRect[SOURCE_IDLE]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[LEFT_ARROW], &rectangle.cheemsSrcRect[LEFT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[RIGHT_ARROW], &rectangle.cheemsSrcRect[RIGHT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[UP_ARROW], &rectangle.cheemsSrcRect[UP_ARROW]);
			texture.smashedCheems.render(renderer, &rectangle.cheemsDstRect[DOWN_ARROW], &rectangle.cheemsSrcRect[DOWN_ARROW]);
			break;
		}
		case SDLK_RIGHT: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			texture.pressedArrow.render(renderer, &rectangle.pressedArrowDstRect[RIGHT_ARROW], &rectangle.arrowSrcRect[RIGHT_ARROW]);
			texture.smashedCheems.render(renderer, &rectangle.cheemsDstRect[RIGHT_ARROW], &rectangle.cheemsSrcRect[RIGHT_ARROW]);
			texture.muscleDoge.render(renderer, &rectangle.muscleDogeRect[DESTINATION], &rectangle.muscleDogeRect[SOURCE_RIGHT]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[LEFT_ARROW], &rectangle.cheemsSrcRect[LEFT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[UP_ARROW], &rectangle.cheemsSrcRect[UP_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[DOWN_ARROW], &rectangle.cheemsSrcRect[DOWN_ARROW]);
			break;
		}
		default: {
			texture.muscleDoge.render(renderer, &rectangle.muscleDogeRect[DESTINATION], &rectangle.muscleDogeRect[SOURCE_IDLE]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[LEFT_ARROW], &rectangle.cheemsSrcRect[LEFT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[RIGHT_ARROW], &rectangle.cheemsSrcRect[RIGHT_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[UP_ARROW], &rectangle.cheemsSrcRect[UP_ARROW]);
			texture.cheems.render(renderer, &rectangle.cheemsDstRect[DOWN_ARROW], &rectangle.cheemsSrcRect[DOWN_ARROW]);
		}
		}
	}
	else {
		texture.muscleDoge.render(renderer, &rectangle.muscleDogeRect[DESTINATION], &rectangle.muscleDogeRect[SOURCE_IDLE]);
		texture.cheems.render(renderer, &rectangle.cheemsDstRect[LEFT_ARROW], &rectangle.cheemsSrcRect[LEFT_ARROW]);
		texture.cheems.render(renderer, &rectangle.cheemsDstRect[RIGHT_ARROW], &rectangle.cheemsSrcRect[RIGHT_ARROW]);
		texture.cheems.render(renderer, &rectangle.cheemsDstRect[UP_ARROW], &rectangle.cheemsSrcRect[UP_ARROW]);
		texture.cheems.render(renderer, &rectangle.cheemsDstRect[DOWN_ARROW], &rectangle.cheemsSrcRect[DOWN_ARROW]);
	}
}

#endif