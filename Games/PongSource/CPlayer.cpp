#include "CPlayer.h"

CPlayer::CPlayer(s32 x, s32 y, u32 width, u32 height, Controls &controls, CEventReceiver *receiver) : 
	CSprite(x, y, width, height)
{
	this->receiver = receiver;
	this->controls = controls;
	this->speed = 0;
	this->score = 0;
	this->y = y;
	this->timer = new CTimer();
}

CPlayer::~CPlayer()
{
}

s32 CPlayer::getScore() const
{
	return score;
}

void CPlayer::setScore(s32 score)
{
	this->score = score;
}

s32 CPlayer::getPlayerId() const
{
	return playerId;
}

void CPlayer::setPlayerId(s32 playerId)
{
	this->playerId = playerId;
}

f32 CPlayer::getSpeed() const
{
	return speed;
}

void CPlayer::update()
{
	f32 delta = timer->getDelta();

	if (receiver->IsKeyDown(controls.down))
	{
		speed = 200 * delta / 1000.f;
	}
	else
	if (receiver->IsKeyDown(controls.up))
	{
		speed = -200 * delta / 1000.f;
	}
	else
	{
		speed = 0;
	}

	int min = 0;
	int max = device->getVideoDriver()->getScreenSize().Height - getHeight();

	y += speed;

	if (y > max)
		y = max;

	if (y < min)
		y = min;

	setY(y);

	timer->updateDelta();
}

void CPlayer::draw()
{
	device->getVideoDriver()->draw2DRectangle(SColor(255, 255, 255, 255), 
		rect<s32>(getX(), getY(), getX() + getWidth(), getY() + getHeight()));
}
