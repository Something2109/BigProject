#ifndef _Menu_h
#define _Menu_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Texture.h"
#include "Game.h" 
#include "Render.h"
using namespace std;

void menu(SDL_Renderer* renderer, Screen& background, const int &screenUnit) {
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		if (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_RenderClear(renderer);
		background.renderBackground(0);
		SDL_RenderPresent(renderer);
	}
}

#endif
