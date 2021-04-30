#ifndef _File_h
#define _File_h
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
void loadSettingFile(Window& screen, Event& event, string path);

//music reading function
bool loadMusicFile(vector<Music>& musicList, string path);

void saveSettingFile(Window& screen, Event& event, const string path);

#endif