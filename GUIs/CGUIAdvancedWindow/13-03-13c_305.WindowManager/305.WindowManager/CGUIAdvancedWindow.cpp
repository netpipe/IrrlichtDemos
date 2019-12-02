// Xterm-in'Hate

#include "CGUIAdvancedWindow.h"
//#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIButton.h"
#include "IGUIFont.h"
#include "IGUIFontBitmap.h"

//#include<iostream> // DEBUG

namespace irr
{
namespace gui
{

//! constructor
CGUIAdvancedWindow::CGUIAdvancedWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle,
    const wchar_t *text, const core::array<buttoninfo>& buttons, EWINDOW_CLOSE close, const core::position2di& minimized, ICursorControl* cursor, video::ITexture* texture)
: IGUIElement(EGUIET_WINDOW ,environment, parent, id, rectangle),
Skin(environment->getSkin()), DefaultSprites(Skin->getSpriteBank()), CurrentIconColor(video::SColor(255,255,255,255)),
ButtonInfo(buttons), ButtonRectangles(buttons.size()), Buttons(buttons.size()),
Dragging(false), IsDraggable(true), DrawBackground(true), DrawTitlebar(true), IsActive(false), IsMinimized(false), IsBar(false),
IsMinimizable(true), CloseHandling(close), NormalRectangle(rectangle),
MinimizedRectangle(minimized,core::position2di(minimized.X+rectangle.getWidth(),minimized.Y+21)),
BarRectangle(rectangle.UpperLeftCorner,core::position2di(rectangle.LowerRightCorner.X,rectangle.UpperLeftCorner.Y+21)),
UseGradient((Skin->getType() == EGST_WINDOWS_METALLIC) || (Skin->getType() == EGST_BURNING_SKIN)), Cursor(cursor),
IsNotifying(false), NotifyTimer(0), NotifyState(false), Texture(texture)
{
	#ifdef _DEBUG
	setDebugName("CGUIAdvancedWindow");
	#endif
    setText(text);
    // create the buttons
	s32 buttonw = Skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
	s32 posx = RelativeRect.getWidth() - buttonw - 4;
    for (size_t index=0; index!=ButtonInfo.size(); ++index)
    {
        ButtonRectangles.push_back(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw));
        posx -= buttonw + 2;
        switch(ButtonInfo[index].Type)
        {
        case EWBT_CLOSE:
            {
                Buttons.push_back(Environment->addButton(ButtonRectangles[index], this, -1,
                    L"", Skin->getDefaultText(EGDT_WINDOW_CLOSE) ));
            }
            break;
        case EWBT_MINIMIZE:
            {
                Buttons.push_back(Environment->addButton(ButtonRectangles[index], this, -1,
                    L"", Skin->getDefaultText(EGDT_WINDOW_MINIMIZE) ));
            }
            break;
        case EWBT_PIN:
            {
                Buttons.push_back(Environment->addButton(ButtonRectangles[index], this, -1,
                    L"", L"Pin " ));             /// \todo Skin lack of a pin button text. Must upgrade the Irrlicht source code.
            }
            break;
        case EWBT_USER_DEFINED:
            {
                Buttons.push_back(Environment->addButton(ButtonRectangles[index], this, ButtonInfo[index].UserEventId,
                    ButtonInfo[index].Text, ButtonInfo[index].ToolTipText ));             /// \todo Skin lack of a pin button text. Must upgrade the Irrlicht source code.
            }
            break;
        }
        Buttons[index]->setSubElement(true);
        Buttons[index]->setSubElement(true);
        Buttons[index]->setTabStop(false);
        Buttons[index]->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
        Buttons[index]->grab();
    }
	// this element is a tab group
	setTabGroup(true);
	setTabStop(true);
	setTabOrder(-1);
    // refresh all
	refreshSprites();
	refreshButtons();
	updateClientRect();
}

//! destructor
CGUIAdvancedWindow::~CGUIAdvancedWindow()
{
    for (size_t index=0; index!=Buttons.size(); ++index)
    {
        Buttons[index]->drop();
    }
}

void CGUIAdvancedWindow::refreshSprites()
{
	CurrentIconColor = Skin->getColor(isEnabled() ? (EGUI_DEFAULT_COLOR)EGAWC_ACTIVE_WINDOW_SYMBOL : (EGUI_DEFAULT_COLOR)EGAWC_INACTIVE_WINDOW_SYMBOL);
    for (size_t index=0; index!=Buttons.size(); ++index)
    {
        switch(ButtonInfo[index].Type)
        {
        case EWBT_CLOSE:
            {
                Buttons[index]->setSpriteBank(DefaultSprites);
                Buttons[index]->setSprite(EGBS_BUTTON_UP, Skin->getIcon(EGDI_WINDOW_CLOSE), CurrentIconColor);
                Buttons[index]->setSprite(EGBS_BUTTON_DOWN, Skin->getIcon(EGDI_WINDOW_CLOSE), CurrentIconColor);
            }
            break;
        case EWBT_MINIMIZE:
            {
                Buttons[index]->setSpriteBank(DefaultSprites);
                if (IsMinimized)
                {
                    Buttons[index]->setSprite(EGBS_BUTTON_UP, Skin->getIcon(EGDI_WINDOW_RESTORE), CurrentIconColor);
                    Buttons[index]->setSprite(EGBS_BUTTON_DOWN, Skin->getIcon(EGDI_WINDOW_RESTORE), CurrentIconColor);
                }
                else
                {
                    Buttons[index]->setSprite(EGBS_BUTTON_UP, Skin->getIcon(EGDI_WINDOW_MINIMIZE), CurrentIconColor);
                    Buttons[index]->setSprite(EGBS_BUTTON_DOWN, Skin->getIcon(EGDI_WINDOW_MINIMIZE), CurrentIconColor);
                }
            }
            break;
        case EWBT_PIN:
            {
                Buttons[index]->setSpriteBank(DefaultSprites);
                if (IsDraggable)
                {
                    Buttons[index]->setSprite(EGBS_BUTTON_UP, Skin->getIcon(EGDI_RADIO_BUTTON_CHECKED), CurrentIconColor);
                    Buttons[index]->setSprite(EGBS_BUTTON_DOWN, Skin->getIcon(EGDI_RADIO_BUTTON_CHECKED), CurrentIconColor);
                }
                else
                {
                    Buttons[index]->setSprite(EGBS_BUTTON_UP, Skin->getIcon(EGDI_EXPAND), CurrentIconColor);
                    Buttons[index]->setSprite(EGBS_BUTTON_DOWN, Skin->getIcon(EGDI_EXPAND), CurrentIconColor);
                }
            }
            break;
        case EWBT_USER_DEFINED:
            {
                Buttons[index]->setSpriteBank(ButtonInfo[index].Sprite);
                Buttons[index]->setSprite(EGBS_BUTTON_UP, ButtonInfo[index].SpriteIndex, CurrentIconColor);
                Buttons[index]->setSprite(EGBS_BUTTON_DOWN, ButtonInfo[index].SpriteIndex, CurrentIconColor);
            }
            break;
        }
    }
}

u32 CGUIAdvancedWindow::refreshButtons()
{
    u32 position = 0;
    for (size_t index=0; index!=Buttons.size(); ++index)
    {
        core::rect<s32> rectangle = ButtonRectangles[position];
        if( isMinimized())
        {
            rectangle = ButtonRectangles[position]+core::position2di(MinimizedRectangle.getWidth()-NormalRectangle.getWidth(),0);
        }
        switch(ButtonInfo[index].Type)
        {
        case EWBT_CLOSE:
        case EWBT_PIN:
        case EWBT_USER_DEFINED:
            {
                if ( (ButtonInfo[index].VisibleWhenNormal && !IsMinimized && !IsBar )
                     || (ButtonInfo[index].VisibleWhenMinimized && IsMinimized)
                     || (ButtonInfo[index].VisibleWhenBar && IsBar && !IsMinimized) )
                {
                    Buttons[index]->setVisible(true);
                    Buttons[index]->setRelativePosition(rectangle);
                    ++position;
                }
                else
                {
                    Buttons[index]->setVisible(false);
                }
            }
            break;
        case EWBT_MINIMIZE:
            {
                Buttons[index]->setVisible(true);
                    Buttons[index]->setRelativePosition(rectangle);
                    ++position;
            }
            break;
        }
    }
    return position;
}

//! called if an event happened.
bool CGUIAdvancedWindow::OnEvent(const SEvent& event)
{
	if (isEnabled())
	{
		switch(event.EventType)
		{
		case EET_GUI_EVENT:
			if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
			{
				Dragging = false;
				IsActive = false;
			}
			else
			if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED)
			{
				if (Parent && ((event.GUIEvent.Caller == this) || isMyChild(event.GUIEvent.Caller)))
				{
					Parent->bringToFront(this);
					IsActive = true;
				}
				else
				{
					IsActive = false;
				}
			}
			else
			if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
			{
			    for (size_t index=0; index!=Buttons.size(); ++index)
                {
                    if (Buttons[index]==event.GUIEvent.Caller)
                    {
                      switch(ButtonInfo[index].Type)
                        {
                        case EWBT_CLOSE:
                            {
                                if (Parent)
                                {
                                    // send close event to parent
                                    SEvent e;
                                    e.EventType = EET_GUI_EVENT;
                                    e.GUIEvent.Caller = this;
                                    e.GUIEvent.Element = 0;
                                    e.GUIEvent.EventType = EGET_ELEMENT_CLOSED;

                                    // if the event was not absorbed
                                    if (!Parent->OnEvent(e))
                                        close();

                                    return true;

                                }
                                else
                                {
                                    close();
                                    return true;
                                }
                            }
                            break;
                        case EWBT_PIN:
                            {
                                IsDraggable = ! IsDraggable;
                            }
                            break;
                        case EWBT_USER_DEFINED:
                            {
                                SEvent e;
                                e.EventType = EET_GUI_EVENT;
                                e.GUIEvent.Caller = Buttons[index];
                                e.GUIEvent.Element = 0;
                                e.GUIEvent.EventType = EGET_BUTTON_CLICKED;
                                // send this event to the user event receiver
                                Parent->OnEvent(e);
                            }
                            return true;
                        case EWBT_MINIMIZE:
                            {
                                setMinimized(!IsMinimized);
                            }
                            break;
                        }
                    }
                }
			}
			break;

		case EET_MOUSE_INPUT_EVENT:
            IsNotifying = false;
			switch(event.MouseInput.Event)
			{
            case EMIE_LMOUSE_DOUBLE_CLICK:
                {
                    // do not process double click on buttons
                    for (size_t index=0; index!=Buttons.size(); ++index)
                    {
                        if (Buttons[index]->isVisible() && Buttons[index]->getAbsolutePosition().isPointInside(core::position2di(event.MouseInput.X,event.MouseInput.Y)))
                            return true;
                    }
                    if( IsMinimized )
                    {
                        setMinimized(!IsMinimized);
                    }
                    else if(IsBar)
                    {
                        IsBar = false;
                        NormalRectangle = core::rect<s32>(
                            getRelativePosition().UpperLeftCorner.X,
                            getRelativePosition().UpperLeftCorner.Y,
                            getRelativePosition().LowerRightCorner.X,
                            getRelativePosition().UpperLeftCorner.Y+NormalRectangle.LowerRightCorner.Y-NormalRectangle.UpperLeftCorner.Y);
                        setRelativePosition(NormalRectangle);
                    }
                    else
                    {
                        IsBar = true;
                        NormalRectangle = getRelativePosition();
                        BarRectangle = core::rect<s32>(NormalRectangle.UpperLeftCorner.X,NormalRectangle.UpperLeftCorner.Y,NormalRectangle.LowerRightCorner.X,NormalRectangle.UpperLeftCorner.Y+21);
                        setRelativePosition(BarRectangle);

                    }
                }
                return true;
			case EMIE_LMOUSE_PRESSED_DOWN:

				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				Dragging = IsDraggable && !IsMinimized;
				if (Parent)
					Parent->bringToFront(this);
				return true;
			case EMIE_LMOUSE_LEFT_UP:
				Dragging = false;
				return true;
			case EMIE_MOUSE_MOVED:
				if (!event.MouseInput.isLeftPressed())
					Dragging = false;

				if (Dragging)
				{
					// gui window should not be dragged outside its parent
					if (Parent &&
						(event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
							event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
							event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
							event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1))
						return true;

                    core::position2d<s32> delta = core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y);
                    move(delta);
                    if (getRelativePosition().UpperLeftCorner.X<5)
                        move(core::position2d<s32>(-getRelativePosition().UpperLeftCorner.X,0));
                    if (getRelativePosition().UpperLeftCorner.Y<5)
                        move(core::position2d<s32>(0,-getRelativePosition().UpperLeftCorner.Y));
                    if (getRelativePosition().LowerRightCorner.X>Parent->getAbsolutePosition().LowerRightCorner.X-5)
                        move(core::position2d<s32>(Parent->getAbsolutePosition().LowerRightCorner.X-getRelativePosition().LowerRightCorner.X,0));
                    if (getRelativePosition().LowerRightCorner.Y>Parent->getAbsolutePosition().LowerRightCorner.Y-5)
                        move(core::position2d<s32>(0,Parent->getAbsolutePosition().LowerRightCorner.Y-getRelativePosition().LowerRightCorner.Y));
					DragStart.X = event.MouseInput.X;
					DragStart.Y = event.MouseInput.Y;
					return true;
				}
				break;
			default:
				break;
			}
		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);
}

//! Updates the absolute position.
void CGUIAdvancedWindow::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();
}

//! draws the element and its children
void CGUIAdvancedWindow::draw()
{
	if (IsVisible)
	{
		// update each time because the skin is allowed to change this always.
		updateClientRect();
        refreshSprites();
        u32 buttonCount = refreshButtons();

		core::rect<s32> rect = AbsoluteRect;

		// draw body fast
		if (DrawBackground)
		{
			rect = draw3DWindowBackgroundMouseLightning(this, DrawTitlebar,
					Skin->getColor( (IsActive&&IsDraggable) || (NotifyState&&IsNotifying) ? (EGUI_DEFAULT_COLOR)EGAWC_ACTIVE_TITTLE_BAR : (EGUI_DEFAULT_COLOR)EGAWC_INACTIVE_TITTLE_BAR),
					AbsoluteRect, &AbsoluteClippingRect);

			if (DrawTitlebar && Text.size())
			{
				rect.UpperLeftCorner.X += Skin->getSize(EGDS_TITLEBARTEXT_DISTANCE_X);
				rect.UpperLeftCorner.Y += Skin->getSize(EGDS_TITLEBARTEXT_DISTANCE_Y);
				rect.LowerRightCorner.X -= (Skin->getSize(EGDS_WINDOW_BUTTON_WIDTH)+2)*buttonCount + 4;

				IGUIFont* font = Skin->getFont(EGDF_WINDOW);
				if (font)
				{
					font->draw(Text.c_str(), rect,
							Skin->getColor( (IsActive&&IsDraggable) || (NotifyState&&IsNotifying) ? (EGUI_DEFAULT_COLOR)EGAWC_ACTIVE_TITTLE_TEXT : (EGUI_DEFAULT_COLOR)EGAWC_INACTIVE_TITTLE_TEXT),
							false, true, &rect); //AbsoluteClippingRect
				}
			}
		}
	}
	IGUIElement::draw();
}

void LightCoef( const core::rect<s32>& rect, const core::vector2df& center, const core::vector2df& light, f32* coef )
{
    core::vector2df corner[4];
    corner[0] = (core::vector2df(rect.UpperLeftCorner.X,rect.UpperLeftCorner.Y)-center).normalize();
    corner[1] = (core::vector2df(rect.LowerRightCorner.X,rect.UpperLeftCorner.Y)-center).normalize();
    corner[2] = (core::vector2df(rect.UpperLeftCorner.X,rect.LowerRightCorner.Y)-center).normalize();
    corner[3] = (core::vector2df(rect.LowerRightCorner.X,rect.LowerRightCorner.Y)-center).normalize();

    coef[0]= 0.5*corner[0].dotProduct(light)+0.5;
    coef[1]= 0.5*corner[1].dotProduct(light)+0.5;
    coef[2]= 0.5*corner[2].dotProduct(light)+0.5;
    coef[3]= 0.5*corner[3].dotProduct(light)+0.5;
}

//void LightAlpha( const core::rect<s32>& rect, const core::vector2df& center, const core::vector2df& light,
//    const video::SColor& ocolor, video::SColor * color )
//{
//    f32 coef[4];
//    LightCoef( rect, center, light, coef );
//    color[0] = video::SColor( ocolor.getAlpha()*(1-coef[0]), ocolor.getRed(), ocolor.getBlue(), ocolor.getGreen() );
//    color[1] = video::SColor( ocolor.getAlpha()*(1-coef[1]), ocolor.getRed(), ocolor.getBlue(), ocolor.getGreen() );
//    color[2] = video::SColor( ocolor.getAlpha()*(1-coef[2]), ocolor.getRed(), ocolor.getBlue(), ocolor.getGreen() );
//    color[3] = video::SColor( ocolor.getAlpha()*(1-coef[3]), ocolor.getRed(), ocolor.getBlue(), ocolor.getGreen() );
//}

void LightInterpolate( const core::rect<s32>& rect, const core::vector2df& center, const core::vector2df& light,
    const video::SColor& ocolor, const video::SColor& colorTo, video::SColor * color )
{
    f32 coef[4];
    LightCoef( rect, center, light, coef );
    color[0]=ocolor.getInterpolated(colorTo,coef[0]);
    color[1]=ocolor.getInterpolated(colorTo,coef[1]);
    color[2]=ocolor.getInterpolated(colorTo,coef[2]);
    color[3]=ocolor.getInterpolated(colorTo,coef[3]);
}

//! draws a window background
// return where to draw title bar text.
core::rect<s32> CGUIAdvancedWindow::draw3DWindowBackgroundMouseLightning(IGUIElement* element,
				bool drawTitleBar, video::SColor titleBarColor,
				const core::rect<s32>& r,
				const core::rect<s32>* clip,
				core::rect<s32>* checkClientArea)
{
    video::IVideoDriver* Driver = Environment->getVideoDriver();
	if (!Driver)
	{
		if ( checkClientArea )
		{
			*checkClientArea = r;
		}
		return r;
	}

	core::rect<s32> rect = r;

	/// init light algorithm
    core::vector2df center = core::vector2df(rect.getCenter().X,rect.getCenter().Y);
	core::vector2df light = core::vector2df(
        Cursor->getRelativePosition().X*Parent->getAbsolutePosition().getWidth()-center.X,
        Cursor->getRelativePosition().Y*Parent->getAbsolutePosition().getHeight()-center.Y
    ).normalize();
    video::SColor color[4];

	// top border
	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	rect.LowerRightCorner.Y = r.UpperLeftCorner.Y + 1;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_HIGH_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_DARK_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}
	// inner top border
	rect.UpperLeftCorner.X = r.UpperLeftCorner.X + 1;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y + 1;
	rect.LowerRightCorner.X = r.LowerRightCorner.X - 1;
	rect.LowerRightCorner.Y = r.UpperLeftCorner.Y + 2;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}
	// left border
	rect.UpperLeftCorner.X = r.UpperLeftCorner.X ;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
	rect.LowerRightCorner.X = r.UpperLeftCorner.X + 1;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_HIGH_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_DARK_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}
	// left border
	rect.UpperLeftCorner.X = r.UpperLeftCorner.X + 1;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y + 1;
	rect.LowerRightCorner.X = r.UpperLeftCorner.X + 2;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y - 1;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}
	// right border dark outer line
	rect.UpperLeftCorner.X = r.LowerRightCorner.X - 1;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	rect.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_HIGH_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_DARK_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}

	// right border bright innner line
	rect.UpperLeftCorner.X -= 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y += 1;
	rect.LowerRightCorner.Y -= 1;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}

	// bottom border dark outer line
	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_HIGH_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_DARK_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}

	// bottom border bright inner line
	rect.UpperLeftCorner.X += 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y -= 1;
	rect.LowerRightCorner.Y -= 1;
	if ( !checkClientArea )
	{
        LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_SHADOW), color);
        Driver->draw2DRectangle(
            rect,
            color[0],
            color[1],
            color[2],
            color[3],
            clip);
	}

	// client area for background
	rect = r;
	rect.UpperLeftCorner.X +=2;
	rect.UpperLeftCorner.Y +=2;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y -= 2;
	if (checkClientArea)
	{
		*checkClientArea = rect;
	}

	if ( !checkClientArea )
	{
		if (!UseGradient)
		{
			Driver->draw2DRectangle(Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_LIGHT), rect, clip);
		}
		else if ( Skin->getType()  == EGST_BURNING_SKIN )
		{
			const video::SColor c1 = Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_LIGHT).getInterpolated ( 0xFFFFFFFF, 0.9f );
			const video::SColor c2 = Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_LIGHT).getInterpolated ( 0xFFFFFFFF, 0.8f );

			Driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
		}
		else
		{
            video::SColor color[4];
            LightInterpolate(rect, center, light, Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_LIGHT), Skin->getColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_SHADOW), color);
            if(Texture)
            {
                video::SColor icolor[4] = {color[0], color[2], color[3], color[1]};
                Driver->draw2DImage(
                Texture,
                rect,
                core::rect<s32>(0,0,rect.getWidth(),rect.getHeight()),
                clip,
                icolor,
                false
                );
            }
            else
                Driver->draw2DRectangle(
                rect,
                color[0],
                color[1],
                color[2],
                color[3],
                clip);
		}
	}

	// title bar
	rect = r;
	rect.UpperLeftCorner.X += 2;
	rect.UpperLeftCorner.Y += 2;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + Skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 2;

	if (drawTitleBar )
	{
		if (checkClientArea)
		{
			(*checkClientArea).UpperLeftCorner.Y = rect.LowerRightCorner.Y;
		}
		else
		{
			// draw title bar
			if (!UseGradient)
				Driver->draw2DRectangle(titleBarColor, rect, clip);
			else
			if ( Skin->getType() == EGST_BURNING_SKIN )
			{
				const video::SColor c = titleBarColor.getInterpolated( video::SColor(titleBarColor.getAlpha(),255,255,255), 0.8f);
				Driver->draw2DRectangle(rect, titleBarColor, titleBarColor, c, c, clip);
			}
			else
			{
				const video::SColor c = titleBarColor.getInterpolated(video::SColor(titleBarColor.getAlpha(),0,0,0), 0.2f);
				Driver->draw2DRectangle(rect, titleBarColor, c, titleBarColor, c, clip);
			}
		}
	}

	return rect;
}

//! Returns true if the window is draggable, false if not
bool CGUIAdvancedWindow::isDraggable() const
{
	return IsDraggable;
}

//! Sets whether the window is draggable
void CGUIAdvancedWindow::setDraggable(bool draggable)
{
	IsDraggable = draggable;

	if (Dragging && !IsDraggable)
		Dragging = false;
}

//! Set if the window background will be drawn
void CGUIAdvancedWindow::setDrawBackground(bool draw)
{
	DrawBackground = draw;
}

//! Get if the window background will be drawn
bool CGUIAdvancedWindow::getDrawBackground() const
{
	return DrawBackground;
}

//! Set if the window titlebar will be drawn
void CGUIAdvancedWindow::setDrawTitlebar(bool draw)
{
	DrawTitlebar = draw;
}


//! Get if the window titlebar will be drawn
bool CGUIAdvancedWindow::getDrawTitlebar() const
{
	return DrawTitlebar;
}

void CGUIAdvancedWindow::updateClientRect()
{
	if (! DrawBackground )
	{
		ClientRect = core::rect<s32>(0,0, AbsoluteRect.getWidth(), AbsoluteRect.getHeight());
		return;
	}
	IGUISkin* skin = Environment->getSkin();
	skin->draw3DWindowBackground(this, DrawTitlebar,
			skin->getColor(IsActive&&IsDraggable ? EGDC_ACTIVE_BORDER : EGDC_INACTIVE_BORDER),
			AbsoluteRect, &AbsoluteClippingRect, &ClientRect);
	ClientRect -= AbsoluteRect.UpperLeftCorner;
}


//! Returns the rectangle of the drawable area (without border, without titlebar and without scrollbars)
core::rect<s32> CGUIAdvancedWindow::getClientRect() const
{
	return ClientRect;
}

//! Implementation of window close behavior
void CGUIAdvancedWindow::close()
{
    switch(CloseHandling)
    {
    case EWC_IGNORE:
        break;
    case EWC_REMOVE:
        remove();
        break;
    case EWC_HIDE:
        setVisible(false);
        break;
    }
}

void CGUIAdvancedWindow::notify(bool force)
{
    if(IsBar || IsMinimized || force)
    {
        IsNotifying = true;
        NotifyState = true;
        NotifyTimer = 0;
    }
}

void CGUIAdvancedWindow::OnPostRender(u32 timeMs)
{
    if ( IsNotifying && NotifyTimer < timeMs)
    {
        NotifyTimer = timeMs + 1000;
        NotifyState = ! NotifyState;
    }
    IGUIElement::OnPostRender(timeMs);
}

bool CGUIAdvancedWindow::isMinimized() const
{
    return IsMinimized;
}

void CGUIAdvancedWindow::setMinimized(bool minimize)
{
    if( IsMinimized == minimize )
        return;

    if(!IsMinimizable)
        return;

    if (IsMinimized)
    {
        if (IsBar)
        {
            setRelativePosition(BarRectangle);
        }
        else
        {
            setRelativePosition(NormalRectangle);
        }
    }
    else
    {
        if (IsBar)
        {
            BarRectangle = getRelativePosition();
        }
        else
        {
            NormalRectangle = getRelativePosition();
        }
        setRelativePosition(MinimizedRectangle);
    }
    IsMinimized = minimize;
}

void CGUIAdvancedWindow::setMinimizable(bool allow)
{
    IsMinimizable = allow;
}

void CGUIAdvancedWindow::setMinimizedPosition(const core::position2di& minimized)
{
    MinimizedRectangle = core::rect<s32>( minimized, core::position2di(minimized.X+MinimizedRectangle.getWidth(),minimized.Y+21));
}

void CGUIAdvancedWindow::setMinimizedWidth(u32 width)
{
    MinimizedRectangle = core::rect<s32>( MinimizedRectangle.UpperLeftCorner, core::position2di(MinimizedRectangle.UpperLeftCorner.X+width,MinimizedRectangle.UpperLeftCorner.Y+21));

}

void CGUIAdvancedWindow::setSkin(IGUISkin* skin)
{
    Skin = skin;
    UseGradient = Skin->getType() == EGST_WINDOWS_METALLIC || Skin->getType() == EGST_BURNING_SKIN;
}

} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_

