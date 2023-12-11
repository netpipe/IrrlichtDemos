
#include "Positioner.h"

// ****************** Positioner ****************

void Positioner::SetTopLeft(gui::IGUIElement *element, u16 x, u16 y)
{
	element->setRelativePosition(core::position2di(x,y));
}

void Positioner::SetTopRight(gui::IGUIElement *element, u16 x, u16 y)
{
	core::rect<s32> rect = element->getRelativePosition();
	s32 width = rect.getWidth();
	s32 height = rect.getHeight();
	
	rect.UpperLeftCorner.X = driver->getScreenSize().Width - x - width;
	rect.LowerRightCorner.X = driver->getScreenSize().Width - x;
	
	rect.UpperLeftCorner.Y = y;
	rect.LowerRightCorner.Y = y + height;
	
	element->setRelativePosition(rect);
}

Positioner::Positioner(video::IVideoDriver *driver, s32 spacing)
{
	this->driver = driver;
	this->spacing = spacing;
}

void Positioner::Add(gui::IGUIElement *element, s32 id)
{
	elements.push_back(element);
	ids.push_back(id);
}

void Positioner::Reset()
{
	for (u32 i = 0; i < elements.size(); i ++)
		elements[i]->remove();
	
	elements.clear();
	ids.clear();
}

s32 Positioner::GetMouseOverId()
{
	for (u32 i = 0; i < elements.size(); i ++)
	{
		if (elements[i]->getAbsolutePosition().isPointInside(
				GetEngine()->GetIrrlichtDevice()->getCursorControl()->getPosition()
				))
		{
			return ids[i];
		}
	}
	return -1;
}

gui::IGUIElement *Positioner::GetMouseOverElement()
{
	for (u32 i = 0; i < elements.size(); i ++)
	{
		if (elements[i]->getAbsolutePosition().isPointInside(
				GetEngine()->GetIrrlichtDevice()->getCursorControl()->getPosition()
				))
		{
			return elements[i];
		}
	}
	return NULL;
}

/*gui::IGUIElement *Positioner::GetElementById(s32 id)
{
	for (u32 i = 0; i < ids.size(); i ++)
	{
		if (ids[i] == id)
			return elements[i];
	}
	return NULL;
}*/

const std::vector<gui::IGUIElement *> &Positioner::GetElements()
{
	return elements;
}


// ****************** RowPositioner ***************

RowPositioner::RowPositioner(video::IVideoDriver *driver, s32 yPos, s32 spacing, bool vertCentred)
	: Positioner(driver, spacing)
{
	this->yPos = yPos;
	this->vertCentred = vertCentred;
	
	title = NULL;
}

void RowPositioner::SetTitle(gui::IGUIElement *element)
{
	title = element;
}

void RowPositioner::Apply()
{
	if (!elements.size())
		return;
	
	// get total widths
	
	s32 sumWidths = 0;
	
	for (u32 i = 0; i < elements.size(); i ++)
		sumWidths += elements[i]->getRelativePosition().getWidth();
	
	// centred
	s32 xPos = driver->getScreenSize().Width/2;
	
	xPos -= sumWidths/2;
	xPos -= (spacing * (elements.size()-1)) / 2;
	
	for (u32 i = 0; i < elements.size(); i ++)
	{
		core::rect<s32> rect = elements[i]->getRelativePosition();
		
		SetTopLeft(elements[i],
				xPos,
				yPos - ( vertCentred ? (rect.getHeight()/2) : 0 )  // Vertically centred??
				);
		
		xPos += rect.getWidth();
		xPos += spacing;
	}
	
	// Title?
	if (title)
	{
		xPos = driver->getScreenSize().Width/2;
		core::rect<s32> rect = title->getRelativePosition();
		
		SetTopLeft(title,
				driver->getScreenSize().Width/2 - rect.getWidth()/2,
				yPos - rect.getHeight() * 2
				);
	}
}

void RowPositioner::Reset()
{
	Positioner::Reset();
	
	if (title)
	{
		title->remove();
		title = NULL;
	}
}


// ********************** ColumnPositioner *********************

ColumnPositioner::ColumnPositioner(video::IVideoDriver *driver, s32 spacing)
	: Positioner(driver, spacing)
{
}

void ColumnPositioner::Apply()
{
	if (!elements.size())
		return;
	
	// this currently just aligns to bottom left corner...
	// with a hard coded margin.
	// we use fractions of screen size too... for different screen resolutions...
	
	f32 marginLeft = 0.1;
	f32 marginBottom = 0.1;
	
	// get total height
	
	s32 totalHeight = 0;
	
	for (u32 i = 0; i < elements.size(); i ++)
		totalHeight += elements[i]->getRelativePosition().getHeight();
	
	totalHeight += spacing * (elements.size()-1);
	
	s32 xPos = (s32)(marginLeft*driver->getScreenSize().Width);
	s32 yPos = driver->getScreenSize().Height - (s32)((marginBottom*driver->getScreenSize().Height)) - totalHeight;
	
	for (u32 i = 0; i < elements.size(); i ++)
	{
		core::rect<s32> rect = elements[i]->getRelativePosition();
		
		SetTopLeft(elements[i],
				xPos,
				yPos
				);
		
		yPos += rect.getHeight();
		yPos += spacing;
	}
}


// ************* ColumnPositionerCentred ****************


ColumnPositionerCentred::ColumnPositionerCentred(video::IVideoDriver *driver, s32 spacing, f32 marginBottom)
	: Positioner(driver, spacing)
{
	this->marginBottom = marginBottom;
}

void ColumnPositionerCentred::Apply()
{
	if (!elements.size())
		return;
	
	// get total height
	
	s32 totalHeight = 0;
	
	for (u32 i = 0; i < elements.size(); i ++)
		totalHeight += elements[i]->getRelativePosition().getHeight();
	
	totalHeight += spacing * (elements.size()-1);
	
	s32 xPos = driver->getScreenSize().Width/2;
	s32 yPos = driver->getScreenSize().Height - (s32)((marginBottom*driver->getScreenSize().Height)) - totalHeight;
	
	for (u32 i = 0; i < elements.size(); i ++)
	{
		core::rect<s32> rect = elements[i]->getRelativePosition();
		
		SetTopLeft(elements[i],
				xPos - rect.getWidth()/2,
				yPos
				);
		
		yPos += rect.getHeight();
		yPos += spacing;
	}
}


// ****************** EitherSidePositioner ***************

// this is very hacked together.
// all this menu stuff is.

EitherSidePositioner::EitherSidePositioner(video::IVideoDriver *driver)
	: Positioner(driver, 0)
{
	enabledLeft = true;
	enabledRight = true;
}

void EitherSidePositioner::Enable(bool left, bool right)
{
	ASSERT(elements.size() == 2);
	
	enabledLeft = left;
	enabledRight = right;
	
	if (!enabledLeft && !enabledRight)
	{
		Reset();
		return;
	}
	
	if (!enabledLeft)
	{
		elements[0]->remove();
		elements.erase(elements.begin());
		ids.erase(ids.begin());
		return;
	}
	if (!enabledRight)
	{
		elements[1]->remove();
		elements.erase(elements.begin()+1);
		ids.erase(ids.begin()+1);
		return;
	}
}

void EitherSidePositioner::Apply()
{
	if (!elements.size())
		return;
	
	// hard coded...
	// from 0 to 0.5 (since total screen is 1.0)
	f32 screenCentreOffsetFract = 0.45;
	
	s32 yPos = driver->getScreenSize().Height/2;
	s32 screenWidth = driver->getScreenSize().Width;
	s32 screenCentreOffset = (s32)(screenWidth*screenCentreOffsetFract);
	
	for (u32 i = 0; i < elements.size(); i ++)
	{
		core::rect<s32> rect = elements[i]->getRelativePosition();
		
		if (i == 0 && enabledLeft)
		{
			SetTopLeft(elements[i],
					screenWidth/2 - screenCentreOffset, // snug against screen edge
					yPos - rect.getHeight()/2 // Vertically centred
					);
		}
		else if ((i == 1 && enabledRight) || (i == 0 && !enabledLeft && enabledRight))
		{
			SetTopLeft(elements[i],
					screenWidth/2 + screenCentreOffset - rect.getWidth(), // snug against screen edge
					yPos - rect.getHeight()/2 // Vertically centred
					);
		}
	}
}




