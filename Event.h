#ifndef _Event_h
#define _Event_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

class Event {
	SDL_Event e;
	const Uint8* keyState = NULL;
	Uint8* keyControl = NULL;
public:
	void updateEvent() {
		SDL_Event state;
		SDL_PollEvent(&state);
		e = state;
		keyState = SDL_GetKeyboardState(NULL);
	}

	bool quitGame() {
		if (e.type == SDL_QUIT) {
			return true;
		}
		return false;
	}

	SDL_Event getEvent() {
		return e;
	}

	const Uint8* getKeyState() {
		return keyState;
	}
};

#endif
