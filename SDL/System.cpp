#include "System.h"

int System::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL_INIT failed: %s\n", SDL_GetError());
		return 0;
	}

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image failed: %s\n", IMG_GetError());
		return 0;
	}

	if (TTF_Init() == 1)
	{
		printf("SDL_ttf failed: %s\n", TTF_GetError());
	}
	SDL_StartTextInput();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer failed: %s\n", Mix_GetError());
		return 0;
	}

	return 1;
}

void System::close()
{
	SDL_StopTextInput();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}