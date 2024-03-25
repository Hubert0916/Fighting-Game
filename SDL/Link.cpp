#include "Link.h"
#include <SDL2_gfxPrimitives.h>
#include "SDL_ttf.h"
#include "Image.h"
#include <stdio.h>
#include <cmath>
#include "constants.h"

Link::Link(const char* p, SDL_Renderer* rr, int nn)
{
	image = new Image[nn];
	num = nn;
	frame = 0;
	alpha = 255;
	xv = 0;
	yv = 0;
	for (int i = 0; i < num; i++)
	{
		char file[100];
		sprintf_s(file, "%s%02d.png", p, i + 1);
		image[i].setRenderer(rr);
		image[i].setPath(file);
		image[i].setColor(0, 0, 0, 0);
		image[i].generateTexture();
		image[i].setAlpha(255);
		image[i].setFlip(SDL_FLIP_NONE);
	}
	renderer = rr;
	turtle.setAlpha(255);
	turtle.setPath("../image/turtle.png");
	turtle.setRenderer(renderer);
	turtle.setFlip(SDL_FLIP_NONE);
	turtle.setColor(0, 0, 0, 0);
	turtle.generateTexture();
	ice.setAlpha(255);
	ice.setPath("../image/ice.png");
	ice.setRenderer(renderer);
	ice.setFlip(SDL_FLIP_NONE);
	ice.setColor(0, 0, 0, 0);
	ice.generateTexture();
	sword.setPath("../audio/sword.wav");
	sword.setType(EFFECT);
	sword.load();
	fuck.setPath("../audio/fuck.wav");
	fuck.setType(EFFECT);
	fuck.load();
	hitEffect.setPath("../audio/hit.wav");
	hitEffect.setType(MUSIC);
	hitEffect.load();
	tackle.setPath("../audio/Tackle.wav");
	tackle.setType(EFFECT);
	tackle.load();
	wave.setPath("../audio/wave.wav");
	wave.setType(EFFECT);
	wave.load();
	for (int i = 0; i < 2; i++)
	{
		beam.push_back(Beam("../image/beam.png", renderer, 30, 20, 10));
		beam[i].setAlpha(0);
		beam[i].setPosition(0, 0);
	}
	attack = false;
	farattack = false;
	slow = false;
	freeze = false;
	touch = true;
	setSlowStart(0);
	setUntouchStart(0);
	setFreezeStart(0);
	setTouch(true);
	setHealth(HP);
	setSpeed(5);
}

void Link::setPosition(int xx, int yy)
{
	x = xx; y = yy;
}

int Link::getX()
{
	return x;
}

int Link::getY()
{
	return y;
}
int Link::getWidth()
{
	return image[frame].getWidth();
}
int Link::getHeight()
{
	return image[frame].getHeight();
}

void Link::draw()
{
	if (getHealth() <= 0)
		return;

	if(!(freeze || slow || attack || farattack))
	{ 
		x += xv;
		if (x < 0 || x > WIDTH - image[frame].getWidth() / 2)
			x -= xv;
		y += yv;
		if (y < 0 || y > HEIGHT - image[frame].getHeight() / 2)
			y -= yv;
	}
	else if(!freeze && (slow || attack || farattack))
	{
		x += xv / 2;
		if (x < 0 || x > WIDTH - image[frame].getWidth() / 2)
			x -= xv / 2;
		y += yv / 2;
		if (y < 0 || y > HEIGHT - image[frame].getHeight() / 2)
			y -= yv / 2;
	}
	
	if (x < 0)
		x = 0;
	else if (x > WIDTH - image[frame].getWidth())
		x = WIDTH - image[frame].getWidth();
	else if (y < 0)
		y = 0;
	else if (y > HEIGHT - image[frame].getHeight())
		y = HEIGHT - image[frame].getHeight();

	SDL_Rect d;
	d.x = x;
	d.y = y;
	d.w = image[frame].getWidth();
	d.h = image[frame].getHeight();
	image[frame].setAlpha(alpha);
	image[frame].draw(renderer, d, { ALLREGION });

	for (int i = 0; i < beam.size(); i++)
	{
		if(beam[i].getAlpha() == 255)
			beam[i].draw();
	}
}

void Link::close()
{
	for (int i = 0; i < num; i++)
		image[i].close();
	for (int i = 0; i < beam.size(); i++)
		beam[i].close();
	turtle.close();
	ice.close();
	sword.close();
	fuck.close();
	tackle.close();
	hitEffect.close();
	wave.close();
	SDL_RemoveTimer(timer);
}

Uint32 Link::displayattack(Uint32 interval, void* param)
{
	Link* pt = (Link*)param;
	if (pt->time != 0)
		pt->frame = (pt->frame + 1) % (pt->num);
	else
		return 0;
	return interval;
}

void Link::startTimer(Uint32 t)
{
	time = t;
	timer = SDL_AddTimer(time, displayattack, this);
}

void Link::stopTimer()
{
	time = 0;
	SDL_RemoveTimer(timer);
}

void Link::setSpeed(int s)
{
	speed = s;
}

void Link::setv(int xxv, int yyv)
{
	xv = xxv; yv = yyv;
}

void Link::movinghandle(SDL_Event& e, Link& role)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_w: role.setv(xv, yv - speed); break;
			case SDLK_s: role.setv(xv, yv + speed); break;
			case SDLK_a: role.setv(xv - speed, yv); break;
			case SDLK_d: role.setv(xv + speed, yv); break;
		}
		if (e.key.keysym.sym == SDLK_SPACE)
		{
			x += 15 * xv;
			if (x > WIDTH - image[frame].getWidth() || x < 0)
				x -= 15 * xv;
			y += 15 * yv;
			if (y > HEIGHT - image[frame].getHeight() || y < 0)
				y -= 15 * yv;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_w: role.setv(xv, yv + speed); break;
			case SDLK_s: role.setv(xv, yv - speed); break;
			case SDLK_a: role.setv(xv + speed, yv); break;
			case SDLK_d: role.setv(xv - speed, yv); break;
		}
	}
	if (e.type == SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&mousex, &mousey);
		if (mousex < x + image[frame].getWidth() / 2)
		{
			for (int i = 0; i < num; i++)
				image[i].setFlip(SDL_FLIP_NONE);
		}
		else if (mousex > x + image[frame].getWidth() / 2)
		{
			for (int i = 0; i < num; i++)
				image[i].setFlip(SDL_FLIP_HORIZONTAL);
		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN  && e.button.button == SDL_BUTTON_LEFT && !freeze)
	{
		sword.playEffect(ONE, -1);
		attack = true;
		startTimer(50);
	}
	else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
	{
		frame = 0;
		attack = false;
		stopTimer();
	}

	if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT && !freeze)
	{
		farattack = true;
		frame = 3;
		for (int i = 0; i < beam.size(); i++)
		{
			if (beam[i].getAlpha() != 255)
			{
				wave.playEffect(ONE, -1);
				beam[i].setHealth(1);
				beam[i].setAlpha(255);
				beam[i].setPosition(x + getWidth() / 2, y + getHeight() / 2);
				int x1 = beam[i].getX() + beam[i].getWidth() / 2, y1 = beam[i].getY() + beam[i].getHeight() / 2;
				int x2 = mousex, y2 = mousey;
				beam[i].setSpeedXY((x2 - x1) / (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2))) * beam[i].getSpeed(), (y2 - y1) / (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2))) * beam[i].getSpeed());
				if (x1 == x2)
				{
					if (y1 < y2)
						beam[i].setAngle(90);
					else
						beam[i].setAngle(270);
				}
				else
				{
					if(x2 > x1)
						beam[i].setAngle(atan(double((y2-y1))/(x2-x1)) * 180 / M_PI);
					else
						beam[i].setAngle(180 + atan(double((y2 - y1)) / (x2 - x1)) * 180 / M_PI);
				}
				break;
			}
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
	{
		farattack = false;
		frame = 0;
	}

	if (e.type == SDL_MOUSEWHEEL)
	{
		if (e.wheel.y > 5)
			printf("True\n");
	}
}

void Link::setHealth(int hp)
{
	if (hp < 0)
		hp = 0;
	currenthealth = hp;
}

int Link::getHealth()
{
	return currenthealth;
}

bool Link::hitbox(int ea, int eb, double touchx, double touchy)
{
	int orx, ory;
	orx = x + image[frame].getWidth() / 2;
	ory = y + image[frame].getHeight() / 2;

	double dis;
	dis = (orx - touchx) * (orx - touchx) / ea / ea + (ory - touchy) * (ory - touchy) / eb / eb;
	dis = sqrt(dis);
	if (dis <= 1 && touch)
	{
		setHealth(currenthealth - 1);
		//fuck.playEffect(ONE, -1);
		tackle.playEffect(ONE, -1);
		x += (orx - touchx) * 5;
		y += (ory - touchy) * 5;
		return true;
	}
	else
		return false;
}

double* Link::caltouchxy(int ex, int ey, double ra, double rb)
{
	if ((x + image[frame].getWidth() / 2 - ex) != 0)
	{
		double m = (y + image[frame].getHeight() / 2 - ey) / (x + image[frame].getWidth() / 2 - ex);
		double a, b, c;
		a = rb * rb + ra * ra * m * m;
		b = -2 * rb * rb * ex - 2 * ra * ra * m * m * ex;
		c = rb * rb * ex * ex + ra * ra * m * m * ex * ex - ra * ra * rb * rb;

		double dd;
		double* ans1 = new double[2];
		double* ans2 = new double[2];
		dd = sqrt(b * b - 4 * a * c);
		ans1[0] = (-1 * b + dd) / 2 / a; // x1
		ans2[0] = (-1 * b - dd) / 2 / a; // x2
		ans1[1] = m * ans1[0] - m * ex + ey; // y1 
		ans2[1] = m * ans2[0] - m * ex + ey; // y2

		double d1 = (ans1[0] - (x + image[frame].getWidth() / 2)) * (ans1[0] - (x + image[frame].getWidth() / 2)) + (ans1[1] - (y + image[frame].getHeight() / 2)) * (ans1[1] - (y + image[frame].getHeight() / 2));
		double d2 = (ans2[0] - (x + image[frame].getWidth() / 2)) * (ans2[0] - (x + image[frame].getWidth() / 2)) + (ans2[1] - (y + image[frame].getHeight() / 2)) * (ans2[1] - (y + image[frame].getHeight() / 2));
		if (d1 < d2)
			return ans1;
		else
			return ans2;
	}
	else
	{
		double* notouch = new double[2];
		notouch[0] = ex;
		notouch[1] = (ey > y + image[frame].getHeight() / 2) ? ey - rb : ey + rb;
		return notouch;
	}
}

void Link::AtHitbox(int ea, int eb, double touchx, double touchy)
{
	int orx, ory;
	orx = x + image[frame].getWidth() / 2;
	ory = y + image[frame].getHeight() / 2;
	double dis;
	dis = (touchx - orx) * (touchx - orx) / (ea * ea) + (touchy - ory) * (touchy - ory) / (eb * eb);
	//printf("touchx = %.2f, touchy = %.2f, orx = %d, ory = %d, dis = %.2f\n", touchx, touchy, orx, ory, dis);
	dis = sqrt(dis);
	if (dis <= 1 && attack)
	{
		//printf("HIT!\n");
		if(!hitEffect.isPlaying())
			hitEffect.play(0);
		hit = true;
	}
	else
		hit = false;
}

bool Link::getHit()
{
	return hit;
}

void Link::setTouch(bool f)
{
	touch = f;
}

void Link::setUntouchStart(int t)
{
	untouchStart = t;
	touch = false;
}

void Link::remainUntouch(int t)
{
	if (!(untouchStart && t - untouchStart <= 1))
	{
		touch = true;
		untouchStart = 0;
	}
}

void Link::setSlowStart(int t)
{
	slowStart = t;
	slow = true;
}

void Link::remainSlow(int t)
{
	if (slowStart && t - slowStart <= 1)
	{
		SDL_Rect d;
		d.x = x;
		d.y = y;
		d.w = turtle.getWidth();
		d.h = turtle.getHeight();
		turtle.draw(renderer, d, { ALLREGION });
	}
	else
	{
		slow = false;
		slowStart = 0;
	}
}

void Link::setFreezeStart(int t)
{
	freezeStart = t;
	freeze = true;
}

void Link::remainFreeze(int t)
{
	if (freezeStart && t - freezeStart <= 1)
	{
		frame = 0;
		alpha = 125;
		attack = false;
		stopTimer();

		SDL_Rect d;
		d.x = x + 20;
		d.y = y - 5;
		d.w = ice.getWidth();
		d.h = ice.getHeight();
		ice.draw(renderer, d, { ALLREGION });
	}
	else
	{
		freeze = false;
		freezeStart = 0;
		alpha = 255;
	}
}

template <class T>
void Link::collide(T &obj)
{
	for (int i = 0; i < beam.size(); i++)
		beam[i].collide(obj);
}
template void Link::collide(Red& obj);
template void Link::collide(Green& obj);
template void Link::collide(Blue& obj);
template void Link::collide(Kecleon& obj);
