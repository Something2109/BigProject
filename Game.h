#ifndef _Game_h_
#define _Game_h_
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Structs.h"
#include "Render.h"
#include "Event.h"
using namespace std;

//this file contains the game running related functions

void game(Screen& background, Event& event, Music& song);
bool loadingGameResource(Screen &background, GameTexture& texture, Music &song, Mix_Chunk*& bonk);
void updateArrowRange(Screen& background, vector <Coordinate>& arrowRange, const int& velocity);
void addNewArrow(vector <Coordinate>& arrowRange, Screen& background);
void freeGamesTexture(GameTexture& texture, Music &song,Mix_Chunk*& bonk);

//main game function
void game(Screen &background, Event &event, Music &song) {

	//game texture
	GameTexture texture;
	Mix_Chunk* bonk = NULL;
	vector <Coordinate> arrowRange;

	//arrow spawn mechanism
	Uint32 startTime;
	Uint32 gameTime;
	unsigned int arrowCount = 0;

	//quit game
	bool quit = false;

	//load game resources
	if (!loadingGameResource(background, texture, song, bonk)) {
		cout << "Failed to loading game" << endl;
	}
	else {
		//pregame load
		song.playMusic(0);
		startTime = SDL_GetTicks();

		//game loop
		while (!quit) {

			//event handle
			event.updateEvent();
			quit = event.quitGame();

			//arrow spawn mechanism
			gameTime = SDL_GetTicks() - startTime;
			if (gameTime > (arrowCount * song.getSpawnRate()) && Mix_PlayingMusic()) {
				addNewArrow(arrowRange, background);
				arrowCount++;
			}

			//render texture
			SDL_RenderClear(background.getRenderer());
			background.renderBackground(GAME);
			background.renderColorStrip(GAME);
			renderPressedArrow(background, event, arrowRange, texture);
			renderDogeScreen(background, event, texture, bonk);
			renderArrowRange(background, arrowRange, texture);
			updateArrowRange(background, arrowRange, song.getVelocity());
			SDL_RenderPresent(background.getRenderer());
		}
	}

	//free game resources
	freeGamesTexture(texture, song, bonk);
}

//loading resources
bool loadingGameResource(Screen &background, GameTexture &texture, Music &song, Mix_Chunk* &bonk) {
	string path = "Resource";
	bool success = true;

	//load the game texture
	if (texture.loadTexture(background, path)) {
		cout << "Game Texture loaded successfully" << endl;

		//load the bonk sound
		bonk = Mix_LoadWAV((path + "/Music/bonk.wav").c_str());
		if (song.loadFromFile(path)) {
			cout << "Music file loaded successfully" << endl;

			if (bonk == NULL) {
				loadErrorLog();
				cout << "failed tp load Bonk sound" << endl;
				success = false;
			}
			else {
				cout << "Bonk sound loaded successfully" << endl;
			}
		}
		else {
			cout << "Failed to load music file" << endl;
		}
	}
	else {
		cout << "Failed to load Game Texture" << endl;
	}
	return success;
}

//update the arrow range
void updateArrowRange(Screen &background, vector <Coordinate> &arrowRange, const int &velocity) {
	int screenUnit = background.getScreenUnit();
	for (int i = 0; i < arrowRange.size(); i++) {
		if (arrowRange[i].getY() <= -screenUnit) {
			arrowRange.erase(arrowRange.begin() + i);
		}
		else {
			arrowRange[i].arrowMove(velocity);
		}
	}
}

//add new arrow to arrow range
void addNewArrow(vector <Coordinate> &arrowRange, Screen &background) {
	int arrowCol = rand() % 4;
	int x = background.getScreenUnit() * (1 + arrowCol * 2);
	int y = background.getScreenHeight();
	Coordinate newArrow(x, y);
	arrowRange.push_back(newArrow);
}

//free all the game textures
void freeGamesTexture(GameTexture &texture, Music &song, Mix_Chunk* &bonk) {
	texture.free();
	cout << "Game Texture free successfully" << endl;
	song.freeMusicLoad();
	cout << "Music load free successfully" << endl;
	Mix_FreeChunk(bonk);
	bonk = NULL;
	cout << "Bonk sound free successfully" << endl;
}

#endif