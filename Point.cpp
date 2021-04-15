#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Point.h"
using namespace std;

//constructor

Point::Point(Screen& screen) {
	setRenderer(screen);
	point = 0;
	totalArrow = 0;
	pressedArrow = 0;
	wrongPressed = 0;
	pointColor = { 0, 0, 0 };
}

//Basic function

bool Point::loadGameFont(string& path) {
	TTF_CloseFont(pointFont);
	pointFont = NULL;
	pointColor = { 255, 255, 255 };

	pointFont = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), screenUnit);
	if (pointFont == NULL) {
		return false;
	}
	else {
		pointRect = new SDL_Rect;
		return true;
	}
}

bool Point::loadPointScreen(string& path) {
	pointText.free();
	totalArrowText.free();
	pressedArrowText.free();
	wrongPressedText.free();
	TTF_CloseFont(pointFont);
	pointFont = NULL;

	bool success = true;
	pointColor = { 250, 153, 28 };
	accuracy = point / (totalArrow * 10.0) * 100;

	pointFont = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), screenUnit * 1 / 2);
	if (pointFont == NULL) {
		return false;
	}
	else {
		if (!pointText.loadFromRenderedText(renderer, "Point: " + to_string(point), pointColor, pointFont)) {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
			success = false;
		}
		else {
			if (!totalArrowText.loadFromRenderedText(renderer, "Total arrows: " + to_string(totalArrow), pointColor, pointFont)) {
				cout << "Log [" << SDL_GetTicks() << "]: ";
				loadErrorLog();
				success = false;
			}
			else {
				if (!accuracyText.loadFromRenderedText(renderer, "Accuracy: " + to_string(accuracy) + '%', pointColor, pointFont)) {
					cout << "Log [" << SDL_GetTicks() << "]: ";
					loadErrorLog();
					success = false;
				}
				else {
					if (!pressedArrowText.loadFromRenderedText(renderer, "Pressed arrows: " + to_string(pressedArrow), pointColor, pointFont)) {
						cout << "Log [" << SDL_GetTicks() << "]: ";
						loadErrorLog();
						success = false;
					}
					else {
						if (!wrongPressedText.loadFromRenderedText(renderer, "Wrong Press: " + to_string(wrongPressed), pointColor, pointFont)) {
							cout << "Log [" << SDL_GetTicks() << "]: ";
							loadErrorLog();
							success = false;
						}
						else {
							createPointRect();
						}
					}
				}
			}
		}
	}
	return success;
}

void Point::free() {
	pointText.free();
	totalArrowText.free();
	accuracyText.free();
	pressedArrowText.free();
	wrongPressedText.free();
	TTF_CloseFont(pointFont);
	delete[] pointRect;
	pointRect = NULL;
	pointFont = NULL;
	point = 0;
	totalArrow = 0;
	pressedArrow = 0;
	wrongPressed = 0;
}

//point count

void Point::addPoint(int& accuracy) {
	if (accuracy <= 15) {
		point += 10;
	}
	else {
		if (accuracy <= 25) {
			point += 8;
		}
		else {
			if (accuracy <= 35) {
				point += 5;
			}
			else {
				if (accuracy <= 50) {
					point += 2;
				}
			}
		}
	}
	pressedArrow++;
}

void Point::arrowCount() {
	totalArrow++;
}

void Point::wrongPressCount() {
	wrongPressed++;
}

//compare function

bool Point::compare(Point& other) {
	if (point < other.point) {
		return false;
	}
	if (point == other.point) {
		if (totalArrow < other.totalArrow) {
			return false;
		}
		if (totalArrow == other.totalArrow) {
			if (wrongPressed < other.wrongPressed) {
				return false;
			}
			if (wrongPressed == wrongPressed) {
				int charPlace = 0;
				while (playerName[charPlace] == other.playerName[charPlace] 
					&& charPlace < min(playerName.length(), other.playerName.length())) {
					charPlace++;
				}
				if (playerName[charPlace] > other.playerName[charPlace]) {
					return false;
				}
			}
		}
	}
	return true;
}