#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Screen.h"
using namespace std;

//int main(int argc, char* argv[]) {
//	srand(toInt(time(NULL)));
//	Window screen;
//	Event event;
//
//	if (init(screen, event)) {
//		Background background(screen);
//		if (!background.load("Resource")) {
//			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to create background" << endl;
//		}
//		else {
//			cout << "Log [" << SDL_GetTicks() << "]: " << "Background created succcessfully" << endl;
//			vector <Music> musicList;
//			if (loadMusicFile(musicList, "Resource", screen.getScreenUnit())) {
//				background.backgroundTransition(BACKGROUND::EXIT, BACKGROUND::MENU);
//				menu(screen, background, event, musicList);
//			}
//		}
//		background.free();
//	}
//	else {
//		cout << "Failed to initialize" << endl;
//	}
//	closeWindow(screen, event);
//	quitSDL();
//	return 0;
//}

int main(int argc, char* argv[]) {
	srand(toInt(time(NULL)));
	Window screen;
	Event event;

	if (init(screen, event)) {
		Menu menu(screen, event);
		menu.run();
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	closeWindow(screen);
	quitSDL();
	return 0;
}
