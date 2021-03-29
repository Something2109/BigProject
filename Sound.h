#ifndef _Sound_h
#define _Sound_h
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

class Music {
	Mix_Music* source = NULL;
	string name;
	string singer;
	int spawnRate = 0;
	int velocity = 0;
public: 
	bool loadFromFile(string path) {
		bool success = true;
		string fileName = name + " -" + singer;
		source = Mix_LoadMUS((path + "/Music/" + fileName +".mp3").c_str());
		if (source == NULL) {
			success = false;
		}
		return success;
	}
	
	void loadMusicInfo(string _name, string _singer, int _spawnRate, int _velocity, const int &screenUnit) {
		name = _name;
		singer = _singer;
		spawnRate = _spawnRate;
		velocity = screenUnit / _velocity;
	}

	void playMusic(int loop) {
		Mix_PlayMusic(source, loop);
	}

	void freeMusicLoad() {
		Mix_FreeMusic(source);
		source = NULL;
	}

	~Music() {
		freeMusicLoad();
		name.erase();
		singer.erase();
		spawnRate = 0;
		velocity = 0;
	}

	int getSpawnRate() {
		return spawnRate;
	}

	int getVelocity() {
		return velocity;
	}
};

void playBonkSound(Mix_Chunk* bonk) {
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayChannel(0, bonk, 0);
	}
}

#endif