// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_GUI_BUTTON_H_INCLUDED__
#define __I_GUI_BUTTON_H_INCLUDED__

#include "IGUIElement.h"

namespace irr
{

namespace video
{
	class ITexture;
}

namespace gui
{
	class IGUIFont;

	//! GUI Button interface.
	class IGUIButton : public IGUIElement
	{
	public:

		//! constructor
		IGUIButton(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
			: IGUIElement(EGUIET_BUTTON, environment, parent, id, rectangle) {}

		//! destructor
		~IGUIButton() {};

		//! Sets another skin independent font.
		/** If this is set to zero, the button uses the font of the skin.
		 \param font: New font to set. */
		virtual void setOverrideFont(IGUIFont* font=0) = 0;

		//! Sets an image which should be displayed on the button when it is in normal state. 
		/** \param image: Image to be displayed */
		virtual void setImage(video::ITexture* image) = 0;

		//! Sets an image which should be displayed on the button when it is in normal state. 
		/** \param image: Texture containing the image to be displayed
		 \param pos: Position in the texture, where the image is located */
		virtual void setImage(video::ITexture* image, const core::rect<s32>& pos) = 0;

		//! Sets an image which should be displayed on the button when it is in pressed state. 
		/** If no images is specified for the pressed state via
		setPressedImage(), this image is also drawn in pressed state.
		\param image: Image to be displayed */
		virtual void setPressedImage(video::ITexture* image) = 0;

		//! Sets an image which should be displayed on the button when it is in pressed state. 
		/** \param image: Texture containing the image to be displayed
		 \param pos: Position in the texture, where the image is located */
		virtual void setPressedImage(video::ITexture* image, const core::rect<s32>& pos) = 0;

		//! Sets if the button should behave like a push button. 
		/** Which means it can be in two states: Normal or Pressed. With a click on the button,
		the user can change the state of the button. */
		virtual void setIsPushButton(bool isPushButton) = 0;

		//! Sets the pressed state of the button if this is a pushbutton
		virtual void setPressed(bool pressed) = 0;

		//! Returns if the button is currently pressed
		virtual bool isPressed() = 0;

		//! Sets if the alpha channel should be used for drawing images on the button (default is false)
		virtual void setUseAlphaChannel(bool useAlphaChannel) = 0;

		//! Returns if the alpha channel should be used for drawing images on the button
		virtual bool getUseAlphaChannel() = 0;
	};


} // end namespace gui
} // end namespace irr

#endif

