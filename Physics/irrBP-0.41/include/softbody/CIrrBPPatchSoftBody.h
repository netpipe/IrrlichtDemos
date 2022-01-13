#ifndef CIrrBP_PATCH_SBODY_H_
#define CIrrBP_PATCH_SBODY_H_

#include "softbody/CIrrBPSoftBody.h"
#include "body/CIrrBPRigidBody.h"
#include <map>

class CIrrBPWorld;

class CIrrBPPatchSoftBody : public CIrrBPSoftBody
{
public:
	CIrrBPPatchSoftBody(const irr::core::vector3df & corner00 ,const irr::core::vector3df & corner01,const irr::core::vector3df & corner10 ,const irr::core::vector3df & corner11,irr::f32 mass,CIrrBPWorld * world,irr::s32 resx,irr::s32 resy);
	~CIrrBPPatchSoftBody();
	void setMass(irr::u32 x,irr::u32 y,irr::f32 mass);

	/*! Returns the mesh to attach to a node.*/
	irr::scene::IMesh * getMesh();
	void drop() { delete this;}
	REALINLINE void update();
protected:
	void createMesh();
	irr::s32 resx,resy;
	irr::core::vector3df m_corner00;
	irr::core::vector3df m_corner01;
	irr::core::vector3df m_corner10;
	irr::core::vector3df m_corner11;
	irr::scene::SMesh * mesh;
	irr::scene::IMeshBuffer *meshbuff;
	std::map<btSoftBody::Node*,int> index;
	std::map<int,btSoftBody::Node*> indexsw; //Swapped index
};

#endif
