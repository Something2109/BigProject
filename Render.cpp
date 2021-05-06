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

//functions to create the default rectangles

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
		score = toInt(BACKGROUND::SCORE),
		chooseMusic = toInt(BACKGROUND::CHOOSE_MUSIC),
		game = toInt(BACKGROUND::GAME),
		exit = toInt(BACKGROUND::EXIT),
		total = toInt(BACKGROUND::TOTAL);

	ColorStripVelo.y = *screenUnit / 20;
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
	bgSourceRect[score] = bgSourceRect[intro1];
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
	bgDestinationRect[score] = bgDestinationRect[intro2];
	bgDestinationRect[chooseMusic] = bgDestinationRect[intro2];
	bgDestinationRect[game] = bgDestinationRect[intro2];
	bgDestinationRect[exit] = bgDestinationRect[intro2];

	colorStripDstRect[intro1] = setScreenUnitRect(13, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[intro2] = setScreenUnitRect(-7, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[intro3] = setScreenUnitRect(10.5, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[start] = setScreenUnitRect(-2, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[menu] = setScreenUnitRect(8, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[setting] = colorStripDstRect[start];
	colorStripDstRect[score] = colorStripDstRect[start];
	colorStripDstRect[chooseMusic] = setScreenUnitRect(-2.5, 9 * currentColorPlace, 10, 9);
	colorStripDstRect[game] = colorStripDstRect[chooseMusic];
	colorStripDstRect[exit] = colorStripDstRect[start];
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

void Title::createDefaultRect()
{
	delete[] titleDstRect;

	titleDstRect = new SDL_Rect[static_cast<int>(TITLE::TOTAL)];

	titleDstRect[toInt(TITLE::INTRO)] = setScreenUnitRect(6.5, 3, 5, 3);
	titleDstRect[toInt(TITLE::START)] = setScreenUnitRect(0.5, 2, 5.5, 2);
	titleDstRect[toInt(TITLE::MENU)] = setScreenUnitRect(10, 1, 5.5, 2);
	titleDstRect[toInt(TITLE::CHOOSE_MUSIC)] = setScreenUnitRect(8, 1, 7, 1.5);
	titleDstRect[toInt(TITLE::GAME)] = setScreenUnitRect(0.5, 1.5, 5, 1.5);
	titleDstRect[toInt(TITLE::FINISH)] = setScreenUnitRect(9, 1, 6, 2);
	titleDstRect[toInt(TITLE::SETTING)] = setScreenUnitRect(8.5, 1, 7, 2);
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

	pointRect[pointNumb] = setTextRect(1, 2, pointTexture[pointNumb].getWidth(), pointTexture[pointNumb].getHeight());
	pointRect[totalArrowNumb] = setTextRect(1, 8.0 / 3, pointTexture[totalArrowNumb].getWidth(), pointTexture[totalArrowNumb].getHeight());
	pointRect[accuracyNumb] = setTextRect(1, 10.0 / 3, pointTexture[accuracyNumb].getWidth(), pointTexture[accuracyNumb].getHeight());
	pointRect[pressedArrowNumb] = setTextRect(1, 4, pointTexture[pressedArrowNumb].getWidth(), pointTexture[pressedArrowNumb].getHeight());
	pointRect[maxComboNumb] = setTextRect(1, 14.0 / 3, pointTexture[maxComboNumb].getWidth(), pointTexture[maxComboNumb].getHeight());
	pointRect[wrongPressedNumb] = setTextRect(1, 16.0 / 3, pointTexture[wrongPressedNumb].getWidth(), pointTexture[wrongPressedNumb].getHeight());
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
	delete[] muscleDogeSrcRect;
	delete[] cheemsSrcRect;
	delete[] khaBankSrcRect;

	int total = toInt(CONTROL::TOTAL_ARROW),
		left = toInt(CONTROL::LEFT_ARROW),
		up = toInt(CONTROL::UP_ARROW),
		down = toInt(CONTROL::DOWN_ARROW),
		right = toInt(CONTROL::RIGHT_ARROW);
	muscleDogeSrcRect = new SDL_Rect[total];
	cheemsSrcRect = new SDL_Rect[total];
	khaBankSrcRect = new SDL_Rect[total];
	bankVelo = *screenUnit / 8;

	//texture to render from image
	muscleDogeSrcRect[left] = { 0, 0, smashingDoge.getWidth() / 2, smashingDoge.getHeight() / 2 };
	muscleDogeSrcRect[up] = { 0,  smashingDoge.getHeight() / 2,  smashingDoge.getWidth() / 2,  smashingDoge.getHeight() / 2 };
	muscleDogeSrcRect[down] = { smashingDoge.getWidth() / 2,  smashingDoge.getHeight() / 2,  smashingDoge.getWidth() / 2,  smashingDoge.getHeight() / 2 };
	muscleDogeSrcRect[right] = { smashingDoge.getWidth() / 2, 0,  smashingDoge.getWidth() / 2,  smashingDoge.getHeight() / 2 };
	cheemsSrcRect[left] = { 0, 0, cheems.getWidth() / 2, cheems.getHeight() / 2 };
	cheemsSrcRect[up] = { 0,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[down] = { cheems.getWidth() / 2,  cheems.getHeight() / 2,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	cheemsSrcRect[right] = { cheems.getWidth() / 2, 0,  cheems.getWidth() / 2,  cheems.getHeight() / 2 };
	khaBankSrcRect[left] = { 0, 0,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };
	khaBankSrcRect[up] = { khaBank.getWidth() / 2, 0,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };
	khaBankSrcRect[down] = { 0, khaBank.getHeight() / 2,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };
	khaBankSrcRect[right] = { khaBank.getWidth() / 2,  khaBank.getHeight() / 2,  khaBank.getWidth() / 2,  khaBank.getHeight() / 2 };

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



//Button rectangle creating functions

void Button::createRect(TITLE type)
{
	createDefaultRect();
	switch (type) {
		case TITLE::INTRO: {
			buttonDstRect[toInt(BUTTON::PRESS_KEY)] = setScreenUnitRect(0.5, 6, 5.5, 1);
			break;
		}
		case TITLE::MENU: {
			createMenuRect();
			break;
		}
		case TITLE::CHOOSE_MUSIC: {
			createChooseMusic();
			break;
		}
		case TITLE::GAME: {
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
		case TITLE::SCORE: {
			createScoreRect();
			break;
		}
		default: {
			return;
		}
	}
}

void Button::createMenuRect() {
	int playNumb = toInt(BUTTON::PLAY),
		settingNumb = toInt(BUTTON::SETTING),
		exitNumb = toInt(BUTTON::EXIT),
		scoreNumb = toInt(BUTTON::SCORE);

	buttonDstRect[playNumb] = setScreenUnitRect(11.5, 3.5, 3, 1);
	buttonDstRect[settingNumb] = setScreenUnitRect(11, 5, 4, 1);
	buttonDstRect[exitNumb] = setScreenUnitRect(11.5, 6.5, 3, 1);
	buttonDstRect[scoreNumb] = setScreenUnitRect(5.85, 0.75, 1.5, 0.5);

	usingButton[playNumb] = true;
	usingButton[settingNumb] = true;
	usingButton[exitNumb] = true;
	usingButton[scoreNumb] = true;
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



//Character rectangle creating functions

void Character::createRect(TITLE type)
{
	delete muscleDogeDstRect;
	delete[] cheemsDstRect;
	delete[] khaBankDstRect;
	muscleDogeDstRect = nullptr;
	cheemsDstRect = nullptr;
	khaBankDstRect = nullptr;

	switch (type) {
		case TITLE::INTRO: {
			createIntroRect();
			break;
		}
		case TITLE::MENU: {
			createMenuRect();
			break;
		}
		case TITLE::CHOOSE_MUSIC: {
			createMusicRect();
			break;
		}
		case TITLE::GAME: {
			createGameRect();
			break;
		}
		default: {
			return;
		}
	}
}

void Character::createIntroRect()
{
	muscleDogeDstRect = new SDL_Rect;
	cheemsDstRect = new SDL_Rect[static_cast<int>(CONTROL::TOTAL_ARROW)];
	khaBankDstRect = new SDL_Rect;

	*khaBankDstRect = setScreenUnitRect(4, 2, 2.5, 4);
	*muscleDogeDstRect = setScreenUnitRect(7.5, 3, 5, 5);
	for (int type = 0; type < toInt(CONTROL::TOTAL_ARROW); type++) {
		bool duplicate = false;
		do {
			duplicate = false;
			int x = rand() % 10,
				y = rand() % 4 + 2;
			cheemsDstRect[type] = setScreenUnitRect(x, y, 1.5, 2);
			if (SDL_HasIntersection(khaBankDstRect, &cheemsDstRect[type])) {
				duplicate = true;
			}
			for (int rect = 0; (rect < type) && !duplicate; rect++) {
				if (SDL_HasIntersection(&cheemsDstRect[rect], &cheemsDstRect[type])) {
					duplicate = true;
					break;
				}
			}
		} while (duplicate);
	}
}

void Character::createMenuRect() {
	cheemsDstRect = new SDL_Rect[static_cast<int>(CONTROL::TOTAL_ARROW)];
	khaBankDstRect = new SDL_Rect;

	*khaBankDstRect = setScreenUnitRect(5.2, 4, 2, 3.2);
	cheemsDstRect[toInt(CONTROL::LEFT_ARROW)] = setScreenUnitRect(6, 1.6, 1.2, 1.6);
	cheemsDstRect[toInt(CONTROL::UP_ARROW)] = setScreenUnitRect(0.8, 5, 1.2, 1.6);
	cheemsDstRect[toInt(CONTROL::DOWN_ARROW)] = setScreenUnitRect(3.8, 6, 1.2, 1.6);
	cheemsDstRect[toInt(CONTROL::RIGHT_ARROW)] = setScreenUnitRect(3, 4, 1.2, 1.6);
}

void Character::createMusicRect() {
	cheemsDstRect = new SDL_Rect[static_cast<int>(CONTROL::TOTAL_ARROW)];
	cheemsDstRect[toInt(CONTROL::LEFT_ARROW)] = setScreenUnitRect(10.75, 2.75, 1.5, 2);
	cheemsDstRect[toInt(CONTROL::UP_ARROW)] = setScreenUnitRect(8, 6, 1.5, 2);
	cheemsDstRect[toInt(CONTROL::DOWN_ARROW)] = setScreenUnitRect(13.5, 6, 1.5, 2);
	cheemsDstRect[toInt(CONTROL::RIGHT_ARROW)] = setScreenUnitRect(10.75, 6.75, 1.5, 2);
}

void Character::createGameRect()
{
	muscleDogeDstRect = new SDL_Rect;
	cheemsDstRect = new SDL_Rect[static_cast<int>(CONTROL::TOTAL_ARROW)];
	khaBankDstRect = new SDL_Rect;

	//texture to render to screen
	*muscleDogeDstRect = setScreenUnitRect(10, 3, 4, 4);
	cheemsDstRect[toInt(CONTROL::LEFT_ARROW)] = setScreenUnitRect(9.5, 5, 1.5, 2);
	cheemsDstRect[toInt(CONTROL::UP_ARROW)] = setScreenUnitRect(10.5, 6, 1.5, 2);
	cheemsDstRect[toInt(CONTROL::DOWN_ARROW)] = setScreenUnitRect(12, 6, 1.5, 2);
	cheemsDstRect[toInt(CONTROL::RIGHT_ARROW)] = setScreenUnitRect(13, 5, 1.5, 2);
	*khaBankDstRect = setScreenUnitRect(16, 5, 2.5, 4);
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

void Character::renderIntro() {
	frame = (frame + 1) % 60;
	khaBank.render(renderer, khaBankDstRect, &khaBankSrcRect[abs(frame / 10 - 3)]);
	for (int clip = 0; clip < toInt(CONTROL::TOTAL_ARROW); clip++) {
		if ((frame / 10) % 2) {
			cheems.render(renderer, &cheemsDstRect[clip], &cheemsSrcRect[clip ^ 3]);
		}
		else {
			cheems.render(renderer, &cheemsDstRect[clip], &cheemsSrcRect[clip]);
		}
	}
}

void Character::renderStart() {
	muscleDoge.render(renderer, muscleDogeDstRect, NULL);
}

void Character::renderChooseMusic() {
	frame = (frame + 1) % 60;
	for (int clip = 0; clip < toInt(CONTROL::TOTAL_ARROW); clip++) {
		if ((frame / 30) % 2) {
			cheems.render(renderer, &cheemsDstRect[clip], &cheemsSrcRect[clip ^ 3]);
		}
		else {
			cheems.render(renderer, &cheemsDstRect[clip], &cheemsSrcRect[clip]);
		}
	}
}

void Character::renderDoge(Event& event)
{
	int left = toInt(CONTROL::LEFT_ARROW),
		up = toInt(CONTROL::UP_ARROW),
		down = toInt(CONTROL::DOWN_ARROW),
		right = toInt(CONTROL::RIGHT_ARROW);
	 
	for (int key = 0; key < toInt(CONTROL::TOTAL_ARROW); key++) {
		if (event.checkKeyState(static_cast<CONTROL>(key))) {
			if (!event.checkKeyRepeat()) {
				playBonkSound();
			}
			switch (static_cast<CONTROL>(key)) {
				case CONTROL::LEFT_ARROW: case CONTROL::RIGHT_ARROW: {
					smashedCheems.render(renderer, &cheemsDstRect[key], &cheemsSrcRect[key]);
					cheems.render(renderer, &cheemsDstRect[(key ^ 3)], &cheemsSrcRect[(key ^ 3)]);
					smashingDoge.render(renderer, muscleDogeDstRect, &muscleDogeSrcRect[key]);
					cheems.render(renderer, &cheemsDstRect[up], &cheemsSrcRect[up]);
					cheems.render(renderer, &cheemsDstRect[down], &cheemsSrcRect[down]);
					return;
				}
				case CONTROL::UP_ARROW: case CONTROL::DOWN_ARROW: {
					cheems.render(renderer, &cheemsDstRect[left], &cheemsSrcRect[left]);
					cheems.render(renderer, &cheemsDstRect[right], &cheemsSrcRect[right]);
					smashingDoge.render(renderer, muscleDogeDstRect, &muscleDogeSrcRect[key]);
					smashedCheems.render(renderer, &cheemsDstRect[key], &cheemsSrcRect[key]);
					cheems.render(renderer, &cheemsDstRect[(key ^ 3)], &cheemsSrcRect[(key ^ 3)]);
					return;
				}
			}
		}
	}
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::LEFT_ARROW)], &cheemsSrcRect[toInt(CONTROL::LEFT_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::RIGHT_ARROW)], &cheemsSrcRect[toInt(CONTROL::RIGHT_ARROW)]);
	muscleDoge.render(renderer, muscleDogeDstRect, NULL);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::UP_ARROW)], &cheemsSrcRect[toInt(CONTROL::UP_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::DOWN_ARROW)], &cheemsSrcRect[toInt(CONTROL::DOWN_ARROW)]);
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