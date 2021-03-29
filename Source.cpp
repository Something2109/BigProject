#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Render.h"
#include "Menu.h"
#include "Game.h"
#include "Event.h"
using namespace std;

int main(int argc, char* argv[]) {
	srand(static_cast<int>(time(NULL)));
	SDL_Window* window = NULL;
	Screen background;
	if (init(window, background)) {
		//menu(background);
		Event event;
		vector <Music> musicList;
		if (loadMusicFile(background, musicList, "Resource")) {
			background.moveBackground(CHOOSE_MUSIC, GAME);
			game(background, event, musicList[0]);
		}
		else {
			cout << "Cannot open music file" << endl;
		}
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	closeWindow(window, background);
	quitSDL();
	return 0;
}

