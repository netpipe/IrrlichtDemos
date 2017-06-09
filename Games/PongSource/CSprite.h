#pragma once

#include "CIrrObject.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;

class CSprite : public CIrrObject
{
public:
	CSprite(s32 x, s32 y, u32 width, u32 height);
	~CSprite();

	rect<s32> getBoundingBox() const;
	s32 getHeight() const;
	vector2di getPosition() const;
	s32 getWidth() const;
	s32 getX() const;
	s32 getY() const;
	
	void setHeight(u32 height);
	void setPosition(vector2di &position);
	void setPosition(s32 x, s32 y);
	void setWidth(u32 width);
	void setX(s32 x);
	void setY(s32 y);

	virtual void draw();
	virtual void update() = 0;

protected:
	IVideoDriver *driver;

private:
	rect<s32> boundingBox;
	
	ITexture *texture;
	
	vector2di position;
	
	u32 width;
	u32 height;
};
