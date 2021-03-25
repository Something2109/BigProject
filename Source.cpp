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
using namespace std;

int main(int argc, char* argv[]) {
	srand(static_cast<int>(time(NULL)));
	SDL_Window* window = NULL;
	Screen background;
	if (init(window, background)) {
		background.loadBackground("Resource");
		game(background, 8, 500);
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	close(window, background);
	return 0;
}
