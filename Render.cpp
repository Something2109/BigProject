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

	int intro1 = toInt(BACKGROUND::INTRO1),
		intro2 = toInt(BACKGROUND::INTRO2),
		intro3 = toInt(BACKGROUND::INTRO3),
		start = toInt(BACKGROUND::START),
		menu = toInt(BACKGROUND::MENU),
		setting = toInt(BACKGROUND::SETTING),
		chooseMusic = toInt(BACKGROUND::CHOOSE_MUSIC),
		game = toInt(BACKGROUND::GAME),
		exit = toInt(BACKGROUND::EXIT),
		total = toInt(BACKGROUND::TOTAL);

	yColorStripVelo = *screenUnit / 20;
	bgSourceRect = new SDL_Rect[total];
	bgDestinationRect = new SDL_Rect[total];
	colorStripDstRect = new SDL_Rect[total];

	//Rectangles to render from source
	bgSourceRect[intro1] = { toInt(background->getHeight() * 5.75 / 9), background->getHeight() / 18, toInt(background->getWidth() / 9), toInt(background->getHeight() / 9) };
	bgSourceRect[intro2] = { background->getHeight() * 5 / 9, 0, toInt(background->getWidth() * 2 / 9), toInt(background->getHeight() * 2 / 9) };
	bgSourceRect[intro3] = { background->getHeight() / 3, background->getHeight() / 3 , toInt(background->getWidth() * 2 / 3), toInt(background->getHeight() * 2 / 3) };
	bgSourceRect[start] = { background->getHeight() * 6 / 9, background->getHeight() / 9, toInt(background->getWidth() * 4 / 9), toInt(background->getHeight() * 4 / 9) };
	bgSourceRect[menu] = { 0, 0, background->getWidth(), background->getHeight() };
	bgSourceRect[setting] = bgSourceRect[start];
	bgSourceRect[chooseMusic] = { 0, 0, toInt(background->getWidth() * 2 / 3), toInt(background->getHeight() * 2 / 3) };
	bgSourceRect[game] = { 0, background->getHeight() * 2 / 9 , toInt(background->getWidth() * 2 / 3), toInt(background->getHeight() * 2 / 3) };
	bgSourceRect[exit] = { toInt(background->getWidth() * 4 / 9), 0, toInt(background->getWidth() * 5 / 9), toInt(background->getHeight() * 5 / 9) };

	//Rectangles to render to screen
	bgDestinationRect[intro1] = setScreenUnitRect(0, 0, 14.25, 9);
	bgDestinationRect[intro2] = setScreenUnitRect(1.75, 0, 14.25, 9);
	bgDestinationRect[intro3] = bgDestinationRect[intro1];
	bgDestinationRect[start] = bgDestinationRect[intro2];
	bgDestinationRect[menu] = bgDestinationRect[intro1];
	bgDestinationRect[setting] = bgDestinationRect[intro2];
	bgDestinationRect[chooseMusic] = bgDestinationRect[intro2];
	bgDestinationRect[game] = bgDestinationRect[intro2];
	bgDestinationRect[exit] = bgDestinationRect[intro2];

	colorStripDstRect[intro1] = setScreenUnitRect(13, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[intro2] = setScreenUnitRect(-7, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[intro3] = setScreenUnitRect(10.5, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[start] = setScreenUnitRect(-2, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[menu] = setScreenUnitRect(8, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[setting] = setScreenUnitRect(-2, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[chooseMusic] = setScreenUnitRect(-2.5, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[game] = colorStripDstRect[chooseMusic];
	colorStripDstRect[exit] = setScreenUnitRect(-2, 9 * currentColorPlace, 10, 9);
}

void Button::createDefaultRect()
{
	delete[] buttonDstRect;

	buttonDstRect = new SDL_Rect[static_cast<int>(BUTTON::TOTAL)];
	buttonChoose = -1;
	for (int button = 0; button < toInt(BUTTON::TOTAL); button++) {
		buttonDstRect[button] = setScreenUnitRect(-1, -1, 0, 0);
		usingButton[button] = false;
	}
}

void Button::createRect(TITLE type)
{
	createDefaultRect();
	switch(type) {
		case TITLE::MENU: {
			createMenuRect();
			break;
		}
		case TITLE::CHOOSE_MUSIC: {
			createChooseMusic();
			break;
		}
		case TITLE::PAUSE: {
			createPauseRect();
			break;
		}
		case TITLE::FINISH: {
			createScoreRect();
			break;
		}
		case TITLE::SETTING: {
			createSettingRect();
			break;
		}
	}
}

void Button::createMenuRect() {
	int playNumb = toInt(BUTTON::PLAY),
		settingNumb = toInt(BUTTON::SETTING),
		exitNumb = toInt(BUTTON::EXIT),
		pressKeyNumb = toInt(BUTTON::PRESS_KEY);

	buttonDstRect[playNumb] = setScreenUnitRect(11.5, 3.5 , 3, 1);
	buttonDstRect[settingNumb] = setScreenUnitRect(11, 5, 4, 1);
	buttonDstRect[exitNumb] = setScreenUnitRect(11.5, 6.5, 3, 1);
	buttonDstRect[pressKeyNumb] = setScreenUnitRect(0.5, 6, 5.5, 1);
	usingButton[playNumb] = true;
	usingButton[settingNumb] = true;
	usingButton[exitNumb] = true;
}

void Button::createChooseMusic()
{
	int playNumb = toInt(BUTTON::PLAY),
		exitNumb = toInt(BUTTON::EXIT),
		changeSongLeft = toInt(BUTTON::CHANGE_SONG_LEFT),
		changeSongRight = toInt(BUTTON::CHANGE_SONG_RIGHT);

	buttonDstRect[changeSongLeft] = setScreenUnitRect(0.75, 1, 1, 1);
	buttonDstRect[changeSongRight] = setScreenUnitRect(3.75, 1, 1, 1);
	buttonDstRect[playNumb] = setScreenUnitRect(1.25, 5.5, 3, 1);
	buttonDstRect[exitNumb] = setScreenUnitRect(1.25, 7, 3, 1);
	usingButton[changeSongLeft] = true;
	usingButton[changeSongRight] = true;
	usingButton[playNumb] = true;
	usingButton[exitNumb] = true;
}

void Button::createScoreRect() {
	int exitNumb = toInt(BUTTON::EXIT);

	buttonDstRect[exitNumb] = setScreenUnitRect(1.5, 6.5, 3, 1);;
	usingButton[exitNumb] = true;
}

void Button::createPauseRect() {
	int continueNumb = toInt(BUTTON::CONTINUE),
		settingNumb = toInt(BUTTON::SETTING),
		exitNumb = toInt(BUTTON::EXIT);

	buttonDstRect[continueNumb] = setScreenUnitRect(0.75, 3.5, 4.5, 1);
	buttonDstRect[settingNumb] = setScreenUnitRect(1, 5, 4, 1);
	buttonDstRect[exitNumb] = setScreenUnitRect(1.5, 6.5, 3, 1);
	usingButton[continueNumb] = true;
	usingButton[settingNumb] = true;
	usingButton[exitNumb] = true;
}

void Button::createSettingRect() {
	int saveNumb = toInt(BUTTON::SAVE),
		exitNumb = toInt(BUTTON::EXIT),
		pressKeyNumb = toInt(BUTTON::PRESS_KEY);

	buttonDstRect[saveNumb] = setScreenUnitRect(2, 5.5, 2.5, 1);
	buttonDstRect[exitNumb] = setScreenUnitRect(1.75, 7, 3, 1);
	buttonDstRect[pressKeyNumb] = setScreenUnitRect(0.5, 5.5, 5.5, 1);
	usingButton[exitNumb] = true;

}

void Title::createDefaultRect()
{
	delete[] titleDstRect;

	titleDstRect = new SDL_Rect[static_cast<int>(TITLE::TOTAL)];

	titleDstRect[toInt(TITLE::INTRO1)] = setScreenUnitRect(6.5, 3, 5, 3);
	titleDstRect[toInt(TITLE::MENU)] = setScreenUnitRect(10, 1, 5.5, 2);
	titleDstRect[toInt(TITLE::CHOOSE_MUSIC)] = setScreenUnitRect(8, 1, 7, 1.5);
	titleDstRect[toInt(TITLE::PAUSE)] = setScreenUnitRect(0.5, 1.5, 5, 1.5);
	titleDstRect[toInt(TITLE::FINISH)] = setScreenUnitRect(9, 1, 6, 2);
	titleDstRect[toInt(TITLE::SETTING)] = setScreenUnitRect(8.5, 1, 7, 2);
	titleDstRect[toInt(TITLE::EXIT)] = setScreenUnitRect(0.5, 2, 5.5, 2);
}

void Point::createDefaultRect() {
	delete[] pointRect;

	int pointNumb = toInt(POINT::POINT),
		totalArrowNumb = toInt(POINT::TOTAL_ARROW),
		accuracyNumb = toInt(POINT::ACCURACY),
		pressedArrowNumb = toInt(POINT::PRESSED_ARROW),
		maxComboNumb = toInt(POINT::MAX_COMBO),
		wrongPressedNumb = toInt(POINT::WRONG_PRESSED);
	pointRect = new SDL_Rect[toInt(POINT::TOTAL)];

	pointRect[pointNumb] = setTextRect(1, 2, pointTexture[pointNumb].getWidth(), pointTexture[pointNumb].getHeight() );
	pointRect[totalArrowNumb] = setTextRect(1, 8.0 / 3, pointTexture[totalArrowNumb].getWidth(), pointTexture[totalArrowNumb].getHeight() );
	pointRect[accuracyNumb] = setTextRect(1, 10.0 / 3, pointTexture[accuracyNumb].getWidth(), pointTexture[accuracyNumb].getHeight() );
	pointRect[pressedArrowNumb] = setTextRect(1, 4, pointTexture[pressedArrowNumb].getWidth(), pointTexture[pressedArrowNumb].getHeight() );
	pointRect[maxComboNumb] = setTextRect(1, 14.0 / 3, pointTexture[maxComboNumb].getWidth(), pointTexture[maxComboNumb].getHeight() );
	pointRect[wrongPressedNumb] = setTextRect(1, 16.0 / 3, pointTexture[wrongPressedNumb].getWidth(), pointTexture[wrongPressedNumb].getHeight() );
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
	*blankArrowDstRect = setScreenUnitRect(1, 1, 7, 1);
	pressedArrowDstRect[left] = setScreenUnitRect(1, 1, 1, 1);
	pressedArrowDstRect[up] = setScreenUnitRect(3, 1, 1, 1);
	pressedArrowDstRect[down] = setScreenUnitRect(5, 1, 1, 1);
	pressedArrowDstRect[right] = setScreenUnitRect(7, 1, 1, 1);
}

void Character::createDefaultRect() {
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
	khaBankSrcRect = new SDL_Rect[total];
	khaBankDstRect = new SDL_Rect;
	bankVelo = *screenUnit / 8;

	//texture to render from image
	muscleDogeRect[toInt(DOGE::SOURCE_LEFT)] = { 0, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[toInt(DOGE::SOURCE_IDLE)] = { muscleDoge.getWidth() / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	muscleDogeRect[toInt(DOGE::SOURCE_RIGHT)] = { muscleDoge.getWidth() * 2 / 3, 0,  muscleDoge.getWidth() / 3,  muscleDoge.getHeight() };
	cheemsSrcRect[left] = { 0, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[up] = { 0,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[down] = { cheems.getWidth() / 2,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[right] = { cheems.getWidth() / 2, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	khaBankSrcRect[left] = { 0, 0,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };
	khaBankSrcRect[up] = { khaBank.getWidth() / 2, 0,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };
	khaBankSrcRect[down] = { 0, khaBank.getHeight() / 2,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };
	khaBankSrcRect[right] = { khaBank.getWidth() / 2,  khaBank.getHeight() / 2,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };

	//texture to render to screen
	muscleDogeRect[toInt(DOGE::DESTINATION)] = setScreenUnitRect(10, 3, 4, 4);
	cheemsDstRect[left] = setScreenUnitRect(9.5, 5, 1.5, 2);
	cheemsDstRect[up] = setScreenUnitRect(10.5, 6, 1.5, 2);
	cheemsDstRect[down] = setScreenUnitRect(12, 6, 1.5, 2);
	cheemsDstRect[right] = setScreenUnitRect(13, 5, 1.5, 2);
	*khaBankDstRect = setScreenUnitRect(16, 5, 2.5, 4);
}

void SettingTexture::createDefaultRect() {
	delete[] buttonDstRect;
	delete[] usingButton;

	buttonDstRect = new SDL_Rect[static_cast<int>(CONTROL::TOTAL)];
	usingButton = new bool[static_cast<int>(CONTROL::TOTAL)];

	for (int type = 0; type < toInt(CONTROL::TOTAL); type++) {
		buttonDstRect[type] = setTextRect(1, (type * 2 + 3) / 3.0, button[type].getWidth(), button[type].getHeight() );
		usingButton[type] = true;
	}
}

void MusicTexture::createDefaultRect()
{
	nameRect = setTextRect( 0.75, 7.0 / 3, nameText.getWidth(), nameText.getHeight() );
	authorRect = setTextRect(0.75, 3, authorText.getWidth(), authorText.getHeight() );
	difficultyRect = setTextRect(0.75, 11.0 / 3, difficultyText.getWidth(), difficultyText.getHeight() );
	bpmRect = setTextRect(0.75, 13.0 / 3, bpmText.getWidth(), bpmText.getHeight() );
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

void Character::renderDoge(Event& event)
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

void Character::renderBank(Point& point) {
	bankRender = (point.getCombo() >= 50);
	if (bankRender || khaBankDstRect->x < *screenWidth) {
		frame = (frame + 1) % 60;
		if (!bankRender && khaBankDstRect->x < *screenWidth) {
			khaBankDstRect->x += bankVelo;
		}
		if (bankRender && khaBankDstRect->x > *screenUnit * 13.5) {
			khaBankDstRect->x -= bankVelo;
		}
		khaBank.render(renderer, khaBankDstRect, &khaBankSrcRect[abs(frame / 10 - 3)]);
	}
}