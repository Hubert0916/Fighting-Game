#include "Image.h"
#include <stdio.h>
#include <string.h>
#include <SDL_image.h>
#include "constants.h"

Image::Image()
{
	setColor(0, 0, 0, 0);
	setAlpha(255);
	setAngle(0);
	setFlip(SDL_FLIP_NONE);
}

Image::Image(const char* pp, SDL_Renderer* rr)
{
	setPath(pp);
	setRenderer(rr);
	setColor(0, 0, 0, 0);
	generateTexture();
	setAlpha(255);
	setFlip(SDL_FLIP_NONE);
}

Image::Image(const char* pp, SDL_Renderer* rr, Uint8 r, Uint8 g, Uint8 b)
{
	setPath(pp);
	setRenderer(rr);
	setColor(r, g, b, 255);
	generateTexture();
	setAlpha(255);
	setFlip(SDL_FLIP_NONE);
}

void Image::generateTexture()
{
	SDL_Surface* imgsurface = IMG_Load(path);
	if (imgsurface == NULL)
	{
		printf("IMG_Load failed: %s\n", IMG_GetError());
		exit(0);
	}
	else
	{
		if (color.a == 255)
		{
			SDL_SetColorKey(imgsurface, true, SDL_MapRGB(imgsurface->format, color.r, color.g, color.b));
		}
		texture = SDL_CreateTextureFromSurface(renderer, imgsurface);
		if (texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failec: %s\n", SDL_GetError());
			exit(0);
		}

		width = imgsurface->w;
		height = imgsurface->h;
		SDL_FreeSurface(imgsurface);
	}
}

void Image::close()
{
	SDL_DestroyTexture(texture);
}

void Image::draw()
{
	SDL_Rect* s = &scrRegion, * d = &dstRegion;
	if (scrRegion.x == ALLREGION)
	{
		s = NULL;
	}
	if (dstRegion.x == ALLREGION)
	{
		d = NULL;
	}

	//set and enable standard alpha blending mode for a texture
	if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return;
	}
	//alpha moad
	if (SDL_SetTextureAlphaMod(texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s", SDL_GetError());
		return;
	}

	SDL_RenderCopyEx(renderer, texture, s, d, angle, NULL, flip);
}

void Image::draw(SDL_Renderer* rr, SDL_Rect dst, SDL_Rect src)
{
	setDstRegion(dst);
	setScrRegion(src);
	draw();
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

void Image::setPath(const char* p)
{
	path = new char[strlen(p) + 1];
	strcpy_s(path, strlen(p) + 1, p);
}

void Image::setRenderer(SDL_Renderer* r)
{
	renderer = r;
}

void Image::setScrRegion(SDL_Rect s)
{
	scrRegion = s;
}

void Image::setDstRegion(SDL_Rect d)
{
	dstRegion = d;
}

void Image::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void Image::setFlip(SDL_RendererFlip f)
{
	flip = f;
}

void Image::setAlpha(int a)
{
	alpha = a;
}

int Image::getAlpha()
{
	return alpha;
}

void Image::setAngle(int a)
{
	angle = a;
}

int Image::getAngle()
{
	return angle;
}
