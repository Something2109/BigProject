#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Menu.h"
#include "Game.h"
using namespace std;

int main(int argc, char* argv[]) {
	srand(static_cast<int>(time(NULL)));
	Screen screen;
	if (init(screen)) {
		Background background;
		if (!background.loadBackground(screen, "Resource")) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to create background" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Background created succcessfully" << endl;
			Event event;
			vector <Music> musicList;
			if (loadMusicFile(screen, musicList, "Resource")) {
				menu(screen, background, event, musicList);
				//cout << static_cast<int> (SDLK_LEFT) << ' ' << static_cast<int> (SDL_SCANCODE_LEFT) << endl;
				//cout << static_cast<int> (SDLK_UP) << ' ' << static_cast<int> (SDL_SCANCODE_UP) << endl;
				//cout << static_cast<int> (SDLK_DOWN) << ' ' << static_cast<int> (SDL_SCANCODE_DOWN) << endl;
				//cout << static_cast<int> (SDLK_RIGHT) << ' ' << static_cast<int> (SDL_SCANCODE_RIGHT) << endl;
			}
		}
		background.freeBackground();
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	closeWindow(screen);
	quitSDL();
	return 0;
}

