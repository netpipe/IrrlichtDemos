#ifndef CIrrBP_CDL_ANIM_H_
#define CIrrBP_CDL_ANIM_H_
#include "animator/CIrrBPAnimator.h"

class CIrrBPCollisionObject;
class CIrrBPWorld;

//!Please note that collision callback against soft body is not yet implemented in Bullet Physics.<br> So The Collision Delete Animator DOESN'T works with soft bodies.
class CIrrBPCollisionDeleteAnimator  : public CIrrBPAnimator
{
public:
	CIrrBPCollisionDeleteAnimator(CIB_DFLAG deleteFlag, CIrrBPWorld * world);
	void setBody(CIrrBPCollisionObject* body) ;
	void animate();
	void drop() {delete this;}
private:
	irr::s16 internalStatus;
	CIrrBPWorld * rWorld;
	CIB_DFLAG dFlag; //delete flag

};

#endif