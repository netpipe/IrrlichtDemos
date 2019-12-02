// Xterminhate

#ifndef __C_GUI_WINDOW_H_INCLUDED__
#define __C_GUI_WINDOW_H_INCLUDED__

#include "IrrCompileConfig.h"
//#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIWindow.h"
#include "IGUISkin.h"
#include "ICursorControl.h"

namespace irr
{
namespace gui
{
	//! Window button types
	enum EWINDOW_BUTTON_TYPE
	{
		//! close button
		EWBT_CLOSE,

		//! maximize button
		EWBT_MINIMIZE,

		//! pin button
		EWBT_PIN,

		//! user defined button
		EWBT_USER_DEFINED
	};

    //! Window Close behavior.
	//! Default is EWC_REMOVE
	enum EWINDOW_CLOSE
	{
		//! do nothing - window stays open
		EWC_IGNORE = 0,

		//! remove the gui element
		EWC_REMOVE = 1,

		//! call setVisible(false)
		EWC_HIDE = 2

	 	// next is 4
	};

    //! Enumeration for all skin colors (light source is cursor).
    enum EGUI_ADVANCED_WINDOW_COLOR
    {
        EGAWC_DARK_SHADOW = EGDC_3D_DARK_SHADOW,                ///< for outer edges facing away from the light source
        EGAWC_SHADOW = EGDC_3D_SHADOW,                          ///< for inner edges facing away from the light source
        EGAWC_HIGH_LIGHT = EGDC_3D_HIGH_LIGHT,                  ///< for outer edges facing the light source
        EGAWC_LIGHT = EGDC_3D_LIGHT,                            ///< for inner edges facing the light source
        EGAWC_ACTIVE_TITTLE_BAR = EGDC_ACTIVE_BORDER,           ///< for tittle bar when active (focused)
        EGAWC_INACTIVE_TITTLE_BAR = EGDC_INACTIVE_BORDER,       ///< for tittle bar when inactive
        EGAWC_ACTIVE_TITTLE_TEXT = EGDC_ACTIVE_CAPTION,         ///< for tittle (text) when active (focused)
        EGAWC_INACTIVE_TITTLE_TEXT = EGDC_INACTIVE_CAPTION,     ///< for tittle (text) when inactive
        EGAWC_WINDOW_LIGHT = EGDC_WINDOW,                       ///< for window background facing light source
        EGAWC_WINDOW_SHADOW = EGDC_APP_WORKSPACE,               ///< for window background facing away from  the light source
        EGAWC_ACTIVE_WINDOW_SYMBOL = EGDC_WINDOW_SYMBOL,        ///< for window symbols like on close buttons, scroll bars and check boxes
        EGAWC_INACTIVE_WINDOW_SYMBOL = EGDC_GRAY_WINDOW_SYMBOL, ///< for grayed (disabled) window symbols like on close buttons, scroll bars and check boxes
    };

	class IGUIButton;

	class CGUIAdvancedWindow : public IGUIElement
	{
	public:

	    //!
        struct buttoninfo
        {
            EWINDOW_BUTTON_TYPE Type;   //! see EWINDOW_BUTTON_TYPE
            IGUISpriteBank* Sprite;     //! Button Icon 1/2 (a sprite bank)
            s32 SpriteIndex;            //! Button Icon 2/2 (the index of the srpite in the sprite bank)
            bool VisibleWhenNormal;     //! Button is visible when window is in normal state
            bool VisibleWhenBar;        //! Button is visible when window is in bar state
            bool VisibleWhenMinimized;  //! Button is visible when window is in minimized state
            s32 UserEventId;            //! The ID sent to user event receiver
            const wchar_t* Text;        //! Button Text
            const wchar_t* ToolTipText; //! Button Tooltip Text (doesnt work atm)
        };

		//! constructor
		CGUIAdvancedWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle,
            const wchar_t *text, const core::array<buttoninfo>& buttons, EWINDOW_CLOSE close, const core::position2di& minimized,
            ICursorControl* cursor, video::ITexture* texture = 0 );

		//! destructor
		virtual ~CGUIAdvancedWindow();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! update absolute position
		virtual void updateAbsolutePosition();

		//! draws the element and its children
		virtual void draw();

		//! Returns true if the window is minimized, false if not
		virtual bool isMinimized() const;

		//! Sets whether the window is minimized
		virtual void setMinimized(bool minimize);

		//! Sets whether the window is minimizable
		virtual void setMinimizable(bool allow);

		//! Sets the position of the minimized window
		virtual void setMinimizedPosition(const core::position2di& minimized);

		//! Sets the position of the minimized window
		virtual void setMinimizedWidth(u32 width);

		//! Returns true if the window is draggable, false if not
		virtual bool isDraggable() const;

		//! Sets whether the window is draggable
		virtual void setDraggable(bool draggable);

        //! Set if the window background will be drawn
        virtual void setDrawBackground(bool draw);

        //! Get if the window background will be drawn
        virtual bool getDrawBackground() const;

        //! Set if the window titlebar will be drawn
        //! Note: If the background is not drawn, then the titlebar is automatically also not drawn
        virtual void setDrawTitlebar(bool draw);

        //! Get if the window titlebar will be drawn
        virtual bool getDrawTitlebar() const;

		//! Returns the rectangle of the drawable area (without border and without titlebar)
		virtual core::rect<s32> getClientRect() const;

		//! Makes the tittle bar to blink
		///  Force = false ==> if window is minimized or bar
		///  Force = true ==> in any window state
		virtual void notify(bool force);

        virtual void OnPostRender(u32 timeMs);

        //! overide environment skin
        void setSkin(IGUISkin* skin);

	protected:

		void updateClientRect();
		void refreshSprites();

		//! Refresh button visibility and position according state (normal, bar, minimized), return number of visible buttons
        u32 refreshButtons(); //! ADDED

        //! Implementation of window close behavior
        void close(); //! ADDED

        //! Implementation of window draw with cursor lightning
        core::rect<s32> draw3DWindowBackgroundMouseLightning(IGUIElement* element,
				bool drawTitleBar, video::SColor titleBarColor,
				const core::rect<s32>& r,
				const core::rect<s32>* clip=0,
				core::rect<s32>* checkClientArea=0); //! ADDED

        //! Default skin management
        IGUISkin* Skin; //! ADDED
        IGUISpriteBank* DefaultSprites; //! ADDED
		video::SColor CurrentIconColor;

        //! Button management
        core::array<buttoninfo> ButtonInfo; //! ADDED
        core::array<core::rect<s32> > ButtonRectangles; //! ADDED
        core::array<IGUIButton*> Buttons;

		core::rect<s32> ClientRect;

		//! States and events management
		core::position2d<s32> DragStart;
		bool Dragging;
		bool IsDraggable;
        bool DrawBackground;
        bool DrawTitlebar;
		bool IsActive;
		bool IsMinimized;//! ADDED
		bool IsBar; //! ADDED
        bool IsMinimizable;//! ADDED

        //! Close button handling
        EWINDOW_CLOSE CloseHandling; //! ADDED

        //! Window position for each window state
        core::rect<s32> NormalRectangle; //! ADDED
        core::rect<s32> MinimizedRectangle; //! ADDED
        core::rect<s32> BarRectangle; //! ADDED

        //! Draw data
        bool UseGradient; /// ADDED clone Skin private member data
        ICursorControl* Cursor; //! ADDED

        //! Notification data
        bool IsNotifying; //! ADDED
        u32 NotifyTimer; //! ADDED
        bool NotifyState; //! ADDED

        //! Texture
        video::ITexture* Texture;
	};

} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_

#endif

