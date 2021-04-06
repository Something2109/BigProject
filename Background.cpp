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

//load background
bool Background::loadBackground(Screen& screen, const string& path) {
	renderer = screen.getRenderer();
	setScreenUnit(screen.getScreenUnit());
	yColorStripVelo = screenUnit / 20;
	bool success = true;

	if (!background.loadFromFile(renderer, path + "/Picture/background.png")) {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
		success = false;
	}
	else {
		if (!colorStrip[ORANGE].loadFromFile(renderer, path + "/Picture/ColorStripOrange.png")) {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
			success = false;
		}
		else {
			if (!colorStrip[GREEN].loadFromFile(renderer, path + "/Picture/ColorStripGreen.png")) {
				cout << "Log [" << SDL_GetTicks() << "]: ";
				loadErrorLog();
				success = false;
			}
			else {
				if (!colorStrip[PURPLE].loadFromFile(renderer, path + "/Picture/ColorStripPurple.png")) {
					cout << "Log [" << SDL_GetTicks() << "]: ";
					loadErrorLog();
					success = false;
				}
				else {
					 createBackgroundRect();
				}
			}
		}
	}
	return success;
}

//color strip moving function
void Background::moveColorStrip() {
	moveColorStrip(colorStripDstRect[MENU]);
	moveColorStrip(colorStripDstRect[CHOOSE_MUSIC]);
	moveColorStrip(colorStripDstRect[GAME]);
}

void Background::moveColorStrip(SDL_Rect& movingRect) {
	movingRect.y = (movingRect.y + yColorStripVelo) % screenHeight;
}

void Background::moveComplete(SDL_Rect &movingRect, SDL_Rect &original, SDL_Rect& move) {
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

//background picture moving function
void Background::moveBackground(int original, int move) {
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

//background render function
void Background::freeBackground() {
	background.free();
	for (int i = 0; i < TOTAL_BACKGROUND; i++) {
		colorStrip[i].free();
	}
	delete[] bgSourceRect;
	delete[] bgDestinationRect;
	delete[] colorStripDstRect;
	bgSourceRect = NULL;
	bgDestinationRect = NULL;
	colorStripDstRect = NULL;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Background freed successfully" << endl;
}