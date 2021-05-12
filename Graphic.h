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

//this file contain all the texture classes related to all the game screen

class Graphic : protected Window {
protected:
	bool textureLoaded = false;
public:

	//default constructor
	Graphic();

	Graphic(Window& screen);

	//load the resources
	virtual bool load(const string& path) = 0;

	//render the texture to screen
	virtual void render() = 0;

	//free the texture when closing the program or not using
	virtual void free() = 0;

	//create the default rectangles to render from and to
	virtual void createDefaultRect() = 0;

	//destructor
	~Graphic();

	SDL_Rect setScreenUnitRect(double x, double y, double w, double h);

	SDL_Rect setTextRect(double x, double y, int wText, int hText);
};

//background texture
class Background : public Graphic {
	//background and the related parameters
	LTexture* background;
	SDL_Rect* bgSourceRect;
	SDL_Rect* bgDestinationRect;
	SDL_Rect bgSourceVelo;
	int bgDstVelo;

	//color strips and the related parameters
	LTexture* colorStrip;
	SDL_Rect* colorStripDstRect;
	double currentColorPlace;
	SDL_Point ColorStripVelo;

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

	//constructor
	Background(Window& screen);

	//basic background functions

	bool load(const string& path);

	void render();

	void free();

	void createDefaultRect();


	//background moving function

	//set background render type as the type of the screen
	void setType(BACKGROUND _type);

	//set background transition velocity
	void setBgVelocity(BACKGROUND origin, BACKGROUND moving);

	//move colorstrip in the height direction of the screen
	void moveColorStrip(SDL_Rect& movingRect);

	//background main transition 
	void backgroundTransition(BACKGROUND origin, BACKGROUND moving, Event& event);

	//check if background has finished moving or not
	void checkBgTransition(SDL_Rect& transition, SDL_Rect& original, SDL_Rect& move);


	//render function

	//render the FPS number
	void renderFps();

	//render colorstrip
	void renderColorStrip(SDL_Rect &rectangle);

};

//button handler texture
class Button : public Graphic {
protected:

	LButton* button;
	SDL_Rect* buttonDstRect;
	
public:
	bool* usingButton;
	int* buttonHover;
	int* totalButton;

	//constructor

	Button();

	Button(Window& screen);

	//basic functions

	bool load(const string& path);

	void render();

	void free();

	void createDefaultRect();

	//event functions

	void pressAnyKeyRender();

	int eventHandle(Event& event);

	int mouseEventHandle(Event& event);

	int keyEventHandle(Event& event);

	//render functions

	void buttonEnable(BUTTON type);

	void buttonDisable(BUTTON type);

	void createRect(TITLE type);

	void createMenuRect();

	void createChooseMusic();

	void createFinishRect();

	void createPauseRect();

	void createSettingRect();

	void createScoreRect();
};

//title class
class Title : public Graphic {
	LTexture* title;
	SDL_Rect* titleDstRect;
	TITLE titleType;
public:

	//constructor

	Title(Window& screen);

	//basic functions

	bool load(const string& path);

	void render();

	void free();

	void createDefaultRect();

	void setType(TITLE type);
};

//music info texture
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

//point texture
class Point : public Graphic {
	LButton* nameTexture;
	LTexture* pointTexture;
	SDL_Rect* nameRect;
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

	bool loadPlayerName();

	bool loadPointWindow(string& path);

	void render();

	void free();

	void createDefaultRect();

	// point count

	bool addPoint(int& accuracy);

	void missedArrowCount();

	void wrongPressCount();

	void nameEdit(Event& event);

	//compare score

	bool compare(Point& point);

	//render functions

	BUTTON eventHandle(Event& event);

	void renderPointScreen();

	void saveScore(const string& path);

	int getCombo();

};

//character texture
class Character : public Graphic {
	LTexture muscleDoge,
		smashingDoge,
		cheems,
		smashedCheems,
		khaBank;
	Mix_Chunk* bonk;

	SDL_Rect* muscleDogeSrcRect;
	SDL_Rect* muscleDogeDstRect;
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

	void createDefaultRect();

	//play sound

	void playBonkSound();

	//render function

	void createRect(TITLE type);

	void createIntroRect();

	void createMenuRect();

	void createMusicRect();

	void createGameRect();

	void renderIntro();

	void renderStart();

	void renderChooseMusic();

	void renderDoge(Event& event);

	void renderBank(Point& point);

};

//game arrow texture
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

//setting handler texture
class SettingTexture : public Button, public Event {
	TTF_Font* font;
	SDL_Color color;
	
public:
	SettingTexture(Window& screen, Event& event, Button& button);

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
