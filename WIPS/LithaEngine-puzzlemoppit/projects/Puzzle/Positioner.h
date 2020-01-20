
#ifndef POSITIONER_H
#define POSITIONER_H

#include "Litha.h"
#include <vector>

class Positioner
{
protected:
	video::IVideoDriver *driver;
	
	std::vector<gui::IGUIElement *> elements;
	std::vector<s32> ids;
	
	s32 spacing;
	
	void SetTopLeft(gui::IGUIElement *element, u16 x, u16 y);
	void SetTopRight(gui::IGUIElement *element, u16 x, u16 y);
	
public:
	Positioner(video::IVideoDriver *driver, s32 spacing);
	
	void Add(gui::IGUIElement *element, s32 id);
	void SetSpacing(s32 spacing) { this->spacing = spacing; }
	
	virtual void Reset();
	virtual void Apply() = 0;
	
	// returns an id of the first element which the mouse is over
	// (we assume that elements will not intersect!)
	// -1 if none.
	s32 GetMouseOverId();
	
	// or NULL
	gui::IGUIElement *GetMouseOverElement();
	
	// This disabled as it's possible multiple buttons will share an ID.
	//gui::IGUIElement *GetElementById(s32 id);
	
	const std::vector<gui::IGUIElement *> &GetElements();
};

class RowPositioner : public Positioner
{
	s32 yPos;
	bool vertCentred;
	gui::IGUIElement *title;
	
public:
	// if not vertCentred, yPos is the top
	RowPositioner(video::IVideoDriver *driver, s32 yPos, s32 spacing, bool vertCentred = true);
	
	// a "title" element that appears above the row...
	void SetTitle(gui::IGUIElement *element);
	
	void Apply();
	void Reset();
};

class ColumnPositioner : public Positioner
{
public:
	ColumnPositioner(video::IVideoDriver *driver, s32 spacing);
	
	void Apply();
};

class ColumnPositionerCentred : public Positioner
{
	f32 marginBottom;
	
public:
	ColumnPositionerCentred(video::IVideoDriver *driver, s32 spacing, f32 marginBottom = 0.1);
	
	void Apply();
};

class EitherSidePositioner : public Positioner
{
	bool enabledLeft,enabledRight;
	
public:
	EitherSidePositioner(video::IVideoDriver *driver);
	
	// this is really hacked in.
	// call only after two items have been added
	// and before Apply.
	void Enable(bool left, bool right);
	
	void Apply();
};

#endif

