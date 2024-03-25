#pragma once
#include "Ball.h"
#include "Red.h"
#include "Green.h"
#include "Blue.h"
#include "Kecleon.h"
#include "Image.h"
#include "Mixer.h"
#include "Green.h"
#include "Red.h"
#include "Blue.h"
#include "Kecleon.h"

class Beam :
    public Ball
{
	public:
		Beam();
		Beam(const char*, SDL_Renderer*, int, int, int);
		void draw();
		void close();
		template <class T>
		void collide(T&);
	private:
		Mixer farhit;
};

