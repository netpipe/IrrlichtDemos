#pragma once
#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;

class CIrrlichtObject
{
private:
	IShadowVolumeSceneNode* m_pShadowNode;

protected:


public:
	IAnimatedMesh* m_pMesh;
	ISceneNode* m_pNode;

	CIrrlichtObject(void);
	virtual ~CIrrlichtObject(void);

	virtual void SetPosition(const core::vector3df &v);
	virtual void SetRotation(const core::vector3df &v);
	bool LoadMeshFromFile(const c8* filename);
	f32 GetMaxExtent(void);
	aabbox3d<f32> GetBoundingBox(void);
};
