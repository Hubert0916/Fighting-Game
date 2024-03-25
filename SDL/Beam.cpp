#include "Beam.h"
#include <stdio.h>
Beam::Beam()
{

}

Beam::Beam(const char* path, SDL_Renderer* rr, int radius, int M, int speed) :Ball(rr, radius, M, speed, 0, 0, 0, 'B')
{
	image = new Image;
	image->setRenderer(rr);
	image->setPath("../image/beam.png");
	image->generateTexture();
	farhit.setPath("../audio/farhit.wav");
	farhit.setType(EFFECT);
	farhit.load();
}

void Beam::draw()
{
	x += xv;
	y += yv;
	if (x <= rangea || x >= WIDTH - rangea || y <= rangea || y >= HEIGHT - rangea || currenthealth <= 0)
	{
		alpha = 0;
		setHealth(0);
	}
	SDL_Rect d;
	d.x = x - getWidth() / 2;
	d.y = y - getHeight() / 2;
	d.w = image->getWidth();
	d.h = image->getHeight();
	image->setAngle(angle);
	image->setAlpha(alpha);
	image->draw(renderer, d, { ALLREGION });
}

template <class T>
void Beam::collide(T &obj)
{
	aaellipseRGBA(renderer, x, y, rangea, rangeb, 125, 125, 125, 255);
	double dis = sqrt(pow(obj.getX() + obj.getWidth() / 2 - getX() - getWidth() / 2, 2) + pow(obj.getY() + obj.getHeight() / 2 - getY() - getHeight() / 2, 2));
	if (dis < obj.getrange()[0] + getrange()[0] && getHealth() > 0)
	{
		setHealth(0);
		farhit.playEffect(ONE, -1);
		obj.setHealth(obj.getHealth() - 5);
	}
}
template void Beam::collide(Red& obj);
template void Beam::collide(Green& obj);
template void Beam::collide(Blue& obj);
template void Beam::collide(Kecleon& obj);

void Beam::close()
{
	image->close();
	effect.close();
	farhit.close();
}
