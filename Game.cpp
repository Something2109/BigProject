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
	ArrowTexture arrowTexture(screen);
	DogeTexture dogeTexture(screen);

	Point point(screen);

	//quit game
	bool quit = false;

	//load game resources
	if (!loadingGameResource(screen, arrowTexture, dogeTexture, song, point)) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to loading game" << endl;
	}
	else {
		//pregame load
		song.playMusic(0);
		arrowTexture.setTimeline(song);

		//game loop
		while (!quit) {

			//event handle
			event.updateEvent();

			//arrow spawn mechanism
			arrowTexture.addNewArrow(song, point);

			//render texture
			screen.clearRenderer();
			background.render(BACKGROUND::GAME);
			point.renderGamePoint();
			arrowTexture.renderPressedArrow(event, point);
			dogeTexture.render(event);
			arrowTexture.renderArrowRange();
			arrowTexture.updateArrowRange(song, point);
			screen.presentRenderer();

			quit = !Mix_PlayingMusic() || event.quit();
		}
		Mix_PauseMusic();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Game ending time: " << SDL_GetTicks() << endl;

		string path = "Resource";
		quit = !point.loadPointScreen(path) || event.quit();
		
		while (!quit) {
			event.updateEvent();

			screen.clearRenderer();
			background.render(BACKGROUND::GAME);
			point.renderPointScreen();
			dogeTexture.render(event);
			screen.presentRenderer();

			quit = event.quit();
		}
	}

	//free game resources
	freeGamesTexture(arrowTexture, dogeTexture, song, point);
}

//loading resources
bool loadingGameResource(Screen& screen, ArrowTexture& arrowTexture, DogeTexture &dogeTexture, Music& song, Point& point) {
	string path = "Resource";
	bool success = true;

	//load the game texture
	if (arrowTexture.load(path)) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Arrow Texture loaded successfully" << endl;

		if (dogeTexture.load(path)) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Doge Texture loaded successfully" << endl;

			if (song.loadFromFile(path)) {
				cout << "Log [" << SDL_GetTicks() << "]: " << song.getSongName() << " loaded successfully" << endl;

				if (point.loadGameFont(path)) {
					cout << "Log [" << SDL_GetTicks() << "]: " << " Point font loaded successfully" << endl;
				}
				else {
					cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load point font" << endl;
					success = false;
				}
			}
			else {
				cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load " << song.getSongName() << endl;
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

//free all the game textures
void freeGamesTexture(ArrowTexture& arrowTexture, DogeTexture& dogeTexture, Music& song, Point& point) {
	arrowTexture.free();
	dogeTexture.free();
	song.freeMusicLoad();
	point.free();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Music load freed successfully" << endl;
}



//ArrowTexture

//constructor

ArrowTexture::ArrowTexture(Screen& screen) {
	setRenderer(screen);
}

//basic functions

bool ArrowTexture::load(const string& path) {
	bool success = true;

	blankArrow.free();
	arrow.free();
	pressedArrow.free();

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
				createRect();
			}
		}
	}
	return success;
}

void ArrowTexture::render(Event& event, Point& point) {
	renderPressedArrow(event, point);
	renderArrowRange();
}

void ArrowTexture::free() {
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
	cout << "Log [" << SDL_GetTicks() << "]: " << "Doge Texture freed successfully" << endl;
}

//Arrow spawn functions

void ArrowTexture::setTimeline(Music &song) {
	startGameTime = SDL_GetTicks();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game starting time: " << startGameTime << endl;
	spawnDuration = song.getDuration() - (screenHeight + screenUnit) * 1000 / song.getVelocity() / 60;
}

//add new arrow to arrow range
void ArrowTexture::addNewArrow(Music &song, Point &point) {
	gameTime = SDL_GetTicks() - startGameTime;
	if (gameTime < spawnDuration) {
		if (gameTime > (arrowCount * song.getSpawnRate())) {
			int arrowCol = rand() % 4;
			int x = screenUnit * (1 + arrowCol * 2);
			int y = screenHeight;
			SDL_Point newArrow = { x, y };
			arrowRange.push_back(newArrow);
			arrowCount++;
		}
	}
}

void ArrowTexture::scoreCheck(int& keyType, Point &point) {
	if (!arrowRange.empty()) {
		int arrowCol = (arrowRange[0].x / screenUnit - 1) / 2 ;
		if (arrowCol == keyType) {
			int accuracy = abs(arrowRange[0].y * 50 / screenUnit - 50);
			if (accuracy <= 50) {
				point.addPoint(accuracy);
				point.arrowCount();
				SDL_Rect renderRect = { arrowRange[0].x, arrowRange[0].y, screenUnit, screenUnit };
				arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
				arrowRange.erase(arrowRange.begin());
			}
			else {
				point.wrongPressCount();
			}
		}
		else {
			point.wrongPressCount();
		}
	}
}

void ArrowTexture::updateArrowRange(Music &song, Point &point) {
	int velocity = song.getVelocity();
	for (int i = 0; i < arrowRange.size(); i++) {
		if (arrowRange[i].y <= -screenUnit) {
			arrowRange.erase(arrowRange.begin() + i);
			point.arrowCount();
		}
		else {
			arrowRange[i].y -= velocity;
		}
	}
}



//DogeTexture
 
//Constructor

DogeTexture::DogeTexture(Screen& screen)
{
	setRenderer(screen);
}

//Basic functions

bool DogeTexture::load(const string& path) {
	bool success = true;

	muscleDoge.free();
	cheems.free();
	smashedCheems.free();
	Mix_FreeChunk(bonk);

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
				createRect();
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

void DogeTexture::free() {
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

//play sound

void DogeTexture::playBonkSound() {
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayChannel(0, bonk, 0);
	}
}
