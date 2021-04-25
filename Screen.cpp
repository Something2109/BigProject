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
	event = nullptr;
	arrow = nullptr;
	doge = nullptr;
	music = nullptr;
	setting = nullptr;
	point = nullptr;

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



//Menu

Menu::Menu(Window& screen) {
	setRenderer(screen);
	background = new Background(screen);
	button = new Button(screen);
	event = new Event();
	title = new Title(screen);
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
		background->setType(BACKGROUND::EXIT);
		title->setType(TITLE::MENU);
		event->loadDefaultSetting();
		button->createMenuRect();
		title->createDefaultRect();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Menu loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load menu" << endl;
	}
	return success;
}

void Menu::run()
{
	string path = "Resource";
	if (load(path)) {
		while (!quit && !event->checkKeyEvent() && event->getMouseButton() == 0) {
			clearRenderer();
			event->updateEvent();
			background->render();
			quit = event->quit();
			presentRenderer();
		}
		if (!quit) {
			background->backgroundTransition(BACKGROUND::EXIT, BACKGROUND::MENU);
		}
		while (!quit) {
			clearRenderer();

			event->updateEvent();
			choose = button->eventHandle(*event);

			background->render();
			title->render();
			button->render();
			presentRenderer();

			changeScreen();

			quit = quit || event->quit();
		}
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
			button->createMenuRect();
			if (!event->quit()) {
				background->backgroundTransition(BACKGROUND::SETTING, BACKGROUND::MENU);
			}
			break;
		}
		case BUTTON::EXIT: {
			quit = true;
			break;
		}
	}
	title->setType(TITLE::MENU);
}

void Menu::free()
{
	background->free();
	button->free();
	event->freeEventControl();
	title->free();
}



//ChooseMusic

ChooseMusic::ChooseMusic(Screen* screen)
{
	setRenderer(*screen);
	pass(screen);
	music = new MusicTexture(*screen);
	musicChoose = 0;
}

bool ChooseMusic::load(const string& path)
{
	bool success = true;
	success = success && background->load(path);
	success = success && button->load(path);
	success = success && loadMusicFile(musicList, path, *screenUnit);
	success = success && music->load(path);
	success = success && music->loadSong(musicList[musicChoose]);
	success = success && musicList[musicChoose].loadFromFile(path);
	if (success) {
		event->loadDefaultSetting();
		button->createChooseMusic();
		musicList[musicChoose].playMusic(-1);
	}
	return success;
}

void ChooseMusic::run()
{
	string path = "Resource";
	if (load(path)) {
		while (!quit) {
			event->updateEvent();

			choose = button->eventHandle(*event);
			clearRenderer();
			background->render();
			music->render();
			button->render();
			presentRenderer();
			changeMusic();
			changeScreen();
		}
	}
	Mix_HaltMusic();
	button->createMenuRect();
}

void ChooseMusic::changeScreen()
{
	switch (choose) {
		case BUTTON::PLAY: {
			Mix_HaltMusic();
			background->backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::GAME);
			Game game(this, musicList[musicChoose]);
			game.run();
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
	musicList.erase(musicList.begin(), musicList.end());
	music->free();
	musicList[musicChoose].freeLoad();
}

void ChooseMusic::changeMusic()
{
	string path = "Resource";
	if ((event->checkKeyEventDown(CONTROL::LEFT_ARROW) && !event->checkKeyRepeat()) || (choose == BUTTON::CHANGE_SONG_LEFT)) {
		Mix_HaltMusic();
		musicList[musicChoose].freeLoad();
		musicChoose = (musicChoose - 1) % musicList.size();
		musicList[musicChoose].loadFromFile(path);
		music->loadSong(musicList[musicChoose]);
		musicList[musicChoose].playMusic(-1);
	}
	if ((event->checkKeyEventDown(CONTROL::RIGHT_ARROW) && !event->checkKeyRepeat()) || (choose == BUTTON::CHANGE_SONG_RIGHT)) {
		Mix_HaltMusic();
		musicList[musicChoose].freeLoad();
		musicChoose = (musicChoose - 1) % musicList.size();
		musicList[musicChoose].loadFromFile(path);
		music->loadSong(musicList[musicChoose]);
		musicList[musicChoose].playMusic(-1);
	}
}



//Game

Game::Game(Window& window)
{
	setRenderer(window);
	background = new Background(window);
	button = new Button(window);
	event = new Event();
}

Game::Game(Screen* screen, Music& _song)
{
	setRenderer(*screen);
	pass(screen);
	arrow = new ArrowTexture(*screen);
	doge = new DogeTexture(*screen);
	point = new Point(*screen);
	song = &_song;
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
		title->setType(TITLE::PAUSE);
		button->createPauseRect();
		title->createDefaultRect();
		arrow->createDefaultRect();
		doge->createDefaultRect();
	}
	return success;
}

void Game::run()
{
	string path = "Resource";
	if (load(path)) {
		arrow->setTimeline(*song);
		while (!quit) {
			time = SDL_GetTicks();

			if (!pause) {
				//event handle
				event->updateEvent();

				clearRenderer();
				background->render();
				//render texture
				arrow->updateArrowRange(*point);
				point->render();
				arrow->renderPressedArrow(*event, *point);
				arrow->render();
				doge->renderDoge(*event);
				presentRenderer();

				//arrow spawn mechanism
				arrow->addArrow(time);

				pause = event->pause();
				quit = !Mix_PlayingMusic() || event->quit();
			}
			else {
				renderPause();
			}

			lastPauseTime = time;
		}
		Mix_HaltMusic();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Game ending time: " << SDL_GetTicks() << endl;

		quit = (quit && pause) || !point->loadPointWindow(path) || event->quit();
		title->setType(TITLE::FINISH);
		if (!quit) {
			renderScoreScreen();
		}
	}
	button->createMenuRect();
	free();
	background->backgroundTransition(BACKGROUND::GAME, BACKGROUND::MENU);
}

void Game::changeScreen()
{
	switch (choose) {
	case BUTTON::CONTINUE: {
		Mix_ResumeMusic();
		pause = false;
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
}

void Game::renderPause()
{
	//event handle
	event->updateEvent();

	clearRenderer();
	background->render();
	title->render();
	button->render();
	choose = button->eventHandle(*event);
	presentRenderer();

	arrow->addPauseTime(time - lastPauseTime);
	changeScreen();
	quit = quit || event->quit();
}

void Game::renderScoreScreen()
{
	button->createScoreRect();

	while (!quit) {
		event->updateEvent();

		choose = button->eventHandle(*event);
		clearRenderer();
		background->render();
		title->render();
		button->render();
		point->renderPointScreen();
		doge->renderDoge(*event);
		presentRenderer();

		quit = event->quit() || event->pause() || (choose == BUTTON::EXIT);
	}

}



//Setting

Setting::Setting(Screen* screen) {
	setRenderer(*screen);
	pass(screen);
}

bool Setting::load(const string& path)
{
	button->createSettingRect();
	title->setType(TITLE::SETTING);
	return true;
}

void Setting::run()
{
	string path = "Resource";
	if (load(path)) {
		while (!quit) {
			event->updateEvent();
			choose = button->eventHandle(*event);

			clearRenderer();
			background->render();
			title->render();
			button->render();
			presentRenderer();

			quit = event->quit() || event->pause();

			changeScreen();
		}
	}
	free();
}

void Setting::changeScreen()
{
	switch (choose) {
		case BUTTON::SAVE: {
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
}















//void menu(Window& screen, Background& background, Event& event, vector<Music>& musicList) {
//	Button button(screen);
//	bool quit = false;
//	BUTTON choose = BUTTON::NOT_CHOOSE;
//
//	if (button.load("Resource")) {
//		background.setType(BACKGROUND::MENU);
//		button.createMenuRect();
//		while (!quit) {
//			event.updateEvent();
//
//			//cout << "Menu loop" << endl;
//			screen.clearRenderer();
//			choose = button.eventHandle(event);
//
//			background.render();
//			button.render();
//			changeMenuScreen(screen, background, button, event, musicList, choose, quit);
//			screen.presentRenderer();
//
//		}
//	}
//}
//
//void chooseMusic(Window& screen, Background& background, Button& button, Event& event, vector<Music>& musicList)
//{
//	MusicTexture song(screen);
//	bool quit = false;
//	BUTTON choose = BUTTON::NOT_CHOOSE;
//	int musicChoose = 0;
//	string path = "Resource";
//
//	song.load(path);
//	song.loadSong(musicList[musicChoose]);
//	background.setType(BACKGROUND::CHOOSE_MUSIC);
//	button.createChooseMusic();
//	musicList[musicChoose].loadFromFile(path);
//	musicList[musicChoose].playMusic(-1);
//	while (!quit) {
//		event.updateEvent();
//
//		choose = button.eventHandle(event);
//		screen.clearRenderer();
//		background.render();
//		song.render();
//		button.render();
//		screen.presentRenderer();
//		changeSong(song, event, musicList, choose, musicChoose);
//		changeMusicScreen(screen, background, button, event, musicList, choose, musicChoose, quit);
//	}
//	Mix_HaltMusic();
//	song.free();
//	musicList[musicChoose].freeLoad();
//	background.setType(BACKGROUND::MENU);
//	button.createMenuRect();
//}
//
////main game function
//void game(Window& screen, Background& background, Button& button, Event& event, Music& song)
//{
//	//game texture
//	ArrowTexture arrowTexture(screen);
//	DogeTexture dogeTexture(screen);
//
//	Point point(screen);
//
//	//quit game
//	BUTTON choose = BUTTON::NOT_CHOOSE;
//	bool quit = false, 
//		pause = false;
//
//	Uint32 lastPauseTime = 0,
//		time = 0;
//
//	//load game resources
//	if (!loadingGameResource(screen, arrowTexture, dogeTexture, song, point)) {
//		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to loading game" << endl;
//	}
//	else {
//		button.createPauseRect();
//		background.setType(BACKGROUND::GAME);
//		//pregame load
//		arrowTexture.setTimeline(song);
//
//		//game loop
//		while (!quit) {
//			time = SDL_GetTicks();
//
//			//event handle
//			event.updateEvent();
//
//			screen.clearRenderer();
//			background.render();
//
//			if (!pause) {
//				//render texture
//				arrowTexture.updateArrowRange(point);
//				point.render();
//				arrowTexture.renderPressedArrow(event, point);
//				arrowTexture.render();
//				dogeTexture.renderDoge(event);
//				screen.presentRenderer();
//
//				//arrow spawn mechanism
//				arrowTexture.addArrow(time);
//
//				pause = event.pause();
//				quit = !Mix_PlayingMusic() || event.quit();
//			}
//
//			else {
//				arrowTexture.addPauseTime(time - lastPauseTime);
//				button.render();
//				choose = button.eventHandle(event);
//				screen.presentRenderer();
//				changeGameScreen(lastPauseTime, choose, quit, pause);
//
//				quit = quit || event.quit();
//			}
//			lastPauseTime = time;
//		}
//		Mix_PauseMusic();
//		cout << "Log [" << SDL_GetTicks() << "]: " << "Game ending time: " << SDL_GetTicks() << endl;
//
//		string path = "Resource";
//		quit = (quit && pause) || !point.loadPointWindow(path) || event.quit();
//		button.createScoreRect();
//
//		while (!quit) {
//			event.updateEvent();
//
//			choose = button.eventHandle(event);
//			screen.clearRenderer();
//			background.render();
//			button.render();
//			point.renderPointScreen();
//			dogeTexture.renderDoge(event);
//			screen.presentRenderer();
//
//			quit = event.quit() || event.pause() || (choose == BUTTON::EXIT);
//		}
//		background.setType(BACKGROUND::MENU);
//		button.createMenuRect();
//	}
//
//	//free game resources
//	freeGamesTexture(arrowTexture, dogeTexture, song, point);
//}
//
//void setting(Window& screen, Background& background, Button& button, Event& event, Music& song) {
//	bool quit = false;
//	background.setType(BACKGROUND::SETTING);
//	while (!quit) {
//		event.updateEvent();
//
//		screen.clearRenderer();
//		background.render();
//		screen.presentRenderer();
//
//		quit = event.quit() || event.pause();
//
//	}
//	background.setType(BACKGROUND::MENU);
//
//}
//
////loading resources
//bool loadingGameResource(Window& screen, ArrowTexture& arrowTexture, DogeTexture &dogeTexture, Music& song, Point& point) {
//	string path = "Resource";
//	bool success = true;
//
//	//load the game texture
//	success = success && arrowTexture.load(path);
//	success = success && dogeTexture.load(path);
//	success = success && song.loadFromFile(path);
//	success = success && point.load(path);
//
//	return success;
//}
//
////free all the game textures
//void freeGamesTexture(ArrowTexture& arrowTexture, DogeTexture& dogeTexture, Music& song, Point& point) {
//	arrowTexture.free();
//	dogeTexture.free();
//	song.freeLoad();
//	point.free();
//}
//
////event handler
//void changeSong(MusicTexture& song, Event& event, vector<Music>& musicList, BUTTON& choose, int& musicChoose) {
//	string path = "Resource";
//	if ((event.checkKeyEventDown(CONTROL::LEFT_ARROW) && !event.checkKeyRepeat()) || (choose == BUTTON::CHANGE_SONG_LEFT)) {
//		Mix_HaltMusic();
//		musicList[musicChoose].freeLoad();
//		musicChoose = (musicChoose - 1) % musicList.size();
//		musicList[musicChoose].loadFromFile(path);
//		song.loadSong(musicList[musicChoose]);
//		musicList[musicChoose].playMusic(-1);
//	}
//	if ((event.checkKeyEventDown(CONTROL::RIGHT_ARROW) && !event.checkKeyRepeat()) || (choose == BUTTON::CHANGE_SONG_RIGHT)) {
//		Mix_HaltMusic();
//		musicList[musicChoose].freeLoad();
//		musicChoose = (musicChoose - 1) % musicList.size();
//		musicList[musicChoose].loadFromFile(path);
//		song.loadSong(musicList[musicChoose]);
//		musicList[musicChoose].playMusic(-1);
//	}
//}
//
//void changeGameScreen(Uint32& lastPauseTime, BUTTON& choose, bool& quit, bool& pause) {
//	switch (choose) {
//	case BUTTON::CONTINUE: {
//		Mix_ResumeMusic();
//		lastPauseTime = 0;
//		pause = false;
//		break;
//	}
//	case BUTTON::EXIT: {
//		quit = true;
//		break;
//	}
//	}
//}
//
//void changeMenuScreen(Window& screen, Background& background, Button& button, Event& event, vector<Music>& musicList, BUTTON& choose, bool& quit) {
//	switch (choose) {
//		case BUTTON::PLAY: {
//			background.backgroundTransition(BACKGROUND::MENU, BACKGROUND::CHOOSE_MUSIC);
//			chooseMusic(screen, background, button, event, musicList);
//			break;
//		}
//		case BUTTON::SETTING: {
//			background.backgroundTransition(BACKGROUND::MENU, BACKGROUND::SETTING);
//			setting(screen, background, button, event, musicList[0]);
//			if (!event.quit()) {
//				background.backgroundTransition(BACKGROUND::SETTING, BACKGROUND::MENU);
//			}
//			break;
//		}
//		case BUTTON::EXIT: {
//			background.backgroundTransition(BACKGROUND::MENU, BACKGROUND::EXIT);
//			quit = true;
//			break;
//		}
//		default: {
//			quit = event.quit();
//		}
//	}
//}
//
//void changeMusicScreen (Window& screen, Background& background, Button& button, Event& event, vector<Music>& musicList, BUTTON& choose, int& musicChoose, bool& quit) {
//	switch (choose) {
//		case BUTTON::PLAY: {
//			Mix_HaltMusic();
//			background.backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::GAME);
//			game(screen, background, button, event, musicList[musicChoose]);
//		}
//		case BUTTON::EXIT: {
//			quit = true;
//			background.backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::MENU);
//			break;
//		}
//		default: {
//			quit = event.quit() || event.pause();
//			if (event.pause()) {
//				background.backgroundTransition(BACKGROUND::CHOOSE_MUSIC, BACKGROUND::MENU);
//			}
//		}
//	}
//}

