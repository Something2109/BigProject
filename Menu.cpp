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
	SDL_Renderer* renderer = screen.getRenderer();
	MenuTexture texture;
	bool quitMenu = false;

	if (texture.loadMenuTexture(screen, "Resource")) {
		while (!quitMenu) {
			event.updateEvent();
			quitMenu = event.quit();

			//cout << "Menu loop" << endl;
			SDL_RenderClear(screen.getRenderer());
			texture.eventHandle(event);
			background.renderBackground(MENU);
			background.renderColorStrip(MENU);
			texture.renderButton();
			texture.changeScreen(screen, background, event, musicList, quitMenu);
			SDL_RenderPresent(screen.getRenderer());
		}
	}
}

bool MenuTexture::loadMenuTexture(Screen& screen, const string& path) {
	bool success = true;
	renderer = screen.getRenderer();
	setScreenUnit(screen.getScreenUnit());
	SDL_Color textColor = { 255, 213, 8 };

	menuButton = new LTexture[3];
	if (!menuButton[PLAY].loadFromFile(renderer, path + "/Picture/playGameButton.png")) {
		loadErrorLog();
		success = false;
	}
	else {
		if (!menuButton[SETTING].loadFromFile(renderer, path + "/Picture/settingButton.png")) {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
			success = false;
		}
		else {
			if (!menuButton[EXIT].loadFromFile(renderer, path + "/Picture/exitButton.png")) {
				cout << "Log [" << SDL_GetTicks() << "]: ";
				loadErrorLog();
				success = false;
			}
			else {
				createMenuRect();
				cout << "Log [" << SDL_GetTicks() << "]: " << "Menu texture created successfully" << endl;
			}
		}
	}
	return success;
}

void MenuTexture::eventHandle(Event& event) {
	SDL_Point mousePos = event.getMousePos();
	SDL_Event e = event.getEvent();
	const Uint8* keyState = event.getKeyState();

	if (SDL_PointInRect(&mousePos, &menuDstRect[MENU])) {
		menuChoose = 0;
	}
	else {
		if (SDL_PointInRect(&mousePos, &menuDstRect[SETTING])) {
			menuChoose = 1;
		}
		else {
			if (SDL_PointInRect(&mousePos, &menuDstRect[EXIT])) {
				menuChoose = 2;
			}
			else {
				menuChoose = -1;
			}
		}
	}
	if (event.getMouseButton() != 0) {
		buttonClick = true;
	}
	else {
		buttonClick = false;
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

void MenuTexture::renderClickedButton(int &buttonType) {
	int clicked = screenUnit / 10;
	SDL_Rect clickedButton;

	menuButton[buttonType].setColor(255, 191, 255);
	clickedButton = { menuDstRect[buttonType].x + clicked, menuDstRect[buttonType].y + clicked,
				menuDstRect[buttonType].w - clicked, menuDstRect[buttonType].h - clicked };
	menuButton[buttonType].render(renderer, &clickedButton, NULL);
	menuButton[buttonType].setColor(255, 255, 255);
}

void MenuTexture::renderButton() {
	switch (menuChoose)
	{
	case PLAY: {
		if (buttonClick) {
			renderClickedButton(menuChoose);
		}
		else {
			menuButton[menuChoose].setColor(255, 191, 255);
			menuButton[menuChoose].render(renderer, &menuDstRect[menuChoose], NULL);
			menuButton[menuChoose].setColor(255, 255, 255);
		}
		menuButton[SETTING].render(renderer, &menuDstRect[SETTING], NULL);
		menuButton[EXIT].render(renderer, &menuDstRect[EXIT], NULL);
		break;
	}
	case SETTING: {
		if (buttonClick) {
			renderClickedButton(menuChoose);
		}
		else {
			menuButton[menuChoose].setColor(255, 191, 255);
			menuButton[menuChoose].render(renderer, &menuDstRect[menuChoose], NULL);
			menuButton[menuChoose].setColor(255, 255, 255);
		}
		menuButton[PLAY].render(renderer, &menuDstRect[PLAY], NULL);
		menuButton[EXIT].render(renderer, &menuDstRect[EXIT], NULL);
		break;
	}
	case EXIT: {
		if (buttonClick) {
			renderClickedButton(menuChoose);
		}
		else {
			menuButton[menuChoose].setColor(255, 191, 255);
			menuButton[menuChoose].render(renderer, &menuDstRect[menuChoose], NULL);
			menuButton[menuChoose].setColor(255, 255, 255);
		}
		menuButton[PLAY].render(renderer, &menuDstRect[PLAY], NULL);
		menuButton[SETTING].render(renderer, &menuDstRect[SETTING], NULL);
		break;
	}
	default: {
		menuButton[PLAY].render(renderer, &menuDstRect[PLAY], NULL);
		menuButton[SETTING].render(renderer, &menuDstRect[SETTING], NULL);
		menuButton[EXIT].render(renderer, &menuDstRect[EXIT], NULL);
	}
	}
}

void MenuTexture::changeScreen(Screen &screen, Background &background, Event &event, vector<Music> &musicList, bool &quit) {
	if (buttonClick) {
		switch (menuChoose) {
		case PLAY: {
			background.moveBackground(MENU, GAME);
			game(screen, background, event, musicList[0]);
			//background.moveBackground(CHOOSE_MUSIC, GAME);
			background.moveBackground(GAME, MENU);
			break;
		}
		case EXIT: {
			quit = true;
			break;
		}
		}
	}
}

void MenuTexture::freeMenu() {
	for (int i = 0; i < TOTAL_ARROW; i++) {
		menuButton[PLAY].free();
		menuButton[SETTING].free();
		menuButton[EXIT].free();
	}
	delete[] menuButton;
	delete[] menuDstRect;
	menuDstRect = NULL;
	renderer = NULL;
	setScreenUnit(0);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Menu Texture freed successfully" << endl;
}
