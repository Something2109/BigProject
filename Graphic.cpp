#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Graphic.h"
using namespace std;

Graphic::Graphic()
{
	renderer = NULL;
	setScreenUnit(0);
}

Graphic::Graphic(Window& screen)
{
	setRenderer(screen);
}

Graphic::~Graphic()
{
	renderer = NULL;
}


//background constructor
Background::Background(Window& screen)
{
	setRenderer(screen);

	bgSourceRect = nullptr;
	bgDestinationRect = nullptr;
	colorStrip = new LTexture[static_cast<int>(COLORSTRIP::TOTAL)];
	colorStripDstRect = nullptr;
	yColorStripVelo = *screenUnit / 20;
	currentTime = SDL_GetTicks();
	lastFrameTime = SDL_GetTicks() - 1;
	fpsColor = { 255, 255, 255 };
	backgroundType = BACKGROUND::MENU;
}

//basic background functions

bool Background::load(const string& path) {
	if (!loaded) {
		background.free();
		for (int type = 0; type < toInt(COLORSTRIP::TOTAL); type++) {
			colorStrip[type].free();
		}
		TTF_CloseFont(fpsFont);

		loaded = background.loadFromFile(renderer, path + "/Picture/Background/background.png");
		fpsFont = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit / 4);
		loaded = loaded && (fpsFont != NULL);

		colorStrip = new LTexture[static_cast<int>(COLORSTRIP::TOTAL)];
		for (int type = 0; type < toInt(COLORSTRIP::TOTAL); type++) {
			string colorPath = path + "/Picture/Background/";
			switch (static_cast<COLORSTRIP> (type)) {
			case COLORSTRIP::GREEN: {
				colorPath += "ColorStripGreen.png";
				break;
			}
			case COLORSTRIP::PURPLE: {
				colorPath += "ColorStripPurple.png";
				break;
			}
			case COLORSTRIP::ORANGE: {
				colorPath += "ColorStripOrange.png";
				break;
			}
			}
			loaded = loaded && colorStrip[type].loadFromFile(renderer, colorPath);
		}

		if (loaded) {
			createDefaultRect();
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
		}
	}
	return loaded;
}

void Background::render() {
	renderBackground(backgroundType);
	renderColorStrip(backgroundType);
	renderFps();
}

void Background::free() {
	background.free();
	for (int i = 0; i < toInt(COLORSTRIP::TOTAL); i++) {
		colorStrip[i].free();
	}
	fpsTexture.free();
	TTF_CloseFont(fpsFont);

	delete[] bgSourceRect;
	delete[] bgDestinationRect;
	delete[] colorStrip;
	delete[] colorStripDstRect;
	bgSourceRect = nullptr;
	bgDestinationRect = nullptr;
	colorStrip = nullptr;
	colorStripDstRect = nullptr;
	fpsFont = nullptr;
	
	cout << "Log [" << SDL_GetTicks() << "]: " << "Background Texture freed successfully" << endl;
}

void Background::setType(BACKGROUND type) {
	backgroundType = type;
}

//background moving functions

void Background::moveColorStrip() {
	for (int type = 0; type < toInt(BACKGROUND::TOTAL); type++) {
		moveColorStrip(colorStripDstRect[type]);
	}
}

void Background::moveColorStrip(SDL_Rect& movingRect) {
	movingRect.y = (movingRect.y + yColorStripVelo) % *screenHeight;
}

void Background::backgroundTransition(BACKGROUND origin, BACKGROUND moving) {
	int original = toInt (origin);
	int move = toInt (moving);

	//set transition rectangles
	cout << "Log [" << SDL_GetTicks() << "]: " << "Moving background" << endl;
	SDL_Rect bgSourceTrans = bgSourceRect[original];
	SDL_Rect bgDestinationTrans = bgDestinationRect[original];
	SDL_Rect colorStripDstTransLeft = colorStripDstRect[original],
		colorStripDstTransRight = colorStripDstRect[original];

	//set transition velocity
	int xSrcVelo = (bgSourceRect[move].x - bgSourceRect[original].x) / 50,
		ySrcVelo = (bgSourceRect[move].y - bgSourceRect[original].y) / 50,
		wSrcVelo = (bgSourceRect[move].w - bgSourceRect[original].w) / 50,
		hSrcVelo = (bgSourceRect[move].h - bgSourceRect[original].h) / 50,
		xDstVelo = (bgDestinationRect[move].x - bgDestinationRect[original].x) / 40,
		xColorStripVelo = 0;

	//set colorstrip movement

	//check if the distance is too far for the strip to move as 1 object
	if (abs(colorStripDstRect[move].x - colorStripDstRect[original].x) >= *screenWidth / 2) {

		//move as 2 strips that are opposite each other at 2 sides of the screen 

		//check to set direction the strip entering the screen and velocity of the colorstrip
		if (colorStripDstRect[original].x < *screenWidth / 2) {
			//the strip is entering from the right side of the screen
			colorStripDstTransRight.x = colorStripDstTransRight.x + toInt(*screenWidth + *screenUnit * 2.5);
			xColorStripVelo = (- (*screenUnit * 10) - colorStripDstRect[original].x) / 60;
		}
		else {
			//the strip is entering from the left side of the screen
			colorStripDstTransLeft.x = colorStripDstTransRight.x - toInt(*screenWidth + *screenUnit * 2.5);
			xColorStripVelo = (*screenWidth - colorStripDstRect[original].x) / 60;
		}
	}
	else {
		//move as 1 strip
		xColorStripVelo = (colorStripDstRect[move].x - colorStripDstRect[original].x) * 8 / *screenUnit / 5;
	}

	//transition loop
	while (!SDL_RectEquals(&bgSourceTrans, &bgSourceRect[move]) || !SDL_RectEquals(&bgDestinationTrans, &bgDestinationRect[move]) || 
		(!SDL_RectEquals(&colorStripDstTransLeft, &colorStripDstRect[move]) && !SDL_RectEquals(&colorStripDstTransRight, &colorStripDstRect[move]))) {
		SDL_RenderClear(renderer);

		//move the background and the colorstrips
		bgSourceTrans.x += xSrcVelo;
		bgSourceTrans.y += ySrcVelo;
		bgSourceTrans.w += wSrcVelo;
		bgSourceTrans.h += hSrcVelo;
		bgDestinationTrans.x += xDstVelo;
		colorStripDstTransLeft.x += xColorStripVelo;
		colorStripDstTransRight.x += xColorStripVelo;

		//check if the background has completed moving or not 
		checkBgTransition(bgSourceTrans, bgSourceRect[original], bgSourceRect[move]);
		checkBgTransition(bgDestinationTrans, bgDestinationRect[original], bgDestinationRect[move]);
		
		//render the background
		background.render(renderer, &bgDestinationTrans, &bgSourceTrans);

		//check if the colorstrips have completed moving or not 
		if (xColorStripVelo > 0 && colorStripDstTransLeft.x > colorStripDstRect[move].x) {
			colorStripDstTransLeft = colorStripDstRect[move];
		}
		if (xColorStripVelo < 0 && colorStripDstTransRight.x < colorStripDstRect[move].x) {
			colorStripDstTransRight = colorStripDstRect[move];
		}

		//move the y direction of the colorstrip 
		moveColorStrip();
		moveColorStrip(colorStripDstTransLeft);
		moveColorStrip(colorStripDstTransRight);

		//render colorstrips
		renderColorStrip(colorStripDstTransLeft);
		if (!SDL_RectEquals(&colorStripDstTransLeft, &colorStripDstTransRight)) {
			renderColorStrip(colorStripDstTransRight);
		}

		//renderFps
		renderFps();

		SDL_RenderPresent(renderer);
	}
	backgroundType = moving;
}

void Background::checkBgTransition(SDL_Rect& movingRect, SDL_Rect& original, SDL_Rect& move) {
	/*bool complete = ((move.x > original.x) ^ (movingRect.x < move.x));
	complete = complete || ((move.y > original.y) ^ (movingRect.y < move.y));
	complete = complete || ((move.w > original.w) ^ (movingRect.w < move.w));
	complete = complete || ((move.h > original.h) ^ (movingRect.h < move.h));
	if (complete) {
		movingRect = move;
	}*/
	if ((move.x > original.x) ^ (movingRect.x < move.x)) {
		movingRect.x = move.x;
	}
	if ((move.y > original.y) ^ (movingRect.y < move.y)) {
		movingRect.y = move.y;
	}
	if ((move.w > original.w) ^ (movingRect.w < move.w)) {
		movingRect.w = move.w;
	}
	if ((move.h > original.h) ^ (movingRect.h < move.h)) {
		movingRect.h = move.h;
	}
}

//background rendering functions

void Background::renderColorStrip(BACKGROUND type) {
	moveColorStrip();
	int backgroundType = toInt(type);
	renderColorStrip(colorStripDstRect[backgroundType]);
}

void Background::renderColorStrip(SDL_Rect& rectangle) {
	for (int type = 0; type < toInt(COLORSTRIP::TOTAL); type++) {
		if (type % 2 == 1) {
			rectangle.y = *screenHeight - rectangle.y;
		}
		colorStrip[type].render(renderer, &rectangle, NULL);
		rectangle.y -= *screenHeight;
		colorStrip[type].render(renderer, &rectangle, NULL);
		rectangle.y += *screenHeight;
		if (type % 2 == 1) {
			rectangle.y = *screenHeight - rectangle.y;
		}
	}
}

void Background::renderBackground(BACKGROUND type) {
	int backgroundType = toInt(type);
	background.render(renderer, &bgDestinationRect[backgroundType], &bgSourceRect[backgroundType]);
}

void Background::renderFps()
{
	currentTime = SDL_GetTicks();
	frameCount++;
	if (currentTime > fpsTimer * 500) {
		fps = frameCount * 1000 / (currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		fpsTexture.loadFromRenderedText(renderer, to_string(fps), fpsColor, fpsFont);
		frameCount = 0;
		fpsTimer++;
	}
	fpsTexture.render(renderer, NULL, NULL);
}


//Button

Button::Button()
{
	button = nullptr;
	buttonDstRect = nullptr;
	usingButton = nullptr;
	buttonChoose = toInt(BUTTON::NOT_CHOOSE);
}

//constructor
Button::Button(Window& screen)
{
	setRenderer(screen);
	button = nullptr;
	buttonDstRect = nullptr;
	usingButton = nullptr;
	buttonChoose = toInt(BUTTON::NOT_CHOOSE);
	button = new LButton[static_cast<int>(BUTTON::TOTAL)];
	usingButton = new bool[static_cast<int>(BUTTON::TOTAL)];
	for (int button = 0; button < toInt(BUTTON::TOTAL); button++) {
		usingButton[button] = false;
	}
}

//basic function

bool Button::load(const string& path) {
	if (!loaded) {
		loaded = true;

		for (int type = 0; type < toInt(BUTTON::TOTAL); type++) {
			button[type].free();
			string buttonPath = path + "/Picture/Button/";
			switch (static_cast<BUTTON> (type)) {
				case BUTTON::PLAY: {
					buttonPath += "playGameButton.png";
					break;
				}
				case BUTTON::CONTINUE: {
					buttonPath += "continueButton.png";
					break;
				}
				case BUTTON::SAVE: {
					buttonPath += "saveButton.png";
					break;
				}
				case BUTTON::SETTING: {
					buttonPath += "settingButton.png";
					break;
				}
				case BUTTON::EXIT: {
					buttonPath += "exitButton.png";
					break;
				}
				case BUTTON::CHANGE_SONG_LEFT: {
					buttonPath += "changeButtonLeft.png";
					break;
				}
				case BUTTON::CHANGE_SONG_RIGHT: {
					buttonPath += "changeButtonRight.png";
					break;
				}
			}
			loaded = loaded && button[type].loadFromFile(renderer, buttonPath);
		}

		if (loaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Button created successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
		}
	}
	return loaded;
}

void Button::render() {
	for (int type = 0; type < toInt (BUTTON::TOTAL); type++) {
		button[type].render(renderer, &buttonDstRect[type], NULL);
	}
}

void Button::free() {
	for (int type = 0; type < toInt (BUTTON::TOTAL); type++) {
		button[type].free();
	}
	delete[] button;
	delete[] buttonDstRect;
	delete[] usingButton;
	button = nullptr;
	buttonDstRect = nullptr;
	usingButton = nullptr;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Button Texture freed successfully" << endl;
}

//event function

BUTTON Button::eventHandle(Event& event)
{
	if (event.checkKeyEvent()) {
		return keyEventHandle(event);
	}
	
	if (event.checkMouseEvent()) {
		return mouseEventHandle(event);
	}
	if (buttonChoose != toInt(BUTTON::NOT_CHOOSE)) {
		button[buttonChoose].hovered();
	}
	return BUTTON::NOT_CHOOSE;
}

BUTTON Button::mouseEventHandle(Event& event) {
	SDL_Point mousePos = event.getMousePos();

	for (int type = 0; type < toInt(BUTTON::TOTAL); type++) {
		if (SDL_PointInRect(&mousePos, &buttonDstRect[type])) {
			buttonChoose = type;
			button[type].hovered();
			if (event.getMouseButton() != 0) {
				button[type].clicked();
				return static_cast<BUTTON>(type);
			}
			else {
				return BUTTON::NOT_CHOOSE;
			}
		}
	}
	buttonChoose = toInt(BUTTON::NOT_CHOOSE);
	return BUTTON::NOT_CHOOSE;
}

BUTTON Button::keyEventHandle(Event& event)
{
	if (event.checkKeyEventDown(CONTROL::CHOOSE)) {
		if (buttonChoose >= 0) {
			button[buttonChoose].clicked();
			return static_cast<BUTTON>(buttonChoose);
		}
	}
	if (event.checkKeyEventDown(CONTROL::UP_ARROW) || event.checkKeyEventDown(CONTROL::DOWN_ARROW)) {
		do {
			if (event.checkKeyEventDown(CONTROL::UP_ARROW)) {
				buttonChoose--;
			}
			if (event.checkKeyEventDown(CONTROL::DOWN_ARROW)) {
				buttonChoose++;
			}
			buttonChoose = (buttonChoose + toInt(BUTTON::TOTAL)) % toInt(BUTTON::TOTAL);
		} while (!usingButton[buttonChoose]);
	}
	return BUTTON::NOT_CHOOSE;
}



//Title

//Constructor

Title::Title(Window& screen)
{
	setRenderer(screen);
	title = nullptr;
	titleDstRect = new SDL_Rect[static_cast<int>(TITLE::TOTAL)];
	titleType = TITLE::MENU;
}

bool Title::load(const string& path)
{
	if (!loaded) {
		loaded = true;
		title = new LTexture[static_cast<int>(TITLE::TOTAL)];

		for (int type = 0; type < toInt(TITLE::TOTAL); type++) {
			string titlePath = path + "/Picture/Title/";
			switch (static_cast<TITLE> (type)) {
			case TITLE::MENU: {
				titlePath += "menuTitle.png";
				break;
			}
			case TITLE::PAUSE: {
				titlePath += "pauseTitle.png";
				break;
			}
			case TITLE::FINISH: {
				titlePath += "finishTitle.png";
				break;
			}
			case TITLE::SETTING: {
				titlePath += "settingTitle.png";
				break;
			}
			}
			loaded = loaded && title[type].loadFromFile(renderer, titlePath);
		}

		if (loaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Button created successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
		}
	}
	return loaded;
}

void Title::render()
{
	title[toInt(titleType)].render(renderer, &titleDstRect[toInt(titleType)], NULL);
}

void Title::free()
{
	for (int type = 0; type < toInt(TITLE::TOTAL); type++) {
		title[type].free();
	}
	delete[] title;
	delete[] titleDstRect;
	title = nullptr;
	titleDstRect = nullptr;
}

void Title::setType(TITLE type)
{
	titleType = type;
}



//Point

//constructor

Point::Point(Window& screen) {
	setRenderer(screen);
	pointTexture = nullptr;
	pointRect = nullptr;
	font = nullptr;

	point = 0;
	totalArrow = 0;
	accuracy = 0;
	pressedArrow = 0;
	wrongPressed = 0;
	combo = 0;
	maxCombo = 0;
	color = { 255, 255, 255 };
}

//Basic function

bool Point::load(const string& path) {
	if (!loaded) {
		TTF_CloseFont(font);
		font = NULL;
		color = { 254, 205, 8 };

		font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit);
		pointTexture = new LTexture[static_cast<int>(POINT::TOTAL)];
		if (font != NULL) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Point font loaded successfully" << endl;
			loaded = true;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load point font" << endl;
			loaded = false;
		}
	}
	return loaded;
}

bool Point::loadPointWindow(string& path) {
	TTF_CloseFont(font);
	font = NULL;

	bool success = true;
	color = { 250, 153, 28 };
	accuracy = toInt(pressedArrow / (totalArrow * 10.0) * 100);

	font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit * 1 / 2);
	success = success && (font != NULL);

	for (int type = 0; type < toInt(POINT::TOTAL); type++) {
		pointTexture[type].free();
		string renderText;
		switch (static_cast<POINT>(type)) {
			case POINT::POINT: {
				renderText = "Point: " + to_string(point);
				break;
			}
			case POINT::TOTAL_ARROW: {
				renderText = "Total arrows: " + to_string(totalArrow);
				break;
			}

			case POINT::ACCURACY: {
				renderText = "Accuracy: " + to_string(accuracy);
				break;
			}

			case POINT::PRESSED_ARROW: {
				renderText = "Pressed arrows: " + to_string(pressedArrow);
				break;
			}

			case POINT::MAX_COMBO: {
				renderText = "Max combo: " + to_string(maxCombo);
				break;
			}

			case POINT::WRONG_PRESSED: {
				renderText = "Wrong pressed: " + to_string(wrongPressed);
				break;
			}
		}
		success = success && pointTexture[type].loadFromRenderedText(renderer, renderText, color, font);
	}

	if (success) {
		createDefaultRect();
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
	}
	return success;
}

void Point::render() {
	if (!loaded) {
		loaded = pointTexture[toInt(POINT::POINT)].loadFromRenderedText(renderer, ("Point: " + to_string(point)), color, font);
		if (combo > 1) {
			loaded = pointTexture[toInt(POINT::MAX_COMBO)].loadFromRenderedText(renderer, ("Combo: " + to_string(combo)), color, font);
		}
	}
	SDL_Rect pointRect = { *screenUnit * 10 , *screenUnit, pointTexture[toInt(POINT::POINT)].getWidth(), pointTexture[toInt(POINT::POINT)].getHeight() };
	pointTexture[toInt(POINT::POINT)].render(renderer, &pointRect, NULL);
	if (combo > 1) {
		SDL_Rect comboRect = { *screenUnit * 12, *screenUnit * 2, pointTexture[toInt(POINT::MAX_COMBO)].getWidth() / 2, pointTexture[toInt(POINT::MAX_COMBO)].getHeight() / 2 };
		pointTexture[toInt(POINT::MAX_COMBO)].render(renderer, &comboRect, NULL);
	}
}

void Point::free() {
	for (int type = 0; type < toInt(POINT::TOTAL); type++) {
		pointTexture[type].free();
	}

	TTF_CloseFont(font);
	delete[] pointTexture;
	delete[] pointRect;
	pointTexture = nullptr;
	pointRect = nullptr;
	font = nullptr;

	point = 0;
	totalArrow = 0;
	accuracy = 0;
	pressedArrow = 0;
	wrongPressed = 0;
	combo = 0;
	maxCombo = 0;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Point Texture freed successfully" << endl;
}

//point count

bool Point::addPoint(int& accuracy) {
	if (accuracy <= 50) {
		totalArrow++;
		pressedArrow++;
		combo++;
		maxCombo = max(combo, maxCombo);
		point += 2;
		if (accuracy <= 35) {
			point += 3;
			if (accuracy <= 25) {
				point += 2;
				if (accuracy <= 15) {
					point += 3;
				}
			}
		}
		loaded = false;
		return true;
	}
	else {
		return false;
	}
}

void Point::missedArrowCount() {
	totalArrow++;
	maxCombo = max(combo, maxCombo);
	combo = 0;
	loaded = false;
}

void Point::wrongPressCount() {
	wrongPressed++;
	maxCombo = max(combo, maxCombo);
	combo = 0;
	loaded = false;
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



//ArrowTexture

//constructor

ArrowTexture::ArrowTexture(Window& screen) {
	setRenderer(screen);
	blankArrowDstRect = nullptr;
	arrowSrcRect = nullptr;
	pressedArrowDstRect = nullptr;
}

//basic functions

bool ArrowTexture::load(const string& path) {
	if (!loaded) {
		blankArrow.free();
		arrow.free();
		pressedArrow.free();

		loaded = blankArrow.loadFromFile(renderer, path + "/Picture/Arrow/blankArrow.png");
		loaded = loaded && arrow.loadFromFile(renderer, path + "/Picture/Arrow/arrow.png");
		loaded = loaded && pressedArrow.loadFromFile(renderer, path + "/Picture/Arrow/pressedArrowVietCong.png");

		if (loaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Arrow Texture loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load Arrow Texture" << endl;
			loadErrorLog();
		}
	}
	return loaded;
}

void ArrowTexture::render()
{
	blankArrow.render(renderer, blankArrowDstRect, NULL);
	for (int i = 0; i < arrowRange.size(); i++) {
		SDL_Rect renderRect = { arrowRange[i].x, arrowRange[i].y, *screenUnit, *screenUnit };
		int arrowCol = (arrowRange[i].x / *screenUnit - 1) / 2;
		arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
	}
}

void ArrowTexture::free() {
	arrow.free();
	blankArrow.free();
	pressedArrow.free();

	delete blankArrowDstRect;
	delete[] pressedArrowDstRect;
	delete[] arrowSrcRect;
	blankArrowDstRect = nullptr;
	arrowSrcRect = nullptr;
	pressedArrowDstRect = nullptr;

	cout << "Log [" << SDL_GetTicks() << "]: " << "Arrow Texture freed successfully" << endl;
}

//Arrow spawn functions

void ArrowTexture::setTimeline(Music& song) {
	Uint32 time = SDL_GetTicks();
	spawnRate = static_cast<Uint32>(round(60000.0 / song.getBpm()));
	song.playMusic(0);
	velocity = song.getVelocity();
	nextSpawnTime = time + song.getStart() - (*screenHeight - *screenUnit) * 1000 / velocity / 60;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game starting time: " << time << endl;
	spawnDuration = time + song.getDuration() - (*screenHeight - *screenUnit) * 1000 / velocity / 60;
}

void ArrowTexture::addPauseTime(Uint32 time) {
	pauseTime += time;
}

//add new arrow to arrow range
void ArrowTexture::addArrow(Uint32& time) {
	gameTime = time - pauseTime;
	cout << pauseTime << endl;
	if (gameTime > nextSpawnTime && gameTime < spawnDuration) {
		int arrowCol = rand() % 4;
		int x = *screenUnit * (1 + arrowCol * 2);
		int y = toInt(*screenHeight - velocity * (gameTime - nextSpawnTime) * 60.0 / 1000);
		SDL_Point newArrow = { x, y };
		arrowRange.push_back(newArrow);
		nextSpawnTime += spawnRate;

	}
}

void ArrowTexture::scoreCheck(int& keyType, Point& point) {
	if (!arrowRange.empty()) {
		int arrowCol = (arrowRange[0].x / *screenUnit - 1) / 2;
		if (arrowCol == keyType) {
			int accuracy = abs(arrowRange[0].y * 50 / *screenUnit - 50);
			if (point.addPoint(accuracy)) {
				SDL_Rect renderRect = { arrowRange[0].x, arrowRange[0].y, *screenUnit, *screenUnit };
				arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
				arrowRange.erase(arrowRange.begin());
			}
			else {
				point.wrongPressCount();
			}
		}
		else {
			point.wrongPressCount();
		}
	}
}

void ArrowTexture::updateArrowRange(Point& point) {
	for (int i = 0; i < arrowRange.size(); i++) {
		if (arrowRange[i].y <= -*screenUnit) {
			arrowRange.erase(arrowRange.begin() + i);
			point.missedArrowCount();
		}
		else {
			arrowRange[i].y -= velocity;
		}
	}
}



//DogeTexture

//Constructor

DogeTexture::DogeTexture(Window& screen)
{
	setRenderer(screen);
	bonk = nullptr;
	muscleDogeRect = nullptr;
	cheemsSrcRect = nullptr;
	cheemsDstRect = nullptr;
}

//Basic functions

bool DogeTexture::load(const string& path) {
	if (!loaded) {

		muscleDoge.free();
		cheems.free();
		smashedCheems.free();
		Mix_FreeChunk(bonk);

		loaded = muscleDoge.loadFromFile(renderer, path + "/Picture/Doge/muscleDoge.png");
		loaded = loaded && cheems.loadFromFile(renderer, path + "/Picture/Doge/cheems.png");
		loaded = loaded && smashedCheems.loadFromFile(renderer, path + "/Picture/Doge/smashedCheems.png");
		bonk = Mix_LoadWAV((path + "/Music/bonk.wav").c_str());
		loaded = loaded && (bonk != NULL);

		if (loaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Doge Texture loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load Doge Texture" << endl;
			loadErrorLog();
		}
	}
	return loaded;
}

void DogeTexture::render()
{
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::LEFT_ARROW)], &cheemsSrcRect[toInt(CONTROL::LEFT_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::RIGHT_ARROW)], &cheemsSrcRect[toInt(CONTROL::RIGHT_ARROW)]);
	muscleDoge.render(renderer, &muscleDogeRect[toInt(DOGE::DESTINATION)], &muscleDogeRect[toInt(DOGE::SOURCE_IDLE)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::UP_ARROW)], &cheemsSrcRect[toInt(CONTROL::UP_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::DOWN_ARROW)], &cheemsSrcRect[toInt(CONTROL::DOWN_ARROW)]);
}

void DogeTexture::free() {
	muscleDoge.free();
	cheems.free();
	smashedCheems.free();
	Mix_FreeChunk(bonk);
	bonk = nullptr;

	delete[] muscleDogeRect;
	muscleDogeRect = nullptr;
	delete[] cheemsSrcRect;
	cheemsSrcRect = nullptr;
	delete[] cheemsDstRect;
	cheemsDstRect = nullptr;

	cout << "Log [" << SDL_GetTicks() << "]: " << "Doge Texture freed successfully" << endl;
}

//play sound

void DogeTexture::playBonkSound() {
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayChannel(0, bonk, 0);
	}
}



//Setting Texture

//constructor
SettingTexture::SettingTexture(Window& screen)
{
	setRenderer(screen);
	font = nullptr;
}

//basic functions
bool SettingTexture::load(const string& path)
{
	if (!loaded) {
		font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit * 2 / 3);
	}
	return loaded;
}

void SettingTexture::render()
{
}

void SettingTexture::free()
{
	TTF_CloseFont(font);
	font = nullptr;
}



//Music Texture

//constructor
MusicTexture::MusicTexture(Window& screen)
{
	setRenderer(screen);
	font = nullptr;
	color = { 255, 255, 255 };
	nameRect = { 0, 0, 0, 0 };
	authorRect = { 0, 0, 0, 0 };
	difficultyRect = { 0, 0, 0, 0 };
	bpmRect = { 0, 0, 0, 0 };
}

//basic functions
bool MusicTexture::load(const string& path)
{
	TTF_CloseFont(font);
	font = NULL;
	color = { 254, 205, 8 };

	font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit / 2);
	if (font != NULL) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music font loaded successfully" << endl;
		loaded = true;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load music font" << endl;
		loaded = false;
	}
	return loaded;
}


void MusicTexture::render()
{
	nameText.render(renderer, &nameRect, NULL);
	authorText.render(renderer, &authorRect, NULL);
	difficultyText.render(renderer, &difficultyRect, NULL);
	bpmText.render(renderer, &bpmRect, NULL);
}

void MusicTexture::free()
{
	nameText.free();
	authorText.free();
	difficultyText.free();
	bpmText.free();

	TTF_CloseFont(font);
	font = nullptr;
	cout << "Log [" << SDL_GetTicks() << "]: " << "Music Texture freed successfully" << endl;
}

bool MusicTexture::loadSong(Music& song)
{
	getInfo(song);
	loaded = nameText.loadFromRenderedText(renderer, "Name: " + name, color, font);
	loaded = loaded && authorText.loadFromRenderedText(renderer, "Author: " + singer, color, font);
	loaded = loaded && difficultyText.loadFromRenderedText(renderer, "Difficulty: " + to_string(velocity), color, font);
	loaded = loaded && bpmText.loadFromRenderedText(renderer, "BPM: " + to_string(bpm), color, font);
	if (loaded) {
		createDefaultRect();
	}
	return loaded;
}