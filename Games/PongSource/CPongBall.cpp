#include "CPongBall.h"

CPongBall::CPongBall(s32 x, s32 y, u32 width, u32 height, CPlayer *p1, CPlayer *p2) :
	CSprite(x, y, width, height)
{
	this->x= x;
	this->y = y;

	this->angle = 200;
	this->speed = 400;

	this->p1 = p1;
	this->p2 = p2;

	this->timer = new CTimer();

	c = false;

//	soundEngine = createIrrKlangDevice();
}

CPongBall::~CPongBall()
{
}

void CPongBall::checkCollision(const CPlayer *player1, const CPlayer *player2)
{
	// collide top & bottom
	if ( y <= 1 || y >= 479 - getHeight())
	{
		angle = 360 - angle;
//		soundEngine->play2D("bleep.wav");
	}

	// collide padels
	if (getBoundingBox().isRectCollided(player1->getBoundingBox()) ||
		getBoundingBox().isRectCollided(player2->getBoundingBox()))
	{
//		soundEngine->play2D("bloop.wav");

		angle = 180 - angle + player1->getSpeed();

		if (getBoundingBox().isRectCollided(player1->getBoundingBox()))
		{
			x = player1->getX() + player1->getWidth() + 1;
		}
		else
		{

			x = player2->getX() - getWidth();
		}
	}
}

void CPongBall::update()
{
	f32 delta = timer->getDelta();

	if (device->getTimer()->getTime() > 2000)
	{
		x += cos(-angle * PI / 180) * speed * delta / 1000.f;
		y += sin(-angle * PI / 180) * speed * delta / 1000.f;

		if (x < -20 || x > 660)
		{
			x = 240;
		}

		setPosition(x, y);
	}

	timer->updateDelta();
}

void CPongBall::draw()
{
	device->getVideoDriver()->draw2DRectangle(SColor(255, 255, 255, 255), getBoundingBox());
}
