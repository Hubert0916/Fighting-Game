#pragma once
#include <SDL_mixer.h>

class Mixer
{
	public:
		Mixer();
		Mixer(const char* p, int t);
		void close();
		bool load();
		void setPath(const char* p);
		void setType(int t);

		bool isPlaying();
		void play(int n);
		bool isPaused();
		void resume();
		void pause();
		void stop();
		void playEffect(int n);
		void playEffect(int n, int c);
		void stopEffect(int n);

	private:
		Mix_Music* music;
		Mix_Chunk* effect;

	char* path;
	int type;
};

