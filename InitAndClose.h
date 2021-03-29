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
#include "FileReadAndWrite.h"
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
		cout << "SDL initiated successfully" << endl;
		int imgFlags = IMG_INIT_PNG;
		if (!(imgFlags & IMG_INIT_PNG))
		{
			loadErrorLog();
			success = false;
		}
		else {
			cout << "SDL_Image initiated successfully" << endl;
			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 8192) < 0)
			{
				loadErrorLog();
				success = false;
			}
			else {
				cout << "SDL_Mix initiated successfully" << endl;
				if (TTF_Init() == -1)
				{
					loadErrorLog();
					success = false;
				}
				else {
					cout << "SDL_ttf initiated successfully" << endl;
				}
			}
		}
	}
	return success;
}

//Window initiation function
bool initWindow(SDL_Window* &window, Screen &background) {
	bool success = true;
	if (!loadSettingFile(background, "Resource")) {
		background.setScreenUnit(80);
		cout << "Couldn't open file, using default setting" << endl;
	}
	else {
		cout << "Setting file loaded successfully" << endl;
	}
	int screenWidth = background.getScreenWidth(), screenHeight = background.getScreenHeight();
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	if (window == NULL) {
		loadErrorLog();
		success = false;
	}
	else
	{
		if (!background.createRenderer(window)) {
			success = false;
			cout << "Failed to load the renderer" << endl;
		}
		else {
			cout << "Renderer loaded successfully" << endl;
			if (!background.loadBackground("Resource")) {
				success = false;
				cout << "Failed to create background" << endl;
			}
			else {
				cout << "Background created succcessfully" << endl;
			}
		}
	}
	return success;
}

//close window and SDL function
void closeWindow(SDL_Window*& window, Screen& background)
{
	background.freeScreen();
	cout << "Background free successfully" << endl;
	SDL_DestroyWindow(window);
	window = NULL;
	cout << "Window closed successfully" << endl;
}

void quitSDL() {
	TTF_Quit();
	cout << "SDL_ttf quit successfully" << endl;
	Mix_Quit();
	cout << "SDL_Mix quit successfully" << endl;
	IMG_Quit();
	cout << "SDL_IMG quit successfully" << endl;
	SDL_Quit();
	cout << "SDL quit successfully" << endl;
}

#endif