// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef __C_GUI_TEXT_SLIDER_H_INCLUDED__
#define __C_GUI_TEXT_SLIDER_H_INCLUDED__

#include "IGUITextSlider.h"

namespace irr
{
namespace gui
{
    class IGUIButton;
    class IGUIStaticText;

	class CGUITextSlider : public IGUITextSlider
	{
	public:

		//! constructor
		CGUITextSlider(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle, f32 buttonAspectRatio);

		//! destructor
		~CGUITextSlider();

		virtual s32 getCurrentTextId() const;
		virtual void setCurrentTextId(s32 id_);

		virtual void addText(const wchar_t* text);
		virtual void addText(const irr::core::stringw& text);
		virtual const wchar_t* getCurrentText() const;
		virtual s32 getNumTexts() const;
        virtual void clearTexts();

        virtual void setOverrideFont(IGUIFont* font=0);
        virtual void setLeftImage(video::ITexture* image);
        virtual void setLeftImagePressed(video::ITexture* image);
        virtual void setRightImage(video::ITexture* image);
        virtual void setRightImagePressed(video::ITexture* image);
        virtual void setFocusedImage(video::ITexture* image);

        //! Sets if the alpha channel should be used for drawing images on the button (default is false)
		virtual void setUseAlphaChannel(bool useAlphaChannel);

		//! Returns if the alpha channel should be used for drawing images on the button
		virtual bool getUseAlphaChannel() const;

        //! Sets the enabled state of this element.
        virtual void setEnabled(bool enabled);

        //! draws the element and its children
        virtual void draw();

        //! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

        //! when the last/first text is reached, wrap around to first/last text
		virtual void setWrapAround(bool wrap_);
		virtual bool getWrapAround(bool wrap_) const;

        //! only element returned is textslider itself. No children.
        virtual IGUIElement* getElementFromPoint(const core::position2d<s32>& point)
        {
            if (!IsVisible)
                return 0;

            if (AbsoluteClippingRect.isPointInside(point) )
                return this;

            return 0;
        }

        // Workaround for mobile-phones with small screens
        virtual void makeLargeButtons();

    protected:
        // that text is most likely not the one any other class wants to use as it's not the one displayed
        virtual const wchar_t* getText() const  { return IGUITextSlider::getText(); }

        bool hasFocus();
        void changeSlider(bool increase_);

    private:
        s32                         mCurrentTextId;
        core::array<core::stringw>  mTexts;

        IGUIStaticText *    StaticText;
        IGUIButton *        ButtonLeft;
        IGUIButton *        ButtonRight;
        video::ITexture*    FocusedImage;
        video::ITexture*    LeftButtonImage;	// Irrlicht has no getter functions yet for images so we have to keep them for the large-button hack
        video::ITexture*    LeftButtonPressedImage;
        video::ITexture*    RightButtonImage;
        video::ITexture*    RightButtonPressedImage;
        bool                UseAlphaChannel;
        bool                WrapAround;
        bool				HasLargeButtons;
	};
} // end namespace gui
} // end namespace irr

#endif // __C_GUI_TEXT_SLIDER_H_INCLUDED__

