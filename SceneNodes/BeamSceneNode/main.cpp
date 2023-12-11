
#include <irrlicht.h>
#include "CBeamSceneNode.h"
#include "CBeamSceneNode2.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
int main()
{
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16,
			false, false, false, 0);
    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
		driver->getTexture("../../media/irrlicht2_up.jpg"),driver->getTexture("../../media/irrlicht2_dn.jpg"),
		driver->getTexture("../../media/irrlicht2_lf.jpg"),driver->getTexture("../../media/irrlicht2_rt.jpg"),
		driver->getTexture("../../media/irrlicht2_ft.jpg"),driver->getTexture("../../media/irrlicht2_bk.jpg"));


    ///BITPLANE LASER
        scene::CBeamSceneNode2 *myNode = new scene::CBeamSceneNode2(
        smgr->getRootSceneNode(), smgr, 666, core::vector3df(50,0,0), core::vector3df(0,20,300), 2);

        video::ITexture* tex = driver->getTexture("beam.png");
      //  driver->makeColorKeyTexture(tex, core::position2d<s32>(0,0));
        myNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        myNode->setMaterialTexture(0, tex);
        scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.0f));
        if(anim){
            myNode->addAnimator(anim);
            anim->drop();
            anim = 0;
        }

        myNode->drop();
        myNode = 0;
    ///

	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node){
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
		node->setPosition(vector3df(0,0,0));
		//node->setDebugDataVisible(scene::EDS_FULL);
	}

	ISceneNode* cube = smgr->addCubeSceneNode();
	cube->setPosition(vector3df(10,0,10));
	cube->setMaterialFlag(EMF_LIGHTING,false);
	cube->setMaterialTexture(0,driver->getTexture("laserlf7.png"));

	scene::CBeamNode* beam = new scene::CBeamNode(smgr->getRootSceneNode(), smgr, -1, "laserlf6.png" , "beam.png" );
	//now set the beam
	beam->setLine(core::vector3df(0,0,0), core::vector3df(100,100,100), 5.0f);
	beam->drop();
	scene::CBeamNode* beam1 = new scene::CBeamNode(smgr->getRootSceneNode(), smgr, -1, "laserlf7.png" , "beam.png" );
	//now set the beam
	beam1->setLine(core::vector3df(0,52,0), core::vector3df(100,0,100), 20.0f);
	beam1->drop();
    scene::CBeamNode* beam2 = new scene::CBeamNode(smgr->getRootSceneNode(), smgr, -1, "laserlf6.png" , "beam.png" );
	//now set the beam
	beam2->setLine(core::vector3df(0,20,0), core::vector3df(100,100,100), 5.0f);
	beam2->drop();
	//ISceneNode* root = smgr->getRootSceneNode();
	//root->addChild(beam);
	//

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100,0.5f);
    camera->setPosition(vector3df(100,0,50));
	int lastFPS = -1;
	stringw caption;
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(0,10,10,100));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
		int fps = driver->getFPS();
		if (fps!=lastFPS)
		{
			lastFPS = fps;
			caption = L"Driver [";
			caption+= driver->getName();
			caption+=L"] FPS : ";
			caption+=fps;
			device->setWindowCaption(caption.c_str());
		}
	}
	//beam->drop();
	device->drop();
	return 0;
}

