#include "Kecleon.h"
#include "Image.h"
#include <stdio.h>
#include "constants.h"

Kecleon::Kecleon()
{

}

Kecleon::Kecleon(const char* path, int n, SDL_Renderer* r, int ra, int rb, int s, int health) :Enemy(r, ra, rb, s, health)
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
	cry.setPath("../audio/kecleon.wav");
	cry.setType(EFFECT);
	cry.load();
	invision.setPath("../audio/invisible.wav");
	invision.setType(EFFECT);
	invision.load();
	link_x = WIDTH / 2;
	link_y = HEIGHT / 2;
	touchLink = false;
	maxhealth = currenthealth;
	alpha = 255;
	frame = 0;
	renderer = r;
}

void Kecleon::draw()
{
	if (getHealth() == 0)
	{
		alpha = 0;
		return;
	}

	if (getX() + getWidth() / 2 > link_x)
		for (int i = 0; i < num; i++)
			image[i].setFlip(SDL_FLIP_NONE);
	else
		for (int i = 0; i < num; i++)
			image[i].setFlip(SDL_FLIP_HORIZONTAL);

	if (alpha != 255)
	{
		aaFilledEllipseRGBA(renderer, x + getWidth() / 2, y + getHeight(), 50, 20, 160, 201, 169, 20);
		setSpeed(15);
	}
	else
		setSpeed(2);

	int x_fix = x + image[frame].getWidth() / 2;
	int y_fix = y + image[frame].getHeight() / 2;
	//aaellipseRGBA(renderer, x_fix, y_fix, rangea, rangeb, 255, 255, 0, alpha);
	boxRGBA(renderer, x_fix - rangea, y_fix - rangeb - 10, x_fix + rangea - rangea * 2 * (maxhealth - currenthealth) / maxhealth, y_fix - rangeb - 5, 255, 0, 0, alpha);
	SDL_Rect d;
	d.x = x;
	d.y = y;
	d.w = image[frame].getWidth();
	d.h = image[frame].getHeight();
	image[frame].setAlpha(alpha);
	image[frame].draw(renderer, d, { ALLREGION });
}


void Kecleon::close()
{
	for (int i = 0; i < num; i++)
		image[i].close();
	time_trace = 0;
	time_motion = 0;
	time_invisible = 0;
	cry.close();
	invision.close();
	SDL_RemoveTimer(timer);

}

int Kecleon::getWidth()
{
	return image[frame].getWidth();
}

int Kecleon::getHeight()
{
	return image[frame].getHeight();
}

int Kecleon::getSpeed()
{
	return speed;
}

void Kecleon::getLinkPos(int x, int y)
{
	link_x = x;
	link_y = y;
}

void Kecleon::starttrace(Uint32 t)
{
	time_trace = t;
	timer = SDL_AddTimer(time_trace, traceLink, this);
}

void Kecleon::startmotion(Uint32 t)
{
	time_motion = t;
	timer = SDL_AddTimer(time_motion, RoutineMotion, this);
}

void Kecleon::startinvisible(Uint32 t)
{
	time_invisible = t;
	timer = SDL_AddTimer(time_invisible, invisible, this);
}

void Kecleon::stoptrace()
{
	time_trace = 0;
}

void Kecleon::stopmotion()
{
	time_motion = 0;
}

void Kecleon::stopinvisible()
{
	time_invisible = 0;
}

Uint32 Kecleon::RoutineMotion(Uint32 interval, void* param)
{
	Kecleon* p = (Kecleon*)param;

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

Uint32 Kecleon::traceLink(Uint32 interval, void* param)
{
	Kecleon* p = (Kecleon*)param;

	if (p->time_trace != 0)
	{
		int x1 = p->x + p->getWidth() / 2;
		int y1 = p->y + p->getHeight() / 2;
		int x2 = p->link_x;
		int y2 = p->link_y;

		if (!(x2 == x1 && y2 == y1))
			p->setPosition(p->getX() + (x2 - x1) / (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2))) * p->getSpeed()
				, p->getY() + (y2 - y1) / (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2))) * p->getSpeed());

		return interval;
	}
	else
		return 0;
}

Uint32 Kecleon::invisible(Uint32 interval, void* param)
{
	Kecleon* p = (Kecleon*)param;
	static int i = -1;
	if (p->time_invisible != 0)
	{
		if(p->alpha==255)
			p->invision.playEffect(ONE, -1);
		p->alpha += 255 * i;
		i *= -1;
		return interval;
	}
	else
		return 0;
}

void Kecleon::damaged(bool hit)
{
	if (hit)
		setHealth(currenthealth - 1);
}

void Kecleon::setHealth(int hp)
{
	currenthealth = hp;
	if (currenthealth <= 0)
	{
		currenthealth = 0;
		cry.playEffect(ONE, -1);
	}
}

int Kecleon::getAlpha()
{
	return alpha;
}