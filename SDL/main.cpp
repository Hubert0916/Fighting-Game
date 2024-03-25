#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <time.h>
#include "System.h"
#include "RenderWindow.h"
#include "image.h"
#include "Text.h"
#include "Mixer.h"
#include "constants.h"
#include "Link.h"
#include "Enemy.h"
#include "Green.h"
#include "Red.h"
#include "Blue.h"
#include "Kecleon.h"
#include "Ball.h"
#include "Beam.h"

int main(int argc, char* args[])
{
	System sdl;

	if (!sdl.init()) 
	{
		printf("SDL initailize failed in main funciton\n");
		return -1;
	}
	RenderWindow window("oop test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT);
	SDL_Event e;
	bool quit = false;
	int now_fire = time(NULL), now_ice = time(NULL), t_fire, t_ice;
	srand(time(NULL));
	double* touchxy;
	Link link("../image/link/", window.getRenderer(), 14);
	Green green("../image/grimer/Grimer-", 79, window.getRenderer(), 30, 30, 3, 1000);
	Blue blue("../image/Blue/Snover-", 90, window.getRenderer(), 30, 30, 1, 800);
	Image hp("../image/heart.png", window.getRenderer());
	Mixer lowHP("../audio/lowHP.wav", MUSIC);
	Text hp_num("10", "../fonts/lazy.ttf", 50, TTF_STYLE_BOLD, { 255, 255, 255 }, BLENDED, { 0, 0, 0 }, window.getRenderer(), { 90, 15 }, { NULL, NULL }, 0, SDL_FLIP_NONE, 255);
	Red Slugma("../image/Slugma/Slugma-", 60, window.getRenderer(), 30, 40, 1, 800);
	Kecleon kecleon("../image/Kecleon/Kecleon-", 59, window.getRenderer(), 30, 25, 2, 500);
	vector<Ball> fire;
	vector<Ball> ice;

	int linkHitboxX = link.getWidth() / 5, linkHitboxY = link.getHeight() / 2;
	link.setPosition(WIDTH / 4, HEIGHT / 2);
	green.setPosition(0, HEIGHT / 2);
	green.startmotion(50);
	green.starttrace(50);
	Slugma.setPosition(800, 600);
	Slugma.startTimer(10);
	blue.setPosition(400, 100);
	blue.startTimer(20);
	kecleon.setPosition(WIDTH / 6, HEIGHT / 3);
	kecleon.startmotion(50);
	kecleon.starttrace(50);
	kecleon.startinvisible(1500);
	
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;

			if (link.getHealth() > 0)
				link.movinghandle(e, link);
		}
		
		window.clear();

		if (Slugma.getHealth() > 0)
		{
			touchxy = link.caltouchxy(Slugma.getX() + Slugma.getWidth() / 2, Slugma.getY() + Slugma.getHeight() / 2, Slugma.getrange()[0], Slugma.getrange()[1]); // enemy x, y; range a, range b
			if (link.hitbox(linkHitboxX, linkHitboxY, touchxy[0], touchxy[1]))
				link.setUntouchStart(time(NULL));
			link.AtHitbox(link.getWidth(), link.getHeight() / 2, touchxy[0], touchxy[1]);
			link.collide(Slugma);
			Slugma.damaged(link.getHit());
		}

		if (blue.getHealth() > 0)
		{
			touchxy = link.caltouchxy(blue.getX() + blue.getWidth() / 2, blue.getY() + blue.getHeight() / 2, blue.getrange()[0], blue.getrange()[1]); // enemy x, y; range a, range b
			if (link.hitbox(linkHitboxX, linkHitboxY, touchxy[0], touchxy[1]))
				link.setUntouchStart(time(NULL));
			link.AtHitbox(link.getWidth(), link.getHeight() / 2, touchxy[0], touchxy[1]);
			link.collide(blue);
			blue.damaged(link.getHit());
		}

		for (int i = 0; i < fire.size(); i++)
		{
			if (fire[i].getHealth() > 0)
			{
				if (fire.size() > 1)
					for (int j = i + 1; j < fire.size(); j++)
						fire[i].Colide(fire[j]);

				touchxy = link.caltouchxy(fire[i].getX(), fire[i].getY(), fire[i].getrange()[0], fire[i].getrange()[0]);
				if (link.hitbox(linkHitboxX, linkHitboxY, touchxy[0], touchxy[1]))
				{
					fire[i].setHealth(0);
					link.setUntouchStart(time(NULL));
				}
				link.AtHitbox(link.getWidth(), link.getHeight() / 2, touchxy[0], touchxy[1]);
				fire[i].damaged(link.getHit());
			}
		}

		for (int i = 0; i < ice.size(); i++)
		{
			if (ice[i].getHealth() > 0)
			{
				if (ice.size() > 1)
					for (int j = i + 1; j < ice.size(); j++)
						ice[i].Colide(ice[j]);

				touchxy = link.caltouchxy(ice[i].getX(), ice[i].getY(), ice[i].getrange()[0], ice[i].getrange()[0]);
				if (link.hitbox(linkHitboxX, linkHitboxY, touchxy[0], touchxy[1]))
				{
					ice[i].setHealth(0);
					link.setFreezeStart(time(NULL));
					link.setUntouchStart(time(NULL));
				}
				link.AtHitbox(link.getWidth(), link.getHeight() / 2, touchxy[0], touchxy[1]);
				ice[i].damaged(link.getHit());
			}
		}

		if (green.getHealth() > 0 && link.getHealth() > 0)
		{
			touchxy = link.caltouchxy(green.getX() + green.getWidth() / 2, green.getY() + green.getHeight() / 2, green.getrange()[0], green.getrange()[1]);
			if (link.hitbox(linkHitboxX, linkHitboxY, touchxy[0], touchxy[1]))
			{
				link.setSlowStart(time(NULL));
				link.setUntouchStart(time(NULL));
			}
			link.AtHitbox(link.getWidth(), link.getHeight() / 2, touchxy[0], touchxy[1]);
			link.collide(green);
			green.damaged(link.getHit());
			green.getLinkPos(link.getX() + link.getWidth() / 2, link.getY() + link.getHeight() / 2);
		}
		else
			green.stoptrace();

		if (kecleon.getHealth() > 0 && link.getHealth() > 0)
		{
			touchxy = link.caltouchxy(kecleon.getX() + kecleon.getWidth() / 2, kecleon.getY() + kecleon.getHeight() / 2, kecleon.getrange()[0], green.getrange()[1]);
			if (link.hitbox(linkHitboxX, linkHitboxY, touchxy[0], touchxy[1]))
				link.setUntouchStart(time(NULL));
			link.AtHitbox(link.getWidth(), link.getHeight() / 2, touchxy[0], touchxy[1]);
			link.collide(kecleon);
			kecleon.damaged(link.getHit());
			kecleon.getLinkPos(link.getX() + link.getWidth() / 2, link.getY() + link.getHeight() / 2);
		}
		else
			kecleon.stoptrace();

		link.remainFreeze(time(NULL));
		link.remainSlow(time(NULL));
		link.remainUntouch(time(NULL));
		
		if (link.getHealth() == 1 && !lowHP.isPlaying())
			lowHP.play(0);
		if (link.getHealth() == 0)
			lowHP.stop();

		int h = link.getHealth();
		char currentHP[10] = "x ",  s[10] = "\0";
		if (h)
		{
			for (int i = 0; h; i++, h /= 10)
				s[i] = h % 10 + '0';
			for (int i = 0; i < strlen(s) / 2; i++)
			{
				char tmp = s[i];
				s[i] = s[strlen(s) - i - 1];
				s[strlen(s) - i - 1] = tmp;
			}
			strncat_s(currentHP, s, strlen(s));
		}
		else
			strncpy_s(currentHP, "x 0", 3);
		hp_num.setString(currentHP);
		hp_num.generateTexture();

		t_fire = time(NULL);
		if (t_fire - now_fire > 3 && Slugma.getHealth() > 0 && fire.size() < 4)
		{
			now_fire = t_fire;
			fire.push_back(Ball(window.getRenderer(), 15, 40, 1, 245, 84 ,30, 'f'));
			fire.back().setSpeedXY(int(pow(-1, rand() %2)*(rand() % 3 + 1)), int(pow(-1, rand() % 2) * (rand() % 3 + 1)));
			fire.back().setPosition(Slugma.getX() + Slugma.getWidth() / 2, Slugma.getY() + Slugma.getHeight() / 2);
		}

		t_ice = time(NULL);
		if (t_ice - now_ice > 3 && blue.getHealth() > 0 && ice.size() < 2)
		{
			now_ice = t_ice;
			ice.push_back(Ball(window.getRenderer(), 15, 40, 1, 75, 215, 250, 'i'));
			ice.back().setSpeedXY(int(pow(-1, rand() % 2) * (rand() % 3 + 1)), int(pow(-1, rand() % 2) * (rand() % 3 + 1)));
			ice.back().setPosition(blue.getX() + blue.getWidth() / 2, blue.getY() + blue.getHeight() / 2);
		}
		
		link.draw();
		green.draw();
		blue.draw();
		kecleon.draw();
		Slugma.draw();
		for (int i = 0; i < fire.size(); i++)
		{
			if (Slugma.getHealth() <= 0)
			{
				for (int j = 0; j < fire.size(); j++)
					fire[j].close();
				fire.clear();
			}
			else if (fire[i].getHealth() > 0)
				fire[i].draw();
			else
			{
				fire[i].close();
				fire.erase(fire.begin() + i);
			}
		}
		for (int i = 0; i < ice.size(); i++)
		{
			if (blue.getHealth() <= 0)
			{
				for (int j = 0; j < ice.size(); j++)
					ice[j].close();
				ice.clear();
			}
			else if (ice[i].getHealth() > 0)
				ice[i].draw();
			else
			{
				ice[i].close();
				ice.erase(ice.begin() + i);
			}
		}
		hp_num.draw();
		hp.draw(window.getRenderer(), {0, 0, hp.getWidth(), hp.getHeight() }, { ALLREGION });

		aaellipseRGBA(window.getRenderer(), kecleon.getX() + kecleon.getWidth() / 2, kecleon.getY() + kecleon.getHeight() / 2, kecleon.getrange()[0], kecleon.getrange()[1], 125, 125, 125, 255);
		aaellipseRGBA(window.getRenderer(), Slugma.getX() + Slugma.getWidth() / 2, Slugma.getY() + Slugma.getHeight() / 2, Slugma.getrange()[0], Slugma.getrange()[1], 125, 125, 125, 255);
		aaellipseRGBA(window.getRenderer(), green.getX() + green.getWidth() / 2, green.getY() + green.getHeight() / 2, green.getrange()[0], green.getrange()[1], 125, 125, 125, 255);
		aaellipseRGBA(window.getRenderer(), link.getX() + link.getWidth() / 2, link.getY() + link.getHeight() / 2, linkHitboxX, linkHitboxY, 125, 125, 125, 255);
		aaellipseRGBA(window.getRenderer(), blue.getX() + blue.getWidth() / 2, blue.getY() + blue.getHeight() / 2, blue.getrange()[0], blue.getrange()[1], 125, 125, 125, 255);
		window.display();
	}

	hp.close();
	hp_num.close();
	link.close();
	green.close();
	blue.close();
	kecleon.close();
	for (int i = 0; i < fire.size(); i++)
		fire[i].close();
	for (int i = 0; i < ice.size(); i++)
		ice[i].close();
	Slugma.close();
	window.close();
	sdl.close();

	return 0;
}