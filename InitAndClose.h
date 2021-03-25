#ifndef _InitAndClose_h
#define _InitAndClose_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Texture.h"
using namespace std;

//this file contains functions to initiate and close SDL, renderer, window

const char* windowTitle = "Game";

bool init(SDL_Window*& window, Screen &background);
bool initSDL();
bool initWindow(SDL_Window*& window, Screen &background);
void loadErrorLog();
void close(SDL_Window*& window, Screen &background);

// function contain all the initiation function
bool init(SDL_Window*& window, Screen& background) {
	bool success = true;
	if (!initSDL()) {
		cout << "Failed to initialize SDL" << endl;
		success = false;
	}
	else {
		if (!initWindow(window, background)) {
			cout << "Failed to initialize Window" << endl;
			success = false;
		}
	}
	return success;
}

//SDL initiation function
bool initSDL()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		loadErrorLog();
		success = false;
	}
	else {
		int imgFlags = IMG_INIT_PNG;
		if (!(imgFlags & IMG_INIT_PNG))
		{
			loadErrorLog();
			success = false;
		}
		else {
			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				loadErrorLog();
				success = false;
			}
		}
	}
	return success;
}

//Window initiation function
bool initWindow(SDL_Window* &window, Screen &background) {
	bool success = true;
	int screenHeight = background.getScreenUnit() * 9, screenWidth = background.getScreenUnit() * 16;
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	if (window == NULL) {
		loadErrorLog();
		success = false;
	}
	else
	{
		success = background.createRenderer(window);
	}
	return success;
}

//close window and SDL function
void close(SDL_Window*& window, Screen& background)
{
	background.freeScreen();
	SDL_DestroyWindow(window);
	window = NULL;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

#endif