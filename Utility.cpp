#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Utility.h"
using namespace std;

//error loading function
void loadErrorLog()
{
	printf("\nSDL_Error: %s\n", SDL_GetError());
	printf("SDL_Error: %s\n", IMG_GetError());
	printf("SDL_Error: %s\n", Mix_GetError());
	printf("SDL_Error: %s\n\n", TTF_GetError());
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

//LButton

void LButton::hovered() {
	hover = true;
}

void LButton::clicked() {
	click = true;
}

void LButton::render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect)
{
	if (sourceRect == NULL) {
		SDL_Rect render = { 0, 0, mWidth, mHeight };
		sourceRect = &render;
	}
	if (renderRect == NULL) {
		SDL_Rect render = { 0, 0, mWidth, mHeight };
		renderRect = &render;
	}
	if (click) {
		SDL_Rect clickedRect = { renderRect->x + renderRect->w * 10 / 100, renderRect->y + renderRect->h * 10 / 100,
			renderRect->w - renderRect->w * 10 / 100, renderRect->h - renderRect->h * 10 / 100 };
		renderRect = &clickedRect;
	}
	if (hover) {
		setColor(255, 191, 255);
	}
	else {
		setColor(255, 255, 255);
	}

	if (mTexture != NULL) {
		SDL_RenderCopy(renderer, mTexture, sourceRect, renderRect);
	}
	hover = false;
	click = false;
}

//Screen

//pass the renderer value to the subclass
void Screen::setRenderer(Screen& screen) {
	renderer = screen.renderer;
	screenUnit = screen.screenUnit;
	screenWidth = screen.screenWidth;
	screenHeight = screen.screenHeight;
}

//set the renderer parameters
void Screen::setScreenUnit(int number) {
	screenUnit = number;
	screenWidth = screenUnit * 16;
	screenHeight = screenUnit * 9;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Set screen resolution to " << screenWidth << " x " << screenHeight << endl;
}

void Screen::defaultScreenUnit() {
	int defaultScreenUnit = 80;
	setScreenUnit(defaultScreenUnit);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Couldn't open file, using default resolution 1280 x 720" << endl;
}

//create window
bool Screen::createWindow() {
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	if (window == NULL) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to create window" << endl;
		loadErrorLog();
		return false;
	}
	return true;
}

//create renderer
bool Screen::createRenderer() {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load the renderer" << endl;
		return false;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Renderer loaded successfully in " << renderer << endl;
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
	cout << "Log [" << SDL_GetTicks() << "]: " << "Renderer closed successfully" << endl;
	SDL_DestroyWindow(window);
	window = NULL;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Window closed successfully" << endl;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

//get the renderer parameters

int Screen::getScreenUnit() {
	return screenUnit;
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

string Music::getSongName() {
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
	if (!quitProgram) {
		SDL_Event state;
		SDL_PollEvent(&state);
		e = state;
		keyState = SDL_GetKeyboardState(NULL);
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			mouseButton = SDL_GetMouseState(&mousePos.x, &mousePos.y);
		}
	}
}

void Event::loadDefaultSetting() {
	freeEventControl();

	scanControl = new SDL_Scancode[static_cast<int> (CONTROL::TOTAL)];
	keyControl = new SDL_Keycode[static_cast<int> (CONTROL::TOTAL)];

	scanControl[static_cast<int> (CONTROL::LEFT_ARROW)] = SDL_SCANCODE_LEFT;
	scanControl[static_cast<int> (CONTROL::UP_ARROW)] = SDL_SCANCODE_UP;
	scanControl[static_cast<int> (CONTROL::DOWN_ARROW)] = SDL_SCANCODE_DOWN;
	scanControl[static_cast<int> (CONTROL::RIGHT_ARROW)] = SDL_SCANCODE_RIGHT;
	scanControl[static_cast<int> (CONTROL::CHOOSE)] = SDL_SCANCODE_SPACE;
	scanControl[static_cast<int> (CONTROL::ESCAPE)] = SDL_SCANCODE_ESCAPE;

	keyControl[static_cast<int> (CONTROL::LEFT_ARROW)] = SDL_GetKeyFromScancode(scanControl[static_cast<int> (CONTROL::LEFT_ARROW)]);
	keyControl[static_cast<int> (CONTROL::UP_ARROW)] = SDL_GetKeyFromScancode(scanControl[static_cast<int> (CONTROL::UP_ARROW)]);
	keyControl[static_cast<int> (CONTROL::DOWN_ARROW)] = SDL_GetKeyFromScancode(scanControl[static_cast<int> (CONTROL::DOWN_ARROW)]);
	keyControl[static_cast<int> (CONTROL::RIGHT_ARROW)] = SDL_GetKeyFromScancode(scanControl[static_cast<int> (CONTROL::RIGHT_ARROW)]);
	keyControl[static_cast<int> (CONTROL::CHOOSE)] = SDL_GetKeyFromScancode(scanControl[static_cast<int> (CONTROL::CHOOSE)]);
	keyControl[static_cast<int> (CONTROL::ESCAPE)] = SDL_GetKeyFromScancode(scanControl[static_cast<int> (CONTROL::ESCAPE)]);
 }

bool Event::quit() {
	switch (e.type) {
	case SDL_QUIT: {
		quitProgram = true;
		return true;
		break;
	}
	case SDL_KEYDOWN: {
		if (e.key.keysym.sym == keyControl[static_cast<int> (CONTROL::ESCAPE)]) {
			e.key.keysym.sym = SDLK_UNKNOWN;
			return true;
		}
		else {
			return false;
		}
		break;
	}
	default: {
		return false;
	}
	}
}

void Event::freeEventControl() {
	delete[] scanControl;
	delete[] keyControl;
}

bool Event::checkKeyState(CONTROL key) {
	return keyState[scanControl[toInt(key)]];
}

bool Event::checkEvent(CONTROL key) {
	return (e.key.keysym.sym == keyControl[toInt(key)]);
}

bool Event::checkRepeat() {
	return (e.key.repeat != 0);
}


SDL_Point Event::getMousePos() {
	return mousePos;
}

Uint32 Event::getMouseButton() {
	return mouseButton;
}