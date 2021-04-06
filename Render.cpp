#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Background.h"
#include "Menu.h"
#include "Game.h"
using namespace std;

//this file contains contents and functions to render

//functions to create the texture to print

void Background::createBackgroundRect() 
{
	//Rectangles to render from source
	bgSourceRect = new SDL_Rect[TOTAL_BACKGROUND];
	bgSourceRect[MENU] = { 0, 0, background.getWidth(), background.getHeight() };
	bgSourceRect[CHOOSE_MUSIC] = { 0, 0, static_cast<int>(background.getWidth() * 2 / 3), static_cast<int>(background.getHeight() * 2 / 3) };
	bgSourceRect[GAME] = { 0, background.getHeight() * 2 / 9 , static_cast<int>(background.getWidth() * 2 / 3), static_cast<int>(background.getHeight() * 2 / 3) };

	//Rectangles to render to screen
	bgDestinationRect = new SDL_Rect[TOTAL_BACKGROUND];
	colorStripDstRect = new SDL_Rect[TOTAL_COLOR_STRIP];
	bgDestinationRect[MENU] = { 0, 0, static_cast <int>(screenUnit * 19 * 3 / 4) , screenUnit * 9 };
	bgDestinationRect[CHOOSE_MUSIC] = { static_cast<int> (screenUnit * 1.75), 0, static_cast <int>(screenUnit * 19 * 3 / 4) , screenUnit * 9 };
	bgDestinationRect[GAME] = bgDestinationRect[CHOOSE_MUSIC];
	colorStripDstRect[MENU] = { screenUnit * 8, 0, screenUnit * 10, screenUnit * 9 };
	colorStripDstRect[CHOOSE_MUSIC] = { static_cast<int> (-screenUnit * 2.5), 0, screenUnit * 10, screenUnit * 9 };
	colorStripDstRect[GAME] = { static_cast<int> (-screenUnit * 2.5), 0, screenUnit * 10, screenUnit * 9 };
}

void MenuTexture::createMenuRect() {
	menuDstRect = new SDL_Rect[TOTAL_BUTTON];
	menuDstRect[PLAY] = { static_cast<int>(screenUnit * 11.5), static_cast<int>(screenUnit * 3.5) , screenUnit * 3, screenUnit * 1 };
	menuDstRect[SETTING] = { static_cast<int>(screenUnit * 11), static_cast<int>(screenUnit * 5) , screenUnit * 4, screenUnit * 1 };
	menuDstRect[EXIT] = { static_cast<int>(screenUnit * 11.5), static_cast<int>(screenUnit * 6.5) , screenUnit * 3, screenUnit * 1 };
}

void ArrowTexture::createArrowRect()
{
	blankArrowDstRect = new SDL_Rect;
	arrowSrcRect = new SDL_Rect[TOTAL_ARROW];
	pressedArrowDstRect = new SDL_Rect[TOTAL_ARROW];

	//texture to render from image
	arrowSrcRect[LEFT_ARROW] = { 0, 0,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[UP_ARROW] = { arrow.getWidth() / 2, 0,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[DOWN_ARROW] = { 0,  arrow.getHeight() / 2,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[RIGHT_ARROW] = { arrow.getWidth() / 2,  arrow.getHeight() / 2,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };

	//texture to render to screen
	*blankArrowDstRect = { screenUnit, screenUnit, screenUnit * 7, screenUnit };
	pressedArrowDstRect[LEFT_ARROW] = { screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[UP_ARROW] = { 3 * screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[DOWN_ARROW] = { 5 * screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[RIGHT_ARROW] = { 7 * screenUnit, screenUnit, screenUnit, screenUnit };
}

void DogeTexture::createDogeRect() {
	muscleDogeRect = new SDL_Rect[TOTAL_DOGE_RECT];
	cheemsSrcRect = new SDL_Rect[TOTAL_ARROW];
	cheemsDstRect = new SDL_Rect[TOTAL_ARROW];


	//texture to render from image
	muscleDogeRect[SOURCE_LEFT] = { 0, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[SOURCE_IDLE] = { muscleDoge.getWidth() / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[SOURCE_RIGHT] = { muscleDoge.getWidth() * 2 / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	cheemsSrcRect[LEFT_ARROW] = { 0, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[UP_ARROW] = { 0,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[DOWN_ARROW] = { cheems.getWidth() / 2,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[RIGHT_ARROW] = { cheems.getWidth() / 2, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };

	//texture to render to screen
	muscleDogeRect[DESTINATION] = { static_cast<int>(screenUnit * 10), screenUnit * 3, screenUnit * 4, screenUnit * 4 };
	cheemsDstRect[LEFT_ARROW] = { static_cast<int>(screenUnit * 9.5), screenUnit * 5, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[UP_ARROW] = { static_cast<int>(screenUnit * 10.5), screenUnit * 6, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[DOWN_ARROW] = { static_cast<int>(screenUnit * 12), screenUnit * 6, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[RIGHT_ARROW] = { static_cast<int>(screenUnit * 13), screenUnit * 5, static_cast<int>(screenUnit * 1.5), screenUnit * 2 };

}

//Rendering function
void Background::renderColorStrip(int backgroundType) {
	moveColorStrip();
	renderColorStrip(colorStripDstRect[backgroundType]);
}

void Background::renderColorStrip(SDL_Rect& rectangle) {
	for (int i = 0; i < TOTAL_COLOR_STRIP; i++) {
		if (i % 2 == 1) {
			rectangle.y = screenHeight - rectangle.y;
		}
		colorStrip[i].render(renderer, &rectangle, NULL);
		rectangle.y -= screenHeight;
		colorStrip[i].render(renderer, &rectangle, NULL);
		rectangle.y += screenHeight;
		if (i % 2 == 1) {
			rectangle.y = screenHeight - rectangle.y;
		}
	}
}

void Background::renderBackground(int backgroundType) {
	background.render(renderer, &bgDestinationRect[backgroundType], &bgSourceRect[backgroundType]);
}

void ArrowTexture::renderPressedArrow(Event& event)
{
	SDL_Event e = event.getEvent();
	const Uint8* keyState = event.getKeyState();
	int keyType;
	blankArrow.render(renderer, blankArrowDstRect, NULL);
	if (keyState[SDL_SCANCODE_LEFT]) {
		pressedArrow.render(renderer, &pressedArrowDstRect[LEFT_ARROW], &arrowSrcRect[LEFT_ARROW]);
		if (e.key.keysym.sym == SDLK_LEFT && e.key.repeat == 0) {
			keyType = LEFT_ARROW;
			scoreCheck(keyType);
		}
	}
	if (keyState[SDL_SCANCODE_UP]) {
		pressedArrow.render(renderer, &pressedArrowDstRect[UP_ARROW], &arrowSrcRect[UP_ARROW]);
		if (e.key.keysym.sym == SDLK_UP && e.key.repeat == 0) {
			keyType = UP_ARROW;
			scoreCheck(keyType);
		}
	}
	if (keyState[SDL_SCANCODE_DOWN]) {
		pressedArrow.render(renderer, &pressedArrowDstRect[DOWN_ARROW], &arrowSrcRect[DOWN_ARROW]);
		if (e.key.keysym.sym == SDLK_DOWN && e.key.repeat == 0) {
			keyType = DOWN_ARROW;
			scoreCheck(keyType);
		}
	}
	if (keyState[SDL_SCANCODE_RIGHT]) {
		pressedArrow.render(renderer, &pressedArrowDstRect[RIGHT_ARROW], &arrowSrcRect[RIGHT_ARROW]);
		if (e.key.keysym.sym == SDLK_RIGHT && e.key.repeat == 0) {
			keyType = RIGHT_ARROW;
			scoreCheck(keyType);
		}
	}
}

void ArrowTexture::renderArrowRange()
{
	for (int i = 0; i < arrowRange.size(); i++) {
		SDL_Rect renderRect = arrowRange[i].getRect(screenUnit, screenUnit);
		int arrowCol = arrowRange[i].getArrowCol(screenUnit);
		arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
	}
}

void DogeTexture::renderDogeScreen(Event& event)
{
	SDL_Event e = event.getEvent();
	const Uint8* keyState = event.getKeyState();
	if (keyState[SDL_SCANCODE_LEFT]) {
		if (e.key.repeat == 0) {
			playBonkSound();
		}
		smashedCheems.render(renderer, &cheemsDstRect[LEFT_ARROW], &cheemsSrcRect[LEFT_ARROW]);
		muscleDoge.render(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_LEFT]);
		cheems.render(renderer, &cheemsDstRect[RIGHT_ARROW], &cheemsSrcRect[RIGHT_ARROW]);
		cheems.render(renderer, &cheemsDstRect[UP_ARROW], &cheemsSrcRect[UP_ARROW]);
		cheems.render(renderer, &cheemsDstRect[DOWN_ARROW], &cheemsSrcRect[DOWN_ARROW]);
	}
	else {
		if (keyState[SDL_SCANCODE_UP]) {
			if (e.key.repeat == 0) {
				playBonkSound();
			}
			muscleDoge.render(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
			cheems.render(renderer, &cheemsDstRect[LEFT_ARROW], &cheemsSrcRect[LEFT_ARROW]);
			cheems.render(renderer, &cheemsDstRect[RIGHT_ARROW], &cheemsSrcRect[RIGHT_ARROW]);
			smashedCheems.render(renderer, &cheemsDstRect[UP_ARROW], &cheemsSrcRect[UP_ARROW]);
			cheems.render(renderer, &cheemsDstRect[DOWN_ARROW], &cheemsSrcRect[DOWN_ARROW]);
		}
		else {
			if (keyState[SDL_SCANCODE_DOWN]) {
				if (e.key.repeat == 0) {
					playBonkSound();
				}
				muscleDoge.render(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
				cheems.render(renderer, &cheemsDstRect[LEFT_ARROW], &cheemsSrcRect[LEFT_ARROW]);
				cheems.render(renderer, &cheemsDstRect[RIGHT_ARROW], &cheemsSrcRect[RIGHT_ARROW]);
				cheems.render(renderer, &cheemsDstRect[UP_ARROW], &cheemsSrcRect[UP_ARROW]);
				smashedCheems.render(renderer, &cheemsDstRect[DOWN_ARROW], &cheemsSrcRect[DOWN_ARROW]);
			}
			else {
				if (keyState[SDL_SCANCODE_RIGHT]) {
					if (e.key.repeat == 0) {
						playBonkSound();
					}
					smashedCheems.render(renderer, &cheemsDstRect[RIGHT_ARROW], &cheemsSrcRect[RIGHT_ARROW]);
					muscleDoge.render(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_RIGHT]);
					cheems.render(renderer, &cheemsDstRect[LEFT_ARROW], &cheemsSrcRect[LEFT_ARROW]);
					cheems.render(renderer, &cheemsDstRect[UP_ARROW], &cheemsSrcRect[UP_ARROW]);
					cheems.render(renderer, &cheemsDstRect[DOWN_ARROW], &cheemsSrcRect[DOWN_ARROW]);
				}
				else {
					muscleDoge.render(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
					cheems.render(renderer, &cheemsDstRect[LEFT_ARROW], &cheemsSrcRect[LEFT_ARROW]);
					cheems.render(renderer, &cheemsDstRect[RIGHT_ARROW], &cheemsSrcRect[RIGHT_ARROW]);
					cheems.render(renderer, &cheemsDstRect[UP_ARROW], &cheemsSrcRect[UP_ARROW]);
					cheems.render(renderer, &cheemsDstRect[DOWN_ARROW], &cheemsSrcRect[DOWN_ARROW]);
				}
			}
		}
	}
}