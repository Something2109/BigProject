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
	Background* background;
	Button* button;
	Title* title;
	Event* event;
	ArrowTexture* arrow;
	Character* doge;
	MusicTexture* music;
	SettingTexture* setting;
	Point* point;

	TITLE titleType;
	bool quit;
	BUTTON choose;

public:
	Screen();

	Screen(Window& window);

	virtual void pass(Screen* screen);

	virtual void createRect();

	virtual void windowHandle();

	virtual bool load(const string& path) = 0;

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

	void renderIntro();

	void renderStartScreen();

	void renderMenu();
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

	void renderGame();

	void renderPause();

	void renderScoreScreen();
};

class Setting : public Screen {
	CONTROL settingChoose;
	bool settingChange;

public:
	Setting(Screen* screen);

	bool load(const string& path);

	void run();

	void changeScreen();

	void free();

	void settingHandler();

};

#endif