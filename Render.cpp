#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Graphic.h"
#include "Screen.h"
using namespace std;

//this file contains contents and functions to render

//functions to create the texture to print

void Background::createDefaultRect() 
{
	delete[] bgSourceRect;
	delete[] bgDestinationRect;
	delete[] colorStripDstRect;

	int menu = toInt(BACKGROUND::MENU),
		setting = toInt(BACKGROUND::SETTING),
		chooseMusic = toInt(BACKGROUND::CHOOSE_MUSIC),
		game = toInt(BACKGROUND::GAME),
		exit = toInt(BACKGROUND::EXIT),
		total = toInt(BACKGROUND::TOTAL);

	bgSourceRect = new SDL_Rect[total];
	bgDestinationRect = new SDL_Rect[total];
	colorStripDstRect = new SDL_Rect[total];

	//Rectangles to render from source
	bgSourceRect[menu] = { 0, 0, background.getWidth(), background.getHeight() };
	bgSourceRect[setting] = { background.getHeight() * 6 / 9, background.getHeight() / 9, toInt(background.getWidth() * 4 / 9), toInt(background.getHeight() * 4 / 9) };
	bgSourceRect[chooseMusic] = { 0, 0, toInt(background.getWidth() * 2 / 3), toInt(background.getHeight() * 2 / 3) };
	bgSourceRect[game] = { 0, background.getHeight() * 2 / 9 , toInt(background.getWidth() * 2 / 3), toInt(background.getHeight() * 2 / 3) };
	bgSourceRect[exit] = { toInt(background.getWidth() * 4 / 9), 0, toInt(background.getWidth() * 5 / 9), toInt(background.getHeight() * 5 / 9) };

	//Rectangles to render to screen
	bgDestinationRect[menu] = { 0, 0, toInt(*screenUnit * 19 * 3 / 4) , *screenUnit * 9 };
	bgDestinationRect[setting] = { toInt(*screenUnit * 1.75), 0, toInt(*screenUnit * 19 * 3 / 4) , *screenUnit * 9 };
	bgDestinationRect[chooseMusic] = bgDestinationRect[setting];
	bgDestinationRect[game] = bgDestinationRect[chooseMusic];
	bgDestinationRect[exit] = bgDestinationRect[setting];
	colorStripDstRect[menu] = { *screenUnit * 8, 0, *screenUnit * 10, *screenUnit * 9 };
	colorStripDstRect[setting] = { toInt(-*screenUnit * 2.5), 0, *screenUnit * 10, *screenUnit * 9 };
	colorStripDstRect[chooseMusic] = colorStripDstRect[setting];
	colorStripDstRect[game] = colorStripDstRect[chooseMusic];
	colorStripDstRect[exit] = { -*screenUnit * 2, 0,  *screenUnit * 10, *screenUnit * 9 };
}

void Button::createDefaultRect()
{
	delete[] buttonDstRect;

	int playNumb = toInt(BUTTON::PLAY),
		continueNumb = toInt(BUTTON::CONTINUE),
		saveNumb = toInt(BUTTON::SAVE),
		settingNumb = toInt(BUTTON::SETTING),
		exitNumb = toInt(BUTTON::EXIT),
		changeSongLeft = toInt(BUTTON::CHANGE_SONG_LEFT),
		changeSongRight = toInt(BUTTON::CHANGE_SONG_RIGHT);
	buttonDstRect = new SDL_Rect[toInt(BUTTON::TOTAL)];
	buttonChoose = -1;
	for (int button = 0; button < toInt(BUTTON::TOTAL); button++) {
		usingButton[button] = false;
	}

	buttonDstRect[playNumb] = { *screenWidth, *screenHeight, 0, 0 };
	buttonDstRect[continueNumb] = { *screenWidth, *screenHeight, 0, 0 };
	buttonDstRect[saveNumb] = { *screenWidth, *screenHeight, 0, 0 };
	buttonDstRect[settingNumb] = { *screenWidth, *screenHeight, 0, 0 };
	buttonDstRect[exitNumb] = { *screenWidth, *screenHeight, 0, 0 };
	buttonDstRect[changeSongLeft] = { *screenWidth, *screenHeight, 0, 0 };
	buttonDstRect[changeSongRight] = { *screenWidth, *screenHeight, 0, 0 };
}

void Button::createMenuRect() {
	createDefaultRect();
	int playNumb = toInt(BUTTON::PLAY),
		settingNumb = toInt(BUTTON::SETTING),
		exitNumb = toInt(BUTTON::EXIT);

	buttonDstRect[playNumb] = { toInt(*screenUnit * 11.5), toInt(*screenUnit * 3.5) , *screenUnit * 3, *screenUnit * 1 };
	buttonDstRect[settingNumb] = { toInt(*screenUnit * 11), toInt(*screenUnit * 5) , *screenUnit * 4, *screenUnit * 1 };
	buttonDstRect[exitNumb] = { toInt(*screenUnit * 11.5), toInt(*screenUnit * 6.5) , *screenUnit * 3, *screenUnit * 1 };
	usingButton[playNumb] = true;
	usingButton[settingNumb] = true;
	usingButton[exitNumb] = true;
}

void Button::createChooseMusic()
{
	createDefaultRect();
	int playNumb = toInt(BUTTON::PLAY),
		exitNumb = toInt(BUTTON::EXIT),
		changeSongLeft = toInt(BUTTON::CHANGE_SONG_LEFT),
		changeSongRight = toInt(BUTTON::CHANGE_SONG_RIGHT);

	buttonDstRect[changeSongLeft] = { *screenUnit, *screenUnit, *screenUnit, *screenUnit };
	buttonDstRect[changeSongRight] = { *screenUnit * 4, *screenUnit, *screenUnit, *screenUnit };
	buttonDstRect[playNumb] = { toInt(*screenUnit * 1.5), toInt(*screenUnit * 5.5) , *screenUnit * 3, *screenUnit * 1 };
	buttonDstRect[exitNumb] = { toInt(*screenUnit * 1.5), toInt(*screenUnit * 7) , *screenUnit * 3, *screenUnit * 1 };
	usingButton[changeSongLeft] = true;
	usingButton[changeSongRight] = true;
	usingButton[playNumb] = true;
	usingButton[exitNumb] = true;
}

void Button::createScoreRect() {
	createDefaultRect();
	int exitNumb = toInt(BUTTON::EXIT);

	buttonDstRect[exitNumb] = { toInt(*screenUnit * 1.5), toInt(*screenUnit * 6.5) , *screenUnit * 3, *screenUnit * 1 };
	usingButton[exitNumb] = true;
}

void Button::createPauseRect() {
	createDefaultRect();
	int continueNumb = toInt(BUTTON::CONTINUE),
		settingNumb = toInt(BUTTON::SETTING),
		exitNumb = toInt(BUTTON::EXIT);

	buttonDstRect[continueNumb] = { toInt(*screenUnit * 0.75), toInt(*screenUnit * 3.5) , toInt(*screenUnit * 4.5), *screenUnit * 1 };
	buttonDstRect[settingNumb] = { toInt(*screenUnit * 1), toInt(*screenUnit * 5) , *screenUnit * 4, *screenUnit * 1 };
	buttonDstRect[exitNumb] = { toInt(*screenUnit * 1.5), toInt(*screenUnit * 6.5) , *screenUnit * 3, *screenUnit * 1 };
	usingButton[continueNumb] = true;
	usingButton[settingNumb] = true;
	usingButton[exitNumb] = true;
}

void Button::createSettingRect() {
	createDefaultRect();
	int saveNumb = toInt(BUTTON::SAVE),
		exitNumb = toInt(BUTTON::EXIT);

	buttonDstRect[saveNumb] = { toInt(*screenUnit * 1.75), toInt(*screenUnit * 5.5) , toInt(*screenUnit * 2.5), *screenUnit * 1 };
	buttonDstRect[exitNumb] = { toInt(*screenUnit * 1.5), toInt(*screenUnit * 7) , *screenUnit * 3, *screenUnit * 1 };
	usingButton[saveNumb] = true;
	usingButton[exitNumb] = true;

}

void Title::createDefaultRect()
{
	titleDstRect[toInt(TITLE::MENU)] = { *screenUnit * 10, *screenUnit, toInt(*screenUnit * 5.5), *screenUnit * 2 };
	titleDstRect[toInt(TITLE::PAUSE)] = { toInt(*screenUnit * 0.5), toInt(*screenUnit * 1.5), toInt(*screenUnit * 5), toInt(*screenUnit * 1.5) };
	titleDstRect[toInt(TITLE::FINISH)] = { *screenUnit * 9, *screenUnit * 1, toInt(*screenUnit * 6), *screenUnit * 2 };
	titleDstRect[toInt(TITLE::SETTING)] = { *screenUnit * 8, *screenUnit * 1, toInt(*screenUnit * 7), *screenUnit * 2 };
}

void Point::createDefaultRect() {
	delete pointRect;

	int pointNumb = toInt(POINT::POINT),
		totalArrowNumb = toInt(POINT::TOTAL_ARROW),
		accuracyNumb = toInt(POINT::ACCURACY),
		pressedArrowNumb = toInt(POINT::PRESSED_ARROW),
		maxComboNumb = toInt(POINT::MAX_COMBO),
		wrongPressedNumb = toInt(POINT::WRONG_PRESSED);
	pointRect = new SDL_Rect[toInt(POINT::TOTAL)];

	pointRect[pointNumb] = { *screenUnit, toInt(*screenUnit * 2), pointTexture[pointNumb].getWidth(), pointTexture[pointNumb].getHeight() };
	pointRect[totalArrowNumb] = { *screenUnit, toInt(*screenUnit * 8 / 3), pointTexture[totalArrowNumb].getWidth(), pointTexture[totalArrowNumb].getHeight() };
	pointRect[accuracyNumb] = { *screenUnit, toInt(*screenUnit * 10 / 3), pointTexture[accuracyNumb].getWidth(), pointTexture[accuracyNumb].getHeight() };
	pointRect[pressedArrowNumb] = { *screenUnit, toInt(*screenUnit * 4), pointTexture[pressedArrowNumb].getWidth(), pointTexture[pressedArrowNumb].getHeight() };
	pointRect[maxComboNumb] = { *screenUnit, toInt(*screenUnit * 14 / 3), pointTexture[maxComboNumb].getWidth(), pointTexture[maxComboNumb].getHeight() };
	pointRect[wrongPressedNumb] = { *screenUnit, toInt(*screenUnit * 16 / 3), pointTexture[wrongPressedNumb].getWidth(), pointTexture[wrongPressedNumb].getHeight() };
}

void ArrowTexture::createDefaultRect()
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
	*blankArrowDstRect = { *screenUnit, *screenUnit, *screenUnit * 7, *screenUnit };
	pressedArrowDstRect[left] = { *screenUnit, *screenUnit, *screenUnit, *screenUnit };
	pressedArrowDstRect[up] = { 3 * *screenUnit, *screenUnit, *screenUnit, *screenUnit };
	pressedArrowDstRect[down] = { 5 * *screenUnit, *screenUnit, *screenUnit, *screenUnit };
	pressedArrowDstRect[right] = { 7 * *screenUnit, *screenUnit, *screenUnit, *screenUnit };
}

void DogeTexture::createDefaultRect() {
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
	muscleDogeRect[toInt(DOGE::DESTINATION)] = { toInt(*screenUnit * 10), *screenUnit * 3, *screenUnit * 4, *screenUnit * 4 };
	cheemsDstRect[left] = { toInt(*screenUnit * 9.5), *screenUnit * 5, toInt(*screenUnit * 1.5), *screenUnit * 2 };
	cheemsDstRect[up] = { toInt(*screenUnit * 10.5), *screenUnit * 6, toInt(*screenUnit * 1.5), *screenUnit * 2 };
	cheemsDstRect[down] = { toInt(*screenUnit * 12), *screenUnit * 6, toInt(*screenUnit * 1.5), *screenUnit * 2 };
	cheemsDstRect[right] = { toInt(*screenUnit * 13), *screenUnit * 5, toInt(*screenUnit * 1.5), *screenUnit * 2 };

}

void SettingTexture::createDefaultRect() {

}

void MusicTexture::createDefaultRect()
{
	nameRect = { *screenUnit, *screenUnit * 7 / 3, nameText.getWidth(), nameText.getHeight() };
	authorRect = { *screenUnit, toInt(*screenUnit * 3), authorText.getWidth(), authorText.getHeight() };
	difficultyRect = { *screenUnit, toInt(*screenUnit * 11 / 3), difficultyText.getWidth(), difficultyText.getHeight() };
	bpmRect = { *screenUnit, toInt(*screenUnit * 13 / 3), bpmText.getWidth(), bpmText.getHeight() };
}



//Rendering function

void Point::renderPointScreen() {
	pointTexture[toInt(POINT::POINT)].render(renderer, &pointRect[toInt(POINT::POINT)], NULL);
	pointTexture[toInt(POINT::TOTAL_ARROW)].render(renderer, &pointRect[toInt(POINT::TOTAL_ARROW)], NULL);
	pointTexture[toInt(POINT::ACCURACY)].render(renderer, &pointRect[toInt(POINT::ACCURACY)], NULL);
	pointTexture[toInt(POINT::PRESSED_ARROW)].render(renderer, &pointRect[toInt(POINT::PRESSED_ARROW)], NULL);
	pointTexture[toInt(POINT::MAX_COMBO)].render(renderer, &pointRect[toInt(POINT::MAX_COMBO)], NULL);
	pointTexture[toInt(POINT::WRONG_PRESSED)].render(renderer, &pointRect[toInt(POINT::WRONG_PRESSED)], NULL);
}

void ArrowTexture::renderPressedArrow(Event& event, Point& point)
{
	for (int type = 0; type < toInt(CONTROL::TOTAL_ARROW); type++) {
		if (event.checkKeyState(static_cast<CONTROL>(type))) {
			pressedArrow.render(renderer, &pressedArrowDstRect[type], &arrowSrcRect[type]);
			if (!event.checkKeyRepeat() && event.checkKeyEventDown(static_cast<CONTROL>(type))) {
				scoreCheck(type, point);
			}
		}
	}
}

void DogeTexture::renderDoge(Event& event)
{
	int left = toInt(CONTROL::LEFT_ARROW),
		up = toInt(CONTROL::UP_ARROW),
		down = toInt(CONTROL::DOWN_ARROW),
		right = toInt(CONTROL::RIGHT_ARROW);
	 
	if (event.checkKeyState(CONTROL::LEFT_ARROW)) {
		if (!event.checkKeyRepeat()) {
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
			if (!event.checkKeyRepeat()) {
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
				if (!event.checkKeyRepeat()) {
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
					if (!event.checkKeyRepeat()) {
						playBonkSound();
					}
					smashedCheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
					cheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
					muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_RIGHT)]);
					cheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
					cheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
				}
				else {
					render();
				}
			}
		}
	}
}
