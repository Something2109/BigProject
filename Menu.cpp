#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Menu.h"
using namespace std;

void menu(Screen& screen, Background& background, Event& event, vector<Music> &musicList) {
	MenuTexture texture(screen);
	bool quitMenu = false;

	if (texture.load("Resource")) {
		while (!quitMenu) {
			event.updateEvent();

			//cout << "Menu loop" << endl;
			screen.clearRenderer();
			texture.eventHandle(event);
			background.renderBackground(BACKGROUND::MENU);
			background.renderColorStrip(BACKGROUND::MENU);
			texture.render();
			texture.changeScreen(screen, background, event, musicList, quitMenu);
			screen.presentRenderer();

			quitMenu = quitMenu || event.quit();
		}
	}
}



//MenuTexture

//constructor
MenuTexture::MenuTexture(Screen& screen)
{
	setRenderer(screen);
}

//basic function

bool MenuTexture::load(const string& path) {
	bool success = true;
	SDL_Color textColor = { 255, 213, 8 };

	menuButton = new LButton[static_cast<int> (MENU::TOTAL)];
	for (int type = 0; type < static_cast<int> (MENU::TOTAL); type++) {
		string buttonPath = path;
		switch (static_cast<MENU> (type)) {
			case MENU::PLAY: {
				buttonPath += "/Picture/playGameButton.png";
				break;
			}
			case MENU::SETTING: {
				buttonPath += "/Picture/settingButton.png";
				break;
			}
			case MENU::EXIT: {
				buttonPath += "/Picture/exitButton.png";
				break;
			}
		}
		if (!menuButton[type].loadFromFile(renderer, buttonPath)) {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
			success = false;
			break;
		}
	}
	if (success) {
		createRect();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Menu texture created successfully" << endl;
	}
	return success;
}

void MenuTexture::render() {
	for (int type = 0; type < static_cast<int> (MENU::TOTAL); type++) {
		menuButton[type].render(renderer, &menuDstRect[type], NULL);
	}
}

void MenuTexture::free() {
	for (int type = 0; type < static_cast<int> (MENU::TOTAL); type++) {
		menuButton[type].free();
	}
	delete[] menuButton;
	delete[] menuDstRect;
	menuDstRect = NULL;
	renderer = NULL;
	setScreenUnit(0);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Menu Texture freed successfully" << endl;
}

//event function

void MenuTexture::eventHandle(Event& event) {
	SDL_Point mousePos = event.getMousePos();

	if (event.getMouseButton() != 0) {
		buttonClick = true;
	}
	else {
		buttonClick = false;
	}

	if (SDL_PointInRect(&mousePos, &menuDstRect[static_cast<int> (MENU::PLAY)])) {
		menuChoose = MENU::PLAY;
	}
	else {
		if (SDL_PointInRect(&mousePos, &menuDstRect[static_cast<int> (MENU::SETTING)])) {
			menuChoose = MENU::SETTING;
		}
		else {
			if (SDL_PointInRect(&mousePos, &menuDstRect[static_cast<int> (MENU::EXIT)])) {
				menuChoose = MENU::EXIT;
			}
			else {
				menuChoose = MENU::NOT_CHOOSE;
			}
		}
	}
	if (menuChoose != MENU::NOT_CHOOSE) {
		menuButton[static_cast<int> (menuChoose)].hovered();
		if (event.getMouseButton() != 0) {
			menuButton[static_cast<int> (menuChoose)].clicked();
		}
	}
	/*if (menuChoose == -1) {
		if (e.key.keysym.sym == SDLK_DOWN) {
			cout << "Down Arrow" << endl;
			menuChoose = 0;
		}
		if (e.key.keysym.sym == SDLK_UP) {
			menuChoose = 2;
		}
	}
	else {
		if (e.key.keysym.sym == SDLK_DOWN && e.key.repeat == 0) {
			menuChoose = (menuChoose + 1) % TOTAL_BUTTON;			
			cout << "Down Arrow" << endl;

		}
		if (e.key.keysym.sym == SDLK_UP && e.key.repeat == 0) {
			menuChoose = (menuChoose * 2 - 1) % TOTAL_BUTTON;
		}
	}*/
}

void MenuTexture::changeScreen(Screen &screen, Background &background, Event &event, vector<Music> &musicList, bool &quit) {
	if (buttonClick) {
		switch (menuChoose) {
		case MENU::PLAY: {
			background.moveBackground(BACKGROUND::MENU, BACKGROUND::GAME);
			game(screen, background, event, musicList[1]);
			//background.moveBackground(CHOOSE_MUSIC, GAME);
			if (!event.quit()) {
				background.moveBackground(BACKGROUND::GAME, BACKGROUND::MENU);
			}
			break;
		}
		case MENU::EXIT: {
			quit = true;
			break;
		}
		}
	}
}
