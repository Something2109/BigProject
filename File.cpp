#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "File.h"
using namespace std;

//this file contains all the file reading and writing function

//load the saved setting 
bool loadSettingFile(Screen& screen, string path) {
	bool success = true;
	ifstream file((path + "/Setting.txt").c_str());
	cout << "Log [" << SDL_GetTicks() << "]: " << "Setting file opened" << endl;
	if (file.is_open()) {
		string str;
		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string type;
			int number;
			ss >> type >> number;
			if (type == "ScreenUnit" && number > 0) {
				screen.setScreenUnit(number);
				cout << "Log [" << SDL_GetTicks() << "]: " << "Set screen resolution to " << number * 16 << " x " << number * 9 << endl;
			}
		}
	}

	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "cannot open default file" << endl;
		success = false;
	}

	return success;
}

//music reading function
bool loadMusicFile(Screen& screen, vector<Music>& musicList, string path) {
	bool success = true;
	ifstream file((path + "/Music.txt").c_str());

	if (file.is_open()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music file opened" << endl;
		string str;

		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string name, singer, add;
			Uint32 duration;
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
			ss >> duration >> velocity >> spawnRate;
			Music song;
			song.loadMusicInfo(name, singer, duration ,velocity, spawnRate, screen.getScreenUnit());
			musicList.push_back(song);
			cout << "Log [" << SDL_GetTicks() << "]: " << name << " -" << singer << " is opened" << endl;
		}

		success = true;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Cannot open music file" << endl;
		success = false;
	}
	return success;
}
