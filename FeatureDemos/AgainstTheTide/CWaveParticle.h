#ifndef CWAVEPARTICLE_H_HG_INCLUDE
#define CWAVEPARTICLE_H_HG_INCLUDE

#include <irrlicht.h>

class CWaveParticle : public irr::video::IShaderConstantSetCallBack
{
public:
	CWaveParticle(irr::IrrlichtDevice* irrlichtDevice,
		irr::scene::IMeshSceneNode* meshSceneNode, irr::f32 height);

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	irr::scene::SMesh* mesh;
private:
	void addQuad(irr::core::vector3df position);
	irr::f32 getFRand() const;
	
	irr::scene::SMeshBuffer* meshBuffer;
	irr::scene::ITriangleSelector* selector;
	irr::IrrlichtDevice* device;
	irr::scene::ISceneManager* smgr;
	irr::scene::IMeshSceneNode* mSceneNode;
	irr::core::vector3df center;
	irr::s32 pMat;
};

#endif