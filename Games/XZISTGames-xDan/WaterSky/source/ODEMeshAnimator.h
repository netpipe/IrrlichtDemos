
#ifndef __ODEMeshAnimator_h
#define __ODEMeshAnimator_h

#include "ODEAnimator.h"

class ODEMeshAnimator : public ODEAnimator
{
public:
    ODEMeshAnimator(scene::ISceneNode *, scene::IAnimatedMesh *, ODEPhysics *, u32);
    
private:
	core::array <dTriMeshDataID> dTriMeshData;
	core::array <dVector3 *> dVertices;
	core::array <int *> dIndices;
	
    void clearAll();
    void makeFromMesh(scene::IMesh *, core::vector3df);
};

#endif
