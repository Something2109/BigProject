#ifndef _FileReadAndWrite_h
#define _FileReadAndWrite_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Utility.h" 
using namespace std;

//this file contains all the file reading and writing function

//load the saved setting 
bool loadSettingFile(Screen& screen, Event& event, string path);

//music reading function
bool loadMusicFile(Screen& background, vector<Music>& musicList, string path);

#endif