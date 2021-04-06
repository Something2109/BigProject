#ifndef _Texture_h
#define _Texture_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "ErrorAndEnum.h"
using namespace std;

//this file contains functions to use textures more easily

//coordinate used by the arrowRange
class Coordinate {
	int x;
	int y;

public:

	//Create coordinate
	Coordinate(int _x, int _y);

	//create arrow rectangle to render
	SDL_Rect getRect(const int& width, const int& height);

	//arrow moving 
	void arrowMove(const int& velocity);

	//get arrow position 
	int getArrowCol(const int& screenUnit);

	//get the height coordinate of the arrow
	int getY();
};

//texture class handler
class LTexture {

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

//renderer
class Screen {
protected:

	//renderer and the related parameters
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	const char* windowTitle = "Game";
	int screenUnit = 0,
		screenWidth = 0,
		screenHeight = 0;

public:

	//set the renderer parameters
	void setScreenUnit(int number);

	//create window
	bool createWindow();

	//create renderer
	bool createRenderer();

	void clearRenderer();

	void presentRenderer();

	//free the screen
	void freeScreen();

	//get renderer pointer
	SDL_Renderer* getRenderer();

	//get the renderer parameters
	const char* getWindowTitle();

	int getScreenUnit();

	int getScreenWidth();

	int getScreenHeight();

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

	string getMusicName();

	Uint32 getDuration();

	int getSpawnRate();

	int getVelocity();
};

class Event {
	SDL_Event e;
	SDL_Point mousePos;
	Uint32 mouseButton = 0;
	const Uint8* keyState = NULL;
	Uint8* keyControl = NULL;

public:
	void updateEvent();

	bool quit();

	SDL_Event getEvent();

	const Uint8* getKeyState();
	
	SDL_Point getMousePos();

	Uint32 getMouseButton();
};



#endif