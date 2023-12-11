#ifndef CIrrBP_FPSCAM_ANIM_H_
#define CIrrBP_FPSCAM_ANIM_H_
#include "animator/CIrrBPAnimator.h"
#include "CIrrBPCamera.h"
class CIrrBPCollisionObject;
/*
This class has not been implemented yet.
Only for future implementations
*/
class CIrrBPFPSCameraAnimator : public CIrrBPAnimator,irr::scene::ISceneNodeAnimatorCameraFPS
{
public:
	CIrrBPFPSCameraAnimator();
	virtual bool OnEvent(const irr::SEvent& event);
	void setBody(CIrrBPCollisionObject* body) ;
	void animate();
	void drop() {delete this;}
private:
	CIrrBPCamera * camera;

};

#endif