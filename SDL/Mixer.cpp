#include "Mixer.h"
#include <stdio.h>
#include <string.h>
#include "constants.h"

Mixer::Mixer()
{
}

Mixer::Mixer(const char* p, int t)
{
	setPath(p);
	setType(t);
	load();
}

void Mixer::close()
{
	if(type == EFFECT)
		Mix_FreeChunk(effect);

	if (type == MUSIC)
		Mix_FreeMusic(music);
}

bool Mixer::load()
{
	if (type == MUSIC)
	{
		music = Mix_LoadMUS(path);
		if (music == NULL)
		{
			printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
			return false;
		}
	}

	if (type == EFFECT)
	{
		effect = Mix_LoadWAV(path);
		if (effect == NULL)
		{
			printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			return false;
		}
	}

	return true;
}

void Mixer::setPath(const char* p)
{
	path = new char[strlen(p) + 1];
	strcpy_s(path, strlen(p) + 1, p);
}

void Mixer::setType(int t)
{
	type = t;
}

bool Mixer::isPlaying()
{
	return Mix_PlayingMusic();
}

void Mixer::play(int n)
{
	Mix_PlayMusic(music, n);
}

bool Mixer::isPaused()
{
	return Mix_PausedMusic();
}

void Mixer::resume()
{
	Mix_ResumeMusic();
}

void Mixer::pause()
{
	Mix_PauseMusic();
}

void Mixer::stop()
{
	Mix_HaltMusic();
}

void Mixer::playEffect(int n)
{
	Mix_PlayChannel(-1, effect, n);
}

void Mixer::playEffect(int n, int c)
{
	Mix_PlayChannel(c, effect, n);
}

void Mixer::stopEffect(int n)
{
	Mix_HaltChannel(n);
}