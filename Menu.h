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
#include "Utility.h"
#include "Background.h"
#include "Game.h"
using namespace std;

class MenuTexture : private Screen {

	LButton* menuButton = NULL;
	SDL_Rect* menuDstRect = NULL;
	MENU menuChoose = MENU::NOT_CHOOSE;
	bool buttonClick = false;

public:

	//constructor

	MenuTexture(Screen& screen);

	//basic functions

	bool load(const string& path);

	void render();

	void free();

	//event functions

	void eventHandle(Event& event);

	void changeScreen(Screen& screen, Background& background, Event& event, vector<Music>& musicList, bool& quit);

	//render functions

	void createRect();

};

void menu(Screen& screen, Background& background, Event& event, vector<Music>&musicList);

#endif
