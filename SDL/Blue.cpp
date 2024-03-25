#include "Blue.h"
#include <stdio.h>

Blue::Blue()
{
}

Blue::Blue(const char* pp, int picnumber, SDL_Renderer* rr, int ra, int rb, int s, int health) :Enemy(rr, ra, rb, s, health), frame(0), num(picnumber)
{
	image = new Image[num];
	for (int i = 0; i < num; i++)
	{
		char file[100];
		sprintf_s(file, 100, "%s%02d.png", pp, i + 1);
		image[i].setRenderer(rr);
		image[i].setPath(file);
		image[i].setColor(0, 0, 0, 0);
		image[i].generateTexture();
		image[i].setAlpha(255);
		image[i].setFlip(SDL_FLIP_NONE);
	}
	cry.setPath("../audio/blue.wav");
	cry.setType(EFFECT);
	cry.load();
	maxhealth = currenthealth;
	alpha = 255;
	speed = s;
	xv = speed;
	yv = speed;
}

void Blue::startTimer(Uint32 t)
{
	time = t;
	timer = SDL_AddTimer(time, ChangeData, this);
}

Uint32 Blue::ChangeData(Uint32 interval, void* param)
{
	Blue* blue = (Blue*)param;
	if (blue->time != 0)
	{
		blue->x += blue->xv;
		blue->y += blue->yv;
		if (blue->x <= blue->rangea || blue->x >= WIDTH - 2 * blue->rangea)
		{
			blue->x -= blue->xv;
			blue->xv *= -1;
		}
		if (blue->y <= blue->rangeb || blue->y >= HEIGHT - 2 * blue->rangeb)
		{
			blue->y -= blue->yv;
			blue->yv *= -1;
		}
		blue->frame = (blue->frame + 1) % blue->num;
		return interval;
	}
	else
		return 0;
}

void Blue::draw()
{
	if (image == NULL)
		aaellipseRGBA(renderer, x, y, rangea, rangeb, 255, 0, 0, 255);
	else
	{
		if (currenthealth <= 0 && alpha > 0)
		{
			alpha -= 5;
			image[frame].setAlpha(alpha);
		}
		if (alpha == 0)
			return;

		int xf = x + image[frame].getWidth() / 2;
		int yf = y + image[frame].getHeight() / 2;
		boxRGBA(renderer, xf - rangea, yf - rangeb - 10, xf + rangea - rangea * 2 * (maxhealth - currenthealth) / maxhealth, yf - rangeb - 5, 255, 0, 0, alpha);

		SDL_Rect d;
		d.x = x;
		d.y = y;
		d.w = image[frame].getWidth();
		d.h = image[frame].getHeight();
		if (d.x < WIDTH / 2)
			image[frame].setFlip(SDL_FLIP_HORIZONTAL);
		else
			image[frame].setFlip(SDL_FLIP_NONE);
		image[frame].setAlpha(alpha);
		image[frame].draw(renderer, d, { ALLREGION });
	}
}

int Blue::getWidth()
{
	return image[frame].getWidth();
}
int Blue::getHeight()
{
	return image[frame].getHeight();
}

int Blue::getHealth()
{
	return currenthealth;
}

void Blue::close()
{
	for (int i = 0; i < num; i++)
		image[i].close();
	time = 0;
	cry.close();
	SDL_RemoveTimer(timer);
}
void Blue::playEffect(int f)
{
	cry.playEffect(ONE, -1);
}

void Blue::damaged(bool hit)
{
	if (hit)
		setHealth(currenthealth - 1);
}

void Blue::setHealth(int hp)
{
	currenthealth = hp;
	if (currenthealth <= 0)
	{
		currenthealth = 0;
		cry.playEffect(ONE, -1);
	}
}
