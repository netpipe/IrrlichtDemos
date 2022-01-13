#ifndef _CIRRBPACTIONINTERFACE_H
#define _CIRRBPACTIONINTERFACE_H
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>

static int UNIQUE_ACTION_ID_GENERATOR = 0;

class CIrrBPActionInterface
{
public:
	CIrrBPActionInterface()
	{
		m_action = NULL;
		m_uActionID = UNIQUE_ACTION_ID_GENERATOR++;
	}
	virtual void drop() = 0;
	btActionInterface * getPtr() { return m_action;}
	ACTIONINT_OBJ_TYPE * getObjectType();

	/*! 
		Returns the status of the node. Not yet implemented.
	*/
	virtual bool isValid(){ return m_ActionStatus;}

	/*! 
		Setting the value to false will cause the node's deletion. Not yet implemented.
	*/
	virtual void setValidStatus (bool newStat) { m_ActionStatus = newStat;}

	virtual void setName(const irr::core::stringc & newName)	{		m_ActionName = newName;	}
	virtual void setID(const irr::s32 & newID)	{		m_ActionId = newID;	}
	virtual const irr::c8* getName() const { return m_ActionName.c_str();}
	virtual irr::s32 getID() const { return m_ActionId;}

	virtual irr::u32 getUniqueID() const { return m_uActionID;}
protected:
	irr::core::stringc m_ActionName;
	irr::s32 m_ActionId; //User setted ID

	irr::u32 m_uActionID;
	bool m_ActionStatus;
	ACTIONINT_OBJ_TYPE m_objType;
	btActionInterface * m_action;
};
#endif