#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Background.h"
using namespace std;

//background constructor

Background::Background(Screen& screen)
{
	setRenderer(screen);
	yColorStripVelo = screenUnit / 20;
}

//basic background functions

bool Background::load(const string& path) {
	bool success = true;

	background.free();
	for (int type = 0; type < static_cast<int>(COLORSTRIP::TOTAL); type++) {
		colorStrip[type].free();
	}

	if (!background.loadFromFile(renderer, path + "/Picture/background.png")) {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
		success = false;
	}
	else {
		for (int type = 0; type < static_cast<int>(COLORSTRIP::TOTAL); type++) {
			string colorPath = path;
			switch (static_cast<COLORSTRIP> (type)) {
				case COLORSTRIP::GREEN: {
					colorPath += "/Picture/ColorStripGreen.png";
					break;
				}
				case COLORSTRIP::PURPLE: {
					colorPath += "/Picture/ColorStripPurple.png";
					break;
				}
				case COLORSTRIP::ORANGE: {
					colorPath += "/Picture/ColorStripOrange.png";
					break;
				}
			}
			if (!colorStrip[type].loadFromFile(renderer, colorPath)) {
				cout << "Log [" << SDL_GetTicks() << "]: ";
				loadErrorLog();
				success = false;
				break;
			}
		}
		if (success == true) {
			createRect();
		}
	}
	return success;
}

void Background::render(BACKGROUND type) {
	renderBackground(type);
	renderColorStrip(type);
}

void Background::free() {
	background.free();
	for (int i = 0; i < static_cast<int>(COLORSTRIP::TOTAL); i++) {
		colorStrip[i].free();
	}
	delete[] bgSourceRect;
	delete[] bgDestinationRect;
	delete[] colorStripDstRect;
	bgSourceRect = NULL;
	bgDestinationRect = NULL;
	colorStripDstRect = NULL;
	setScreenUnit(0);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Background freed successfully" << endl;
}

//background moving functions

void Background::moveColorStrip() {
	for (int type = 0; type < static_cast<int>(BACKGROUND::TOTAL); type++) {
		moveColorStrip(colorStripDstRect[type]);
	}
}

void Background::moveColorStrip(SDL_Rect& movingRect) {
	movingRect.y = (movingRect.y + yColorStripVelo) % screenHeight;
}

void Background::moveBackground(BACKGROUND origin, BACKGROUND moving) {
	int original = static_cast<int> (origin);
	int move = static_cast<int> (moving);

	cout << "Log [" << SDL_GetTicks() << "]: " << "Moving background" << endl;
	SDL_Rect bgSourceTrans = bgSourceRect[original];
	SDL_Rect bgDestinationTrans = bgDestinationRect[original];
	SDL_Rect colorStripDstTransLeft = colorStripDstRect[original], 
		colorStripDstTransRight = colorStripDstRect[original];

	int xSrcVelo = (bgSourceRect[move].x - bgSourceRect[original].x) * 8 / screenUnit / 5,
		ySrcVelo = (bgSourceRect[move].y - bgSourceRect[original].y) * 8 / screenUnit / 5,
		wSrcVelo = (bgSourceRect[move].w - bgSourceRect[original].w) * 8 / screenUnit / 5,
		hSrcVelo = (bgSourceRect[move].h - bgSourceRect[original].h) * 8 / screenUnit / 5,
		xDstVelo = (bgDestinationRect[move].x - bgDestinationRect[original].x) * 8 / screenUnit / 5,
		xColorStripVelo = 0;

	if (((colorStripDstRect[original].x >= screenWidth / 2) ^ (colorStripDstRect[move].x >= screenWidth / 2))) {
		if (colorStripDstRect[original].x < screenWidth / 2) {
			colorStripDstTransRight.x = colorStripDstTransRight.x + static_cast<int>(screenWidth + screenUnit * 2.5);
			xColorStripVelo = (-(screenUnit * 10) - colorStripDstRect[original].x) * 8 / screenUnit / 5;

		}
		else {
			colorStripDstTransLeft.x = colorStripDstTransRight.x - static_cast<int>(screenWidth + screenUnit * 2.5);
			xColorStripVelo = (screenWidth - colorStripDstRect[original].x) * 8 / screenUnit / 5;
		}
	}
	else {
			xColorStripVelo = (colorStripDstRect[move].x - colorStripDstRect[original].x) * 8 / screenUnit / 5;
	}

	while (!SDL_RectEquals(&bgSourceTrans, &bgSourceRect[move]) || !SDL_RectEquals(&bgDestinationTrans, &bgDestinationRect[move]) || 
		(!SDL_RectEquals(&colorStripDstTransLeft, &colorStripDstRect[move]) && !SDL_RectEquals(&colorStripDstTransRight, &colorStripDstRect[move]))) {
		SDL_RenderClear(renderer);
		bgSourceTrans.x += xSrcVelo;
		bgSourceTrans.y += ySrcVelo;
		bgSourceTrans.w += wSrcVelo;
		bgSourceTrans.h += hSrcVelo;
		bgDestinationTrans.x += xDstVelo;
		colorStripDstTransLeft.x += xColorStripVelo;
		colorStripDstTransRight.x += xColorStripVelo;

		moveComplete(bgSourceTrans, bgSourceRect[original], bgSourceRect[move]);
		moveComplete(bgDestinationTrans, bgDestinationRect[original], bgDestinationRect[move]);
		
		background.render(renderer, &bgDestinationTrans, &bgSourceTrans);

		if (xColorStripVelo > 0 && colorStripDstTransLeft.x > colorStripDstRect[move].x) {
			colorStripDstTransLeft = colorStripDstRect[move];
		}
		if (xColorStripVelo < 0 && colorStripDstTransRight.x < colorStripDstRect[move].x) {
			colorStripDstTransRight = colorStripDstRect[move];
		}
		moveColorStrip();
		moveColorStrip(colorStripDstTransLeft);
		moveColorStrip(colorStripDstTransRight);
		renderColorStrip(colorStripDstTransLeft);
		if (!SDL_RectEquals(&colorStripDstTransLeft, &colorStripDstTransRight)) {
			renderColorStrip(colorStripDstTransRight);
		}
		SDL_RenderPresent(renderer);
	}

	xSrcVelo = 0;
	ySrcVelo = 0;
	wSrcVelo = 0;
	hSrcVelo = 0;
}

void Background::moveComplete(SDL_Rect& movingRect, SDL_Rect& original, SDL_Rect& move) {
	if (((move.x > original.x) ^ (movingRect.x < move.x))) {
		movingRect = move;
	}
	if (((move.y > original.y) ^ (movingRect.y < move.y))) {
		movingRect = move;
	}
	if (((move.w > original.w) ^ (movingRect.w < move.w))) {
		movingRect = move;
	}
	if (((move.h > original.h) ^ (movingRect.h < move.h))) {
		movingRect = move;
	}
}


//background rendering functions

void Background::renderColorStrip(BACKGROUND type) {
	moveColorStrip();
	int backgroundType = toInt(type);
	renderColorStrip(colorStripDstRect[backgroundType]);
}

void Background::renderColorStrip(SDL_Rect& rectangle) {
	for (int type = 0; type < static_cast<int>(COLORSTRIP::TOTAL); type++) {
		if (type % 2 == 1) {
			rectangle.y = screenHeight - rectangle.y;
		}
		colorStrip[type].render(renderer, &rectangle, NULL);
		rectangle.y -= screenHeight;
		colorStrip[type].render(renderer, &rectangle, NULL);
		rectangle.y += screenHeight;
		if (type % 2 == 1) {
			rectangle.y = screenHeight - rectangle.y;
		}
	}
}

void Background::renderBackground(BACKGROUND type) {
	int backgroundType = toInt(type);
	background.render(renderer, &bgDestinationRect[backgroundType], &bgSourceRect[backgroundType]);
}