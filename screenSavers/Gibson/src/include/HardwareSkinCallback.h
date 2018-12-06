#ifndef _HARDWARESKINCB_
#define _HARDWARESKINCB_

#include <irrlicht.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class HWSkinCB : public video::IShaderConstantSetCallBack
{
private:
	IrrlichtDevice *device;
    ISkinnedMesh* mesh;
	void copyMat(f32* pointer, matrix4 &mat);
	u32 oldtimer;
	u32 delay;
	bool lightsenabled;

public:

    HWSkinCB();

	~HWSkinCB();

    static HWSkinCB* getInstance();

	void setupNode(IrrlichtDevice *device, IAnimatedMeshSceneNode* node, u32 refresh);

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);
};

#endif
