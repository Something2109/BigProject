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
	saveSettingFile(screen, event, "Resource");
	closeWindow(screen, event);
	quitSDL();
	return 0;
}