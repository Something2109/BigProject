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
#include "Background.h"
#include "Game.h"
using namespace std;

class MenuTexture : private Screen {

	LTexture* menuButton = NULL;
	SDL_Rect* menuDstRect = NULL;
	int menuChoose = -1;
	bool buttonClick = false;

public:

	void createMenuRect();

	bool loadMenuTexture(Screen& screen, const string& path);

	void eventHandle(Event& event);

	void renderButton();

	void renderClickedButton(int &buttonType);

	void changeScreen(Screen& screen, Background& background, Event& event, vector<Music>& musicList, bool& quit);

	void freeMenu();

};

void menu(Screen& screen, Background& background, Event& event, vector<Music>&musicList);

#endif
