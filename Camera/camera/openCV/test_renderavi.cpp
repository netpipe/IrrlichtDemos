#include <irrlicht.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "CvIrrRenderAvi.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;

int main()
{
	device = createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16, false, false, false, 0);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	//something for 3d reference
	IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	ITexture* sydtex = driver->getTexture("media/sydney.bmp");
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, sydtex );
		node->setPosition(vector3df(10,10,-10));
	}

	//and some more

	device->getFileSystem()->addZipFileArchive("media/map-20kdm2.pk3");
	IAnimatedMesh* mesh2 = smgr->getMesh("20kdm2.bsp");
	ISceneNode* node2 = 0;
	if (mesh2)
		node2 = smgr->addOctTreeSceneNode(mesh2->getMesh(0), 0, -1, 128);
	if (node2)
		node2->setPosition(vector3df(-1300,-144,-1249));


	//our billboard
	IBillboardSceneNode* our_bill_node = smgr->addBillboardSceneNode(NULL, dimension2d<f32>(25.0f, 25.0f), vector3df(0,0,0));
	our_bill_node->setMaterialFlag(video::EMF_LIGHTING, false);

	//camera stuff
	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(60,10,0));
	camera->setTarget(vector3df(0,0,0));

	//create our class
	CvIrrRenderAvi* render_avi = new CvIrrRenderAvi(driver, "output.avi", 640, 480);

	//create a writer
	CvVideoWriter* aviOut = cvCreateVideoWriter("output.avi", CV_FOURCC('I', 'V', '4', '1'), 30, cvSize(640, 480), 1);
	//other cv test
	cvNamedWindow("rt",1);

	//start render shiz
	ITexture* rt = 0;
	ICameraSceneNode* fixedCam = 0;
	if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
	{
		//rt = driver->createRenderTargetTexture(core::dimension2d<s32>(640,480));
		rt = driver->addTexture(core::dimension2d<s32>(640, 480), "rt", video::ECF_R8G8B8);
	}

	//set it to the billboard too
	our_bill_node->setMaterialTexture( 0, rt );

	while(device->run())
	{
		driver->beginScene(false, false, SColor(255,100,101,140));

		driver->setRenderTarget(rt, false, false, video::SColor(0,0,0,255));
		our_bill_node->setVisible(false);
		smgr->drawAll();

		driver->setRenderTarget(0, true, true, 0);
		our_bill_node->setVisible(true);
		smgr->drawAll();

		driver->endScene();


		//save frame
		IplImage* cv_img = render_avi->ITexture_to_IplImage(rt);

		//cvWriteFrame(aviOut, cv_img);
		cvShowImage("rt",cv_img);
		cvWaitKey(10);
		//save frame end
	}

	device->drop();

	return 0;
}
