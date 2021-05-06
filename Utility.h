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

//error loading
void loadErrorLog();


//normal texture class
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

	//render texture to the screen with given rectangle
	void render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect);

	//free the texture
	void free();

	//Destructor
	~LTexture();

	//set print color 
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//get the width of the loaded picture
	int getWidth();

	//get the height of the loaded picture
	int getHeight();
};

//button class
class LButton : public LTexture {
	bool hover = false;
	bool click = false;

public:

	//indicate the button is hovered
	void hovered();

	//indicate the button is clicked
	void clicked();

	//render button  to the screen with given rectangle
	void render(SDL_Renderer* renderer, SDL_Rect* renderRect, SDL_Rect* sourceRect);
};

//music infomation
class Music {
	Mix_Music* source = NULL;
	bool musicLoaded = false;

protected:
	string name;
	string singer;
	Uint32 start = 0;
	Uint32 duration = 0;
	int bpm = 0;
	double velocity = 0;

public:

	//constructor
	Music();

	Music (string& _name, string& _singer, Uint32& _start, Uint32& _duration, int& _bpm, double& _velocity);

	//load music file
	bool loadFromFile(string path);

	//load music infomation
	void playMusic(int loop);

	//free music file
	void freeLoad();

	//free all the infomation
	void free();

	//destructor
	~Music();

	//pass the infomation to subclasses or other music parameters 
	void getInfo(Music& song);

	//pass the parameter to other 
	Uint32 getStart();

	Uint32 getDuration();

	int getBpm();

	double getVelocity();
};

//event handler
class Event {
	SDL_Event e;
	SDL_Point mousePos = { 0, 0 };
	Uint32 mouseButton = 0;
	const Uint8* keyState = NULL;
	bool quitProgram = false;
	bool mouseRepeat = false;
protected:
	SDL_Scancode* scanControl = NULL;
	SDL_Keycode* keyControl = NULL;
public:

	//load the control keys from a set of type and key
	void loadEventCode(string& type, int& key);

	//load default control key
	void loadDefaultSetting();

	//poll the new event
	void updateEvent();

	//free the event control 
	void freeEventControl();

	//pass to the subclasses or other event 
	void passEventControl(Event* event);

	//return if the quit event has been executed
	bool quit();

	//return if the user want to pause the game
	bool pause();

	//write the key control set to a file
	void saveKey(ofstream& file);

	//event check

	bool checkKeyState(CONTROL key);

	bool checkKeyEventDown(CONTROL key);

	bool checkKeyEventUp(CONTROL key);

	bool checkKeyRepeat();

	bool checkMouseEvent();

	void checkInputText(string& inputText);

	//get mouse state
	SDL_Point getMousePos();

	Uint32 getMouseButton();

	Uint32 getEventType();

	Uint8 getWindowEvent();

	SDL_Keycode getKeyEvent();

	void getWindowParameter(int& windowWidth, int& windowHeight);
};

//renderer
class Window {

	//window and its title
	const char* windowTitle = "Game";

protected:

	SDL_Window* window = NULL;

	//renderer and the related parameters
	SDL_Renderer* renderer = nullptr;
	int* screenUnit = nullptr;
	int* screenWidth = nullptr;
	int* screenHeight = nullptr;
	bool* sizeChange = nullptr;
	bool* fullscreen = nullptr;

public:
	Window();

	//pass the renderer value to the subclass
	void setRenderer(Window& screen);

	//set the renderer parameters
	void setScreenUnit(int number);

	//set the renderer parameters to default values
	void defaultScreenUnit();

	//create window
	bool createWindow();

	//set renderer parameters as the change of window
	void setDisplaySize(Event* event);

	//save the parameters to a file
	void saveParameter(ofstream& file);

	//create renderer
	bool createRenderer();

	//clear the current renderer
	void clearRenderer();

	//present the current renderer
	void presentRenderer();

	//free the window
	void free();

	SDL_Renderer* getRenderer() {
		return renderer;
	}
};


#endif