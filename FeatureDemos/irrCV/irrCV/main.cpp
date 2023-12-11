/*
Coded by Ahmet YILDIRIM
mclightning & whizgeek .com
Release Date: 02.06.11
*/
#include "headers.h"
int main()
{
	eventReceiver receiver;
	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<u32>(1366, 768), 16,false, false, false,&receiver);
	device->setWindowCaption(L"(WhizGeek || Mclightning).com");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	scene::ICameraSceneNode* kam= smgr->addCameraSceneNode(NULL,vector3df(0,0,200),vector3df(0,0,0));

	//kam->setPosition(vector3df(0,0,200));

	ISceneNode* kutu=smgr->addCubeSceneNode(50,0,2,vector3df(50,0,0));
	ISceneNode* kutu2=smgr->addCubeSceneNode(50,0,2,vector3df(-50,0,0));
	ITexture *duvar=driver->getTexture("wall.jpg");
	kutu->setMaterialTexture(0,duvar);
	kutu->setMaterialFlag(video::EMF_LIGHTING, false);
	kutu2->setMaterialTexture(0,duvar);
	kutu2->setMaterialFlag(video::EMF_LIGHTING, false);

		IAnimatedMesh* mesh = smgr->getMesh("sydney.md2");
		IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
		node->setRotation(vector3df(0,270,0));

	irFinder ir("test",true,230,255);
	CvPoint in;
	while(device->run())
	{
		if(receiver.IsKeyDown(KEY_ESCAPE))
		{
			device->drop();
			return 0;
		}
		driver->beginScene(true, true, SColor(255,0,0,255));
		in=ir.yenile();
		//node->setPosition(vector3df(30*in.x/320,30*(240-in.y)/240,0));
		kam->setPosition(vector3df(in.x-160,(240-in.y),200));
		smgr->drawAll();
		driver->endScene();
	}
	device->drop();
}
