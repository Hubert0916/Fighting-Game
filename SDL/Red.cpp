#include "Red.h"
#include <stdio.h>

Red::Red()
{
}

Red::Red(SDL_Renderer* rr, int ra, int rb, int s, int health) :Enemy(rr, ra, rb, s, health), frame(0), num(1)
{
	image = NULL;
	speed = s;
	xv = speed;
	yv = speed;
}
Red::Red(const char* pp, int picnumber, SDL_Renderer* rr, int ra, int rb, int s, int health) :Enemy(rr, ra, rb, s, health), frame(0), num(picnumber)
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
	cry.setPath("../audio/slugma.wav");
	cry.setType(EFFECT);
	cry.load();
	maxhealth = currenthealth;
	alpha = 255;
	speed = s;
	xv = speed;
	yv = speed;
}

void Red::startTimer(Uint32 t)
{
	time = t;
	timer = SDL_AddTimer(time, ChangeData, this);
}

Uint32 Red::ChangeData(Uint32 interval, void* param)
{
	Red* red = (Red*)param;
	if (red->time != 0)
	{
		red->x += red->xv;
		red->y += red->yv;
		if (red->x <= red->rangea || red->x >= WIDTH - 2 * red->rangea)
		{
			red->x -= red->xv;
			red->xv *= -1;
		}
		if (red->y <= red->rangeb || red->y >= HEIGHT - 2 * red->rangeb)
		{
			red->y -= red->yv;
			red->yv *= -1;
		}
		red->frame = (red->frame + 1) % red->num;
		return interval;
	}
	else
		return 0;
}

void Red::draw()
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

		int x_fix = x + image[frame].getWidth() / 2;
		int y_fix = y + image[frame].getHeight() / 2;
		boxRGBA(renderer, x_fix - rangea, y_fix - rangeb - 10, x_fix + rangea - rangea * 2 * (maxhealth - currenthealth) / maxhealth, y_fix - rangeb - 5, 255, 0, 0, alpha);
		
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

int Red::getWidth()
{
	return image[frame].getWidth();
}
int Red::getHeight()
{
	return image[frame].getHeight();
}

int Red::getHealth()
{
	return currenthealth;
}

void Red::close()
{
	for (int i = 0; i < num; i++)
		image[i].close();
	time = 0;
	cry.close();
	SDL_RemoveTimer(timer);
}
void Red::playEffect(int f)
{
	cry.playEffect(ONE, -1);
}

void Red::damaged(bool hit)
{
	if (hit)
		setHealth(currenthealth - 3);
}

void Red::setHealth(int hp)
{
	currenthealth = hp;
	if (currenthealth <= 0)
	{
		currenthealth = 0;
		cry.playEffect(ONE, -1);
	}
}
