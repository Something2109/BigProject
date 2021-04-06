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
using namespace std;

//this file contains functions to initiate and close SDL, renderer, window

// function contain all the initiation function
bool init(Screen& screen) {
	bool success = true;
	if (!initSDL()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to initialize SDL" << endl;
		success = false;
	}
	else {
		if (!initWindow(screen)) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to initialize Window" << endl;
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
			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 10240) < 0)
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
bool initWindow(Screen& screen) {
	bool success = true;
	if (!loadSettingFile(screen, "Resource")) {
		screen.setScreenUnit(80);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Couldn't open file, using default resolution 1280 x 720" << endl;
	}
	int screenWidth = screen.getScreenWidth(), screenHeight = screen.getScreenHeight();
	
	if (!screen.createWindow()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to create window" << endl;
		loadErrorLog();
		success = false;
	}
	else
	{
		if (!screen.createRenderer()) {
			success = false;
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load the renderer" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Renderer loaded successfully in " << screen.getRenderer()  << endl;
		}
	}
	return success;
}

//close window and SDL function
void closeWindow(Screen& background)
{
	background.freeScreen();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Window closed successfully" << endl;
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