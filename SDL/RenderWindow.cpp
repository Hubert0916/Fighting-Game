#include <stdio.h>
#include "RenderWindow.h"

RenderWindow::RenderWindow(const char* caption, int winx, int winy, int width, int height)
{
	window = SDL_CreateWindow(caption, winx, winy, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
	}

	printf("SDL system initializes susccessfully!!!\n");
}

void RenderWindow::close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


void RenderWindow::clear()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);
}

void RenderWindow::clear(Uint32 color)
{
	SDL_SetRenderDrawColor(renderer, color/0x1000000, color/0x10000%0x100, color/0x100%0x100, color%0x100);
	SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* RenderWindow::getRenderer()
{
	return renderer;
}