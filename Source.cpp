#include <iostream>
#include <string>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Game.h" 
using namespace std;

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int screenWidth = 1280, screenHeight = 720;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	if (init(window, renderer, screenWidth, screenHeight)) {
		game(renderer, screenWidth, screenHeight);
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	close(window, renderer);
	return 0;
}