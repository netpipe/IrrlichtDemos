#pragma once
#include <ISceneNodeAnimator.h>
#include <ode/ode.h>

class CPhysicAnimator :
	public irr::scene::ISceneNodeAnimator
{
private:
	dGeomID	m_geom;	//! will synchronize scene node with this geom object

	void QuaternionToEuler(const dQuaternion quat, irr::core::vector3df &euler);
public:
	CPhysicAnimator(dGeomID geom) : irr::scene::ISceneNodeAnimator(), m_geom(geom) {}
	virtual ~CPhysicAnimator(void) {}

	//! Animates a scene node.
	/** \param node Node to animate.
	\param timeMs Current time in milli seconds. */
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling this. */
	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
			irr::scene::ISceneManager* newManager=0);
};

