#pragma once
#include "constants.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Mixer.h"
#include "Image.h"

class Enemy
{
	public:
		Enemy();
		Enemy(SDL_Renderer*, int, int, int, int);
		void setPosition(int xx, int yy);
		void startTimer(Uint32 t);
		void stopTimer();
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		void setrange(int ra, int rb);
		int* getrange();
		void draw();
		void setHealth(int hp);
		int getHealth();
		void damaged(bool hit);
		void setSpeed(int s);
		int getSpeed();
		void setAngle(int);
		int getAngle();
		void setAlpha(int);
		int getAlpha();
		void close();
		~Enemy();

	protected:
		SDL_Renderer* renderer;
		SDL_TimerID timer;
		Uint32 time;
		Image *image;
		int currenthealth;
		int maxhealth;
		int speed;
		int x;
		int y;
		int rangea;
		int rangeb;
		int alpha;
		int angle;
		static Uint32 RoutineMotion(Uint32 interval, void* param);
};

