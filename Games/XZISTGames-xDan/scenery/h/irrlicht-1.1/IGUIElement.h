 // Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_GUI_ELEMENT_H_INCLUDED__
#define __I_GUI_ELEMENT_H_INCLUDED__

#include "IUnknown.h"
#include "irrList.h"
#include "rect.h"
#include "irrString.h"
#include "IEventReceiver.h"
#include "EGUIElementTypes.h"

namespace irr
{
namespace gui
{

class IGUIEnvironment;

//! Base class of all GUI elements.
class IGUIElement : public virtual IUnknown, public IEventReceiver
{
public:

	//! Constructor
	IGUIElement(EGUI_ELEMENT_TYPE type, IGUIEnvironment* environment, IGUIElement* parent,
		s32 id, core::rect<s32> rectangle)
		: Parent(0), RelativeRect(rectangle), IsVisible(true), IsEnabled(true),
			ID(id), Environment(environment), Type(type)
	{
		AbsoluteRect = RelativeRect;
		AbsoluteClippingRect = AbsoluteRect;

		if (parent)
			parent->addChild(this);

		if (Parent)
		{
			AbsoluteRect += Parent->getAbsolutePosition().UpperLeftCorner;
			AbsoluteClippingRect = AbsoluteRect;
			AbsoluteClippingRect.clipAgainst(Parent->AbsoluteClippingRect);
		}
	}


	//! Destructor
	virtual ~IGUIElement()
	{
		// delete all children
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
		{
			(*it)->Parent = 0;
			(*it)->drop();
		}
	};


	//! Returns parent of this element.
	IGUIElement* getParent()
	{
		return Parent;
	}


	//! Returns the relative rectangle of this element.
	core::rect<s32> getRelativePosition()
	{
		return RelativeRect;
	}


	//! Sets the relative rectangle of this element.
	void setRelativePosition(const core::rect<s32>& r)
	{
		RelativeRect = r;
		updateAbsolutePosition();
	}


	//! Returns the absolute rectangle of element.
	core::rect<s32> getAbsolutePosition()
	{
		return AbsoluteRect;
	}


	//! Updates the absolute position.
	virtual void updateAbsolutePosition()
	{
		core::rect<s32> parentAbsolute(0,0,0,0);
		core::rect<s32> parentAbsoluteClip;

		if (Parent)
		{
			parentAbsolute = Parent->AbsoluteRect;
			parentAbsoluteClip = Parent->AbsoluteClippingRect;
		}

		AbsoluteRect = RelativeRect + parentAbsolute.UpperLeftCorner;

		if (!Parent)
			parentAbsoluteClip = AbsoluteRect;

		AbsoluteClippingRect = AbsoluteRect;
		AbsoluteClippingRect.clipAgainst(parentAbsoluteClip);		

		// update all children
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			(*it)->updateAbsolutePosition();
	}


	//! Returns the child element, which is at the position of the point.
	IGUIElement* getElementFromPoint(const core::position2d<s32>& point)
	{
		IGUIElement* target = 0;

		// we have to search from back to front.

		core::list<IGUIElement*>::Iterator it = Children.getLast();

		if (IsVisible)
			while(it != Children.end())
			{
				target = (*it)->getElementFromPoint(point);
				if (target)
					return target;

				--it;
			}

		if (AbsoluteRect.isPointInside(point) && IsVisible)
			target = this;
		
		return target;
	}


	//! Adds a GUI element as new child of this element.
	virtual void addChild(IGUIElement* child)
	{
		if (child)
		{
			child->grab();
			child->remove();  // remove from old parent
			child->Parent = this;
			Children.push_back(child);						
		}
	}


	//! Removes a child.
	virtual void removeChild(IGUIElement* child)
	{
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			if ((*it) == child)
			{
				(*it)->Parent = 0;
				(*it)->drop();
				Children.erase(it);
				return;
			}
	}


	//! Removes this element from its parent.
	virtual void remove()
	{
		if (Parent)
			Parent->removeChild(this);
	}


	//! Draws the element and its children.
	virtual void draw()
	{
		if (!IsVisible)
			return;

		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			(*it)->draw();
	}


	//! Moves this element.
	virtual void move(core::position2d<s32> absoluteMovement)
	{
		RelativeRect += absoluteMovement;
		updateAbsolutePosition();
	}


	//! Returns true if element is visible.
	virtual bool isVisible()
	{
		_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return IsVisible;
	}



	//! Sets the visible state of this element.
	virtual void setVisible(bool visible)
	{
		IsVisible = visible;
	}


	//! Returns true if element is enabled.
	virtual bool isEnabled()
	{
		_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return IsEnabled;
	}


	//! Sets the enabled state of this element.
	virtual void setEnabled(bool enabled)
	{
		IsEnabled = enabled;
	}


	//! Sets the new caption of this element.
	virtual void setText(const wchar_t* text)
	{
		Text = text;
	}


	//! Returns caption of this element.
	virtual const wchar_t* getText()
	{
		return Text.c_str();
	}


	//! Returns id. Can be used to identify the element.
	virtual s32 getID()
	{
		return ID;
	}

	//! Sets the id of this element
	virtual void setID(s32 id)
	{
		ID = id;
	}


	//! Called if an event happened.
	virtual bool OnEvent(SEvent event)
	{
		if (Parent)
			Parent->OnEvent(event);

		return true;
	}


	//! Brings a child to front
	/** \return Returns true if successful, false if not. */
	virtual bool bringToFront(IGUIElement* element)
	{
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
		{
			if (element == (*it))
			{
				Children.erase(it);
				Children.push_back(element);
				return true;
			}
		}

		_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
		return false;
	}

	//! Returns list with children of this element
	virtual const core::list<IGUIElement*>& getChildren() const
	{
		return Children;
	}

	//! Finds the first element with the given id.
	/** \param id: Id to search for.
	 \param searchchildren: Set this to true, if also children of this 
	 element may contain the element with the searched id and they 
	 should be searched too.
	 \return Returns the first element with the given id. If no element
	 with this id was found, 0 is returned. */
	virtual IGUIElement* getElementFromId(s32 id, bool searchchildren=false) const
	{
		IGUIElement* e = 0;

		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
		{
			if ((*it)->getID() == id)
				return (*it);

			if (searchchildren)
				e = (*it)->getElementFromId(id, true);

			if (e)
				return e;
		}		

		return e;
	}

	//! Returns the type of the gui element. 
	/** This is needed for the .NET wrapper but will be used
	later for serializing and deserializing.
	If you wrote your own GUIElements, you need to set the type for your element as first parameter
	in the constructor of IGUIElement. For own (=unknown) elements, simply use EGUIET_ELEMENT as type */
	EGUI_ELEMENT_TYPE getType()
	{
		return Type;
	}

protected:

	//! List of all children of this element
	core::list<IGUIElement*> Children;

	//! Pointer to the parent
	IGUIElement* Parent;

	//! relative rect of element
	core::rect<s32> RelativeRect;

	//! absolute rect of element
	core::rect<s32> AbsoluteRect;

	//! absolute clipping rect of element
	core::rect<s32> AbsoluteClippingRect;

	//! is visible?
	bool IsVisible;

	//! is enabled?
	bool IsEnabled;

	//! caption
	core::stringw Text;

	//! id
	s32 ID;

	//! GUI Environment
	IGUIEnvironment* Environment;

	//! type of element
	EGUI_ELEMENT_TYPE Type;
};


} // end namespace gui
} // end namespace irr

#endif

