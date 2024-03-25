#include "Ball.h"
#include <stdlib.h>

Ball::Ball() :Enemy()
{
}

Ball::Ball(SDL_Renderer* rr, int radius, int M, int speed, int R, int G, int B, char type) :Enemy(rr, radius, radius, speed, 1)
{
	mass = M;
	r = R;
	g = G;
	b = B;
	if (type == 'f')
		effect.setPath("../audio/Fireball.wav");
	else if (type == 'i')
		effect.setPath("../audio/iceball.wav");
	else if (type == 'B')
		effect.setPath("../audio/beam.wav");
	effect.setType(EFFECT);
	effect.load();
	bonk.setPath("../audio/metal.wav");
	bonk.setType(EFFECT);
	bonk.load();
}

void Ball::setSpeedXY(int xvv, int yvv)
{
	xv = xvv;
	yv = yvv;
}

void Ball::Colide(Ball& fireball2)
{
	if (fireball2.getHealth() > 0)
	{
		double dis = sqrt((x - fireball2.x) * (x - fireball2.x) + (y - fireball2.y) * (y - fireball2.y));
		if (dis <= rangea + fireball2.rangea)
		{
			double temp1;
			double temp2;

			temp1 = ((mass - fireball2.mass) * xv + 2 * fireball2.mass * fireball2.xv) / (mass + fireball2.mass);
			temp2 = ((fireball2.mass - mass + 0.0) * fireball2.xv + 2 * mass * xv) / (mass + fireball2.mass);
			xv = temp1;
			fireball2.xv = temp2;

			temp1 = ((mass - fireball2.mass) * yv + 2 * fireball2.mass * fireball2.yv) / (mass + fireball2.mass);
			temp2 = ((fireball2.mass - mass) * fireball2.yv + 2 * mass * yv) / (mass + fireball2.mass);
			yv = temp1;
			fireball2.yv = temp2;
			x += xv;
			fireball2.x += fireball2.xv;
			y += yv;
			fireball2.y += fireball2.yv;
		}
	}
	else
		return;
}

void Ball::Colide(Red& red)
{
	if (red.getHealth() > 0)
	{
		double m = (red.getY() - y + 0.0) / (red.getX() - x);
		double a = (red.getrange()[1]) * (red.getrange()[1]) + (red.getrange()[0]) * (red.getrange()[0]) * m * m;
		double b = (red.getrange()[1]) * (red.getrange()[1]) * (-2) * red.getX() + (red.getrange()[0]) * (red.getrange()[0]) * (-2) * m * m * red.getX();
		double c = (red.getrange()[1]) * (red.getrange()[1]) * red.getX() * red.getX() + (red.getrange()[0]) * (red.getrange()[0]) * m * m * red.getX() * red.getX() - (red.getrange()[0]) * (red.getrange()[0]) * (red.getrange()[1]) * (red.getrange()[1]);
		double dd = sqrt(b * b - 4 * a * c);
		double x1 = (-1 * b + dd) / (2 * a);
		double x2 = (-1 * b - dd) / (2 * a);
		double y1 = m * x1 - m * red.getX() + red.getY();
		double y2 = m * x2 - m * red.getX() + red.getY();
		double d1 = (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y);
		double d2 = (x2 - x) * (x2 - x) + (y2 - y) * (y2 - y);
		if (sqrt(d1) <= rangea || sqrt(d2) <= rangea)
		{
			red.setHealth(red.getHealth() - mass);
			setHealth(0);
		}
	}
}

void Ball::draw()
{
	x += xv;
	y += yv;
	if (x <= rangea || x >= WIDTH - rangea)
	{
		effect.playEffect(ONE, -1);
		x -= xv;
		xv *= -1;
	}

	if (y <= rangea || y >= HEIGHT - rangea)
	{
		effect.playEffect(ONE, -1);
		y -= yv;
		yv *= -1;
	}
	if (currenthealth > 0)		
		aaFilledEllipseRGBA(renderer, x, y, rangea, rangea, r, g, b, 255);
}

void Ball::setColor(int rr, int gg, int bb)
{
	r = rr;
	g = gg;
	b = bb;
}

void Ball::setRadius(int r)
{
	setrange(r, r);
}

void Ball::setMass(int ms)
{
	mass = ms;
}

void Ball::damaged(bool hit)
{
	if (hit)
	{
		bonk.playEffect(ONE, -1);
		x -= xv;
		y -= yv;
		xv *= -1;
		yv *= -1;
	}
}

int Ball::getHealth()
{
	return currenthealth;
}

void Ball::close()
{
	effect.close();
	bonk.close();
}