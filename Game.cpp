#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Game.h"
using namespace std;

//this file contains the game running related functions

//main game function
void game(Screen& screen, Background& background, Event& event, Music& song) {

	//game texture
	ArrowTexture texture;
	DogeTexture dogeTexture;

	//quit game
	bool quit = false;

	//load game resources
	if (!loadingGameResource(screen, texture, dogeTexture,song)) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to loading game" << endl;
	}
	else {
		//pregame load
		song.playMusic(0);
		texture.setTimeline(song);

		//game loop
		while (!quit) {

			//event handle
			event.updateEvent();
			quit = texture.continueGame(event);

			//arrow spawn mechanism
			texture.addNewArrow(song);

			//render texture
			screen.clearRenderer();
			background.renderBackground(GAME);
			background.renderColorStrip(GAME);
			texture.renderPressedArrow(event);
			dogeTexture.renderDogeScreen(event);
			texture.renderArrowRange();
			texture.updateArrowRange(song.getVelocity());
			screen.presentRenderer();
		
		}
	}

	//free game resources
	freeGamesTexture(texture, dogeTexture, song);
}

//loading resources
bool loadingGameResource(Screen& screen, ArrowTexture& texture, DogeTexture &dogeTexture, Music& song) {
	string path = "Resource";
	bool success = true;

	//load the game texture
	if (texture.loadTexture(screen, path)) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Game Texture loaded successfully" << endl;

		if (dogeTexture.loadTexture(screen, path)) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Doge Texture loaded successfully" << endl;

			if (song.loadFromFile(path)) {
				cout << "Log [" << SDL_GetTicks() << "]: " << song.getMusicName() << " loaded successfully" << endl;

			}
			else {
				cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load " << song.getMusicName() << endl;
				success = false;
			}
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load Doge Texture" << endl;
			success = false;
		}
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load Game Texture" << endl;
		success = false;
	}
	return success;
}

bool ArrowTexture::loadTexture(Screen &screen, const string& path) {
	bool success = true;
	renderer = screen.getRenderer();
	setScreenUnit(screen.getScreenUnit());

	if (!blankArrow.loadFromFile(renderer, path + "/Picture/blankArrow.png")) {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
		success = false;
	}
	else {
		if (!arrow.loadFromFile(renderer, path + "/Picture/arrow.png")) {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
			success = false;
		}
		else {
			if (!pressedArrow.loadFromFile(renderer, path + "/Picture/pressedArrow.png")) {
				cout << "Log [" << SDL_GetTicks() << "]: ";
				loadErrorLog();
				success = false;
			}
			else {
				createArrowRect();
			}
		}
	}
	return success;
}

bool DogeTexture::loadTexture(Screen& screen, const string& path) {
	bool success = true;
	renderer = screen.getRenderer();
	setScreenUnit(screen.getScreenUnit());

	if (!muscleDoge.loadFromFile(renderer, path + "/Picture/muscleDoge.png")) {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
		success = false;
	}
	else {
		if (!cheems.loadFromFile(renderer, path + "/Picture/cheems.png")) {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
			success = false;
		}
		else {
			if (!smashedCheems.loadFromFile(renderer, path + "/Picture/smashedCheems.png")) {
				cout << "Log [" << SDL_GetTicks() << "]: ";
				loadErrorLog();
				success = false;
			}
			else {
				createDogeRect();
				bonk = Mix_LoadWAV((path + "/Music/bonk.wav").c_str());
				if (bonk == NULL) {
					cout << "Log [" << SDL_GetTicks() << "]: ";
					loadErrorLog();
					success = false;
				}
			}
		}
	}
	return success;
}

void ArrowTexture::setTimeline(Music &song) {
	startTime = SDL_GetTicks();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game starting time: " << startTime << endl;
	endTime = SDL_GetTicks() + song.getDuration() + 5000;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game ending time: " << endTime << endl;
}

//add new arrow to arrow range
void ArrowTexture::addNewArrow(Music &song) {
	gameTime = SDL_GetTicks() - startTime;
	Uint32 stopArrowTime = endTime - 2000 - screenHeight / song.getVelocity();

	if (gameTime > (arrowCount * song.getSpawnRate()) && gameTime < stopArrowTime) {
		int arrowCol = rand() % 4;
		int x = screenUnit * (1 + arrowCol * 2);
		int y = screenHeight;
		Coordinate newArrow(x, y);
		arrowRange.push_back(newArrow);
		arrowCount++;
	}
}

bool ArrowTexture::continueGame(Event& event) {
	if (gameTime < endTime && !event.quit()) {
		return false;
	}
	return true;
}

void ArrowTexture::scoreCheck(int& keyType) {
	if (!arrowRange.empty()) {
		int arrowCol = arrowRange[0].getArrowCol(screenUnit);
		if (arrowCol == keyType && arrowRange[0].getY() >= 0 && arrowRange[0].getY() <= 2 * screenUnit) {
			SDL_Rect renderRect = arrowRange[0].getRect(screenUnit, screenUnit);
			arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
			arrowRange.erase(arrowRange.begin());
		}
	}
}

void DogeTexture::playBonkSound() {
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayChannel(0, bonk, 0);
	}
}

//update the arrow range
void ArrowTexture::updateArrowRange(const int& velocity) {
	for (int i = 0; i < arrowRange.size(); i++) {
		if (arrowRange[i].getY() <= -screenUnit) {
			arrowRange.erase(arrowRange.begin() + i);
		}
		else {
			arrowRange[i].arrowMove(velocity);
		}
	}
}

//free all the texture
void ArrowTexture::freeArrowTexture() {
	arrow.free();
	blankArrow.free();
	pressedArrow.free();

	delete blankArrowDstRect;
	blankArrowDstRect = NULL;
	delete[] arrowSrcRect;
	arrowSrcRect = NULL;
	delete[] pressedArrowDstRect;
	pressedArrowDstRect = NULL;

	renderer = NULL;
	setScreenUnit(0);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game Texture freed successfully" << endl;
}

void DogeTexture::freeDogeTexture() {
	muscleDoge.free();
	cheems.free();
	smashedCheems.free();
	Mix_FreeChunk(bonk);
	bonk = NULL;

	delete[] muscleDogeRect;
	muscleDogeRect = NULL;
	delete[] cheemsSrcRect;
	cheemsSrcRect = NULL;
	delete[] cheemsDstRect;
	cheemsDstRect = NULL;

	renderer = NULL;
	setScreenUnit(0);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Doge Texture freed successfully" << endl;
}

//free all the game textures
void freeGamesTexture(ArrowTexture& texture, DogeTexture &dogeTexture, Music& song) {
	texture.freeArrowTexture();
	dogeTexture.freeDogeTexture();
	song.freeMusicLoad();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Music load freed successfully" << endl;
}
