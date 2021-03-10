#ifndef _Render_h
#define _Render_h
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Texture.h"
using namespace std;

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

void createSourceRect(SDL_Rect* arrowSourceRect, SDL_Rect* muscleDogeRect, SDL_Rect* cheemsSourceRect, SDL_Rect* cheemsDestinationRect, 
	const int& screenUnit) 
{
	//rectangle to render from image
	arrowSourceRect[LEFT_ARROW] = { 0, 0, 80, 80 };
	arrowSourceRect[UP_ARROW] = { 80, 0, 80, 80 };
	arrowSourceRect[DOWN_ARROW] = { 0, 80, 80, 80 };
	arrowSourceRect[RIGHT_ARROW] = { 80, 80, 80, 80 };
	muscleDogeRect[SOURCE_LEFT] = { 0, 0, 320, 320 };
	muscleDogeRect[SOURCE_IDLE] = { 320, 0, 320, 320 };
	muscleDogeRect[SOURCE_RIGHT] = { 640, 0, 320, 320 };
	cheemsSourceRect[LEFT_ARROW] = { 0, 0, 120, 160 };
	cheemsSourceRect[UP_ARROW] = { 0, 160, 120, 160 };
	cheemsSourceRect[DOWN_ARROW] = { 120, 160, 120, 160 };
	cheemsSourceRect[RIGHT_ARROW] = { 120, 0, 120, 160 };
	//rectangle to render to screen
	muscleDogeRect[DESTINATION] = { screenUnit * 10, screenUnit * 3, screenUnit * 4, screenUnit * 4 };
	cheemsDestinationRect[LEFT_ARROW] = { static_cast<int>(screenUnit * 9.5), screenUnit * 5, 120, 160 };
	cheemsDestinationRect[UP_ARROW] = { static_cast<int>(screenUnit * 10.5), screenUnit * 6, 120, 160 };
	cheemsDestinationRect[DOWN_ARROW] = { screenUnit * 12, screenUnit * 6, 120, 160 };
	cheemsDestinationRect[RIGHT_ARROW] = { screenUnit * 13, screenUnit * 5, 120, 160 };
}

void renderArrowRange(SDL_Renderer* renderer, const int& screenUnit, bool** arrowRange, const int& arrowRangeCol, const int& arrowRangeRow,
	LTexture& arrow, SDL_Rect* arrowSourceRect) 
{
	for (int i = 0; i < arrowRangeCol; i++) {
		for (int j = 0; j < arrowRangeRow; j++) {
			if (arrowRange[i][j] == true) {
				arrow.clipRender(renderer, static_cast<int>(screenUnit * (i + 0.5) * 2), screenUnit * (j + 1), &arrowSourceRect[i]);
			}
		}
	}
}

void renderDogeScreen(SDL_Renderer* renderer, SDL_Event& e, LTexture& muscleDoge, LTexture& cheems, LTexture& smashedCheems,
	SDL_Rect* muscleDogeRect, SDL_Rect* cheemsSourceRect, SDL_Rect* cheemsDestinationRect, Mix_Chunk* bonk, int screenUnit)
{
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_LEFT: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			smashedCheems.rectRender(renderer, &cheemsDestinationRect[LEFT_ARROW], &cheemsSourceRect[LEFT_ARROW]);
			muscleDoge.rectRender(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_LEFT]);
			cheems.rectRender(renderer, &cheemsDestinationRect[RIGHT_ARROW], &cheemsSourceRect[RIGHT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[UP_ARROW], &cheemsSourceRect[UP_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[DOWN_ARROW], &cheemsSourceRect[DOWN_ARROW]);
			break;
		}
		case SDLK_UP: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			muscleDoge.rectRender(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
			cheems.rectRender(renderer, &cheemsDestinationRect[LEFT_ARROW], &cheemsSourceRect[LEFT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[RIGHT_ARROW], &cheemsSourceRect[RIGHT_ARROW]);
			smashedCheems.rectRender(renderer, &cheemsDestinationRect[UP_ARROW], &cheemsSourceRect[UP_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[DOWN_ARROW], &cheemsSourceRect[DOWN_ARROW]);
			break;
		}
		case SDLK_DOWN: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			muscleDoge.rectRender(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
			cheems.rectRender(renderer, &cheemsDestinationRect[LEFT_ARROW], &cheemsSourceRect[LEFT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[RIGHT_ARROW], &cheemsSourceRect[RIGHT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[UP_ARROW], &cheemsSourceRect[UP_ARROW]);
			smashedCheems.rectRender(renderer, &cheemsDestinationRect[DOWN_ARROW], &cheemsSourceRect[DOWN_ARROW]);
			break;
		}
		case SDLK_RIGHT: {
			if (e.key.repeat == 0) {
				Mix_PlayChannel(-1, bonk, 0);
			}
			smashedCheems.rectRender(renderer, &cheemsDestinationRect[RIGHT_ARROW], &cheemsSourceRect[RIGHT_ARROW]);
			muscleDoge.rectRender(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_RIGHT]);
			cheems.rectRender(renderer, &cheemsDestinationRect[LEFT_ARROW], &cheemsSourceRect[LEFT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[UP_ARROW], &cheemsSourceRect[UP_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[DOWN_ARROW], &cheemsSourceRect[DOWN_ARROW]);
			break;
		}
		default: {
			muscleDoge.rectRender(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
			cheems.rectRender(renderer, &cheemsDestinationRect[LEFT_ARROW], &cheemsSourceRect[LEFT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[RIGHT_ARROW], &cheemsSourceRect[RIGHT_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[UP_ARROW], &cheemsSourceRect[UP_ARROW]);
			cheems.rectRender(renderer, &cheemsDestinationRect[DOWN_ARROW], &cheemsSourceRect[DOWN_ARROW]);
		}
		}
	}
	else {
		muscleDoge.rectRender(renderer, &muscleDogeRect[DESTINATION], &muscleDogeRect[SOURCE_IDLE]);
		cheems.rectRender(renderer, &cheemsDestinationRect[LEFT_ARROW], &cheemsSourceRect[LEFT_ARROW]);		
		cheems.rectRender(renderer, &cheemsDestinationRect[RIGHT_ARROW], &cheemsSourceRect[RIGHT_ARROW]);
		cheems.rectRender(renderer, &cheemsDestinationRect[UP_ARROW], &cheemsSourceRect[UP_ARROW]);
		cheems.rectRender(renderer, &cheemsDestinationRect[DOWN_ARROW], &cheemsSourceRect[DOWN_ARROW]);
	}
}

#endif