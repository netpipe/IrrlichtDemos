// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_GUI_FONT_H_INCLUDED__
#define __I_GUI_FONT_H_INCLUDED__

#include "IUnknown.h"
#include "rect.h"
#include "irrTypes.h"
#include "SColor.h"

namespace irr
{
namespace gui
{

//! Font interface.
class IGUIFont : public virtual IUnknown
{
public:

	//! Destructor
	virtual ~IGUIFont() {};

	//! Draws an text and clips it to the specified rectangle if wanted.
	/** \param text: Text to draw
	 \param position: Rectangle specifying position where to draw the text.
	 \param color: Color of the text
	 \param hcenter: Specifiies if the text should be centered horizontally into the rectangle.
	 \param vcenter: Specifiies if the text should be centered vertically into the rectangle.
	 \param clip: Optional pointer to a rectalgle against which the text will be clipped.
	 If the pointer is null, no clipping will be done. */
	virtual void draw(const wchar_t* text, const core::rect<s32>& position, 
		video::SColor color, bool hcenter=false, bool vcenter=false,
		const core::rect<s32>* clip=0) = 0;

	//! Calculates the dimension of a text.
	/** \return Returns width and height of the area covered by the text if it would be
	  drawn. */
	virtual core::dimension2d<s32> getDimension(const wchar_t* text) = 0;

	//! Calculates the index of the character in the text which is on a specific position.
	/** \param text: Text string.
	\param pixel_x: X pixel position of which the index of the character will be returned.
	\return Returns zero based index of the character in the text, and -1 if no no character
	is on this position. (=the text is too short). */
	virtual s32 getCharacterFromPos(const wchar_t* text, s32 pixel_x) = 0;
};

} // end namespace gui
} // end namespace irr

#endif

