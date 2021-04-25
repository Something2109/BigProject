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
bool loadSettingFile(Window& screen, Event& event, string path) {
	bool success = true;
	ifstream file((path + "/Setting.txt").c_str());

	if (file.is_open()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Setting file opened" << endl;
		string str;
		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string type;
			int number;
			ss >> type >> number;

			if (type == "ScreenUnit" && number > 0) {
				screen.setScreenUnit(number);
			}
			else {
				//event.loadEventCode(ss);
			}
		}
		
	}

	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "cannot open default file" << endl;
		success = false;
	}

	return success;
}

void Event::loadEventCode(stringstream& input) {
	string type;
	int scancode, totalButton = toInt (CONTROL::TOTAL);

	input >> type >> scancode;

	int buttonType = 0;

	if (type == "LeftArrow") {
		buttonType = toInt(CONTROL::LEFT_ARROW);
	}
	if (type == "UpArrow") {
		buttonType = toInt(CONTROL::UP_ARROW);
	}
	if (type == "DownArrow") {
		buttonType = toInt(CONTROL::DOWN_ARROW);
	}
	if (type == "RightArrow") {
		buttonType = toInt(CONTROL::RIGHT_ARROW);
	}
	if (type == "ChooseButton") {
		buttonType = toInt(CONTROL::CHOOSE);
	}
	if (type == "EscapeButton") {
		buttonType = toInt(CONTROL::ESCAPE);
	}
	scanControl[buttonType] = static_cast<SDL_Scancode>(scancode);
	keyControl[buttonType] = SDL_GetKeyFromScancode(scanControl[buttonType]);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Assign " << scancode << " to " << type << endl;
}

//music reading function
bool loadMusicFile(vector<Music>& musicList, string path, int screenUnit) {
	bool success = true;
	ifstream file((path + "/Music.txt").c_str());

	if (file.is_open()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music file opened" << endl;
		string str;

		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string name, singer, add;
			Uint32 start, duration;
			int velocity,
				bpm;
			ss >> add;
			name = add;
			ss >> add;
			while (add[0] != '-') {
				name = name + ' ' + add;
				ss >> add;
			}
			ss >> add;
			while (add[0] != '-') {
				singer = singer + ' ' + add;
				ss >> add;
			}
			ss >> start >> duration >> velocity >> bpm;
			Music song(name, singer, start, duration ,velocity, bpm, screenUnit);
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
