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
	character = nullptr;
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
	character = nullptr;
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
	character = screen->character;
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
	if (character != nullptr) {
		character->createDefaultRect();
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
	setDisplaySize(event);
	if (*sizeChange) {
		createRect();
		*sizeChange = false;
	}
}



//Menu

Menu::Menu(Window& screen) {
	setRenderer(screen);
	background = new Background(screen);
	button = new Button(screen);
	title = new Title(screen);
	character = new Character(screen);
	event = new Event();
	titleType = TITLE::MENU;
}

Menu::Menu(Window& screen, Event& _event)
{
	setRenderer(screen);
	background = new Background(screen);
	button = new Button(screen);
	title = new Title(screen);
	character = new Character(screen);
	event = &_event;
	titleType = TITLE::MENU;
}

bool Menu::load(const string& path)
{
	bool success = true;
	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	success = success && character->load(path);
	if (success) {
		titleType = TITLE::INTRO;
		background->setType(BACKGROUND::START);		
		button->createRect(titleType);
		title->setType(titleType);
		character->createRect(titleType);
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
		renderMenu();
	}
	free();
}

void Menu::changeScreen()
{
	switch (choose) {
		case BUTTON::PLAY: {
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::CHOOSE_MUSIC, *event);
			ChooseMusic chooseMusic(this);
			chooseMusic.run();
			button->createRect(titleType);
			title->setType(titleType);
			character->createRect(titleType);
			break;
		}
		case BUTTON::SETTING: {
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::SETTING, *event);
			Setting setting(this);
			setting.run();
			if (!event->quit()) {
				background->backgroundTransition(BACKGROUND::SETTING, BACKGROUND::MENU, *event);
			}
			button->createRect(titleType);
			title->setType(titleType);
			character->createRect(titleType);
			break;
		}
		case BUTTON::EXIT: {
			quit = true;
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::EXIT, *event);
			break;
		}
		case BUTTON::SCORE: {
			background->backgroundTransition(BACKGROUND::MENU, BACKGROUND::SCORE, *event);
			Score score(this);
			score.run();
			button->createRect(titleType);
			title->setType(titleType);
			character->createRect(titleType);
			break;
		}
	}
}

void Menu::free()
{
	background->free();
	button->free();
	title->free();
	character->free();

	delete background;
	delete button;
	delete title;
	delete character;
	background = nullptr;
	button = nullptr;
	title = nullptr;
	character = nullptr;
	
	cout << "Log [" << SDL_GetTicks() << "]: " << "Menu freed successfully" << endl;
	cout << endl;
}

void Menu::renderIntro()
{
	Uint32 time;

	if (!quit) {
		background->backgroundTransition(BACKGROUND::INTRO1, BACKGROUND::INTRO2, *event);
		title->setType(TITLE::INTRO);
		quit = event->quit() || event->checkKeyEventDown(CONTROL::CHOOSE);
		time = SDL_GetTicks() + 3000;
		while (!quit && SDL_GetTicks() < time) {
			windowHandle();
			event->updateEvent();

			clearRenderer();
			background->render();
			button->render();
			title->render();
			presentRenderer();

			quit = event->quit() || event->checkKeyEventDown(CONTROL::CHOOSE);
		}
	}

	if (!quit) {
		background->backgroundTransition(BACKGROUND::INTRO2, BACKGROUND::INTRO3, *event);
		title->setType(TITLE::NO_TITLE);
		quit = event->quit() || event->checkKeyEventDown(CONTROL::CHOOSE);
		time = SDL_GetTicks() + 3000;
		while (!quit && SDL_GetTicks() < time) {
			windowHandle();
			event->updateEvent();

			clearRenderer();
			background->render();
			button->render();
			character->renderIntro();
			presentRenderer();

			quit = event->quit() || event->checkKeyEventDown(CONTROL::CHOOSE);
		}
	}

	if (!quit) {
		background->backgroundTransition(BACKGROUND::INTRO3, BACKGROUND::START, *event);
		title->setType(TITLE::START);
		quit = event->quit() || event->checkKeyEventDown(CONTROL::CHOOSE);
		while (!quit && event->getEventType() != SDL_KEYDOWN && event->getMouseButton() == 0) {
			windowHandle();
			event->updateEvent();

			clearRenderer();
			background->render();
			button->render();
			title->render();
			character->renderStart();
			button->pressAnyKeyRender();
			presentRenderer();

			quit = event->quit();
		}
		if (!quit) {
			background->backgroundTransition(BACKGROUND::START, BACKGROUND::MENU, *event);
		}
	}
	quit = event->quit();
}

void Menu::renderMenu()
{
	titleType = TITLE::MENU;
	background->setType(BACKGROUND::MENU);
	button->createRect(titleType);
	title->setType(titleType);
	character->createRect(titleType);

	while (!quit) {

		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		button->render();
		title->render();		
		character->renderIntro();
		presentRenderer();

		choose = static_cast<BUTTON>(button->eventHandle(*event));
		changeScreen();

		quit = quit || event->quit();
	}
}



//ChooseMusic

ChooseMusic::ChooseMusic(Screen* screen)
{
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
		character->createRect(titleType);
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
			character->renderChooseMusic();
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
			background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::GAME, *event);
			Game game(this, musicList[musicChoose]);
			game.run();
			quit = true;
			break;
		}
		case BUTTON::EXIT: {
			quit = true;
			background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::MENU, *event);
			break;
		}
		default: {
			quit = event->quit() || event->pause();
			if (event->pause()) {
				background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::MENU, *event);
			}
		}
	}
}

void ChooseMusic::free()
{
	musicList[musicChoose].freeLoad();
	musicList.erase(musicList.begin(), musicList.end());
	music->free();

	delete music;
	music = nullptr;

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
	titleType = TITLE::GAME;
}

Game::Game(Screen* screen, Music& _song)
{
	setRenderer(*screen);
	pass(screen);
	arrow = new ArrowTexture(*screen);
	point = new Point(*screen);
	song = &_song;
	titleType = TITLE::GAME;
}

bool Game::load(const string& path)
{
	bool success = true;

	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	success = success && arrow->load(path);
	success = success && character->load(path);
	success = success && point->load(path);
	success = success && song->loadFromFile(path);
	if (success) {
		background->setType(BACKGROUND::GAME);
		button->createRect(titleType);
		title->setType(titleType);
		character->createRect(titleType);
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
		if (!quit) {
			renderScoreScreen();
		}
	}
	if (!event->quit()) {
		background->backgroundTransition(BACKGROUND::GAME, BACKGROUND::MENU, *event);
	}
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
		background->backgroundTransition(BACKGROUND::GAME, BACKGROUND::SETTING, *event);
		Setting setting(this);
		setting.run();
		if (!event->quit()) {
			background->backgroundTransition(BACKGROUND::SETTING, BACKGROUND::GAME, *event);
		}
		button->createRect(titleType);
		title->setType(titleType);
		character->createRect(titleType);
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
	point->free();
	song->freeLoad();

	delete arrow;
	delete point;
	arrow = nullptr;
	point = nullptr;

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
	character->renderDoge(*event);
	character->renderBank(*point);
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
	title->setType(TITLE::FINISH);
	button->createRect(TITLE::FINISH);

	while (!quit) {
		windowHandle();
		event->updateEvent();

		clearRenderer();
		background->render();
		button->render();
		title->render();
		point->renderPointScreen();
		character->renderDoge(*event);
		presentRenderer();

		choose = static_cast<BUTTON>(button->eventHandle(*event));
		quit = event->quit() || event->pause() || (choose == BUTTON::EXIT);
	}

}



//Setting

Setting::Setting(Screen* screen) {
	pass(screen);
	setting = new SettingTexture(*screen, *event);
	settingChoose = CONTROL::NOT_CHOOSE;
	settingChange = false;
	titleType = TITLE::SETTING;
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

	delete setting;
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



//Score

Score::Score(Screen* screen)
{
	pass(screen);
	scoreChoose = 0;
	titleType = TITLE::SCORE;
}

bool Score::load(const string& path)
{
	bool success = true;
	success = success && background->load(path);
	success = success && button->load(path);
	success = success && title->load(path);
	if (success) {
		background->setType(BACKGROUND::SCORE);
		button->createRect(titleType);
		title->setType(titleType);
		character->createRect(titleType);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Score loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load score" << endl;
	}
	cout << endl;

	return success;
}

void Score::run()
{
	string path = "Resource";
	if (load("Resource")) {
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
	free();
}

void Score::changeScreen()
{
	switch (choose) {
		case BUTTON::EXIT: {
			quit = true;
			background->backgroundTransition(BACKGROUND::SCORE, BACKGROUND::MENU, *event);
			break;
		}
		default: {
			quit = event->quit() || event->pause();
			if (event->pause()) {
				background->backgroundTransition(BACKGROUND::SCORE, BACKGROUND::MENU, *event);
			}
		}
	}
}

void Score::free()
{
	cout << "Log [" << SDL_GetTicks() << "]: " << "Score freed successfully" << endl;
	cout << endl;
}
