#include "CSprite.h"

CSprite::CSprite(s32 x, s32 y, u32 width, u32 height)
{
	//this->setTexture(driver->getTexture(texture));
	
	this->setX(x);
	this->setY(y);

	this->setWidth(width);
	this->setHeight(height);
}

CSprite::~CSprite()
{
	
}

rect<s32> CSprite::getBoundingBox() const
{
	return rect<s32>(getX(), getY(), getX() + width, getY() + height);
}

s32 CSprite::getHeight() const
{
	return this->height;
}

s32 CSprite::getWidth() const
{
	return this->width;
}

vector2di CSprite::getPosition() const
{
	return this->position;
}

s32 CSprite::getX() const
{
	return this->position.X;
}

s32 CSprite::getY() const
{
	return this->position.Y;
}

/* SET */

void CSprite::setHeight(u32 height)
{
	this->height = height;
}

void CSprite::setWidth(u32 width)
{
	this->width = width;
}

void CSprite::setPosition(vector2di &position)
{
	this->position = position;
}

void CSprite::setPosition(s32 x, s32 y)
{
	setX(x);
	setY(y);
}

/* Set x coordinate
 * @param x - New x coordinate to set
 * @return none */
void CSprite::setX(s32 x)
{
	this->position.X = x;
}

/* Set y coordinate
 * @param y - New y coordinate to set
 * @return none */
void CSprite::setY(s32 y)
{
	this->position.Y = y;
}

void CSprite::draw()
{
	// draw the sprite
	driver->draw2DImage(texture, position, rect<s32>(0, 0, width, height), 0, SColor(255,255,255,255), true);
}