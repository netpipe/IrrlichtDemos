// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_TOUCH_CONTROL_H_INCLUDED__
#define __I_TOUCH_CONTROL_H_INCLUDED__

#include "IReferenceCounted.h"
#include "IEventReceiver.h"
#include "position2d.h"
#include "rect.h"

namespace irr
{
namespace gui
{
	//! Interface to touch events
	class ITouchControl : public virtual IReferenceCounted
	{
	public:
		
		//! Returns if the screen is being touched.
		/** \return True if the screen is being touched, false if not. */	
		virtual bool isActive() = 0;
		
		//! Returns the type of the last touch event.
		/** \return The ETOUCH_INPUT_EVENT of the last touch event. */
		virtual ETOUCH_INPUT_EVENT getEventType() = 0;
		
		//! Returns the number of taps of the last touch event.
		/** \return Returns the number of taps of the last touch event. */
		virtual s32 getNumberOfTaps() = 0;
		
		//! Returns the timestamp of the last touch event.
		/** \return Returns the timestamp of the last touch event. */
		virtual f64 getTimestamp() = 0;
		
		//! Returns the last position of the touch event.
		/** \return Returns the last position of the touch event. The returned position
		is the position of the touch in pixel units. */
		virtual core::position2d<s32> getPosition() = 0;

		//! Returns the last position of the touch event relative to the window.
		/** \return Returns the last position of the touch. The returned position
		is a value between (0.0f, 0.0f) and (1.0f, 1.0f), where (0.0f, 0.0f) is
		the top left corner and (1.0f, 1.0f) is the bottom right corner of the
		render window. */
		virtual core::position2d<f32> getRelativePosition() = 0;
	};

} // end namespace gui
} // end namespace irr

#endif

