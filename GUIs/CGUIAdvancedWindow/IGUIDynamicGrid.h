// Xterm-in'Hate

#ifndef __I_GUI_DYNAMIC_GRID_H_INCLUDED__
#define __I_GUI_DYNAMIC_GRID_H_INCLUDED__

#include "IGUIElement.h"
#include "IGUIEnvironment.h"

namespace irr
{
namespace gui
{
	//! Enumeration for all behaviors of the GUI Dynamic Grid element.
    enum EGUI_DYNAMIC_GRID_BEHAVIOR
    {
        EGDGB_RESET=0,                   ///< Do nothing. Every child elements move freely.
        EGDGB_ATTACH_RUN_ONCE,          ///< Try to attach every nearer child elements together (then go to RUN behavior).
        EGDGB_ATTACH_FOCUSED,           ///< Try to attach the focused child element with the nearer child elements.
        EGDGB_RUN,                      ///< Attached child elements remain attached, others move freely.
        EGDGB_DETACH_FOCUSED,           ///< Try to dettach the focused child element (it moves freely).
        EGDGB_DETACH_ALL                ///< Try to dettach every child elements (then go to RUN behavior).
    };

	//! Names for all behaviors of the GUI Dynamic Grid element.
	const c8* const GUIDynamicGridBehaviorNames[] =
	{
		"Reset",
		"Attach Once (Near)",
		"Attach Auto (Focus)",
		"Running",
		"Detach Auto (Focus)",
		"Detach All",
	};

	//! GUI Dynamic Grid interface.
	class IGUIDynamicGrid : public IGUIElement
	{
	public:
		//! constructor
		IGUIDynamicGrid(IGUIEnvironment* environment, IGUIElement* parent)
        : IGUIElement(EGUIET_ELEMENT, environment, parent, -1,
            core::rect<s32>(0,0,parent->getAbsolutePosition().getWidth(),parent->getAbsolutePosition().getHeight())
        ) {}

        //! show/hide joints (DEBUG)
		virtual void setJointVisible(bool visible) = 0;

        //! set the behavior of the Dynamic Grid
		virtual void setBehavior(EGUI_DYNAMIC_GRID_BEHAVIOR behavior) = 0;

        //! get the behavior of the Dynamic Grid
		virtual EGUI_DYNAMIC_GRID_BEHAVIOR getBehavior() const = 0;
	};

} // end namespace gui
} // end namespace irr

#endif
