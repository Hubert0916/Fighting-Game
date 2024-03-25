#pragma once
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include <vector>
#include "Image.h"
#include "Mixer.h"
#include "constants.h"
#include "Beam.h"
#include "Green.h"
#include "Red.h"
#include "Blue.h"
#include "Kecleon.h"
using namespace std;

class Link
{
	public:
		Link(const char* p, SDL_Renderer* rr, int nn);
		void setPosition(int xx, int yy);
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		void draw();
		void close();
		void startTimer(Uint32 t);
		void stopTimer();
		void setSpeed(int s);
		void setv(int xxv, int yyv);
		void movinghandle(SDL_Event& e, Link& role);
		void setHealth(int);
		int getHealth();
		bool hitbox(int ea, int eb, double touchx, double touchy); // ea,eb are param of hitbox, touchx,y is param of earliest point
		double* caltouchxy(int ex, int ey, double ra, double rb); //calculate the earliest point between enemy and character
		void AtHitbox(int ea, int eb, double touchx, double touchy);
		bool getHit();
		void setSlowStart(int);
		void remainSlow(int);
		void setTouch(bool);
		void setUntouchStart(int);
		void remainUntouch(int);
		void setFreezeStart(int);
		void remainFreeze(int);
		template <class T>
		void collide(T&);

	private:
		int x;
		int y;
		int alpha;
		int num; 
		int frame;
		int mousex;
		int mousey; 
		int direction;
		int currenthealth;
		int xv;
		int yv;
		int speed;
		int untouchStart;
		int slowStart;
		int freezeStart;
		char* path;
		int width;
		int height;
		bool farattack;
		bool attack;
		bool hit;
		bool slow;
		bool freeze;
		bool touch;
		vector<Beam> beam;
		Image turtle;
		Image ice;
		Mixer sword;
		Mixer fuck;
		Mixer tackle;
		Mixer hitEffect;
		Mixer wave;
		SDL_Renderer* renderer;
		SDL_TimerID timer;
		Image* image;
		Uint32 time;
		static Uint32 displayattack(Uint32 interval, void* param);
};

