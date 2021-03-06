#include "Graphic.h"
using namespace std;

//default constructor of Graphic
Graphic::Graphic()
{
	renderer = NULL;
	setScreenUnit(0);
}

//constructor with window parameter to pass screen parameters
Graphic::Graphic(Window& screen)
{
	setRenderer(screen);
}

//destructor of Graphix
Graphic::~Graphic()
{
	renderer = NULL;
}

//set rectangle with screenUnit as the unit
SDL_Rect Graphic::setScreenUnitRect(double x, double y, double w, double h) {
	return { toInt(*screenUnit * x), toInt(*screenUnit * y), toInt(*screenUnit * w) , toInt(*screenUnit * h) };
}

//set rectangle with screenUnit as the unit and the length changes depend on the length of text 
SDL_Rect Graphic::setTextRect(double x, double y, int wText, int hText)
{
	return { toInt(*screenUnit * x), toInt(*screenUnit * y), wText , hText };
}




//background constructor
Background::Background(Window& screen)
{
	setRenderer(screen);

	background = nullptr;
	bgSourceRect = nullptr;
	bgDestinationRect = nullptr;
	bgSourceVelo = { 0, 0, 0, 0 };
	bgDstVelo = 0;

	colorStrip = nullptr;
	colorStripDstRect = nullptr;
	ColorStripVelo = { 0, 0};
	currentColorPlace = 0;

	fpsTexture = nullptr;
	currentTime = SDL_GetTicks();
	lastFrameTime = SDL_GetTicks() - 1;
	fpsColor = { 255, 255, 255 };
	backgroundType = BACKGROUND::INTRO1;
}

//basic background functions

bool Background::load(const string& path) {
	if (!textureLoaded) {
		TTF_CloseFont(fpsFont);

		background = new LTexture;
		colorStrip = new LTexture[static_cast<int>(COLORSTRIP::TOTAL)];
		fpsTexture = new LTexture;

		fpsFont = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit / 4);
		textureLoaded = (fpsFont != NULL);

		textureLoaded = textureLoaded && background->loadFromFile(renderer, path + "/Picture/Background/background.png");
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
				default: {
					continue;
				}
			}
			textureLoaded = textureLoaded && colorStrip[type].loadFromFile(renderer, colorPath);
		}

		if (textureLoaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Background created successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
		}
	}
	return textureLoaded;
}

void Background::render() {
	int type = toInt(backgroundType);
	moveColorStrip(colorStripDstRect[type]);

	background->render(renderer, &bgDestinationRect[type], &bgSourceRect[type]);
	renderColorStrip(colorStripDstRect[type]);
	renderFps();
}

void Background::free() {
	background->free();
	for (int i = 0; i < toInt(COLORSTRIP::TOTAL); i++) {
		colorStrip[i].free();
	}
	fpsTexture->free();
	TTF_CloseFont(fpsFont);

	delete background;
	delete[] bgSourceRect;
	delete[] bgDestinationRect;

	delete[] colorStrip;
	delete[] colorStripDstRect;

	delete fpsTexture;

	background = nullptr;
	bgSourceRect = nullptr;
	bgDestinationRect = nullptr;

	colorStrip = nullptr;
	colorStripDstRect = nullptr;

	fpsTexture = nullptr;
	fpsFont = nullptr;
	
	cout << "Log [" << SDL_GetTicks() << "]: " << "Background Texture freed successfully" << endl;
}

void Background::setType(BACKGROUND type) {
	backgroundType = type;
}

//background moving functions

void Background::setBgVelocity(BACKGROUND origin, BACKGROUND moving)
{
	int original = toInt(origin);
	int move = toInt(moving);

	bgSourceVelo.x = toInt(ceil(static_cast<double>(bgSourceRect[move].x - bgSourceRect[original].x) / TOTAL_BG_TRANSITION_FRAME));
	bgSourceVelo.y = toInt(ceil(static_cast<double>(bgSourceRect[move].y - bgSourceRect[original].y) / TOTAL_BG_TRANSITION_FRAME));
	bgSourceVelo.w = toInt(ceil(static_cast<double>(bgSourceRect[move].w - bgSourceRect[original].w) / TOTAL_BG_TRANSITION_FRAME));
	bgSourceVelo.h = toInt(ceil(static_cast<double>(bgSourceRect[move].h - bgSourceRect[original].h) / TOTAL_BG_TRANSITION_FRAME));
	bgDstVelo = toInt(ceil(static_cast<double>(bgDestinationRect[move].x - bgDestinationRect[original].x) / FPS * 2));
}

void Background::moveColorStrip(SDL_Rect& movingRect) {
	movingRect.x = movingRect.x + ColorStripVelo.x;
	movingRect.y = (movingRect.y + ColorStripVelo.y) % *screenHeight;
	currentColorPlace = movingRect.y / static_cast<double>(*screenHeight);
}

void Background::backgroundTransition(BACKGROUND origin, BACKGROUND moving, Event& event) {
	int original = toInt (origin);
	int move = toInt (moving);
	bool quit = false;

	//set transition rectangles
	cout << "Log [" << SDL_GetTicks() << "]: " << "Moving background" << endl;
	SDL_Rect bgSourceTrans = bgSourceRect[original];
	SDL_Rect bgDestinationTrans = bgDestinationRect[original];
	//SDL_Rect bgSourceVelo;
	SDL_Rect colorStripDstTransLeft = colorStripDstRect[original],
		colorStripDstTransRight = colorStripDstRect[original];

	//set transition velocity

	setBgVelocity(origin, moving);
	
	//set colorstrip movement

	//check if the distance is too far for the strip to move as 1 object
	if (abs(colorStripDstRect[move].x - colorStripDstRect[original].x) >= *screenWidth / 2) {

		//move as 2 strips that are opposite each other at 2 sides of the screen 

		//check to set direction the strip entering the screen and velocity of the colorstrip
		if (colorStripDstRect[original].x < *screenWidth / 2) {
			//the strip is entering from the right side of the screen
			colorStripDstTransRight.x = *screenWidth;
			ColorStripVelo.x = toInt(floor( min((- (*screenUnit * 10) - colorStripDstRect[original].x), (colorStripDstRect[move].x - *screenWidth)) / FPS ));
		}
		else {
			//the strip is entering from the left side of the screen
			colorStripDstTransLeft.x = *screenUnit * (-10);
			ColorStripVelo.x = toInt(ceil( max(*screenWidth - colorStripDstRect[original].x, (colorStripDstRect[move].x + (*screenUnit * 10))) / FPS ));
		}
	}
	else {
		//move as 1 strip
		if (colorStripDstRect[move].x > colorStripDstRect[original].x) {
			ColorStripVelo.x = toInt(ceil(static_cast<double>(colorStripDstRect[move].x - colorStripDstRect[original].x) / FPS ));
		}
		else {
			ColorStripVelo.x = toInt(floor(static_cast<double>(colorStripDstRect[move].x - colorStripDstRect[original].x) / FPS ));
		}
	}

	if (bgDstVelo != 0 && ColorStripVelo.x != 0 && abs(ColorStripVelo.x - bgDstVelo) < 0.5 * bgDstVelo) {
		ColorStripVelo.x = toInt(bgDstVelo * 1.5);
	}

	//transition loop
	while (!quit) 
	{
		event.updateEvent();

		SDL_RenderClear(renderer);

		//move the background and the colorstrips
		bgSourceTrans.x += bgSourceVelo.x;
		bgSourceTrans.y += bgSourceVelo.y;
		bgSourceTrans.w += bgSourceVelo.w;
		bgSourceTrans.h += bgSourceVelo.h;
		bgDestinationTrans.x += bgDstVelo;

		//check if the background has completed moving or not 
		checkBgTransition(bgSourceTrans, bgSourceRect[original], bgSourceRect[move]);
		checkBgTransition(bgDestinationTrans, bgDestinationRect[original], bgDestinationRect[move]);
		
		//render the background
		background->render(renderer, &bgDestinationTrans, &bgSourceTrans);

		//move the y direction of the colorstrip 
		moveColorStrip(colorStripDstTransLeft);
		moveColorStrip(colorStripDstTransRight);

		//check if the colorstrips have completed moving or not 
		if (ColorStripVelo.x > 0 && colorStripDstTransLeft.x > colorStripDstRect[move].x) {
			colorStripDstTransLeft.x = colorStripDstRect[move].x;
		}
		if (ColorStripVelo.x < 0 && colorStripDstTransRight.x < colorStripDstRect[move].x) {
			colorStripDstTransRight.x = colorStripDstRect[move].x;
		}

		//render colorstrips
		renderColorStrip(colorStripDstTransLeft);
		if (!SDL_RectEquals(&colorStripDstTransLeft, &colorStripDstTransRight)) {
			renderColorStrip(colorStripDstTransRight);
		}

		//renderFps
		renderFps();

		SDL_RenderPresent(renderer);

		quit = SDL_RectEquals(&bgSourceTrans, &bgSourceRect[move]) && SDL_RectEquals(&bgDestinationTrans, &bgDestinationRect[move]);
		quit = quit && (colorStripDstTransLeft.x == colorStripDstRect[move].x || colorStripDstTransRight.x == colorStripDstRect[move].x);
		quit = quit || event.quit() || event.checkKeyEventDown(CONTROL::CHOOSE);
	}
	ColorStripVelo.x = 0;
	colorStripDstRect[move].y = colorStripDstTransLeft.y;
	backgroundType = moving;
	cout << endl;
}

void Background::checkBgTransition(SDL_Rect& movingRect, SDL_Rect& original, SDL_Rect& move) {
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

void Background::renderFps()
{
	currentTime = SDL_GetTicks();
	frameCount++;
	if (currentTime > fpsTimer * 500) {
		fps = frameCount * 1000 / (currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		fpsTexture->loadFromRenderedText(renderer, to_string(fps), fpsColor, fpsFont);
		frameCount = 0;
		fpsTimer++;
	}
	fpsTexture->render(renderer, NULL, NULL);
}


//Button

Button::Button()
{
	button = nullptr;
	buttonDstRect = nullptr;
	usingButton = nullptr;
	buttonHover = nullptr;
	totalButton = 0;
}

//constructor
Button::Button(Window& screen)
{
	setRenderer(screen);
	button = nullptr;
	buttonDstRect = nullptr;
	usingButton = nullptr;
	buttonHover = nullptr;
	totalButton = nullptr;
}

//basic function

bool Button::load(const string& path) {
	if (!textureLoaded) {
		textureLoaded = true;

		button = new LButton[static_cast<int>(BUTTON::TOTAL)];
		usingButton = new bool[static_cast<int>(BUTTON::TOTAL)];
		buttonHover = new int(toInt(BUTTON::NOT_CHOOSE));
		totalButton = new int(toInt(BUTTON::TOTAL));

		for (int type = 0; type < *totalButton; type++) {
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
				case BUTTON::SCORE: {
					buttonPath += "scoreButton.png";
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
				case BUTTON::PRESS_KEY: {
					buttonPath += "pressKey.png";
					break;
				}
				default: {
					continue;
				}
			}
			textureLoaded = textureLoaded && button[type].loadFromFile(renderer, buttonPath);
			usingButton[type] = false;
		}

		if (textureLoaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Button created successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
		}
	}
	return textureLoaded;
}

void Button::render() {
	button[*buttonHover].hovered();
	for (int type = 0; type < *totalButton; type++) {
		if (usingButton[type]) {
			button[type].render(renderer, &buttonDstRect[type], NULL);
		}
	}
}

void Button::free() {
	for (int type = 0; type < *totalButton; type++) {
		button[type].free();
	}
	delete[] button;
	delete[] buttonDstRect;
	delete[] usingButton;
	delete buttonHover;
	delete totalButton;
	button = nullptr;
	buttonDstRect = nullptr;
	usingButton = nullptr;
	buttonHover = nullptr;
	totalButton = nullptr;

	cout << "Log [" << SDL_GetTicks() << "]: " << "Button Texture freed successfully" << endl;
}

//event function

void Button::buttonEnable(BUTTON type) {
	usingButton[toInt(type)] = true;
}

void Button::buttonDisable(BUTTON type)
{
	usingButton[toInt(type)] = false;
}

void Button::pressAnyKeyRender() {
	button[toInt(BUTTON::PRESS_KEY)].render(renderer, &buttonDstRect[toInt(BUTTON::PRESS_KEY)], NULL);
}

int Button::eventHandle(Event& event)
{
	if (event.e.type == SDL_KEYDOWN) {
		return keyEventHandle(event);
	}
	
	if (event.checkMouseEvent()) {
		return mouseEventHandle(event);
	}
	return *buttonHover;
}

int Button::mouseEventHandle(Event& event) {
	SDL_Point mousePos = event.getMousePos();

	for (int type = 0; type < *totalButton; type++) {
		bool hovering = usingButton[type] && SDL_PointInRect(&mousePos, &buttonDstRect[type]);
		if (hovering) {
			*buttonHover = type;
			return type;
		}
	}

	*buttonHover = toInt(BUTTON::NOT_CHOOSE);
	return *buttonHover;
}

int Button::keyEventHandle(Event& event)
{
	if (event.checkKeyEventDown(CONTROL::UP_ARROW) || event.checkKeyEventDown(CONTROL::DOWN_ARROW)) {
		do {
			if (event.checkKeyEventDown(CONTROL::UP_ARROW)) {
				(*buttonHover)--;
			}
			if (event.checkKeyEventDown(CONTROL::DOWN_ARROW)) {
				(*buttonHover)++;
			}
			*buttonHover = (*buttonHover + *totalButton) % *totalButton;
		} while (!usingButton[*buttonHover]);
	}
	return *buttonHover;
}



//Title

//Constructor

Title::Title(Window& screen)
{
	setRenderer(screen);
	title = nullptr;
	titleDstRect = nullptr;
	titleType = TITLE::MENU;
}

bool Title::load(const string& path)
{
	if (!textureLoaded) {
		textureLoaded = true;
		title = new LTexture[static_cast<int>(TITLE::TOTAL)];

		for (int type = 0; type < toInt(TITLE::TOTAL); type++) {
			string titlePath = path + "/Picture/Title/";
			switch (static_cast<TITLE> (type)) {
				case TITLE::INTRO: {
					titlePath += "SDLimage.png";
					break;
				}
				case TITLE::MENU: {
					titlePath += "menuTitle.png";
					break;
				}
				case TITLE::CHOOSE_MUSIC: {
					titlePath += "chooseMusicTitle.png";
					break;
				}
				case TITLE::GAME: {
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
				case TITLE::SCORE: {
					titlePath += "scoreTitle.png";
					break;
				}
				default: {
					continue;
				}
			}
			textureLoaded = textureLoaded && title[type].loadFromFile(renderer, titlePath);
		}
		title[toInt(TITLE::START)] = title[toInt(TITLE::MENU)];

		if (textureLoaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Title created successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: ";
			loadErrorLog();
		}
	}
	return textureLoaded;
}

void Title::render()
{
	if (titleType != TITLE::NO_TITLE) {
		title[toInt(titleType)].render(renderer, &titleDstRect[toInt(titleType)], NULL);
	}
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
	cout << "Log [" << SDL_GetTicks() << "]: " << "Title freed successfully" << endl;
}

void Title::setType(TITLE type)
{
	titleType = type;
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
	color = { 254, 205, 8 };

	font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit / 2);
	textureLoaded = (font != NULL);

	if (textureLoaded) {
		cout << "Log [" << SDL_GetTicks() << "]: " << "Music font loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: " << endl;
		loadErrorLog();
	}
	return textureLoaded;
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

	textureLoaded = nameText.loadFromRenderedText(renderer, "Name: " + name, color, font);
	textureLoaded = textureLoaded && authorText.loadFromRenderedText(renderer, "Author: " + singer, color, font);
	textureLoaded = textureLoaded && difficultyText.loadFromRenderedText(renderer, "Difficulty: " + to_string(velocity), color, font);
	textureLoaded = textureLoaded && bpmText.loadFromRenderedText(renderer, "BPM: " + to_string(bpm), color, font);

	if (textureLoaded) {
		createDefaultRect();
		cout << "Log [" << SDL_GetTicks() << "]: " << name << "'s info loaded successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
	}
	return textureLoaded;
}



//Point

//constructor

Point::Point(Window& screen) {
	setRenderer(screen);
	nameTexture = nullptr;
	pointTexture = nullptr;
	nameRect = nullptr;
	pointRect = nullptr;
	font = nullptr;

	playerName = "Player";
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
	if (!textureLoaded) {
		TTF_CloseFont(font);
		font = NULL;

		color = { 254, 205, 8 };
		font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit);

		nameTexture = new LButton;
		pointTexture = new LTexture[static_cast<int>(POINT::TOTAL)];
		textureLoaded = (font != nullptr);

		if (textureLoaded) {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Point font loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << endl;
			loadErrorLog();
		}
	}
	return textureLoaded;
}

bool Point::loadPlayerName() {
	bool success = nameTexture->loadFromRenderedText(renderer, ("Name: " + playerName), color, font);
	if (success) {
		delete nameRect;
		nameRect = new SDL_Rect;
		*nameRect = setTextRect(1, 4.0 / 3, nameTexture->getWidth(), nameTexture->getHeight());
	}
	return success;
}

bool Point::loadPointWindow(string& path) {
	TTF_CloseFont(font);

	accuracy = toInt(pressedArrow * 100.0 / totalArrow);

	color = { 250, 153, 28 };
	font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit * 1 / 2);
	textureLoaded = (font != NULL);

	textureLoaded = textureLoaded && loadPlayerName();
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
				renderText = "Accuracy: " + to_string(accuracy) + "%";
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
			default: {
				continue;
			}
		}
		textureLoaded = textureLoaded && pointTexture[type].loadFromRenderedText(renderer, renderText, color, font);
	}

	if (textureLoaded) {
		createDefaultRect();
		cout << "Log [" << SDL_GetTicks() << "]: " << "Point screen created successfully" << endl;
	}
	else {
		cout << "Log [" << SDL_GetTicks() << "]: ";
		loadErrorLog();
	}
	cout << endl;
	return textureLoaded;
}

void Point::render() {
	if (!textureLoaded) {
		textureLoaded = pointTexture[toInt(POINT::POINT)].loadFromRenderedText(renderer, ("Point: " + to_string(point)), color, font);
		if (combo > 1) {
			textureLoaded = pointTexture[toInt(POINT::MAX_COMBO)].loadFromRenderedText(renderer, ("Combo: " + to_string(combo)), color, font);
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
	nameTexture->free();
	for (int type = 0; type < toInt(POINT::TOTAL); type++) {
		pointTexture[type].free();
	}

	TTF_CloseFont(font);
	delete nameTexture;
	delete[] pointTexture;
	delete nameRect;
	delete[] pointRect;
	nameTexture = nullptr;
	pointTexture = nullptr;
	nameRect = nullptr;
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
		int newPoint = 2;
		if (accuracy <= 35) {
			newPoint += 3;
			if (accuracy <= 25) {
				newPoint += 2;
				if (accuracy <= 15) {
					newPoint += 3;
				}
			}
		}
		point += newPoint;
		if (combo > 20) {
			point += newPoint;
			if (combo > 30) {
				point += newPoint;
				if (combo > 40) {
					point += newPoint;
					if (combo > 50) {
						point += newPoint;
					}
				}
			}
		}
		textureLoaded = false;
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
	textureLoaded = false;
}

void Point::wrongPressCount() {
	wrongPressed++;
	maxCombo = max(combo, maxCombo);
	combo = 0;
	textureLoaded = false;
}

int Point::getCombo()
{
	return combo;
}

BUTTON Point::eventHandle(Event& event) {
	SDL_Point mousePos = event.getMousePos();
	Uint8 mouseButton = event.getMouseButton();
	if (SDL_PointInRect(&mousePos, nameRect)) {
		nameTexture->hovered();
		if (mouseButton != 0) {
			nameTexture->clicked();
			return BUTTON::PLAY;
		}
	}
	return BUTTON::NOT_CHOOSE;
}

void Point::nameEdit(Event& event)
{
	event.checkInputText(playerName);
	while (playerName.length() > MAX_NAME_LENGTH) {
		playerName.pop_back();
	}
	loadPlayerName();
	nameTexture->hovered();
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
	if (!textureLoaded) {

		textureLoaded = blankArrow.loadFromFile(renderer, path + "/Picture/Arrow/blankArrow.png");
		textureLoaded = textureLoaded && arrow.loadFromFile(renderer, path + "/Picture/Arrow/arrow.png");
		textureLoaded = textureLoaded && pressedArrow.loadFromFile(renderer, path + "/Picture/Arrow/pressedArrowVietCong.png");

		if (textureLoaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Arrow Texture loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << endl;
			loadErrorLog();
		}
	}
	return textureLoaded;
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
	velocity = song.getVelocity();
	Uint32 time = SDL_GetTicks();
	spawnRate = static_cast<Uint32>(round(60000.0 / song.getBpm()));
	int currentVelo = toInt(velocity * *screenHeight);
	nextSpawnTime = time + song.getStart() - *screenHeight * 1000 / currentVelo;
	spawnDuration = time + song.getDuration() - *screenHeight * 1000 / currentVelo;
	song.playMusic(0);
	cout << "Log [" << SDL_GetTicks() << "]: " << "Game starting time: " << time << endl;
	cout << endl;
}

void ArrowTexture::addPauseTime(Uint32 time) {
	pauseTime += time;
}

//add new arrow to arrow range
void ArrowTexture::addArrow(Uint32& time) {
	gameTime = time - pauseTime;
	if (gameTime > nextSpawnTime && gameTime < spawnDuration) {
		int arrowCol = rand() % 4;
		int x = *screenUnit * (1 + arrowCol * 2);
		int y = toInt(*screenHeight - velocity * (gameTime - nextSpawnTime) / 1000);
		SDL_Point newArrow = { x, y };
		arrowRange.push_back(newArrow);
		nextSpawnTime += spawnRate;
	}
}

void ArrowTexture::scoreCheck(int& keyType, Point& point) {
	if (!arrowRange.empty()) {
		int arrNumb = 0;
		while (arrNumb < arrowRange.size()) {
			if (arrowRange[arrNumb].y <= *screenUnit * 2) {
				int arrowCol = (arrowRange[arrNumb].x / *screenUnit - 1) / 2;
				if (arrowCol == keyType) {
					int accuracy = abs(arrowRange[arrNumb].y * 50 / *screenUnit - 50);
					if (point.addPoint(accuracy)) {
						SDL_Rect renderRect = { arrowRange[arrNumb].x, arrowRange[arrNumb].y, *screenUnit, *screenUnit };
						arrow.render(renderer, &renderRect, &arrowSrcRect[arrowCol]);
						arrowRange.erase(arrowRange.begin() + arrNumb);
						return;
					}
				}
				arrNumb++;
			}
			else {
				break;
			}
		}
		point.wrongPressCount();
	}
}

void ArrowTexture::updateArrowRange(Point& point, Uint32 timePassed) {
	int currentVelo = toInt(velocity * *screenHeight);
	for (int i = 0; i < arrowRange.size(); i++) {
		if (arrowRange[i].y <= -*screenUnit) {
			arrowRange.erase(arrowRange.begin() + i);
			point.missedArrowCount();
		}
		else {
			arrowRange[i].y -= ( currentVelo * timePassed / 1000 );
		}
	}
}



//Character

//Constructor

Character::Character(Window& screen)
{
	setRenderer(screen);
	bonk = nullptr;
	muscleDogeSrcRect = nullptr;
	muscleDogeDstRect = nullptr;
	cheemsSrcRect = nullptr;
	cheemsDstRect = nullptr;
	khaBankSrcRect = nullptr;
	khaBankDstRect = nullptr;
	frame = 0;
	bankVelo = 0;
	bankRender = false;
}

//Basic functions

bool Character::load(const string& path) {
	if (!textureLoaded) {
		Mix_FreeChunk(bonk);
		string correctPath = path + "/Picture/Character/";

		textureLoaded = muscleDoge.loadFromFile(renderer, correctPath + "muscleDoge.png");
		textureLoaded = smashingDoge.loadFromFile(renderer, correctPath + "smashingDoge.png");
		textureLoaded = textureLoaded && cheems.loadFromFile(renderer, correctPath + "cheems.png");
		textureLoaded = textureLoaded && smashedCheems.loadFromFile(renderer, correctPath + "smashedCheems.png");
		textureLoaded = textureLoaded && khaBank.loadFromFile(renderer, correctPath + "khaBank.png");

		bonk = Mix_LoadWAV((path + "/Music/bonk.wav").c_str());
		textureLoaded = textureLoaded && (bonk != NULL);

		if (textureLoaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Character Texture loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << endl;
			loadErrorLog();
		}
	}
	return textureLoaded;
}

void Character::render()
{
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::LEFT_ARROW)], &cheemsSrcRect[toInt(CONTROL::LEFT_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::RIGHT_ARROW)], &cheemsSrcRect[toInt(CONTROL::RIGHT_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::UP_ARROW)], &cheemsSrcRect[toInt(CONTROL::UP_ARROW)]);
	cheems.render(renderer, &cheemsDstRect[toInt(CONTROL::DOWN_ARROW)], &cheemsSrcRect[toInt(CONTROL::DOWN_ARROW)]);
}

void Character::free() {
	muscleDoge.free();
	smashingDoge.free();
	cheems.free();
	smashedCheems.free();
	khaBank.free();
	Mix_FreeChunk(bonk);
	bonk = nullptr;

	delete muscleDogeSrcRect;
	muscleDogeSrcRect = nullptr;
	delete[] muscleDogeDstRect;
	muscleDogeDstRect = nullptr;
	delete[] cheemsSrcRect;
	cheemsSrcRect = nullptr;
	delete[] cheemsDstRect;
	cheemsDstRect = nullptr;
	delete[] khaBankSrcRect;
	khaBankSrcRect = nullptr;
	delete[] khaBankDstRect;
	khaBankDstRect = nullptr;

	cout << "Log [" << SDL_GetTicks() << "]: " << "Character Texture freed successfully" << endl;
}

//play sound

void Character::playBonkSound() {
	if (Mix_PlayingMusic() == 0) {
		Mix_HaltChannel(-1);
		Mix_PlayChannel(0, bonk, 0);
	}
}



//Setting Texture

//constructor
SettingTexture::SettingTexture(Window& screen, Event& event, Button& button)
{
	setRenderer(screen);
	scanControl = new SDL_Scancode[static_cast<int>(CONTROL::TOTAL)];
	keyControl = new SDL_Keycode[static_cast<int>(CONTROL::TOTAL)];
	cout << scanControl << endl;
	passEventControl(&event);
	font = nullptr;
	color = { 255, 255, 255 };
	totalButton = button.totalButton;
	buttonHover = button.buttonHover;
	usingButton = button.usingButton;
}

//basic functions
bool SettingTexture::load(const string& path)
{
	if (!textureLoaded) {
		if (font == nullptr) {
			font = TTF_OpenFont((path + "/GEDEBOOG.ttf").c_str(), *screenUnit / 2);
			color = { 254, 205, 8 };
			textureLoaded = (font != nullptr);
			button = new LButton[static_cast<int>(CONTROL::TOTAL)];
		}
		else {
			textureLoaded = true;
		}

		for (int type = 0; type < *totalButton; type++) {
			string renderString;
			switch (static_cast<CONTROL>(type)) {
				case CONTROL::LEFT_ARROW: {
					renderString = "Left arrow: ";
					break;
				}
				case CONTROL::UP_ARROW: {
					renderString = "Up arrow: ";
					break;
				}
				case CONTROL::DOWN_ARROW: {
					renderString = "Down arrow: ";
					break;
				}
				case CONTROL::RIGHT_ARROW: {
					renderString = "Right arrow: ";
					break;
				}
				case CONTROL::CHOOSE: {
					renderString = "Choose button: ";
					break;
				}
				case CONTROL::ESCAPE: {
					renderString = "Escape button: ";
					break;
				}
				default: {
					continue;
				}
			}
			renderString.append(SDL_GetKeyName(keyControl[type]));
			textureLoaded = button[type].loadFromRenderedText(renderer, renderString, color, font) && textureLoaded;
		}

		if (textureLoaded) {
			createDefaultRect();
			cout << "Log [" << SDL_GetTicks() << "]: " << "Setting Texture loaded successfully" << endl;
		}
		else {
			cout << "Log [" << SDL_GetTicks() << "]: " << "Failed to load Setting Texture" << endl;
			loadErrorLog();
		}
	}
	return textureLoaded;
}

void SettingTexture::render()
{
	button[*buttonHover].hovered();
	for (int type = 0; type < toInt(CONTROL::TOTAL); type++) {
		button[type].render(renderer, &buttonDstRect[type], NULL);
	}
}

void SettingTexture::free()
{
	for (int type = 0; type < toInt(CONTROL::TOTAL); type++) {
		button[type].free();
	}
	delete[] button;
	delete[] buttonDstRect;
	delete[] keyControl;
	delete[] scanControl;
	button = nullptr;
	buttonDstRect = nullptr;
	keyControl = nullptr;
	scanControl = nullptr;

	TTF_CloseFont(font);
	font = nullptr;

	cout << "Log [" << SDL_GetTicks() << "]: " << "Setting Texture freed successfully" << endl;
}

void SettingTexture::hovered(CONTROL key)
{
	button[toInt(key)].hovered();
}

bool SettingTexture::changeKey(CONTROL changingKey, Event& event) {
	event.updateEvent();
	textureLoaded = false;
	if (event.e.type == SDL_KEYDOWN) {
		SDL_Keycode keyChange = event.e.key.keysym.sym;
		for (int type = 0; type < toInt(CONTROL::TOTAL); type++) {
			if (keyChange == keyControl[type]) {
				return false;
			}
		}
		keyControl[toInt(changingKey)] = keyChange;
		scanControl[toInt(changingKey)] = SDL_GetScancodeFromKey(keyChange);
		return true;
	}
	return false;
}
