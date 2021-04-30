#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Screen.h"
using namespace std;

//this file contains the game running related functions

Screen::Screen()
{
	background = nullptr;
	button = nullptr;
	title = nullptr;
	event = nullptr;
	arrow = nullptr;
	doge = nullptr;
	music = nullptr;
	setting = nullptr;
	point = nullptr;

	titleType = TITLE::MENU;
	quit = false;
	choose = BUTTON::NOT_CHOOSE;
}

Screen::Screen(Window& window)
{
	setRenderer(window);
	background = nullptr;
	button = nullptr;
	title = nullptr;
	event = nullptr;
	arrow = nullptr;
	doge = nullptr;
	music = nullptr;
	setting = nullptr;
	point = nullptr;

	titleType = TITLE::MENU;
	quit = false;
	choose = BUTTON::NOT_CHOOSE;
}

void Screen::pass(Screen* screen)
{
	setRenderer(*screen);
	background = screen->background;
	button = screen->button;
	title = screen->title;
	event = screen->event;
	arrow = screen->arrow;
	doge = screen->doge;
	music = screen->music;
	setting = screen->setting;
	point = screen->point;
}

void Screen::createRect()
{
	if (background != nullptr) {
		background->createDefaultRect();
	}
	if (title != nullptr) {
		title->createDefaultRect();
	}
	if (button != nullptr) {
		button->createRect(titleType);
	}
	if (arrow != nullptr) {
		arrow->createDefaultRect();
	}
	if (doge != nullptr) {
		doge->createDefaultRect();
	}
	if (music != nullptr) {
		background->createDefaultRect();
	}
	if (setting != nullptr) {
		background->createDefaultRect();
	}
	if (point != nullptr) {
		background->createDefaultRect();
	}
}

void Screen::windowHandle()
{
	if (event->getEventType() == SDL_WINDOWEVENT) {
		setDisplaySize(event);
		if (*sizeChange) {
			createRect();
			*sizeChange = false;
		}
	}
}



//Menu

Menu::Menu(Window& screen) {
	setRenderer(screen);
	background = new Background(screen);
	button = new Button(screen);
	event = new Event();
	title = new Title(screen);
	titleType = TITLE::MENU;
}

Menu::Menu(Window& screen, Event& _event)
{
	setRenderer(screen);
	background = new Background(screen);
	button = new Button(screen);
	event = &_event;
	title = new Title(screen);
}

bool Menu::load(const string& path)
{
	bool success = true;
	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	if (success) {
		background->setType(BACKGROUND::START);		
		button->createRect(titleType);
		title->setType(TITLE::INTRO1);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Menu loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load menu" << endl;
	}
	cout << endl;

	return success;
}

void Menu::run()
{
	string path = "Resource";
	if (load(path)) {
		renderIntro(); 
		renderStartScreen();
		renderMenu();
	}
	free();
}

void Menu::changeScreen()
{
	switch (choose) {
		case BUTTON::PLAY: {
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::CHOOSE_MUSIC);
			ChooseMusic chooseMusic(this);
			chooseMusic.run();
			break;
		}
		case BUTTON::SETTING: {
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::SETTING);
			Setting setting(this);
			setting.run();
			button->createRect(titleType);
			if (!event->quit()) {
				background->backgroundTransition(BACKGROUND::SETTING, BACKGROUND::MENU);
			}
			break;
		}
		case BUTTON::EXIT: {
			quit = true;
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::EXIT);
			break;
		}
	}
	title->setType(TITLE::MENU);
}

void Menu::free()
{
	background->free();
	button->free();
	title->free();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Menu freed successfully" << endl;
	cout << endl;
}

void Menu::renderIntro()
{
	background->backgroundTransition(BACKGROUND::INTRO1, BACKGROUND::INTRO2);
	Uint32 time = SDL_GetTicks() + 2000;
	while (SDL_GetTicks() < time) {
		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		title->render();
		presentRenderer();

		quit = event->quit();
	}
	background->backgroundTransition(BACKGROUND::INTRO2, BACKGROUND::INTRO3);
	time = SDL_GetTicks() + 2000;
	while (SDL_GetTicks() < time) {
		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		//title->render();
		presentRenderer();

		quit = event->quit();
	}
	background->backgroundTransition(BACKGROUND::INTRO3, BACKGROUND::START);
	title->setType(TITLE::EXIT);
}

void Menu::renderStartScreen()
{
	while (!quit && event->getEventType() != SDL_KEYDOWN && event->getMouseButton() == 0) {
		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		button->pressAnyKeyRender();
		title->render();		
		presentRenderer();

		quit = event->quit();
	}
	if (!quit) {
		background->backgroundTransition(BACKGROUND::START, BACKGROUND::MENU);
		title->setType(titleType);
	}
}

void Menu::renderMenu()
{
	while (!quit) {

		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		button->render();
		title->render();		
		presentRenderer();

		choose = static_cast<BUTTON>(button->eventHandle(*event));
		changeScreen();

		quit = quit || event->quit();
	}
}



//ChooseMusic

ChooseMusic::ChooseMusic(Screen* screen)
{
	setRenderer(*screen);
	pass(screen);
	music = new MusicTexture(*screen);
	musicChoose = 0;
	titleType = TITLE::CHOOSE_MUSIC;
}

bool ChooseMusic::load(const string& path)
{
	bool success = true;
	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	success = success && loadMusicFile(musicList, path);
	success = success && music->load(path);
	success = success && music->loadSong(musicList[musicChoose]);
	success = success && musicList[musicChoose].loadFromFile(path);
	if (success) {
		background->setType(BACKGROUND::CHOOSE_MUSIC);
		button->createRect(titleType);
		title->setType(titleType);
		musicList[musicChoose].playMusic(-1);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music choose menu loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load music choose menu" << endl;
	}
	cout << endl;
	return success;
}

void ChooseMusic::run()
{
	string path = "Resource";
	if (load(path)) {
		while (!quit) {
			windowHandle();
			event->updateEvent();

			clearRenderer();
			background->render();
			button->render();
			title->render();
			music->render();
			presentRenderer();

			choose = static_cast<BUTTON>(button->eventHandle(*event));
			changeMusic();
			changeScreen();
		}
	}
	Mix_HaltMusic();
	free();
	button->createRect(TITLE::MENU);
}

void ChooseMusic::changeScreen()
{
	switch (choose) {
		case BUTTON::PLAY: {
			Mix_HaltMusic();
			background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::GAME);
			Game game(this, musicList[musicChoose]);
			game.run();
			if (!event->quit()) {
				background->backgroundTransition(BACKGROUND::GAME, BACKGROUND::MENU);
			}
			quit = true;
			break;
		}
		case BUTTON::EXIT: {
			quit = true;
			background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::MENU);
			break;
		}
		default: {
			quit = event->quit() || event->pause();
			if (event->pause()) {
				background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::MENU);
			}
		}
	}
}

void ChooseMusic::free()
{
	musicList[musicChoose].freeLoad();
	musicList.erase(musicList.begin(), musicList.end());
	music->free();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Music choose menu freed successfully" << endl;
	cout << endl;
}

void ChooseMusic::changeMusic()   
{
	bool musicChange = false;
	if ((event->checkKeyEventDown(CONTROL::LEFT_ARROW)) || (choose == BUTTON::CHANGE_SONG_LEFT)) {
		Mix_HaltMusic();
		musicList[musicChoose].freeLoad();
		musicChoose = (musicChoose - 1 + toInt(musicList.size())) % musicList.size();
		musicChange = true;
	}
	if ((event->checkKeyEventDown(CONTROL::RIGHT_ARROW)) || (choose == BUTTON::CHANGE_SONG_RIGHT)) {
		Mix_HaltMusic();
		musicList[musicChoose].freeLoad();
		musicChoose = (musicChoose + 1) % musicList.size();
		musicChange = true;
	}

	if (musicChange) {
		string path = "Resource";
		musicChange = musicList[musicChoose].loadFromFile(path) && music->loadSong(musicList[musicChoose]);
		if (musicChange) {
			musicList[musicChoose].playMusic(-1);
			cout << "Log [" << SDL_GetTicks() << "]: " << "Music changed successfully" << endl;
		}
		else {
			changeMusic();
		}
		cout << endl;
	}
}



//Game

Game::Game(Window& window)
{
	setRenderer(window);
	background = new Background(window);
	button = new Button(window);
	event = new Event();
	titleType = TITLE::PAUSE;
}

Game::Game(Screen* screen, Music& _song)
{
	setRenderer(*screen);
	pass(screen);
	arrow = new ArrowTexture(*screen);
	doge = new Character(*screen);
	point = new Point(*screen);
	song = &_song;
	titleType = TITLE::PAUSE;
}

bool Game::load(const string& path)
{
	bool success = true;

	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	success = success && arrow->load(path);
	success = success && doge->load(path);
	success = success && point->load(path);
	success = success && song->loadFromFile(path);
	if (success) {
		background->setType(BACKGROUND::GAME);
		button->createRect(titleType);
		title->setType(titleType);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Game loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load game" << endl;
	}
	cout << endl;
	return success;
}

void Game::run()
{
	string path = "Resource";
	if (load(path)) {
		arrow->setTimeline(*song);
		while (!quit) {
			time = SDL_GetTicks();
			windowHandle();

			if (!pause) {
				renderGame();
			}
			else {
				renderPause();
			}

			lastPauseTime = time;
		}
		Mix_HaltMusic();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Game ending time: " << SDL_GetTicks() << endl;
		cout << endl;
		quit = (quit && pause) || !point->loadPointWindow(path) || event->quit();
		title->setType(TITLE::FINISH);
		if (!quit) {
			renderScoreScreen();
		}
	}
	button->createRect(TITLE::MENU);
	free();
}

void Game::changeScreen()
{
	switch (choose) {
	case BUTTON::CONTINUE: {
		Mix_ResumeMusic();
		pause = false;
		break;
	}
	case BUTTON::SETTING: {
		background->backgroundTransition(BACKGROUND::GAME, BACKGROUND::SETTING);
		Setting setting(this);
		setting.run();
		button->createRect(titleType);
		if (!event->quit()) {
			background->backgroundTransition(BACKGROUND::SETTING, BACKGROUND::GAME);
		}
		break;
	}
	case BUTTON::EXIT: {
		quit = true;
		break;
	}
	}
}

void Game::free()
{
	arrow->free();
	doge->free();
	point->free();
	song->freeLoad();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game screen freed successfully" << endl;
	cout << endl;
}

void Game::renderGame()
{
	//event handle
	event->updateEvent();
	arrow->updateArrowRange(*point, (time - lastPauseTime));
	arrow->addArrow(time);

	clearRenderer();
	background->render();
	point->render();
	arrow->renderPressedArrow(*event, *point);
	arrow->render();
	doge->renderDoge(*event);
	doge->renderBank(*point);
	presentRenderer();

	//arrow spawn mechanism
	pause = event->pause();
	quit = !Mix_PlayingMusic() || event->quit();

}

void Game::renderPause()
{
	Mix_PauseMusic();

	//event handle
	event->updateEvent();
	arrow->addPauseTime(time - lastPauseTime);

	clearRenderer();
	background->render();
	title->render();
	button->render();
	presentRenderer();

	choose = static_cast<BUTTON>(button->eventHandle(*event));
	changeScreen();
	quit = quit || event->quit();
}

void Game::renderScoreScreen()
{
	button->createRect(TITLE::FINISH);

	while (!quit) {
		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		button->render();
		title->render();
		point->renderPointScreen();
		doge->renderDoge(*event);
		presentRenderer();

		choose = static_cast<BUTTON>(button->eventHandle(*event));
		quit = event->quit() || event->pause() || (choose == BUTTON::EXIT);
	}

}



//Setting

Setting::Setting(Screen* screen) {
	setRenderer(*screen);
	pass(screen);
	titleType = TITLE::SETTING;
	setting = new SettingTexture(*screen, *event);
	settingChoose = CONTROL::NOT_CHOOSE;
	settingChange = false;
}

bool Setting::load(const string& path)
{
	bool success = true;
	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	success = success && setting->load(path);
	if (success) {
		background->setType(BACKGROUND::SETTING);
		button->createRect(titleType);
		title->setType(titleType);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Setting menu loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load setting menu" << endl;
	}
	cout << endl;
	return success;
}

void Setting::run()
{
	string path = "Resource";
	if (load(path)) {
		while (!quit) {
			windowHandle();
			event->updateEvent();

			clearRenderer();
			background->render();
			title->render();
			button->render();
			setting->render();
			presentRenderer();

			choose = static_cast<BUTTON>(button->eventHandle(*event));
			settingChoose = static_cast<CONTROL>(setting->eventHandle(*event));
			settingHandler();
			changeScreen();
			quit = quit || event->quit() || event->pause();
		}
	}
	free();
}

void Setting::changeScreen()
{
	switch (choose) {
		case BUTTON::SAVE: {
			event->passEventControl(setting);
			button->buttonDisable(BUTTON::SAVE);
			break;
		}
		case BUTTON::EXIT: {
			quit = true;
			break;
		}
	}
}

void Setting::free()
{
	setting->free();
	setting = nullptr;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Setting menu freed successfully" << endl;
	cout << endl;
}

void Setting::settingHandler() {
	if (settingChoose != CONTROL::NOT_CHOOSE) {
		button->buttonDisable(BUTTON::SAVE);

		do {
			windowHandle();
			settingChange = setting->changeKey(settingChoose, *event);

			clearRenderer();
			background->render();
			title->render();
			button->render();
			button->pressAnyKeyRender();
			setting->hovered(settingChoose);
			setting->render();
			presentRenderer();

			choose = static_cast<BUTTON>(button->eventHandle(*event));
			changeScreen();
			quit = quit || event->quit() || event->pause();

		} while (!quit && !settingChange);

		string path = "Resource";
		setting->load(path);
		button->buttonEnable(BUTTON::SAVE);
		settingChoose = CONTROL::NOT_CHOOSE;
		cout << "Log [" << SDL_GetTicks() << "]: " << "Button changed successfully" << endl;
	}
}

