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

//conctructor
LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

//basic functions

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

//free the texture
void LTexture::free() {
	if (this != nullptr && mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

//Destructor
LTexture::~LTexture() {
	free();
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
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



//Music

//constructor
Music::Music()
{
	start = 0;
	duration = 0;
	bpm = 0;
	velocity = 0;
}

Music::Music(string& _name, string& _singer, Uint32& _start, Uint32& _duration, int& _bpm, double& _velocity)
{
	name = _name;
	singer = _singer;
	start = _start;
	duration = _duration;
	bpm = _bpm;
	velocity = _velocity;
}

//load music from file
bool Music::loadFromFile(string path) {
	
	if (!musicLoaded) {
		string fileName = name + " -" + singer;
		source = Mix_LoadMUS((path + "/Music/" + fileName + ".mp3").c_str());

		musicLoaded = (source != NULL);
		if (musicLoaded) {
			cout << "Log [" << SDL_GetTicks() << "]: " << fileName << " loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load " << fileName << endl;
		}
	}
	return musicLoaded;
}

void Music::playMusic(int loop) {
	Mix_PlayMusic(source, loop);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Music played" << endl;
}

void Music::freeLoad() {
	if (source != nullptr) {
		Mix_FreeMusic(source);
		source = NULL;
		musicLoaded = false;
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music load freed successfully" << endl;
	}
}

void Music::free() {
	Mix_FreeMusic(source);
	source = NULL;
	name.erase();
	singer.erase();
	bpm = 0;
	velocity = 0;
}

//destructor
Music::~Music() {
	freeLoad();
}

void Music::getInfo(Music& song)
{
	name = song.name;
	singer = song.singer;
	start = song.start;
	duration = song.duration;
	bpm = song.bpm;
	velocity = song.velocity;
}

Uint32 Music::getStart()
{
	return start;
}

Uint32 Music::getDuration() {
	return duration;
}

int Music::getBpm() {
	return bpm;
}

double Music::getVelocity() {
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
			Uint32 mouseTempButton = SDL_GetMouseState(&mousePos.x, &mousePos.y);
			mouseRepeat = (mouseTempButton == mouseButton);
			mouseButton = mouseTempButton;
		}
		else {
			mouseRepeat = true;
		}
	}
}

void Event::loadDefaultSetting() {
	if (scanControl == nullptr || keyControl == nullptr) {
		freeEventControl();

		scanControl = new SDL_Scancode[toInt(CONTROL::TOTAL)];
		keyControl = new SDL_Keycode[toInt(CONTROL::TOTAL)];

		scanControl[toInt(CONTROL::LEFT_ARROW)] = SDL_SCANCODE_LEFT;
		scanControl[toInt(CONTROL::UP_ARROW)] = SDL_SCANCODE_UP;
		scanControl[toInt(CONTROL::DOWN_ARROW)] = SDL_SCANCODE_DOWN;
		scanControl[toInt(CONTROL::RIGHT_ARROW)] = SDL_SCANCODE_RIGHT;
		scanControl[toInt(CONTROL::CHOOSE)] = SDL_SCANCODE_SPACE;
		scanControl[toInt(CONTROL::ESCAPE)] = SDL_SCANCODE_ESCAPE;

		keyControl[toInt(CONTROL::LEFT_ARROW)] = SDL_GetKeyFromScancode(scanControl[toInt(CONTROL::LEFT_ARROW)]);
		keyControl[toInt(CONTROL::UP_ARROW)] = SDL_GetKeyFromScancode(scanControl[toInt(CONTROL::UP_ARROW)]);
		keyControl[toInt(CONTROL::DOWN_ARROW)] = SDL_GetKeyFromScancode(scanControl[toInt(CONTROL::DOWN_ARROW)]);
		keyControl[toInt(CONTROL::RIGHT_ARROW)] = SDL_GetKeyFromScancode(scanControl[toInt(CONTROL::RIGHT_ARROW)]);
		keyControl[toInt(CONTROL::CHOOSE)] = SDL_GetKeyFromScancode(scanControl[toInt(CONTROL::CHOOSE)]);
		keyControl[toInt(CONTROL::ESCAPE)] = SDL_GetKeyFromScancode(scanControl[toInt(CONTROL::ESCAPE)]);
	}
 }

void Event::freeEventControl() {
	delete[] scanControl;
	delete[] keyControl;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Event Control freed successfully" << endl;
}

void Event::passEventControl(Event* event)
{
	for (int key = 0; key < toInt(BUTTON::TOTAL); key++) {
		scanControl[key] = event->scanControl[key];
		keyControl[key] = event->keyControl[key];
	}
}

bool Event::quit() {
	return (e.type == SDL_QUIT);
}

bool Event::pause() {
	return (keyState[scanControl[toInt(CONTROL::ESCAPE)]] || e.window.event == SDL_WINDOWEVENT_MINIMIZED);
}

bool Event::choose()
{
	return (getMouseButton() != 0 || checkKeyEventDown(CONTROL::CHOOSE));
}

void Event::checkInputText(string& inputText)
{
	switch (e.type) {
		case SDL_TEXTINPUT: {
			inputText += e.text.text;
			break;
		}
		case SDL_KEYDOWN: {
			if (e.key.keysym.sym == SDLK_BACKSPACE && !inputText.empty()) {
				inputText.pop_back();
			}
			break;
		}
	}
}

//event check
bool Event::checkKeyState(CONTROL key) {
	return keyState[scanControl[toInt(key)]];
}

bool Event::checkKeyEventDown(CONTROL key) {
	return (e.key.keysym.sym == keyControl[toInt(key)] && e.type == SDL_KEYDOWN);
}

bool Event::checkKeyEventUp(CONTROL key)
{
	return (e.key.keysym.sym == keyControl[toInt(key)] && e.type == SDL_KEYUP);
}

bool Event::checkMouseEvent()
{
	return (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP);
}

//get mouse state
SDL_Point Event::getMousePos() {
	return mousePos;
}

Uint32 Event::getMouseButton() {
	if (!mouseRepeat) {
		return mouseButton;
	}
	return 0;
}

SDL_Keycode Event::getKeyEvent()
{
	if (e.type == SDL_KEYDOWN) {
		return e.key.keysym.sym;
	}
	return 0;
}



//Window

Window::Window()
{
	window = nullptr;
	renderer = nullptr;
	screenUnit = nullptr;
	screenWidth = nullptr;
	screenHeight = nullptr;
	sizeChange = nullptr;
	fullscreen = nullptr;
}

//pass the renderer value to the subclass
void Window::setRenderer(Window& screen) {
	window = screen.window;
	renderer = screen.renderer;
	screenUnit = screen.screenUnit;
	screenWidth = screen.screenWidth;
	screenHeight = screen.screenHeight;
	sizeChange = screen.sizeChange;
	fullscreen = screen.fullscreen;
}

//set the renderer parameters
void Window::setScreenUnit(int number) {
	if (number != 0) {
		if (screenUnit == nullptr) {
			screenUnit = new int(number);
		}
		else {
			*screenUnit = number;
		}

		if (screenWidth == nullptr) {
			screenWidth = new int(number * 16);
		}
		else {
			*screenWidth = number * 16;
		}

		if (screenHeight == nullptr) {
			screenHeight = new int(number * 9);
		}
		else {
			*screenHeight = number * 9;
		}

		if (sizeChange == nullptr) {
			sizeChange = new bool(true);
		}
		else {
			*sizeChange = true;
		}
		if (fullscreen == nullptr) {
			fullscreen = new bool(false);
		}
		cout << "Log [" << SDL_GetTicks() << "]: " << "Set screen resolution to " << *screenWidth << " x " << *screenHeight << endl;
	}
}

void Window::defaultScreenUnit() {
	int defaultScreenUnit = 80;
	setScreenUnit(defaultScreenUnit);
	*fullscreen = false;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Couldn't open file, using default resolution 1280 x 720" << endl;
}

//create window
bool Window::createWindow() {
	Uint32 initiate = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *screenWidth, *screenHeight, initiate);

	bool success = (window != NULL);
	if (success) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Window loaded successfully in " << window << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << endl;
		loadErrorLog();
	}
	return success;
}

//create renderer
bool Window::createRenderer() 
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
	}

	bool success = (renderer != NULL);
	if (success) {
		SDL_RenderSetLogicalSize(renderer, *screenWidth, *screenHeight);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		clearRenderer();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Renderer loaded successfully in " << renderer << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << endl;
		loadErrorLog();
	}
	return success;
}

//renderer present and clear functions
void Window::clearRenderer() {
	SDL_RenderClear(renderer);
}

void Window::presentRenderer() {
	SDL_RenderPresent(renderer);
}

//free the screen
void Window::free() {
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	delete screenWidth;
	delete screenHeight;
	delete screenUnit;
	delete sizeChange;
	delete fullscreen;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Renderer closed successfully" << endl;
	SDL_DestroyWindow(window);
	window = NULL;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Window closed successfully" << endl;
}

void Window::setDisplaySize(Event* event)
{
	switch (event->e.window.event) {
		case SDL_WINDOWEVENT_RESIZED: {
			int windowWidth = event->e.window.data1,
				windowHeight = event->e.window.data2;
			int newScreenUnit = min(toInt(windowWidth / 16), toInt(windowHeight / 9));
			if (*screenUnit != newScreenUnit) {
				setScreenUnit(newScreenUnit);
				SDL_RenderSetLogicalSize(renderer, *screenWidth, *screenHeight);
				*sizeChange = true;
			}
			break;
		}
		case SDL_WINDOWEVENT_MAXIMIZED: {
			SDL_DisplayMode mode;
			if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
				setScreenUnit(min(toInt(mode.w / 16), toInt(mode.h / 9)));
				SDL_RenderSetLogicalSize(renderer, *screenWidth, *screenHeight);
			}
			break;
		}
	}
	if (event->getKeyEvent() == SDLK_F5) {
		if (*fullscreen) {
			SDL_SetWindowFullscreen(window, SDL_FALSE);
			*fullscreen = false;
			*sizeChange = true;
		}
		else {
			SDL_SetWindowFullscreen(window, SDL_TRUE);
			*fullscreen = true;
			*sizeChange = true;
		}
	}
}
