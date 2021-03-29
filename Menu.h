#ifndef _Menu_h
#define _Menu_h
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
using namespace std;

void menu(Screen& background) {
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		if (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_RenderClear(background.getRenderer());
		background.renderBackground(MENU);
		background.renderColorStrip(MENU);
		SDL_RenderPresent(background.getRenderer());
	}
}

#endif
