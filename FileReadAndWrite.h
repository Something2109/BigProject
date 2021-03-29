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
#include "Texture.h" 
#include "Sound.h"
using namespace std;

//this file contains all the file reading and writing function

//load the saved setting 
bool loadSettingFile(Screen &background, string path) {
	bool success = true;
	ifstream file(path.c_str());

	if (file.is_open()) {
		string str;
		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string type;
			int number;
			ss >> type >> number;
			if (type == "ScreenUnit" && number > 0) {
				background.setScreenUnit(number);
			}
		}
	}

	else {
		cout << "cannot open default file" << endl;
		success = false;
	}

	return success;
}

//music reading function
bool loadMusicFile(Screen &background, vector<Music>& musicList, string path) {
	bool success = true;
	ifstream file((path + "/Music.txt").c_str());

	if (file.is_open()) {
		cout << "Music file opened" << endl;
		string str;

		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string name, singer, add;
			int velocity,
				spawnRate;
			ss >> add;
			while (add[0] != '-') {
				name += add;
				ss >> add;
			}
			ss >> add;
			while (add[0] != '-') {
				singer = singer + ' ' + add;
				ss >> add;
			}
			ss >> velocity >> spawnRate;
			Music song;
			song.loadMusicInfo(name, singer, velocity, spawnRate, background.getScreenUnit());
			musicList.push_back(song);
			cout << name << " -" << singer << " is opened" << endl;
		}

		success = true;
	}
	else {
		success = false;
	}
	return success;
}

#endif