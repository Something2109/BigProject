#ifndef _Render_h
#define _Render_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Structs.h"
#include "Event.h"
#include "Sound.h" 
using namespace std;

//this file contains contents and functions to render

enum arrowKeyPress {
	LEFT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
	TOTAL_ARROW,
};

enum muscleDoge {
	SOURCE_LEFT,
	SOURCE_IDLE,
	SOURCE_RIGHT,
	DESTINATION,
	TOTAL_DOGE_RECT
};

void scoreCheck(Screen& background, vector<Coordinate>& arrowRange, GameTexture& texture, int& keyType);

//functions to create the texture to print

void Screen::createBackgroundRect() {


	//Rectangles to render from source
	backgroundUnit = background.getHeight() / 9;
	backgroundSrcRect = new SDL_Rect[TOTAL_BACKGROUND];
	colorStripSrcRect = new SDL_Rect[TOTAL_COLOR_STRIP];
	backgroundSrcRect[MENU] = { 0, 0, background.getWidth(), background.getHeight() };
	backgroundSrcRect[CHOOSE_MUSIC] = { 0, 0, static_cast<int>(background.getWidth() * 2 / 3), static_cast<int>(background.getHeight() * 2 / 3) };
	backgroundSrcRect[GAME] = { 0, backgroundUnit * 2, static_cast<int>(background.getWidth() * 2 / 3), static_cast<int>(background.getHeight() * 2 / 3) };
	colorStripSrcRect[ORANGE] = { 0, 0, colorStrip[ORANGE].getWidth(), colorStrip[ORANGE].getHeight() };
	colorStripSrcRect[GREEN] = { 0, 0, colorStrip[GREEN].getWidth(), colorStrip[GREEN].getHeight() };
	colorStripSrcRect[PURPLE] = { 0, 0, colorStrip[PURPLE].getWidth(), colorStrip[PURPLE].getHeight() };

	//Rectangles to render to screen
	backgroundDstRect = new SDL_Rect[TOTAL_BACKGROUND];
	colorStripDstRect = new SDL_Rect[TOTAL_COLOR_STRIP];
	backgroundDstRect[MENU] = { 0, 0, static_cast <int>(screenUnit * 19 * 3 / 4) , screenUnit * 9 };
	backgroundDstRect[CHOOSE_MUSIC] = { static_cast<int> (screenUnit * 1.75), 0, static_cast <int>(screenUnit * 19 * 3 / 4) , screenUnit * 9 };
	backgroundDstRect[GAME] = backgroundDstRect[CHOOSE_MUSIC];
	colorStripDstRect[MENU] = { screenUnit * 8, 0, screenUnit * 10, screenUnit * 9 };
	colorStripDstRect[CHOOSE_MUSIC] = { static_cast<int> (-screenUnit * 2.5), 0, screenUnit * 10, screenUnit * 9 };
	colorStripDstRect[GAME] = { static_cast<int> (-screenUnit * 2.5), 0, screenUnit * 10, screenUnit * 9 };
}

void GameTexture::createGameRect(const int& screenUnit)
{
	blankArrowSrcRect = new SDL_Rect;
	blankArrowDstRect = new SDL_Rect;
	arrowSrcRect = new SDL_Rect[TOTAL_ARROW];
	pressedArrowDstRect = new SDL_Rect[TOTAL_ARROW];
	muscleDogeRect = new SDL_Rect[TOTAL_DOGE_RECT];
	cheemsSrcRect = new SDL_Rect[TOTAL_ARROW];
	cheemsDstRect = new SDL_Rect[TOTAL_ARROW];

	//texture to render from image
	*blankArrowSrcRect = { 0, 0, blankArrow.getWidth(),  blankArrow.getHeight() };
	arrowSrcRect[LEFT_ARROW] = { 0, 0,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[UP_ARROW] = {  arrow.getWidth() / 2, 0,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[DOWN_ARROW] = { 0,  arrow.getHeight() / 2,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[RIGHT_ARROW] = {  arrow.getWidth() / 2,  arrow.getHeight() / 2,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	muscleDogeRect[SOURCE_LEFT] = { 0, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[SOURCE_IDLE] = {  muscleDoge.getWidth() / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[SOURCE_RIGHT] = {  muscleDoge.getWidth() * 2 / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	cheemsSrcRect[LEFT_ARROW] = { 0, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[UP_ARROW] = { 0,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[DOWN_ARROW] = {  cheems.getWidth() / 2,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[RIGHT_ARROW] = {  cheems.getWidth() / 2, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };

	//texture to render to screen
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

//Rendering function

void Screen::renderColorStrip(int backgroundType) {
	moveColorStrip();
	for (int i = 0; i < TOTAL_COLOR_STRIP; i++) {
		if (i % 2 == 1) {
			colorStripDstRect[backgroundType].y = screenHeight - colorStripDstRect[backgroundType].y;
		}
		colorStrip[i].render(renderer, &colorStripDstRect[backgroundType], &colorStripSrcRect[backgroundType]);
		colorStripDstRect[backgroundType].y -= screenHeight;
		colorStrip[i].render(renderer, &colorStripDstRect[backgroundType], &colorStripSrcRect[backgroundType]);
		colorStripDstRect[backgroundType].y += screenHeight;
		if (i % 2 == 1) {
			colorStripDstRect[backgroundType].y = screenHeight - colorStripDstRect[backgroundType].y;
		}
	}
}

void renderPressedArrow(Screen& background, Event &event, vector <Coordinate>& arrowRange, GameTexture &texture) 
{
	SDL_Renderer* renderer = background.getRenderer();
	int screenUnit = background.getScreenUnit();
	SDL_Event e = event.getEvent();
	const Uint8* keyState = event.getKeyState();
	int keyType;
	texture.blankArrow.render(renderer, texture.blankArrowDstRect, texture.blankArrowSrcRect);
	if (keyState[SDL_SCANCODE_LEFT]) {
		texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[LEFT_ARROW], &texture.arrowSrcRect[LEFT_ARROW]);
		if (e.key.keysym.sym == SDLK_LEFT && e.key.repeat == 0) {
			keyType = LEFT_ARROW;
			scoreCheck(background, arrowRange, texture, keyType);
		}
	}
	if (keyState[SDL_SCANCODE_UP]) {
		texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[UP_ARROW], &texture.arrowSrcRect[UP_ARROW]);
		if (e.key.keysym.sym == SDLK_UP && e.key.repeat == 0) {
			keyType = UP_ARROW;
			scoreCheck(background, arrowRange, texture, keyType);
		}
	}
	if (keyState[SDL_SCANCODE_DOWN]) {
		texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[DOWN_ARROW], &texture.arrowSrcRect[DOWN_ARROW]);
		if (e.key.keysym.sym == SDLK_DOWN && e.key.repeat == 0) {
			keyType = DOWN_ARROW;
			scoreCheck(background, arrowRange, texture, keyType);
		}
	}
	if (keyState[SDL_SCANCODE_RIGHT]) {
		texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[RIGHT_ARROW], &texture.arrowSrcRect[RIGHT_ARROW]);
		if (e.key.keysym.sym == SDLK_RIGHT && e.key.repeat == 0) {
			keyType = RIGHT_ARROW;
			scoreCheck(background, arrowRange, texture, keyType);
		}
	}
}

void scoreCheck(Screen& background, vector<Coordinate>& arrowRange, GameTexture& texture, int& keyType) {
	if (!arrowRange.empty()) {
		int screenUnit = background.getScreenUnit();
		int arrowCol = arrowRange[0].getArrowCol(screenUnit);
		if (arrowCol == keyType && arrowRange[0].getY() >= 0 && arrowRange[0].getY() <= 2 * screenUnit) {
			SDL_Rect renderRect = arrowRange[0].getRect(screenUnit, screenUnit);
			texture.arrow.render(background.getRenderer(), &renderRect, &texture.arrowSrcRect[arrowCol]);
			arrowRange.erase(arrowRange.begin());
		}
	}
}

void renderArrowRange(Screen& background, vector <Coordinate> &arrowRange, GameTexture &texture) 
{
	SDL_Renderer* renderer = background.getRenderer();
	int screenUnit = background.getScreenUnit();
	for (int i = 0; i < arrowRange.size(); i++) {
		SDL_Rect renderRect = arrowRange[i].getRect(screenUnit, screenUnit);
		int arrowCol = arrowRange[i].getArrowCol(screenUnit);
		texture.arrow.render(renderer, &renderRect, &texture.arrowSrcRect[arrowCol]);
	}
}

void renderDogeScreen(Screen& background, Event &event, GameTexture& texture, Mix_Chunk* bonk)
{
	SDL_Renderer* renderer = background.getRenderer();
	SDL_Event e = event.getEvent();
	const Uint8* keyState = event.getKeyState();
	if (keyState[SDL_SCANCODE_LEFT]) {
		if (e.key.repeat == 0) {
			playBonkSound(bonk);
		}
		texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[LEFT_ARROW], &texture.arrowSrcRect[LEFT_ARROW]);
		texture.smashedCheems.render(renderer, &texture.cheemsDstRect[LEFT_ARROW], &texture.cheemsSrcRect[LEFT_ARROW]);
		texture.muscleDoge.render(renderer, &texture.muscleDogeRect[DESTINATION], &texture.muscleDogeRect[SOURCE_LEFT]);
		texture.cheems.render(renderer, &texture.cheemsDstRect[RIGHT_ARROW], &texture.cheemsSrcRect[RIGHT_ARROW]);
		texture.cheems.render(renderer, &texture.cheemsDstRect[UP_ARROW], &texture.cheemsSrcRect[UP_ARROW]);
		texture.cheems.render(renderer, &texture.cheemsDstRect[DOWN_ARROW], &texture.cheemsSrcRect[DOWN_ARROW]);
	}
	else {
		if (keyState[SDL_SCANCODE_UP]) {
			if (e.key.repeat == 0) {
				playBonkSound(bonk);
			}
			texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[UP_ARROW], &texture.arrowSrcRect[UP_ARROW]);
			texture.muscleDoge.render(renderer, &texture.muscleDogeRect[DESTINATION], &texture.muscleDogeRect[SOURCE_IDLE]);
			texture.cheems.render(renderer, &texture.cheemsDstRect[LEFT_ARROW], &texture.cheemsSrcRect[LEFT_ARROW]);
			texture.cheems.render(renderer, &texture.cheemsDstRect[RIGHT_ARROW], &texture.cheemsSrcRect[RIGHT_ARROW]);
			texture.smashedCheems.render(renderer, &texture.cheemsDstRect[UP_ARROW], &texture.cheemsSrcRect[UP_ARROW]);
			texture.cheems.render(renderer, &texture.cheemsDstRect[DOWN_ARROW], &texture.cheemsSrcRect[DOWN_ARROW]);
		}
		else {
			if (keyState[SDL_SCANCODE_DOWN]) {
				if (e.key.repeat == 0) {
					playBonkSound(bonk);
				}
				texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[DOWN_ARROW], &texture.arrowSrcRect[DOWN_ARROW]);
				texture.muscleDoge.render(renderer, &texture.muscleDogeRect[DESTINATION], &texture.muscleDogeRect[SOURCE_IDLE]);
				texture.cheems.render(renderer, &texture.cheemsDstRect[LEFT_ARROW], &texture.cheemsSrcRect[LEFT_ARROW]);
				texture.cheems.render(renderer, &texture.cheemsDstRect[RIGHT_ARROW], &texture.cheemsSrcRect[RIGHT_ARROW]);
				texture.cheems.render(renderer, &texture.cheemsDstRect[UP_ARROW], &texture.cheemsSrcRect[UP_ARROW]);
				texture.smashedCheems.render(renderer, &texture.cheemsDstRect[DOWN_ARROW], &texture.cheemsSrcRect[DOWN_ARROW]);
			}
			else {
				if (keyState[SDL_SCANCODE_RIGHT]) {
					if (e.key.repeat == 0) {
						playBonkSound(bonk);
					}
					texture.pressedArrow.render(renderer, &texture.pressedArrowDstRect[RIGHT_ARROW], &texture.arrowSrcRect[RIGHT_ARROW]);
					texture.smashedCheems.render(renderer, &texture.cheemsDstRect[RIGHT_ARROW], &texture.cheemsSrcRect[RIGHT_ARROW]);
					texture.muscleDoge.render(renderer, &texture.muscleDogeRect[DESTINATION], &texture.muscleDogeRect[SOURCE_RIGHT]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[LEFT_ARROW], &texture.cheemsSrcRect[LEFT_ARROW]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[UP_ARROW], &texture.cheemsSrcRect[UP_ARROW]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[DOWN_ARROW], &texture.cheemsSrcRect[DOWN_ARROW]);
				}
				else {
					texture.muscleDoge.render(renderer, &texture.muscleDogeRect[DESTINATION], &texture.muscleDogeRect[SOURCE_IDLE]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[LEFT_ARROW], &texture.cheemsSrcRect[LEFT_ARROW]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[RIGHT_ARROW], &texture.cheemsSrcRect[RIGHT_ARROW]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[UP_ARROW], &texture.cheemsSrcRect[UP_ARROW]);
					texture.cheems.render(renderer, &texture.cheemsDstRect[DOWN_ARROW], &texture.cheemsSrcRect[DOWN_ARROW]);
				}
			}
		}
	}
}

#endif