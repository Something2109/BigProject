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
#include "Screen.h"
using namespace std;

//this file contains all the file reading and writing function

//load the saved setting 
void loadSettingFile(Window& screen, Event& event, const string path) {
	ifstream file((path + "/Setting.txt").c_str());

	if (file.is_open()) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Setting file opened" << endl;
		string str;
		while (!file.eof() || getline(file, str).good()) {
			getline(file, str);
			stringstream ss(str);
			string type;
			int number;
			ss >> type >> number;

			if (type == "ScreenUnit") {
				if (number >= 30) {
					screen.setScreenUnit(number);
				}
				else {
					screen.defaultScreenUnit();
				}
			}
			else {
				event.loadEventCode(type, number);
			}
		}
		cout << "Log [" << SDL_GetTicks() << "]: " << "Setting file closed" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "cannot open default file" << endl;
		screen.defaultScreenUnit();
		event.loadDefaultSetting();
		SDL_RWops* newFile = SDL_RWFromFile((path + "/Setting.txt").c_str(), "w");
		SDL_RWclose(newFile);
	}
	//file.close();
	cout << endl;
}

void saveSettingFile(Window& screen, Event& event, const string path) {
	ofstream file((path + "/Setting.txt").c_str());
	cout << "Log [" << SDL_GetTicks() << "]: " << "Setting file opened" << endl;
	if (!file.is_open()) {
		file.close();
		SDL_RWops* newFile = SDL_RWFromFile((path + "/Setting.txt").c_str(), "w");
		SDL_RWclose(newFile);
		file.open((path + "/Setting.txt").c_str());
	}
	screen.saveParameter(file);
	event.saveKey(file);
	//file.close();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Setting saved successfully" << endl;
	cout << endl;
}

//music reading function
bool ChooseMusic::loadMusicFile(const string path) {
	ifstream file((path + "/Music.txt").c_str());
	bool success = file.is_open();

	if (success) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music file opened" << endl;
		string str;

		while (!file.eof()) {
			getline(file, str);
			stringstream ss(str);
			string name, singer, add;
			Uint32 start, duration;
			double velocity;
			int bpm;
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
			ss >> start >> duration >> bpm >> velocity;
			Music song(name, singer, start, duration , bpm, velocity);
			musicList.push_back(song);
			cout << "Log [" << SDL_GetTicks() << "]: " << name << " -" << singer << " is opened" << endl;
		}

		cout << "Log [" << SDL_GetTicks() << "]: " << "Music file closed" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Cannot open music file" << endl;
	}
	//file.close();
	cout << endl;
	return success;
}

void Point::saveScore(const string& path)
{
	ofstream file((path + "/Score.txt").c_str(), ios::app);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Score file opened" << endl;
	if (!file.is_open()) {
		file.close();
		SDL_RWops* newFile = SDL_RWFromFile((path + "/Setting.txt").c_str(), "w");
		SDL_RWclose(newFile);
		file.open((path + "/Score.txt").c_str());
	}
	file << playerName << ' ' << point << ' ' << totalArrow << ' ' << accuracy << ' ' << wrongPressed << ' ' << maxCombo << endl;
	//file.close();
	cout << "Log [" << SDL_GetTicks() << "]: " << "Score saved successfully" << endl;
	cout << endl;
}

void Event::loadEventCode(string& type, int& key) {
	int buttonType = -1;
	if (scanControl == nullptr) {
		scanControl = new SDL_Scancode[static_cast<int>(CONTROL::TOTAL)];
	}
	if (keyControl == nullptr) {
		keyControl = new SDL_Keycode[static_cast<int>(CONTROL::TOTAL)];
	}
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
	if (buttonType != -1) {
		scanControl[buttonType] = static_cast<SDL_Scancode>(key);
		keyControl[buttonType] = SDL_GetKeyFromScancode(scanControl[buttonType]);
		cout << "Log [" << SDL_GetTicks() << "]: " << "Assign " << SDL_GetKeyName(keyControl[buttonType]) << " to " << type << endl;
	}
}

void Event::saveKey(ofstream& file)
{
	for (int key = 0; key < toInt(CONTROL::TOTAL); key++) {
		string saveText;
		switch (static_cast<CONTROL>(key)) {
		case CONTROL::LEFT_ARROW: {
			saveText = "LeftArrow ";
			break;
		}
		case CONTROL::UP_ARROW: {
			saveText = "UpArrow ";
			break;
		}
		case CONTROL::DOWN_ARROW: {
			saveText = "DownArrow ";
			break;
		}
		case CONTROL::RIGHT_ARROW: {
			saveText = "RightArrow ";
			break;
		}
		case CONTROL::CHOOSE: {
			saveText = "ChooseButton ";
			break;
		}
		case CONTROL::ESCAPE: {
			saveText = "EscapeButton ";
			break;
		}
		}
		saveText += to_string(scanControl[key]);
		file << saveText << endl;
	}
}

void Window::saveParameter(ofstream& file) {
	file << "ScreenUnit " << *screenUnit << endl;
}