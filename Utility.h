#ifndef _Utility_h
#define _Utility_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "ErrorAndEnum.h"
using namespace std;

//this file contains basic classes

//error loading function
void loadErrorLog();


//texture class handler
class LTexture {
protected:

	//texture and the related parameters
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

public:

	//constructor
	LTexture();

	//basic functions

	//load resource to texture
	bool loadFromFile(SDL_Renderer* renderer, string path);

	//load text from font
	bool loadFromRenderedText(SDL_Renderer* renderer, string textureText, SDL_Color textColor, TTF_Font* font);

	//render part of a texture (in the type of a rectangle) to the screen with given rectangle
	void render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect);

	//free the texture
	void free();

	//Destructor
	~LTexture();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//get the width of the loaded picture
	int getWidth();

	//get the height of the loaded picture
	int getHeight();
};

class LButton : public LTexture {
	bool hover = false;
	bool click = false;

public:

	void hovered();

	void clicked();

	void render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect);
};

//renderer
class Window {

	//window and its title
	SDL_Window* window = NULL;
	const char* windowTitle = "Game";

protected:

	//renderer and the related parameters
	SDL_Renderer* renderer = nullptr;
	int* screenUnit = nullptr;
	int* screenWidth = nullptr;
	int* screenHeight = nullptr;

public:

	//pass the renderer value to the subclass
	void setRenderer(Window& screen);

	//set the renderer parameters
	void setScreenUnit(int number);

	void defaultScreenUnit();

	//create window
	bool createWindow();

	//create renderer
	bool createRenderer();

	void clearRenderer();

	void presentRenderer();

	//free the screen
	void free();

	//get the renderer parameters

	int getScreenUnit();

};

class Music {
	Mix_Music* source = NULL;
	bool musicLoaded = false;

protected:
	string name;
	string singer;
	Uint32 start = 0;
	Uint32 duration = 0;
	int bpm = 0;
	int velocity = 0;

public:
	Music();

	Music (string& _name, string& _singer, Uint32& _start, Uint32& _duration, int& _bpm, int& _velocity, const int& screenUnit);

	//load music from file
	bool loadFromFile(string path);

	//load music infomation

	void playMusic(int loop);

	void freeLoad();

	void free();

	~Music();

	void getInfo(Music& song);

	Uint32 getStart();

	Uint32 getDuration();

	int getBpm();

	int getVelocity();
};

class Event {
	SDL_Event e;
	SDL_Point mousePos = { 0, 0 };
	Uint32 mouseButton = 0;
	const Uint8* keyState = NULL;
	SDL_Scancode* scanControl = NULL;
	SDL_Keycode* keyControl = NULL;
	bool quitProgram = false;
	bool mouseRepeat = false;

public:
	void loadEventCode(stringstream& file);

	void loadDefaultSetting();

	void updateEvent();

	void freeEventControl();

	bool quit();

	bool pause();

	bool changeKey(CONTROL changingKey) {
		updateEvent();
		if (e.type == SDL_KEYDOWN) {
			SDL_Keycode keyChange = e.key.keysym.sym;
			for (int type = 0; type < toInt(CONTROL::TOTAL); type++) {
				if (keyChange == keyControl[type]) {
					return false;
				}
			}
			keyControl[toInt(changingKey)] = keyChange;
			scanControl[toInt(changingKey)] = SDL_GetScancodeFromKey(keyChange);
			return true;
		}
		return false;
	}

	//event check
	bool checkKeyEvent();

	bool checkKeyState(CONTROL key);

	bool checkKeyEventDown(CONTROL key);

	bool checkKeyEventUp(CONTROL key);

	bool checkKeyRepeat();

	bool checkMouseEvent();

	bool checkMouseRepeat();

	void checkInputText(string& inputText);

	//get mouse state
	SDL_Point getMousePos();

	Uint32 getMouseButton();

	Uint32 getEventType();

};


#endif