#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "InitAndClose.h"
using namespace std;

//this file contains functions to initiate and close SDL, renderer, window

// function contain all the initiation function
bool init(Window& screen, Event& event) {
	bool success = true;
	if (!initSDL()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to initialize SDL" << endl;
		success = false;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "SDL initialized successfully" << endl;
		cout << endl;

		if (!initWindow(screen, event)) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to initialize Window" << endl;
			success = false;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Window initialized successfully" << endl;
		}
	}
	cout << endl;
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
		cout << "Log [" << SDL_GetTicks() << "]: " "SDL initiated successfully" << endl;
		int imgFlags = IMG_INIT_PNG;
		if (!(imgFlags & IMG_INIT_PNG))
		{
			loadErrorLog();
			success = false;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " "SDL_Image initiated successfully" << endl;
			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 10240) < 0)
			{
				loadErrorLog();
				success = false;
			}
			else {
				cout << "Log [" << SDL_GetTicks() << "]: " "SDL_Mix initiated successfully" << endl;
				if (TTF_Init() == -1)
				{
					loadErrorLog();
					success = false;
				}
				else {
					cout << "Log [" << SDL_GetTicks() << "]: " "SDL_ttf initiated successfully" << endl;
				}
			}
		}
	}
	return success;
}

//Window initiation function
bool initWindow(Window& screen, Event& event) {
	bool success = true;

	loadSettingFile(screen, event, "Resource");

	if (!screen.createWindow()) {
		success = false;
	}
	else
	{
		if (!screen.createRenderer()) {
			success = false;
		}
	}
	return success;
}

//close window and SDL function
void closeWindow(Window& screen, Event& event)
{
	screen.free();
	event.freeEventControl();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Window closed successfully" << endl;
	cout << endl;
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