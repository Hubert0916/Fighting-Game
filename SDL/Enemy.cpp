#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(SDL_Renderer* r, int ra, int rb, int s, int health)
{
	setrange(ra, rb);
	setHealth(health);
	setSpeed(s);
	maxhealth = currenthealth;
	renderer = r;
	alpha = 255;
}

void Enemy::setPosition(int xx, int yy)
{
	x = xx;
	y = yy;
}

void Enemy::startTimer(Uint32 t)
{
	time = t;
	SDL_AddTimer(time, RoutineMotion, this);
}

Uint32 Enemy::RoutineMotion(Uint32 interval, void* param)
{
	Enemy* em = (Enemy*)param;
	if (em->time != 0)
	{
		em->x = (em->x + em->speed) % WIDTH;
		em->setPosition(em->x, HEIGHT / 2);
	}
	else
	{
		return 0;
	}
	return interval;
}

void Enemy::stopTimer()
{
	time = 0;
}

int Enemy::getX()
{
	return x;
}

int Enemy::getY()
{
	return y;
}

int Enemy::getWidth()
{
	return image->getWidth();
}

int Enemy::getHeight()
{
	return image->getHeight();
}

void Enemy::setrange(int ra, int rb)
{
	rangea = ra;
	rangeb = rb;
}

int* Enemy::getrange()
{
	int range[2] = { rangea, rangeb };
	return range;
}

void Enemy::draw()
{
	if (currenthealth == 0)
		close();
	aaellipseRGBA(renderer, x, y, rangea, rangeb, 255, 255, 0, alpha);
	boxRGBA(renderer, x - rangea, y - rangeb - 10, x - rangea + currenthealth, y - rangeb - 5, 255, 0, 0, alpha);
}

void Enemy::setHealth(int hp)
{
	currenthealth = hp;
	if (currenthealth <= 0)
		currenthealth = 0;
}
int Enemy::getHealth()
{
	return currenthealth;
}

void Enemy::setAngle(int a)
{
	angle = a;
}

int Enemy::getAngle()
{
	return angle;
}

void Enemy::setAlpha(int a)
{
	alpha = a;
}

int Enemy::getAlpha()
{
	return alpha;
}

void Enemy::damaged(bool hit)
{
	if (hit)
		setHealth(currenthealth - 1);
}

void Enemy::setSpeed(int s)
{
	speed = s;
}

int Enemy::getSpeed()
{
	return speed;
}

Enemy::~Enemy()
{

}

void Enemy::close()
{
	time = 0;
	alpha = 0;
}