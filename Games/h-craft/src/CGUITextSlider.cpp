// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "CGUITextSlider.h"
#include "EGUIEventTypesHC.h"

namespace irr
{
namespace gui
{

//! constructor
CGUITextSlider::CGUITextSlider(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle, f32 buttonAspectRatio)
: IGUITextSlider(environment, parent, id, rectangle, buttonAspectRatio)
, mCurrentTextId(-1), FocusedImage(0)
, LeftButtonImage(0), LeftButtonPressedImage(0), RightButtonImage(0), RightButtonPressedImage(0)
, UseAlphaChannel(true), WrapAround(true), HasLargeButtons(false)
{
    core::rect< s32 > rectEdit((int)(rectangle.getHeight()*buttonAspectRatio)+1, 0, RelativeRect.getWidth() - ((int)(rectangle.getHeight()*buttonAspectRatio) + 1), rectangle.getHeight());
    bool border = false;
    bool wordWrap = false;
    bool fillBackground = false;
    StaticText = Environment->addStaticText(L"text", rectEdit, border, wordWrap, this, -1, fillBackground);
    StaticText->setSubElement(true);
    StaticText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    StaticText->grab();

    ButtonLeft = Environment->addButton(core::rect<s32>(0, 0, (int)(rectangle.getHeight()*buttonAspectRatio), rectangle.getHeight()), this, -1, L"<");
    ButtonLeft->grab();
    ButtonLeft->setSubElement(true);
    ButtonLeft->setTabStop(false);
	ButtonLeft->setScaleImage(true);
    ButtonRight = Environment->addButton(core::rect<s32>(RelativeRect.getWidth() - (int)(rectangle.getHeight()*buttonAspectRatio), 0, RelativeRect.getWidth(), rectangle.getHeight()), this, -1, L">");
    ButtonRight->grab();
    ButtonRight->setTabStop(false);
	ButtonRight->setScaleImage(true);
	ButtonRight->setSubElement(true);

    setUseAlphaChannel(UseAlphaChannel);
}

//! destructor
CGUITextSlider::~CGUITextSlider()
{
	if (ButtonLeft)
		ButtonLeft->drop();
    if (ButtonRight)
        ButtonRight->drop();
    if (StaticText)
        StaticText->drop();
    if (FocusedImage)
        FocusedImage->drop();
	if (LeftButtonImage)
		LeftButtonImage->drop();
	if (LeftButtonPressedImage)
		LeftButtonPressedImage->drop();
	if (RightButtonImage)
		RightButtonImage->drop();
	if (RightButtonPressedImage)
		RightButtonPressedImage->drop();
}

s32 CGUITextSlider::getCurrentTextId() const
{
    return mCurrentTextId;
}

void CGUITextSlider::setCurrentTextId(s32 id_)
{
    mCurrentTextId = id_;
    const wchar_t* text = getCurrentText();
    if ( text )
    {
        StaticText->setText(text);
    }
    else
    {
        StaticText->setText(L"");
    }
}

void CGUITextSlider::addText(const wchar_t* text)
{
    mTexts.push_back(text);
    if ( mCurrentTextId < 0 )
        setCurrentTextId(0);
}

void CGUITextSlider::addText(const irr::core::stringw& text)
{
    mTexts.push_back(text);
    if ( mCurrentTextId < 0 )
        setCurrentTextId(0);
}

const wchar_t* CGUITextSlider::getCurrentText() const
{
    if ( mCurrentTextId < 0 || mCurrentTextId >= (s32)mTexts.size() )
        return NULL;
    return mTexts[mCurrentTextId].c_str();
}

s32 CGUITextSlider::getNumTexts() const
{
    return mTexts.size();
}

void CGUITextSlider::clearTexts()
{
    mTexts.clear();
    setCurrentTextId(-1);
}

void CGUITextSlider::setOverrideFont(IGUIFont* font)
{
    if ( StaticText )
        StaticText->setOverrideFont(font);
    if (ButtonLeft)
		ButtonLeft->setOverrideFont(font);
    if (ButtonRight)
        ButtonRight->setOverrideFont(font);
}

void CGUITextSlider::setLeftImage(video::ITexture* image)
{
	if ( image )
		image->grab();
	if ( LeftButtonImage )
		LeftButtonImage->drop();
	LeftButtonImage = image;

    ButtonLeft->setImage(image);
    if ( image )
    {
        ButtonLeft->setText(NULL);
    }
    else
    {
        ButtonLeft->setText(L"<");
    }
}

void CGUITextSlider::setLeftImagePressed(video::ITexture* image)
{
	if ( image )
		image->grab();
	if ( LeftButtonPressedImage )
		LeftButtonPressedImage->drop();
	LeftButtonPressedImage = image;

    ButtonLeft->setPressedImage(image);
}

void CGUITextSlider::setRightImage(video::ITexture* image)
{
	if ( image )
		image->grab();
	if ( RightButtonImage )
		RightButtonImage->drop();
	RightButtonImage = image;

    ButtonRight->setImage(image);
    if ( image )
    {
        ButtonRight->setText(NULL);
    }
    else
    {
        ButtonRight->setText(L">");
    }
}

void CGUITextSlider::setRightImagePressed(video::ITexture* image)
{
	if ( image )
		image->grab();
	if ( RightButtonPressedImage )
		RightButtonPressedImage->drop();
	RightButtonPressedImage = image;

    ButtonRight->setPressedImage(image);
}

void CGUITextSlider::setFocusedImage(video::ITexture* image)
{
	if ( image  )
		image->grab();
    if (FocusedImage)
		FocusedImage->drop();

	FocusedImage = image;
}

//! Sets the enabled state of this element.
void CGUITextSlider::setEnabled(bool enabled)
{
    IGUITextSlider::setEnabled(enabled);
    if (ButtonLeft)
		ButtonLeft->setEnabled(enabled);
    if (ButtonRight)
        ButtonRight->setEnabled(enabled);
    if (StaticText)
        StaticText->setEnabled(enabled);
}

bool CGUITextSlider::hasFocus()
{
    if (     Environment->hasFocus(this)
            ||  (ButtonLeft && Environment->hasFocus(ButtonLeft))
            ||  (ButtonRight && Environment->hasFocus(ButtonRight))
            ||  (StaticText && Environment->hasFocus(StaticText)))
        return true;
    return false;
}

void CGUITextSlider::draw()
{
    IGUITextSlider::draw();

    if (    hasFocus()
        &&  FocusedImage )
    {
        irr::video::IVideoDriver* driver = Environment->getVideoDriver();

        core::rect<s32> rect(AbsoluteRect);
        core::rect<s32> *clip = &AbsoluteClippingRect;
        core::rect<s32> focusedRect = core::rect<s32>(core::position2d<s32>(0,0), FocusedImage->getSize());

		if ( HasLargeButtons  )
		{
			core::rect<s32> rectButton(ButtonLeft->getRelativePosition());

			rect.UpperLeftCorner.Y += rect.getHeight()/4 ;
			rect.LowerRightCorner.Y += rect.getHeight()/4 - rect.getHeight()/2;
			rect.UpperLeftCorner.X += rectButton.getWidth()/2;
			rect.LowerRightCorner.X -= rectButton.getWidth()/2;
		}

        driver->draw2DImage(FocusedImage, rect, focusedRect, clip,
                NULL, UseAlphaChannel);
    }
}

bool CGUITextSlider::OnEvent(const SEvent& event)
{
    if (!IsEnabled)
		return Parent ? Parent->OnEvent(event) : false;

    bool buttonClicked = false;
    switch(event.EventType)
	{
        case EET_GUI_EVENT:
            if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
            {
                if (event.GUIEvent.Caller == ButtonLeft)
                {
                    buttonClicked  = true;
                    changeSlider(false);
                }
                else if ( event.GUIEvent.Caller == ButtonRight)
                {
                    buttonClicked = true;
                    changeSlider(true);
                }
            }
		break;
        case EET_MOUSE_INPUT_EVENT:
        {
            if (    event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN
                ||  event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
            {
                bool buttonHandledIt = false;
                core::position2d<s32> mousePos(event.MouseInput.X, event.MouseInput.Y);
                if (ButtonLeft && (ButtonLeft->isPointInside(mousePos) || event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) && ButtonLeft->OnEvent(event) )
                    buttonHandledIt = true;
                if (ButtonRight && (ButtonRight->isPointInside(mousePos) || event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) && ButtonRight->OnEvent(event) )
                    buttonHandledIt = true;
                if ( buttonHandledIt )
                    return true;
            }
        }
        break;
        case EET_KEY_INPUT_EVENT:
        {
            if (event.KeyInput.PressedDown && isEnabled() )
            {
                if ( event.KeyInput.Key == KEY_LEFT )
                {
                    changeSlider(false);
                    return true;
                }
                else if ( event.KeyInput.Key == KEY_RIGHT )
                {
                    changeSlider(true);
                    return true;
                }
            }
        }
        break;
        default:
        break;
	}

    if ( buttonClicked )
    {
        return true;
    }


    return Parent ? Parent->OnEvent(event) : false;
}

void CGUITextSlider::changeSlider(bool increase_)
{
    bool changeEvent = false;
    if ( increase_ )
    {
        if ( mCurrentTextId >= 0 && mCurrentTextId+1 < (s32)mTexts.size() )
        {
            setCurrentTextId(mCurrentTextId+1);
            changeEvent = true;
        }
        else if ( WrapAround && mCurrentTextId >= 0 && mCurrentTextId+1 == (s32)mTexts.size() )
        {
            setCurrentTextId(0);
            changeEvent = true;
        }
    }
    else
    {
        if ( mCurrentTextId > 0 )
        {
            setCurrentTextId(mCurrentTextId-1);
            changeEvent = true;
        }
        else if ( WrapAround && 0 == mCurrentTextId )
        {
            setCurrentTextId(mTexts.size()-1);
            changeEvent = true;
        }
    }

	if ( changeEvent )
	{
        SEvent e;
        e.EventType = EET_GUI_EVENT;
        e.GUIEvent.Caller = this;
        //fprintf(stderr, "EGET_SPINBOX_CHANGED caller:%p id: %d\n", e.GUIEvent.Caller, e.GUIEvent.Caller->getID() );
        e.GUIEvent.EventType = (EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED;
        if ( Parent )
            Parent->OnEvent(e);
	}
}

void CGUITextSlider::setUseAlphaChannel(bool useAlphaChannel)
{
    UseAlphaChannel = useAlphaChannel;
    if ( ButtonLeft )
        ButtonLeft->setUseAlphaChannel(useAlphaChannel);
    if ( ButtonRight )
        ButtonRight->setUseAlphaChannel(useAlphaChannel);
}

bool CGUITextSlider::getUseAlphaChannel() const
{
    return UseAlphaChannel;
}

void CGUITextSlider::setWrapAround(bool wrap_)
{
    WrapAround = wrap_;
}

bool CGUITextSlider::getWrapAround(bool wrap_) const
{
    return WrapAround;
}

void CGUITextSlider::makeLargeButtons()
{
	if ( !HasLargeButtons )
	{
		HasLargeButtons = true;
		core::rect<s32> rectElement(getRelativePosition());
		core::rect<s32> rectButtonLeft(ButtonLeft->getRelativePosition());
		core::rect<s32> rectButtonRight(ButtonRight->getRelativePosition());
		s32 incWidth = (s32)((float)rectButtonLeft.getWidth()); // also has to be regarded in ::Draw
		rectElement.UpperLeftCorner.Y -= rectButtonLeft.getHeight()/2;
		rectElement.LowerRightCorner.Y += rectButtonLeft.getHeight()/2;
		setRelativePosition(rectElement);

		core::rect<s32> rectStatic(StaticText->getRelativePosition());
		rectStatic.UpperLeftCorner.Y += rectButtonLeft.getHeight()/2;
		rectStatic.LowerRightCorner.Y += rectButtonLeft.getHeight()/2;
		StaticText->setRelativePosition(rectStatic);

		rectButtonRight.UpperLeftCorner.X -= incWidth;
		rectButtonRight.LowerRightCorner.Y += rectButtonLeft.getHeight();
		ButtonRight->setRelativePosition(rectButtonRight);

		rectButtonLeft.LowerRightCorner.X += incWidth;
		rectButtonLeft.LowerRightCorner.Y += rectButtonLeft.getHeight();
		ButtonLeft->setRelativePosition(rectButtonLeft);

	}
}

} // end namespace gui
} // end namespace irr
