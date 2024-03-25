#include "Green.h"
#include "Image.h"
#include <stdio.h>
#include "constants.h"

Green::Green()
{

}
Green::Green(const char* path, int n, SDL_Renderer* r, int ra, int rb, int s, int health):Enemy(r, ra, rb, s, health)
{
	num = n;
	image = new Image[num];
	for (int i = 0; i < num; i++)
	{
		char file[100];
		sprintf_s(file, 100, "%s%02d.png", path, i + 1);
		
		image[i].setAlpha(255);
		image[i].setPath(file);
		image[i].setRenderer(r);
		image[i].setColor(0, 0, 0, 0);
		image[i].setFlip(SDL_FLIP_HORIZONTAL);
		image[i].generateTexture();
	}
	cry.setPath("../audio/grimer.wav");
	cry.setType(EFFECT);
	cry.load();
	zombie.setPath("../audio/zombie.wav");
	zombie.setType(EFFECT);
	zombie.load();
	link_x = WIDTH / 2;
	link_y = HEIGHT / 2;
	touchLink = false;
	maxhealth = currenthealth;
	alpha = 255;
	frame = 0;
	renderer = r;
}

void Green::draw()
{
	if (getHealth() == 0 && alpha >= 0)
	{
		if (alpha == 0)
			return;
		alpha--;
	}

	if (getX() + getWidth() / 2 > link_x)
		for (int i = 0; i < num; i++)
			image[i].setFlip(SDL_FLIP_NONE);

	else
		for (int i = 0; i < num; i++)
			image[i].setFlip(SDL_FLIP_HORIZONTAL);

	int xf = x + image[frame].getWidth() / 2;
	int yf = y + image[frame].getHeight() / 2;
	boxRGBA(renderer, xf - rangea, yf - rangeb - 10, xf + rangea - rangea * 2 * (maxhealth - currenthealth) / maxhealth, yf - rangeb - 5, 255, 0, 0, alpha);
	
	SDL_Rect d;
	d.x = x;
	d.y = y;
	d.w = image[frame].getWidth();
	d.h = image[frame].getHeight();
	image[frame].setAlpha(alpha);
	image[frame].draw(renderer, d, {ALLREGION});
}

void Green::close()
{
	for (int i = 0; i < num; i++)
		image[i].close();
	time_trace = 0;
	time_motion = 0;
	cry.close();
	SDL_RemoveTimer(timer);
}

int Green::getWidth()
{
	return image[frame].getWidth();
}

int Green::getHeight()
{
	return image[frame].getHeight();
}

int Green::getSpeed()
{
	return speed;
}

void Green::getLinkPos(int x, int y)
{
	link_x = x;
	link_y = y;
}

void Green::starttrace(Uint32 t)
{
	time_trace = t;
	timer = SDL_AddTimer(time_trace, traceLink, this);
}

void Green::startmotion(Uint32 t)
{
	time_motion = t;
	timer = SDL_AddTimer(time_motion, RoutineMotion, this);
}

void Green::stoptrace()
{
	time_trace = 0;
	SDL_RemoveTimer(time_trace);
}

void Green::stopmotion()
{
	time_motion = 0;
	SDL_RemoveTimer(time_motion);
}


Uint32 Green::RoutineMotion(Uint32 interval, void* param)
{
	Green* p = (Green*)param;

	if (p->time_motion != 0)
	{
		p->frame = (p->frame + 1) % p->num;
		return interval;
	}
	else
	{
		return 0;
	}
}

Uint32 Green::traceLink(Uint32 interval, void* param)
{
	Green* p = (Green*)param;

	if (p->time_trace != 0)
	{
		int x1 = p->x + p->getWidth() / 2;
		int y1 = p->y + p->getHeight() / 2;
		int x2 = p->link_x;
		int y2 = p->link_y;

		if (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)) < 800 && !(x2 == x1 && y2 == y1))
		{
			p->setPosition(p->getX() + (x2 - x1) / (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2))) * p->getSpeed()
				, p->getY() + (y2 - y1) / (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2))) * p->getSpeed());
		}
		return interval;
	}
	else
		return 0;
}

void Green::damaged(bool hit)
{
	if (hit)
		setHealth(currenthealth - 3);
}

void Green::setHealth(int hp)
{
	currenthealth = hp;
	if (currenthealth <= 0)
	{
		currenthealth = 0;
		cry.playEffect(ONE, -1);
	}
}
