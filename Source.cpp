#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "InitAndClose.h"
#include "Texture.h"
#include "Game.h" 
#include "Render.h"
using namespace std;

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int screenHeight = 720, screenWidth = (screenHeight / 9) * 16;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	if (init(window, renderer, screenWidth, screenHeight)) {
		const int screenUnit = screenHeight / 9;
		game(renderer, screenUnit, 8, 500);
		/*Arrow arrowDemo(5, 0, 80);
		arrowDemo.loadFromFile(renderer, "Resource/arrow.png");
		SDL_Event e;
		int arrowCount = 0;
		bool quit = false;
		while (!quit) {
			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else {
					SDL_RenderClear(renderer);
					arrowDemo.move();
					arrowDemo.render(renderer, 80);
					SDL_RenderPresent(renderer);
				}
			}
		}
		arrowDemo.free();*/
	}
	else {
		cout << "Failed to initialize" << endl;
	}
	close(window, renderer);
	return 0;
}