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
	Event event;

	if (init(screen, event)) {
		Background background(screen);
		if (!background.load("Resource")) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to create background" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Background created succcessfully" << endl;
			vector <Music> musicList;
			if (loadMusicFile(screen, musicList, "Resource")) {
				menu(screen, background, event, musicList);
			}
		}
		background.free();
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	closeWindow(screen, event);
	quitSDL();
	return 0;
}

