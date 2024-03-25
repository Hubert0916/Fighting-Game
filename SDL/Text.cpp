#include "Text.h"
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "constants.h"


Text::Text()
{
}

Text::Text(const char* str, const char* fontPath, int fontSize, int style, SDL_Color fgColor, int textType, SDL_Color bgColor, SDL_Renderer* renderer, SDL_Point pos, SDL_Point center, double angle, SDL_RendererFlip flip, int alpha)
{
	setString(str);
	setFontPath(fontPath);
	setFontSize(fontSize);
	setStyle(style);
	setFgColor(fgColor);
	setTextType(textType);
	setBgColor(bgColor);
	setRenderer(renderer);

	generateTexture();

	setPos(pos);
	setCenterAngle(center, angle);
	setFlip(flip);
	setAlpha(alpha);
}

void Text::generateTexture()
{
	TTF_Font* ttfFont = NULL;

	ttfFont = TTF_OpenFont(fontPath, fontSize);

	if (ttfFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		exit(0);
	}

	TTF_SetFontStyle(ttfFont, style);

	SDL_Surface* surface = NULL;

	if (textType == SOLID)
	{
		surface = TTF_RenderText_Solid(ttfFont, str, fgColor);
	}
	else if (textType == SHADED)
	{
		surface = TTF_RenderText_Shaded(ttfFont, str, fgColor, bgColor);
	}
	else if (textType == BLENDED)
	{
		surface = TTF_RenderUTF8_Blended(ttfFont, str, fgColor);
	}

	TTF_CloseFont(ttfFont);

	if (surface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		exit(0);
	}
	else
	{
		SDL_DestroyTexture(texture);

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
			exit(0);
		}

		width = surface->w;
		height = surface->h;

		SDL_FreeSurface(surface);
	}
}

void Text::close()
{	
	SDL_DestroyTexture(texture);
}

void Text::draw()
{
	SDL_Rect d;
	d.x = pos.x;
	d.y = pos.y;
	d.w = width;
	d.h = height;

	if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return;
	}

	if (SDL_SetTextureAlphaMod(texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return;
	}

	SDL_RenderCopyEx(renderer, texture, NULL, &d, angle, &center, flip);
}

void Text::draw(SDL_Renderer* ren, SDL_Point pos, SDL_Point center, double angle, SDL_RendererFlip flip, int alpha)
{
	setRenderer(ren);
	setPos(pos);
	setCenterAngle(center, angle);
	setFlip(flip);
	setAlpha(alpha);

	draw();
}

int Text::getWidth()
{
	return width;
}

int Text::getHeight()
{
	return height;
}

void Text::setString(const char* s)
{
	if(str)
		delete str;
	str = new char[strlen(s) + 1];
	strcpy_s(str, strlen(s) + 1, s);
}

void Text::setFontPath(const char* f)
{
	fontPath = new char[strlen(f) + 1];
	strcpy_s(fontPath, strlen(f) + 1, f);
}

void Text::setFontSize(int s)
{
	fontSize = s;
}

void Text::setStyle(int s)
{
	style = s;
}

void Text::setFgColor(SDL_Color fc)
{
	fgColor = fc;
}

void Text::setTextType(int t)
{
	textType = t;
}

void Text::setBgColor(SDL_Color bc)
{
	bgColor = bc;
}

void Text::setRenderer(SDL_Renderer* r)
{
	renderer = r;
}

void Text::setPos(SDL_Point p)
{
	pos = p;
}

void Text::setCenterAngle(SDL_Point c, double a)
{
	center = c;
	angle = a;
}

void Text::setFlip(SDL_RendererFlip f)
{
	flip = f;
}

void Text::setAlpha(int a)
{
	alpha = a;
}

char* Text::getString()
{
	return str;
}