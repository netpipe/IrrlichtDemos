//CIrrlichtObject
//The graphical representation of an object

#include "IrrlichtObject.h"
#include "utils.h"

extern IrrlichtDevice* g_pIrrDevice;

#define __SHADOWS__

CIrrlichtObject::CIrrlichtObject(void)
{
}

CIrrlichtObject::~CIrrlichtObject(void)
{
}

void CIrrlichtObject::SetPosition(const core::vector3df &v)
{
	if (m_pNode) m_pNode->setPosition(v);
}
void CIrrlichtObject::SetRotation(const core::vector3df &v)
{
	if (m_pNode) m_pNode->setRotation(v);
}

bool CIrrlichtObject::LoadMeshFromFile(const c8* filename)
{
	GET_SMGR;
	m_pMesh = smgr->getMesh(filename); //returns a IAnimatedMesh*, not a IMesh* as docs say

	if (!m_pMesh) {
		return false;
	}

	m_pNode = smgr->addAnimatedMeshSceneNode(m_pMesh);
	if (!m_pNode) return false;


#if defined __SHADOWS__
//!		m_pShadowNode = ((IAnimatedMeshSceneNode*)m_pNode)->addShadowVolumeSceneNode(-1, true);
#endif

	return true;
}


f32 CIrrlichtObject::GetMaxExtent(void)
{
	if (!m_pNode) return 0;
	aabbox3d<f32> box=m_pNode->getBoundingBox();
	vector3df ext = box.getExtent();
	f32 ret=0;
	if (ext.X>ret) ret=ext.X;
	if (ext.Y>ret) ret=ext.Y;
	if (ext.Z>ret) ret=ext.Z;
	return ret;
}

aabbox3d<f32> CIrrlichtObject::GetBoundingBox(void)
{
	aabbox3d<f32> extent;
	if (m_pNode) extent = m_pNode->getBoundingBox();
	if (extent.isEmpty()) {
		extent.addInternalPoint(vector3df(-0.5,-0.5,-0.5));
		extent.addInternalPoint(vector3df(0.5,0.5,0.5));
		GET_GUIENV;
		//NOTE - loading of (these) .X files will not work properly with Irrlicht 1.2,
		// unless CXAnimationPlayer.cpp is patched (bounding box wrong size)
		//Line 805, updateBoundingBoxFromAnimation(): Drop these 3 lines
			//		if (first)
			//			Box.reset(p);
			//		else
		guienv->addMessageBox(L"Error: ", L"Error in Irrlicht: Bounding box zero size - see IrrlichtObject::GetBoundingBox() code for patch");
	}
	return extent;
}
