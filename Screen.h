#ifndef _Screen_h_
#define _Screen_h_
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
#include "Graphic.h"
using namespace std;

//this file contains the game running related functions

class Screen : protected Window {
protected:
	Background* background = nullptr;
	Button* button = nullptr;
	Title* title = nullptr;
	Event* event = nullptr;
	ArrowTexture* arrow = nullptr;
	DogeTexture* doge = nullptr;
	MusicTexture* music = nullptr;
	SettingTexture* setting = nullptr;
	Point* point = nullptr;

	bool quit = false;
	BUTTON choose = BUTTON::NOT_CHOOSE;

public:
	Screen();

	virtual bool load(const string& path) = 0;

	virtual void pass(Screen* screen);

	virtual void run() = 0;

	virtual void changeScreen() = 0;

	virtual void free() = 0;
};

class Menu : public Screen {
public:
	Menu(Window& window);

	Menu(Window& screen, Event& event);

	bool load(const string& path);

	void run();

	void changeScreen();

	void free();
};

class ChooseMusic : public Screen {
	vector <Music> musicList;
	int musicChoose;
public:
	ChooseMusic(Screen* screen);

	bool load(const string& path);

	void run();

	void changeScreen();

	void free();

	void changeMusic();

};

class Game : public Screen {
	Music* song = nullptr;

	bool pause = false;
	Uint32 lastPauseTime = 0,
		time = 0;

public:
	Game(Window& window);

	Game(Screen* menu, Music& song);

	bool load(const string& path);

	void run();

	void changeScreen();

	void free();

	void renderPause();

	void renderScoreScreen();
};

class Setting : public Screen {
public:
	Setting(Screen* screen);

	bool load(const string& path);

	void run();

	void changeScreen();

	void free();

};

//menu function
void menu(Window& screen, Background& background, Event& event, vector<Music>& musicList);

void chooseMusic(Window& screen, Background& background, Button& button, Event& event, vector<Music>& musicList);

//main game function
void game(Window& screen, Background& background, Button& button, Event& event, Music& song);

//loading resources

bool loadingGameResource(Window& screen, ArrowTexture& texture, DogeTexture& dogeTexture, Music& song, Point& point);

void freeGamesTexture(ArrowTexture& texture, DogeTexture& dogeTexture, Music& song, Point& point);

void changeSong(MusicTexture& song, Event& event, vector<Music>& musicList, BUTTON& choose,int& musicChoose);

void changeGameScreen(Uint32& lastPauseTime, BUTTON& choose, bool& quit, bool& pause);

void changeMenuScreen(Window& screen, Background& background, Button& button, Event& event, vector<Music>& musicList, BUTTON& choose,  bool& quit);

void changeMusicScreen(Window& screen, Background& background, Button& button, Event& event, vector<Music>& musicList, BUTTON& choose, int& musicChoose, bool& quit);
#endif