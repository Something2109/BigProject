#ifndef _Game_h_
#define _Game_h_
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Render.h"
using namespace std;

//this file contains the game running related functions

bool loadingGameResource(SDL_Renderer* renderer, GameTexture& texture, Mix_Chunk*& bonk);
void updateArrowRange(vector <Coordinate>& arrowRange, const int& velocity, const int& screenUnit);
void addNewArrow(vector <Coordinate>& arrowRange, const int& screenUnit);
void renderArrowRange(SDL_Renderer* renderer, const int& screenUnit, vector<Coordinate>& arrowRange, LTexture& arrow, SDL_Rect* arrowSourceRect);
void freeGamesTexture(GameTexture& texture, Mix_Chunk*& bonk);


void game(SDL_Renderer* renderer, const int& screenUnit, const int& velocity, const int& spawnRate) {
	const int arrowRangeCol = 4;
	GameTexture texture;
	GameRect rectangles;
	Mix_Chunk* bonk = NULL;
	vector <Coordinate> arrowRange;
	if (!loadingGameResource(renderer, texture, bonk)) {
		cout << "Failed to loading game" << endl;
	}
	else {
		SDL_Event e;
		Uint32 startTime = SDL_GetTicks();
		Uint32 gameTime;
		unsigned int arrowCount = 0;
		bool quit = false;
		rectangles.createSourceRect(screenUnit, texture);
		while (!quit) {
			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			gameTime = SDL_GetTicks() - startTime;
			if (gameTime >= spawnRate * arrowCount) {
				addNewArrow(arrowRange, screenUnit);
				arrowCount++;
			}
			SDL_RenderClear(renderer);
			texture.background.rectRender(renderer, rectangles.backgroundDstRect, rectangles.backgroundSrcRect);
			texture.blankArrow.rectRender(renderer, rectangles.blankArrowDstRect, rectangles.blankArrowSrcRect);
			renderPressedArrow(renderer, screenUnit, e, arrowRange, texture, rectangles);
			renderDogeScreen(renderer, e, texture, rectangles, bonk, screenUnit);
			renderArrowRange(renderer, screenUnit, arrowRange, texture.arrow, rectangles.arrowSrcRect);
			updateArrowRange(arrowRange, velocity, screenUnit);
			SDL_RenderPresent(renderer);
		}
	}
	freeGamesTexture(texture, bonk);
	rectangles.free();
}

//loading resources to the textures
bool loadingGameResource(SDL_Renderer* renderer, GameTexture& texture, Mix_Chunk* &bonk) {
	string path = "resource";
	bool success = texture.loadTexture(renderer, path);
	bonk = Mix_LoadWAV("resource/bonk.wav");
	if (bonk == NULL) {
		loadErrorLog();
		success = false;
	}
	return success;
}

//update the arrow range to render to screen
void updateArrowRange(vector <Coordinate> &arrowRange, const int &velocity, const int &screenUnit) {
	for (int i = 0; i < arrowRange.size(); i++) {
		if (arrowRange[i].getY() <= -screenUnit) {
			arrowRange.erase(arrowRange.begin() + i);
		}
		else {
			arrowRange[i].arrowMove(velocity);
		}
	}
}

void addNewArrow(vector <Coordinate> &arrowRange, const int &screenUnit) {
	int arrowCol = rand() % 4;
	int x = screenUnit * (1 + arrowCol * 2);
	int y = screenUnit * 9;
	Coordinate newArrow(x, y);
	arrowRange.push_back(newArrow);
}

//free all the game textures
void freeGamesTexture(GameTexture& texture, Mix_Chunk* &bonk) {
	texture.free();
	Mix_FreeChunk(bonk);
	bonk = NULL;
}

#endif