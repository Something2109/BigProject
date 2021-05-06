#ifndef _InitAndClose_h
#define _InitAndClose_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Utility.h"
#include "File.h"
using namespace std;

//this file contains functions to initiate and close SDL, renderer, window

// initiation function
bool init(Window& background, Event& event);

//SDL initiation
bool initSDL();

//Window initiation
bool initWindow(Window& background, Event& event);

//close window
void closeWindow(Window& background, Event& event);

//close SDL
void quitSDL();

#endif