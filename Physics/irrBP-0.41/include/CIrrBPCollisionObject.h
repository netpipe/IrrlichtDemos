#ifndef C_IRRBP_COLLISION_OBJ
#define C_IRRBP_COLLISION_OBJ

#include <irrlicht.h>
#include "convert.h"
#include "types.h"
#include "CMotionState.h"


class CIrrBPAnimator;
static int UNIQUE_BODY_ID_GENERATOR = 0;
class CIrrBPCollisionObject
{
public:
	
	CIrrBPCollisionObject();
	/*!
	Drop Function.
	This function should not be used. The destructor will be called automatically by the World Object.
	*/
	virtual void drop() = 0;
	btCollisionObject * getPtr()
	{
		return collisionObj;
	}
	COLL_OBJ_TYPE getObjectType()
	{
		return m_objType;
	}
	/*! 
		Returns the status of the node. 
	*/
	virtual bool isValid(){ return m_BodyStatus;}

	/*! 
		Setting the value to false will cause the node's deletion.
	*/
	virtual void setValidStatus (bool newStat) { m_BodyStatus = newStat;}

	virtual irr::u32 getAnimatorsNr() {return m_Animators.size();}
	virtual const irr::core::array<CIrrBPAnimator *> getAnimators() const { return m_Animators; }
	virtual void addAnimator(CIrrBPAnimator * anim);

	virtual void setName(const irr::core::stringc & newName)	{		m_BodyName = newName;	}
	virtual void setID(const irr::s32 & newID)	{		m_BodyId = newID;	}
	virtual const irr::c8* getName() const { return m_BodyName.c_str();}
	virtual irr::s32 getID() const { return m_BodyId;}

	virtual irr::u32 getUniqueID() const { return m_uBodyId;}
	
	virtual void setCcdSweptSphereRadius(irr::f32 sphrad) { collisionObj->setCcdSweptSphereRadius(sphrad);}
	virtual void setCcdMotionThreshold(irr::f32 thres) {collisionObj->setCcdMotionThreshold(thres);}
protected:
	irr::core::stringc m_BodyName;
	irr::s32 m_BodyId; //User setted BodyID
	irr::u32 m_uBodyId; //Unique BodyID

	COLL_OBJ_TYPE m_objType;
	btCollisionObject * collisionObj;
	bool m_BodyStatus;
	irr::core::array<CIrrBPAnimator *> m_Animators;
};

#endif