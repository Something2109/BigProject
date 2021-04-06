#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
using namespace std;

//error loading function
void loadErrorLog()
{
	printf("\nSDL_Error: %s\n", SDL_GetError());
	printf("SDL_Error: %s\n", IMG_GetError());
	printf("SDL_Error: %s\n", Mix_GetError());
	printf("SDL_Error: %s\n\n", TTF_GetError());
}

//Coordinate

//Create coordinate
Coordinate::Coordinate(int _x, int _y) {
	x = _x;
	y = _y;
}

//create arrow rectangle to render
SDL_Rect Coordinate::getRect(const int& width, const int& height) {
	SDL_Rect rect = { x, y, width, height };
	return rect;
}

//arrow moving 
void Coordinate::arrowMove(const int& velocity) {
	y -= velocity;
}

//get arrow position 
int Coordinate::getArrowCol(const int& screenUnit) {
	int arrowCol = (x / screenUnit - 1) / 2;
	return arrowCol;
}

//get the height coordinate of the arrow
int Coordinate::getY() {
	return y;
}



//LTexture

//create a texture
LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

//free the texture
void LTexture::free() {
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

//load resource to texture
bool LTexture::loadFromFile(SDL_Renderer* renderer, string path) {
	free();
	bool success = true;
	SDL_Surface* loadedImage = IMG_Load(path.c_str());
	if (loadedImage == NULL) {
		loadErrorLog();
		success = false;
	}
	else {
		mTexture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		if (mTexture == NULL) {
			loadErrorLog();
			success = false;
		}
		else {
			mWidth = loadedImage->w;
			mHeight = loadedImage->h;
		}
		SDL_FreeSurface(loadedImage);
	}
	return success;
}

//load text from font
bool LTexture::loadFromRenderedText(SDL_Renderer* renderer, string textureText, SDL_Color textColor, TTF_Font* font) {
	free();
	bool success = true;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		loadErrorLog();
		success = false;
	}
	else {
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL) {
			loadErrorLog();
			success = false;
		}
		else {
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return success;
}

//render part of a texture (in the type of a rectangle) to the screen with given rectangle
void LTexture::render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect) {
	if (sourceRect == NULL) {
		SDL_Rect render = { 0, 0, mWidth, mHeight };
		sourceRect = &render;
	}
	if (renderRect == NULL) {
		SDL_Rect render = { 0, 0, mWidth, mHeight };
		renderRect = &render;
	}
	if (mTexture != NULL) {
		SDL_RenderCopy(renderer, mTexture, sourceRect, renderRect);
	}
}

//Destructor
LTexture::~LTexture() {
	free();
}

//get the width of the loaded picture
int LTexture::getWidth() {
	return mWidth;
}

//get the height of the loaded picture
int LTexture::getHeight() {
	return mHeight;
}




//Screen

//set the renderer parameters
void Screen::setScreenUnit(int number) {
	screenUnit = number;
	screenWidth = screenUnit * 16;
	screenHeight = screenUnit * 9;
}

//create window
bool Screen::createWindow() {
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	if (window == NULL) {
		loadErrorLog();
		return false;
	}
	return true;
}

//create renderer
bool Screen::createRenderer() {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		return false;
	}
	return true;
}

//renderer present and clear functions
void Screen::clearRenderer() {
	SDL_RenderClear(renderer);
}

void Screen::presentRenderer() {
	SDL_RenderPresent(renderer);
}

//free the screen
void Screen::freeScreen() {
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Screen freed successfully" << endl;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

//get renderer pointer
SDL_Renderer* Screen::getRenderer() {
	return renderer;
}

//get the renderer parameters
const char* Screen::getWindowTitle() {
	return windowTitle;
}

int Screen::getScreenUnit() {
	return screenUnit;
}

int Screen::getScreenWidth() {
	return screenWidth;
}

int Screen::getScreenHeight() {
	return screenHeight;
}



//Music

//load music from file
bool Music::loadFromFile(string path) {
	bool success = true;
	string fileName = name + " -" + singer;
	source = Mix_LoadMUS((path + "/Music/" + fileName + ".mp3").c_str());
	if (source == NULL) {
		success = false;
	}
	return success;
}

//load music infomation
void Music::loadMusicInfo(string& _name, string& _singer, Uint32& _duration, int& _spawnRate, int& _velocity, const int& screenUnit) {
	name = _name;
	singer = _singer;
	spawnRate = _spawnRate;
	velocity = screenUnit / _velocity;
	duration = _duration;
}

void Music::playMusic(int loop) {
	Mix_PlayMusic(source, loop);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Music played" << endl;
}

void Music::freeMusicLoad() {
	Mix_FreeMusic(source);
	source = NULL;
}

void Music::freeMusic() {
	Mix_FreeMusic(source);
	source = NULL;
	name.erase();
	singer.erase();
	spawnRate = 0;
	velocity = 0;
}

Music::~Music() {
	freeMusicLoad();
}

string Music::getMusicName() {
	return (name + " -" + singer);
}

Uint32 Music::getDuration() {
	return duration;
}

int Music::getSpawnRate() {
	return spawnRate;
}

int Music::getVelocity() {
	return velocity;
}



//Event

void Event::updateEvent() {
	SDL_Event state;
	SDL_PollEvent(&state);
	e = state;
	keyState = SDL_GetKeyboardState(NULL);
	if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		mouseButton = SDL_GetMouseState( &mousePos.x, &mousePos.y );
	}
}

bool Event::quit() {
	if (e.type == SDL_QUIT) {
		return true;
	}
	return false;
}

SDL_Event Event::getEvent() {
	return e;
}

const Uint8* Event::getKeyState() {
	return keyState;
}

SDL_Point Event::getMousePos() {
	return mousePos;
}

Uint32 Event::getMouseButton() {
	return mouseButton;
}
