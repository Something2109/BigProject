#ifndef _Texture_h
#define _Texture_h
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

//this file contains functions to use textures more easily

//texture class handler
class LTexture {
protected:

	//texture and the related parameters
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

public:

	//create a texture
	LTexture();

	//free the texture
	void free();

	//load resource to texture
	bool loadFromFile(SDL_Renderer* renderer, string path);

	//load text from font
	bool loadFromRenderedText(SDL_Renderer* renderer, string textureText, SDL_Color textColor, TTF_Font* font);

	void setColor(Uint8 red, Uint8 green, Uint8 blue); 

	//render part of a texture (in the type of a rectangle) to the screen with given rectangle
	void render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect);

	//Destructor
	~LTexture();

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
class Screen {

	//window and its title
	SDL_Window* window = NULL;
	const char* windowTitle = "Game";

protected:

	//renderer and the related parameters
	SDL_Renderer* renderer = NULL;
	int screenUnit = 0,
		screenWidth = 0,
		screenHeight = 0;

public:

	//pass the renderer value to the subclass
	void setRenderer(Screen& screen);

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
	void freeScreen();

	//get the renderer parameters

	int getScreenUnit();


};

class Music {

	Mix_Music* source = NULL;
	string name;
	string singer;
	Uint32 duration = 0;
	int spawnRate = 0;
	int velocity = 0;

public:

	//load music from file
	bool loadFromFile(string path);

	//load music infomation
	void loadMusicInfo(string& _name, string& _singer, Uint32& _duration, int& _spawnRate, int& _velocity, const int& screenUnit);

	void playMusic(int loop);

	void freeMusicLoad();

	void freeMusic();

	~Music();

	string getSongName();

	Uint32 getDuration();

	int getSpawnRate();

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

public:
	void loadEventCode(stringstream& file);

	void loadDefaultSetting();

	void updateEvent();

	bool quit();

	void freeEventControl();

	bool checkKeyState(CONTROL key);

	bool checkEvent(CONTROL key);

	bool checkRepeat();

	SDL_Point getMousePos();

	Uint32 getMouseButton();

};


#endif