#ifndef CIrrBP_ROPE_SBODY_H_
#define CIrrBP_ROPE_SBODY_H_

#include "softbody/CIrrBPSoftBody.h"
#include "body/CIrrBPRigidBody.h"

class CIrrBPWorld;

class CIrrBPRopeSoftBody : public CIrrBPSoftBody
{
public:
	CIrrBPRopeSoftBody(const irr::core::vector3df & from ,const irr::core::vector3df & to,irr::f32 mass,CIrrBPWorld * world,int res=-1);
	~CIrrBPRopeSoftBody();
	void drop() { delete this;}
	/*Set this to true to draw the rope*/
	void setDraw(bool Draw) { draw=Draw;}

	void update();
protected:
	bool draw;
	irr::video::IVideoDriver* driver;
	irr::video::SMaterial drawMat;
	int getAutoFixedRes();
	irr::core::vector3df from;
	irr::core::vector3df to;
};

#endif
