#ifndef _Game_h_
#define _Game_h_
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Render.h"
using namespace std;

bool** createArrowRange(const int& arrowRangeCol, const int& arrowRangeRow);
bool loadingGameResource(SDL_Renderer* renderer, LTexture& blankArrow, LTexture& arrow,
				LTexture& background, LTexture& muscleDoge, LTexture& cheems, LTexture& smashedCheems, Mix_Chunk* &bonk);
void updateArrowRange(bool** arrowRange, const int& arrowRangeCol, const int& arrowRangeRow);
void renderArrowRange(SDL_Renderer* renderer, const int& screenUnit, bool** arrowRange, const int& arrowRangeCol, const int& arrowRangeRow,
				LTexture& arrow, SDL_Rect* arrowSourceRect);
void createSourceRect(SDL_Rect* arrowSourceRect, SDL_Rect* muscleDogeRect, SDL_Rect* cheemsSourceRect, SDL_Rect* cheemsDestinationRect, const int& screenUnit);
void renderDogeScreen(SDL_Renderer* renderer, SDL_Event& e, LTexture& muscleDoge, LTexture& cheems, LTexture& smashedCheems,
	SDL_Rect* muscleDogeRect, SDL_Rect* cheemsSourceRect, SDL_Rect* cheemsDestinationRect, Mix_Chunk* bonk, int screenUnit);
void freeGamesSource(LTexture& blankArrow, LTexture& arrow, LTexture& muscleDoge, LTexture& cheems, LTexture& smashedCheems, LTexture& background,
	SDL_Rect* &arrowSourceRect, SDL_Rect* &muscleDogeRect,SDL_Rect* &cheemsSourceRect, SDL_Rect* &cheemsDestinationRect, Mix_Chunk* &bonk);

void game(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
	const int arrowRangeCol = 4,
		arrowRangeRow = 7;
	int screenUnit = screenWidth / 16;
	LTexture blankArrow;
	LTexture arrow;
	LTexture muscleDoge;
	LTexture cheems;
	LTexture smashedCheems;
	LTexture background;
	SDL_Rect* arrowSourceRect = new SDL_Rect [arrowRangeCol];
	SDL_Rect* muscleDogeRect = new SDL_Rect[arrowRangeCol];
	SDL_Rect* cheemsSourceRect = new SDL_Rect [arrowRangeCol];
	SDL_Rect* cheemsDestinationRect = new SDL_Rect[arrowRangeCol];
	Mix_Chunk* bonk = NULL;
	createSourceRect(arrowSourceRect, muscleDogeRect, cheemsSourceRect, cheemsDestinationRect, screenUnit);
	bool** arrowRange = createArrowRange(arrowRangeCol, arrowRangeRow);
	if (!loadingGameResource(renderer, blankArrow, arrow, background, muscleDoge, cheems, smashedCheems, bonk)) {
		cout << "Failed to loading game" << endl;
	}
	else {
		SDL_Event e;
		Uint32 startTime = SDL_GetTicks();
		Uint32 gameTime;
		int arrowCount = 0;
		bool quit = false;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else {
					SDL_RenderClear(renderer);
					gameTime = SDL_GetTicks() - startTime;
					if (gameTime >= 1500 * arrowCount) {
						updateArrowRange(arrowRange, arrowRangeCol, arrowRangeRow);
						arrowCount++;
					}
					background.render(renderer, 0, 0, NULL);
					blankArrow.render(renderer, screenUnit, screenUnit, NULL);
					renderArrowRange(renderer, screenUnit, arrowRange, arrowRangeCol, arrowRangeRow, arrow, arrowSourceRect);
					renderDogeScreen(renderer, e, muscleDoge,cheems,smashedCheems, muscleDogeRect,cheemsSourceRect, cheemsDestinationRect, bonk, screenUnit);
					SDL_RenderPresent(renderer);
				}
			}
		}
	}
	freeGamesSource(blankArrow, arrow, muscleDoge, cheems, smashedCheems, background, arrowSourceRect, muscleDogeRect, cheemsSourceRect, cheemsDestinationRect, bonk);
}

bool** createArrowRange(const int& arrowRangeCol, const int& arrowRangeRow) {
	bool** arrowRange = new bool* [arrowRangeCol];
	for (int i = 0; i < arrowRangeCol; i++) {
		arrowRange[i] = new bool[arrowRangeRow];
		for (int j = 0; j < arrowRangeRow; j++) {
			arrowRange[i][j] = false;
		}
	}
	return arrowRange;
}

bool loadingGameResource(SDL_Renderer* renderer, LTexture& blankArrow, LTexture& arrow, 
				LTexture& background, LTexture& muscleDoge, LTexture& cheems, LTexture& smashedCheems, Mix_Chunk* &bonk) {
	bool success = true;
	string path = "Resource";
	if (!blankArrow.loadFromFile(renderer, path + "/blankArrow.png")) {
		loadErrorLog();
		success = false;
	}
	if (!arrow.loadFromFile(renderer, path + "/arrow.png")) {
		loadErrorLog();
		success = false;
	}
	if (!background.loadFromFile(renderer, path + "/background.png")) {
		loadErrorLog();
		success = false;
	}
	if (!muscleDoge.loadFromFile(renderer, path + "/muscleDoge.png")) {
		loadErrorLog();
		success = false;
	}
	if (!cheems.loadFromFile(renderer, path + "/cheems.png")) {
		loadErrorLog();
		success = false;
	}
	if (!smashedCheems.loadFromFile(renderer, path + "/smashedCheems.png")) {
		loadErrorLog();
		success = false;
	}
	bonk = Mix_LoadWAV("resource/bonk.wav");
	if (bonk == NULL) {
		loadErrorLog();
		success = false;
	}
	return success;
}

void updateArrowRange(bool **arrowRange, const int& arrowRangeCol, const int& arrowRangeRow) {
	for (int i = 0; i < arrowRangeCol; i++) {
		if (arrowRange[i][0] == true) {
			arrowRange[i][0] = false;
		}
		for (int j = 0; j < arrowRangeRow - 1; j++) {
			if (arrowRange[i][j + 1] == true) {
				arrowRange[i][j] = true;
				arrowRange[i][j + 1] = false;
			}
		}
	}
	int newArrow = rand() % 4;
	arrowRange[newArrow][arrowRangeRow - 1] = true;
}

void freeGamesSource(LTexture& blankArrow, LTexture& arrow, LTexture& muscleDoge, LTexture& cheems, LTexture& smashedCheems, LTexture& background,
				SDL_Rect* &arrowSourceRect, SDL_Rect* &muscleDogeRect, SDL_Rect* &cheemsSourceRect, SDL_Rect* &cheemsDestinationRect, Mix_Chunk* &bonk) {
	blankArrow.free();
	arrow.free();
	muscleDoge.free();
	cheems.free();
	smashedCheems.free();
	background.free();
	Mix_FreeChunk(bonk);
	delete[] arrowSourceRect;
	delete[] muscleDogeRect;
	delete[] cheemsSourceRect;
	delete[] cheemsDestinationRect;
	arrowSourceRect = NULL;
	muscleDogeRect = NULL;
	cheemsSourceRect = NULL;
	cheemsDestinationRect = NULL;
	bonk = NULL;
}

#endif