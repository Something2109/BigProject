#ifndef _InitAndClose_h
#define _InitAndClose_h
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

const char* windowTitle = "Game";

bool init(SDL_Window*& window, SDL_Renderer*& renderer, const int& screenWidth, const int& screenHeight);
bool initSDL();
bool initWindow(SDL_Window*& window, SDL_Renderer*& renderer, const int& screenWidth, const int& screenHeight);
SDL_Texture* loadTexture(SDL_Renderer* renderer, string path);
void loadErrorLog();
void close(SDL_Window*& window, SDL_Renderer*& renderer);

bool init(SDL_Window*& window, SDL_Renderer*& renderer, const int& screenWidth, const int& screenHeight) {
	bool success = true;
	if (!initSDL()) {
		cout << "Failed to initialize SDL" << endl;
		success = false;
	}
	else {
		if (!initWindow(window, renderer, screenWidth, screenHeight)) {
			cout << "Failed to initialize Window" << endl;
			success = false;
		}
	}
	return success;
}

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

bool initWindow(SDL_Window* &window, SDL_Renderer*& renderer, const int& screenWidth, const int& screenHeight) {
	bool success = true;
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	if (window == NULL) {
		loadErrorLog();
		success = false;
	}
	else
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	return true;
}

void loadErrorLog()
{
	printf("Error: %s\n", SDL_GetError());
	printf("Error: %s\n", IMG_GetError());
	printf("Error: %s\n", Mix_GetError());
}

void close(SDL_Window*& window, SDL_Renderer*& renderer)
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

#endif