#pragma once
#include <SDL.h>

class Image
{
	public:
		Image();
		Image(const char* pp, SDL_Renderer* rr);
		Image(const char* pp, SDL_Renderer* rr, Uint8 r, Uint8 g, Uint8 b);
		void generateTexture();
		void close();
		void draw();
		void draw(SDL_Renderer*, SDL_Rect dst, SDL_Rect src);

		int getWidth();
		int getHeight();
		void setPath(const char* p);
		void setRenderer(SDL_Renderer* r);
		void setScrRegion(SDL_Rect s);
		void setDstRegion(SDL_Rect d);
		void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void setFlip(SDL_RendererFlip f);
		void setAlpha(int);
		int getAlpha();
		void setAngle(int);
		int getAngle();

	private:
		char* path;
		SDL_Texture* texture;
		int width;
		int height;
		SDL_Renderer* renderer;
		SDL_Rect scrRegion;
		SDL_Rect dstRegion;
		SDL_Color color;
		SDL_RendererFlip flip;
		int alpha;
		int angle;
};