// Xterm-in'Hate

#ifndef __C_GUI_WINDOW_MANAGER_H_INCLUDED__
#define __C_GUI_WINDOW_MANAGER_H_INCLUDED__

#include "IrrCompileConfig.h"
//#ifdef _IRR_COMPILE_WITH_GUI_

#include "CGUIAdvancedWindow.h"

namespace irr
{
namespace core{

    template<class InputIt, class T>
    InputIt find(InputIt first, InputIt last, const T& value)
    {
        for (; first != last; ++first)
        {
            if (*first == value)
            {
                return first;
            }
        }
        return last;
    }
}
namespace gui
{
	//! Enumeration for all behaviors of the window manager.
    enum EGUI_WINDOW_MANAGER_BEHAVIOR
    {
        EGWMB_RESET=0,                   ///< Do nothing. Every child elements move freely.
        EGWMB_ATTACH_RUN_ONCE,          ///< Try to attach every nearer child elements together (then go to RUN behavior).
        EGWMB_ATTACH_FOCUSED,           ///< Try to attach the focused child element with the nearer child elements.
        EGWMB_RUN,                      ///< Attached child elements remain attached, others move freely.
        EGWMB_DETACH_FOCUSED,           ///< Try to dettach the focused child element (it moves freely).
        EGWMB_DETACH_ALL                ///< Try to dettach every child elements (then go to RUN behavior).
    };

	//! Names for all behaviors of the window manager .
	const c8* const GUIDynamicGridBehaviorNames[] =
	{
		"Reset",
		"Attach Once (Near)",
		"Attach Auto (Focus)",
		"Running",
		"Detach Auto (Focus)",
		"Detach All",
	};

	//! Enumeration for all minimized location .
    enum EGUI_WINDOW_MANAGER_MINIMIZED_POSITION
    {
        EGWMMP_TOP=0,          ///< Minimized windows are placed on the TOP of the screen.
        EGWMMP_RIGHT,          ///< Minimized windows are placed on the TOP of the screen.
        EGWMMP_BOTTOM,         ///< Minimized windows are placed on the TOP of the screen.
        EGWMMP_LEFT            ///< Minimized windows are placed on the TOP of the screen.
    };

    //! Enumeration for all minimized direction .
    enum EGUI_WINDOW_MANAGER_MINIMIZED_DIRECTION
    {
        EGWMMD_FROM_TOP_LEFT=0,    ///< Minimized windows are stacked from the top/left to the bottom/right corner.
        EGWMMD_FROM_BOTTOM_RIGHT   ///< Minimized windows are stacked from the bottom/right to the top/left corner.
    };

	class CGUIWindowManager : public IGUIElement
	{
	public:
		//! constructor
		CGUIWindowManager(IGUIEnvironment* environment, IGUIElement* parent, ICursorControl* cursor);

		//! destructor
		virtual ~CGUIWindowManager();

        //! show joints (DEBUG:To Be Deleted)
		void setJointVisible(bool visible);

        //! set the behavior of the window manager
		void setBehavior(EGUI_WINDOW_MANAGER_BEHAVIOR behavior);

        //! get the behavior of the window manager
		EGUI_WINDOW_MANAGER_BEHAVIOR getBehavior() const;

		//! draws the element and its children
		virtual void draw();

        //! add a GUI Advanced Window
        CGUIAdvancedWindow* addWindow( core::rect<s32> rectangle,
            const wchar_t *text,
            const core::array<CGUIAdvancedWindow::buttoninfo>& buttons,
            EWINDOW_CLOSE close,
            video::ITexture* texture );

        virtual void OnPostRender(u32 timeMs);

        //! overide environment skin
        void setSkin(IGUISkin* skin);

        //! set the position of the minimized windows (default is BOTTOM)
		void setMinimizedPosition(EGUI_WINDOW_MANAGER_MINIMIZED_POSITION position);

        //! set the direction of the minimized windows (default is from LEFT)
		void setMinimizedDirection(EGUI_WINDOW_MANAGER_MINIMIZED_DIRECTION direction);

        //! set the size of the minimized windows (default width is 120)
		void setMinimizedSize(u32 size);

        //! minimize every windows managed by this
		void minimizeAll();

        //! minimize every windows managed by this
		void restoreAll();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

	private:
        //! Child GUI windows, which are sticked together, form a 'body'.
        core::list<CGUIAdvancedWindow*> Windows;

        //! return true if this element is a window managed by this
        bool isChildWindow( IGUIElement* element) const;

        //! Each child GUI window of a body is a 'bone'.
        struct bone; // forward decl.

        //! Orphan GUI window are considered as bodies and bones too.

        //! Each 'bone' has got 8 joints (1 per axis (=2) x 1 per corner (=4)).
        struct joint
        {
            ///! enumaration of all types of joint
            enum EJOINT_TYPE
            {
                EJT_UPPER_RIGHT_TO_TOP=0,
                EJT_UPPER_RIGHT_TO_RIGHT,
                EJT_LOWER_RIGHT_TO_RIGHT,
                EJT_LOWER_RIGHT_TO_BOTTOM,
                EJT_LOWER_LEFT_TO_BOTTOM,
                EJT_LOWER_LEFT_TO_LEFT,
                EJT_UPPER_LEFT_TO_LEFT,
                EJT_UPPER_LEFT_TO_TOP,
                EJT_MAX
            };
            joint(EJOINT_TYPE type, bone* parent_bone, const core::rect<s32>& rectangle);
            ~joint();
            const core::position2di& getPosition() const;
            EJOINT_TYPE getType() const;
            bool isAttached() const;
            bool canAttachToJoint(const joint* j) const;
            void attachToJoint(joint* j);
            void dettach();
            bool isAttachedToBone(const bone* b) const;
            bone* getAttachedBone() const;
            void dragTo(u32 counter, const core::position2di& position);
            void update(u32 counter, const core::rect<s32>& rectangle);
        private:
            EJOINT_TYPE Type;
            bone* ParentBone;
            joint* AttachedJoint;
            core::position2di Position;
            u32 Counter;
        };

        struct bone
        {
            bone(CGUIAdvancedWindow* window);
            ~bone();
            joint* getJoint(size_t index) const;
            bool own(const CGUIAdvancedWindow* window) const;
            void update(u32 counter, const core::position2di& dist = core::position2di(0,0));
            bool canAttachToBone(const bone* b, size_t& j1, size_t& j2 ) const;
            void attachToBone(bone* b, size_t j1, size_t j2 );
            void dettach();
            bool isAttachedToBone(const bone* b) const;
            bool isRectCollidedBody(const bone* b) const;
            ///! Attach together every bones of the body identified by THIS bone, by as many joints as possible
            void consolidate();
            ///! Attach bones THIS and B with as many joint as possible
            void attachToBoneByAllPossibleJoints(bone* b);
            void updateMinimizable();
            const CGUIAdvancedWindow* getWindow() const;
        private:
            //! Recursive algorithm that detects overlapping bones => dont attached a bone to a body if a bone overlapping is detected
            static bool isRectCollidedBodyRecursive(const CGUIWindowManager::bone* b, const core::rect<s32>& rect, core::list<const bone*>& body_bones_processed);
            //! Recursive algorithm that lists the bones of a body, from one bone of this body
            static void listBonesRecursive(const bone* b,core::list<const bone*>& body_bones);
            static void listBonesRecursive(bone* b,core::list<bone*>& body_bones);
            joint* Joints[joint::EJT_MAX];
            CGUIAdvancedWindow* Window;
            u32 Counter;
        };

        //! bones database
        core::list<bone*> Bones;

        //! return the bone of a child element
        bone* getBone( CGUIAdvancedWindow* window );

        //! return true if this element is one of my managed child element (!= irr::isMyChild)
        bool hasBone(const CGUIAdvancedWindow* window, bone*& b) const;

        //! init dynamic grid algorithm
        void init();

        //! reset dynamic grid algorithm
        void reset();

        //! apply dynamic grid algorithm
        void snap();

        //! try to attach this bone to a body
        void tryToAttach(bone* b);

        //! try to form body based on bone proximity
        void tryToAttachAll();

        //! turn bodies into free bones
        void dettachAll();

        ///! appearance of the dynamic grid (DEBUG)
        bool isJointVisible;

        //! behavior of the dynamic grid
        EGUI_WINDOW_MANAGER_BEHAVIOR Behavior;

        //! for advanced window renderer
        ICursorControl* Cursor;

        //! the position of the minimized windows (default is BOTTOM)
		EGUI_WINDOW_MANAGER_MINIMIZED_POSITION MinimizedPosition;

        //! the direction of the minimized windows (default is from LEFT)
		EGUI_WINDOW_MANAGER_MINIMIZED_DIRECTION MinimizedDirection;

        //! the size of the minimized windows (default is 120)
        u32 MinimizedSize;

        //! recalculate the position and the width of all minimized windows
        void recalculateMinimizedWindow();

        //! rectangle of minimized windows location
        core::rect<s32> MinimizedBar;

        //! User skin management
        IGUISkin* Skin;
	};

} // end namespace gui
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_GUI_

#endif // __C_GUI_WINDOW_MANAGER_H_INCLUDED__

