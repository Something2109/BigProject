#ifndef _Graphic_h
#define _Graphic_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "ErrorAndEnum.h"
#include "Utility.h"
using namespace std;

//this file contain all the texture struct related to all the game frame type

class Graphic : protected Window {
protected:
	bool textureLoaded = false;
public:
	Graphic();

	Graphic(Window& screen);

	virtual bool load(const string& path) = 0;

	virtual void render() = 0;

	virtual void free() = 0;

	virtual void createDefaultRect() = 0;

	~Graphic();

	SDL_Rect setScreenUnitRect(double x, double y, double w, double h);

	SDL_Rect setTextRect(double x, double y, int wText, int hText);
};

//menu texture struct

class Background : public Graphic {
	//background and the related parameters
	LTexture* background;
	SDL_Rect* bgSourceRect;
	SDL_Rect* bgDestinationRect;

	//color strips and the related parameters
	LTexture* colorStrip;
	SDL_Rect* colorStripDstRect;
	double currentColorPlace;
	int yColorStripVelo;

	LTexture* fpsTexture;
	TTF_Font* fpsFont = nullptr;
	SDL_Color fpsColor;
	Uint32 currentTime,
		lastFrameTime;
	double fps = 0,
		frameCount = 0;
	unsigned int fpsTimer = 0;

	BACKGROUND backgroundType;

public:

	Background(Window& screen);

	//basic background functions

	bool load(const string& path);

	void createDefaultRect();

	void render();

	void free();

	void setType(BACKGROUND _type);
	
	//background moving function

	void moveColorStrip(SDL_Rect& movingRect);

	void backgroundTransition(BACKGROUND origin, BACKGROUND moving);

	void checkBgTransition(SDL_Rect& transition, SDL_Rect& original, SDL_Rect& move);

	//color strip render function
	void renderFps();

	void renderColorStrip(SDL_Rect &rectangle);

};

class Button : public Graphic {
protected:

	LButton* button;
	SDL_Rect* buttonDstRect;
	bool* usingButton;
	int buttonChoose,
		totalButton;

public:

	//constructor

	Button();

	Button(Window& screen);

	//basic functions

	bool load(const string& path);

	void render();

	void free();

	//event functions

	void pressAnyKeyRender();

	int eventHandle(Event& event);

	int mouseEventHandle(Event& event);

	int keyEventHandle(Event& event);

	//render functions

	void buttonEnable(BUTTON type);

	void buttonDisable(BUTTON type);

	void createDefaultRect();

	void createRect(TITLE type);

	void createMenuRect();

	void createChooseMusic();

	void createScoreRect();

	void createPauseRect();

	void createSettingRect();


};

class Title : public Graphic {
	LTexture* title;
	SDL_Rect* titleDstRect;
	TITLE titleType;
public:
	Title(Window& screen);

	bool load(const string& path);

	void render();

	void free();

	void createDefaultRect();

	void setType(TITLE type);
};

class MusicTexture : public Graphic, private Music {
	LTexture nameText,
		authorText,
		difficultyText,
		bpmText;

	SDL_Rect nameRect,
		authorRect,
		difficultyRect,
		bpmRect;
	TTF_Font* font;
	SDL_Color color;
public:
	//constructor
	MusicTexture(Window& screen);

	//basic functions
	bool load(const string& path);

	void render();

	void free();

	void createDefaultRect();

	//load music
	bool loadSong(Music& song);
};

class Point : public Graphic {
	LTexture* pointTexture;
	SDL_Rect* pointRect;

	string playerName;
	unsigned int point = 0,
		totalArrow = 0,
		accuracy = 0,
		pressedArrow = 0,
		wrongPressed = 0,
		combo = 0,
		maxCombo = 0;

	TTF_Font* font;
	SDL_Color color;

public:

	//constructor

	Point(Window& screen);

	//basic function

	bool load(const string& path);

	bool loadPointWindow(string& path);

	void render();

	void free();

	void createDefaultRect();

	// point count

	bool addPoint(int& accuracy);

	void missedArrowCount();

	void wrongPressCount();

	//compare score

	bool compare(Point& point);

	//render functions
	void renderPointScreen();

	int getCombo();

};

class ArrowTexture : public Graphic {
	LTexture arrow,
		blankArrow,
		pressedArrow;

	vector <SDL_Point> arrowRange;
	Uint32 gameTime = 0,
		nextSpawnTime = 0,
		pauseTime = 0,
		spawnDuration = 0,
		spawnRate = 0;
	double velocity = 0;

	SDL_Rect* blankArrowDstRect;
	SDL_Rect* arrowSrcRect;
	SDL_Rect* pressedArrowDstRect;

public:

	//constructor

	ArrowTexture(Window& screen);

	//basic functions

	bool load(const string& path);

	void render();

	void free();

	//Arrow spawning function

	void setTimeline(Music& song);

	void addArrow(Uint32& time);

	void addPauseTime(Uint32 time);

	void scoreCheck(int& keyType, Point& point);

	void updateArrowRange(Point& point, Uint32 timePassed);

	//render function

	void createDefaultRect();

	void renderPressedArrow(Event& event, Point& point);

};

class Character : public Graphic {
	LTexture muscleDoge,
		cheems,
		smashedCheems,
		khaBank;
	Mix_Chunk* bonk;

	SDL_Rect* muscleDogeRect;
	SDL_Rect* cheemsSrcRect;
	SDL_Rect* cheemsDstRect;
	SDL_Rect* khaBankSrcRect;
	SDL_Rect* khaBankDstRect;

	int frame,
		bankVelo;

	bool bankRender;

public:

	//constructor

	Character(Window& screen);

	//basic functions

	bool load(const string& path);

	void render();

	void free();

	//play sound

	void playBonkSound();

	//render function

	void createDefaultRect();

	void renderDoge(Event& event);

	void renderBank(Point& point);

};

class SettingTexture : public Button, public Event {
	TTF_Font* font;
	SDL_Color color;
	

public:
	SettingTexture(Window& screen, Event& event);

	//basic functions
	bool load(const string& path);

	void render();

	void free();

	//rendering functions

	void createDefaultRect();

	void hovered(CONTROL key);

	bool changeKey(CONTROL changingKey, Event& event);

};

#endif
