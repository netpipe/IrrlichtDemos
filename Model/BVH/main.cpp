#include <irrlicht.h>
#include <IBoneSceneNode.h>
#include "BVHSceneNode.h"
#include <ICameraSceneNode.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(){
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL, dimension2d<u32>(800, 800), 16, false, false, false, 0);
    if(!device){
        return 1;
    }
    device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();


	BVH* pBVH = new BVH();
	pBVH->Load("media/pirouette.bvh");
	if (!(pBVH->IsLoadSuccess())) {
		printf("\nCould not load file");
		return 0;
	}

	BVHSceneNode* pBVHSceneNode = new BVHSceneNode(smgr->getRootSceneNode(), smgr, pBVH->m_pRootJoint, &(pBVH->m_Frames), pBVH->m_Interval);
	ICameraSceneNode* pCameraSceneNode = smgr->addCameraSceneNode(0, vector3df(0, 0, -550), vector3df(0, 0, 0));
    while(device->run()){
        driver->beginScene(true, true, SColor(255, 100, 101, 140));
        smgr->drawAll();
        driver->endScene();
    }
    device->drop();
    return 0;
}
