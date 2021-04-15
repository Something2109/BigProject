#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Background.h"
#include "Menu.h"
#include "Game.h"
using namespace std;

//this file contains contents and functions to render

//functions to create the texture to print

void Background::createRect() 
{
	delete[] bgSourceRect;
	delete[] bgDestinationRect;

	int menu = toInt(BACKGROUND::MENU),
		chooseMusic = toInt(BACKGROUND::CHOOSE_MUSIC),
		game = toInt(BACKGROUND::GAME),
		total = toInt(BACKGROUND::TOTAL);

	bgSourceRect = new SDL_Rect[total];
	bgDestinationRect = new SDL_Rect[total];
	colorStripDstRect = new SDL_Rect[total];

	//Rectangles to render from source
	bgSourceRect[menu] = { 0, 0, background.getWidth(), background.getHeight() };
	bgSourceRect[chooseMusic] = { 0, 0, toInt(background.getWidth() * 2 / 3), toInt(background.getHeight() * 2 / 3) };
	bgSourceRect[game] = { 0, background.getHeight() * 2 / 9 , toInt(background.getWidth() * 2 / 3), toInt(background.getHeight() * 2 / 3) };

	//Rectangles to render to screen
	bgDestinationRect[menu] = { 0, 0, toInt(screenUnit * 19 * 3 / 4) , screenUnit * 9 };
	bgDestinationRect[chooseMusic] = { toInt(screenUnit * 1.75), 0, toInt(screenUnit * 19 * 3 / 4) , screenUnit * 9 };
	bgDestinationRect[game] = bgDestinationRect[chooseMusic];
	colorStripDstRect[menu] = { screenUnit * 8, 0, screenUnit * 10, screenUnit * 9 };
	colorStripDstRect[chooseMusic] = { toInt(- screenUnit * 2.5), 0, screenUnit * 10, screenUnit * 9 };
	colorStripDstRect[game] = colorStripDstRect[chooseMusic];
}

void MenuTexture::createRect() {
	delete[] menuDstRect;
	
	int play = toInt(MENU::PLAY),
		setting = toInt(MENU::SETTING),
		exit = toInt(MENU::EXIT);
	menuDstRect = new SDL_Rect[toInt(MENU::TOTAL)];

	menuDstRect[play] = { toInt(screenUnit * 11.5), toInt(screenUnit * 3.5) , screenUnit * 3, screenUnit * 1 };
	menuDstRect[setting] = { toInt(screenUnit * 11), toInt(screenUnit * 5) , screenUnit * 4, screenUnit * 1 };
	menuDstRect[exit] = { toInt(screenUnit * 11.5), toInt(screenUnit * 6.5) , screenUnit * 3, screenUnit * 1 };
}

void ArrowTexture::createRect()
{
	delete blankArrowDstRect;
	delete[] arrowSrcRect;
	delete[] pressedArrowDstRect;

	int total = toInt(CONTROL::TOTAL_ARROW),
		left = toInt(CONTROL::LEFT_ARROW),
		up = toInt(CONTROL::UP_ARROW),
		down = toInt(CONTROL::DOWN_ARROW),
		right = toInt(CONTROL::RIGHT_ARROW);
	blankArrowDstRect = new SDL_Rect;
	arrowSrcRect = new SDL_Rect[total];
	pressedArrowDstRect = new SDL_Rect[total];

	//texture to render from image
	arrowSrcRect[left] = { 0, 0,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[up] = { arrow.getWidth() / 2, 0,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[down] = { 0,  arrow.getHeight() / 2,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };
	arrowSrcRect[right] = { arrow.getWidth() / 2,  arrow.getHeight() / 2,  arrow.getWidth() / 2,  arrow.getHeight() / 2 };

	//texture to render to screen
	*blankArrowDstRect = { screenUnit, screenUnit, screenUnit * 7, screenUnit };
	pressedArrowDstRect[left] = { screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[up] = { 3 * screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[down] = { 5 * screenUnit, screenUnit, screenUnit, screenUnit };
	pressedArrowDstRect[right] = { 7 * screenUnit, screenUnit, screenUnit, screenUnit };
}

void DogeTexture::createRect() {
	delete[] muscleDogeRect;
	delete[] cheemsSrcRect;
	delete[] cheemsDstRect;

	int total = toInt(CONTROL::TOTAL_ARROW),
		left = toInt(CONTROL::LEFT_ARROW),
		up = toInt(CONTROL::UP_ARROW),
		down = toInt(CONTROL::DOWN_ARROW),
		right = toInt(CONTROL::RIGHT_ARROW);
	muscleDogeRect = new SDL_Rect[toInt(DOGE::TOTAL)];
	cheemsSrcRect = new SDL_Rect[total];
	cheemsDstRect = new SDL_Rect[total];

	//texture to render from image
	muscleDogeRect[toInt(DOGE::SOURCE_LEFT)] = { 0, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[toInt(DOGE::SOURCE_IDLE)] = { muscleDoge.getWidth() / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[toInt(DOGE::SOURCE_RIGHT)] = { muscleDoge.getWidth() * 2 / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	cheemsSrcRect[left] = { 0, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[up] = { 0,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[down] = { cheems.getWidth() / 2,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[right] = { cheems.getWidth() / 2, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };

	//texture to render to screen
	muscleDogeRect[toInt(DOGE::DESTINATION)] = { toInt(screenUnit * 10), screenUnit * 3, screenUnit * 4, screenUnit * 4 };
	cheemsDstRect[left] = { toInt(screenUnit * 9.5), screenUnit * 5, toInt(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[up] = { toInt(screenUnit * 10.5), screenUnit * 6, toInt(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[down] = { toInt(screenUnit * 12), screenUnit * 6, toInt(screenUnit * 1.5), screenUnit * 2 };
	cheemsDstRect[right] = { toInt(screenUnit * 13), screenUnit * 5, toInt(screenUnit * 1.5), screenUnit * 2 };

}

void Point::createPointRect() {
	delete pointRect;

	int point = toInt(POINT::POINT),
		totalArrow = toInt(POINT::TOTAL_ARROW),
		accuracy = toInt(POINT::ACCURACY),
		pressedArrow = toInt(POINT::PRESSED_ARROW),
		wrongPressed = toInt(POINT::WRONG_PRESSED);
	pointRect = new SDL_Rect[toInt(POINT::TOTAL)];

	pointRect[point] = { screenUnit, toInt(screenUnit * 4), pointText.getWidth(), pointText.getHeight() };
	pointRect[totalArrow] = { screenUnit, toInt(screenUnit * 14 / 3), totalArrowText.getWidth(), totalArrowText.getHeight() };
	pointRect[accuracy] = { screenUnit, toInt(screenUnit * 16 / 3), accuracyText.getWidth(), accuracyText.getHeight() };
	pointRect[pressedArrow] = { screenUnit, toInt(screenUnit * 6), pressedArrowText.getWidth(), pressedArrowText.getHeight() };
	pointRect[wrongPressed] = { screenUnit, toInt(screenUnit * 20 / 3), wrongPressedText.getWidth(), wrongPressedText.getHeight() };
}

//Rendering function
void ArrowTexture::renderPressedArrow(Event& event, Point& point)
{
	blankArrow.render(renderer, blankArrowDstRect, NULL);
	for (int type = 0; type < toInt(CONTROL::TOTAL_ARROW); type++) {
		if (event.checkKeyState(static_cast<CONTROL>(type))) {
			pressedArrow.render(renderer, &pressedArrowDstRect[type], &arrowSrcRect[type]);
			if (event.checkEvent(static_cast<CONTROL>(type)) && !event.checkRepeat()) {
				scoreCheck(type, point);
			}
		}
	}
}

void ArrowTexture::renderArrowRange()
{
	for (int i = 0; i < arrowRange.size(); i++) {
		SDL_Rect renderRect = { arrowRange[i].x, arrowRange[i].y, screenUnit, screenUnit };
		int arrowCol = (arrowRange[i].x / screenUnit - 1) / 2;
		arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
	}
}

void DogeTexture::render(Event& event)
{
	int left = toInt(CONTROL::LEFT_ARROW),
		up = toInt(CONTROL::UP_ARROW),
		down = toInt(CONTROL::DOWN_ARROW),
		right = toInt(CONTROL::RIGHT_ARROW);

	if (event.checkKeyState(CONTROL::LEFT_ARROW)) {
		if (!event.checkRepeat()) {
			playBonkSound();
		}
		smashedCheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
		cheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
		muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_LEFT)]);
		cheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
		cheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
	}
	else {
		if (event.checkKeyState(CONTROL::UP_ARROW)) {
			if (!event.checkRepeat()) {
				playBonkSound();
			}
			cheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
			cheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
			muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_IDLE)]);
			smashedCheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
			cheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
		}
		else {
			if (event.checkKeyState(CONTROL::DOWN_ARROW)) {
				if (!event.checkRepeat()) {
					playBonkSound();
				}
				cheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
				cheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
				muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_IDLE)]);
				cheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
				smashedCheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
			}
			else {
				if (event.checkKeyState(CONTROL::RIGHT_ARROW)) {
					if (!event.checkRepeat()) {
						playBonkSound();
					}
					smashedCheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
					cheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
					muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_RIGHT)]);
					cheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
					cheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
				}
				else {
					cheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
					cheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
					muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_IDLE)]);
					cheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
					cheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
				}
			}
		}
	}
}

void Point::renderGamePoint() {
	pointText.free();
	if (pointText.loadFromRenderedText(renderer, ("Point: " + to_string(point)), pointColor, pointFont)) {
		*pointRect = { screenUnit * 10 , screenUnit, pointText.getWidth(), pointText.getHeight() };
		pointText.render(renderer, pointRect, NULL);
	}
}

void Point::renderPointScreen() {
	pointText.render(renderer, &pointRect[toInt(POINT::POINT)], NULL);
	totalArrowText.render(renderer, &pointRect[toInt(POINT::TOTAL_ARROW)], NULL);
	accuracyText.render(renderer, &pointRect[toInt(POINT::ACCURACY)], NULL);
	pressedArrowText.render(renderer, &pointRect[toInt(POINT::PRESSED_ARROW)], NULL);
	wrongPressedText.render(renderer, &pointRect[toInt(POINT::WRONG_PRESSED)], NULL);
}
