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

// function contain all the initiation function
bool init(Window& background, Event& event);

//SDL initiation function
bool initSDL();

//Window initiation function
bool initWindow(Window& background, Event& event);

//close window and SDL function
void closeWindow(Window& background);

//quit SDL
void quitSDL();

#endif