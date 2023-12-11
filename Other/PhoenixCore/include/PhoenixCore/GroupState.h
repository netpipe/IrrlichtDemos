/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#ifndef __PHGROUPSTATE_H__
#define __PHGROUPSTATE_H__

#include "config.h"
#include "boost/shared_ptr.hpp"

namespace phoenix{

// forward decl
class BatchRenderer;

//! Group State Manager
/*!
	This is an abstract interface for objects that manipulate the render
	state for a specific group of geometry. These objects are added to a
	BatchRenderer using the BatchRenderer::addGroupState() and removed using
	BatchRenderer::removeGroupState(). Only one state object can be applied
	to a group at a time, if more are needed, applying the Composite pattern
	should be considered.
*/
class GroupState
{

public:

	GroupState(){};
	virtual ~GroupState(){};

	//! Begin render state.
	/*!
		Should set up all the render states needed for the current group.
		BatchRenderer will pass a reference to itself.
	*/
	virtual void begin( BatchRenderer& r ) = 0;

	//! End render state.
	/*!
		Should restore the render state. Failure to do so properly
		could result in the remaining geometry groups not appearing
		correctly.
		BatchRenderer will pass a reference to itself.
	*/
	virtual void end( BatchRenderer& r ) = 0;

}; // class

//! Friendly name for GroupState objects.
typedef boost::shared_ptr<GroupState> GroupStatePtr;

} //namespace phoenix

#endif //__PHGROUPSTATE_H__