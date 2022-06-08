#include "irrAR.h"

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;
IARManager* armgr;

class MyEventReceiver : public IEventReceiver
{
	public:
		virtual bool OnEvent(const SEvent &event)
		{
			if (event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
			{
				switch(event.KeyInput.Key)
				{
					case KEY_ESCAPE:
						device->closeDevice();
						return true;
				}
			}
			return false;
		}
};

int main()
{
	bool fullscreen = false;
	MyEventReceiver receiver;

	device = createDevice( video::EDT_OPENGL, dimension2du(WIN_WIDTH, WIN_HEIGHT), 16, fullscreen, false, false, &receiver);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
//smgr->addCameraSceneNodeFPS();

	device->setWindowCaption(L"IrrAR Demo by Nighsoft");

	//something for 3d reference
	IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
		node->setMaterialTexture( 0, driver->getTexture("media/sydney.bmp") );
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_POINT);
		node->setPosition(vector3df(0,50,0));
		node->setScale(vector3df(2,2,2));
	}

	//fairy
	IAnimatedMeshSceneNode* fairy = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/faerie.md2"));
	if (fairy)
	{
		fairy->setMaterialTexture(0, driver->getTexture("media/faerie2.bmp"));
		fairy->setMaterialFlag(video::EMF_LIGHTING, false);
		fairy->setMD2Animation(scene::EMAT_POINT);
		fairy->setScale(vector3df(2,2,2));
		fairy->setPosition(vector3df(0,45,0));
		fairy->setRotation(vector3df(0,180,0));
	}

	//dwarf
	IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/dwarf.x"));
	dwarf->setRotation(vector3df(0,180,0));
	dwarf->setMaterialFlag(video::EMF_LIGHTING, false);
	dwarf->setAnimationSpeed(15);

	//water
	mesh = smgr->addHillPlaneMesh("myHill", dimension2d<f32>(80,80), dimension2d<u32>(3,2), 0, 0, dimension2d<f32>(0,0), dimension2d<f32>(10,10));
	ISceneNode* water = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	water->setMaterialTexture(0, driver->getTexture("media/stones.jpg"));
	water->setMaterialTexture(1, driver->getTexture("media/water.jpg"));
	water->setMaterialType(video::EMT_REFLECTION_2_LAYER);
	water->setMaterialFlag(video::EMF_LIGHTING, false);
	water->setPosition(vector3df(100,0,-50));
	water->setParent(dwarf);

	//camera stuff
	vector3df camera_pos = vector3df(0,0,0);
	vector3df camera_target = vector3df(0,0,1);
	ICameraSceneNode* camera = smgr->addCameraSceneNode(0, camera_pos, camera_target);

	//AR stuff
	IARManager* armgr = new IARManager(device);
	//init the camera
	//armgr->beginCamera("ardata/camera_para.dat","ardata/WDM_camera_flipV.xml", "-mode=NTSC -channel=1 -palette=YUV420P -dev=/dev/video0");
	//armgr->beginCamera("ardata/camera_para.dat","ardata/WDM_camera_flipV.xml", "v4l2src device=/dev/video0 use-fixed-fps=false ! ffmpegcolorspace ! capsfilter caps=video/x-raw-yuv,bpp=24,width=640,height=480 ! identity name=artoolkit ! fakesink");
    armgr->beginCamera("ardata/camera_para.dat","ardata/WDM_camera_flipV.xml", "v4l2src device=/dev/video0 ! video/x-raw-rgb,width=640,height=480,framerate=(fraction)30/1 ! ffmpegcolorspace ! identity name=artoolkit ! fakesink");
	//node for the lady, attached the the hiro pattern
	armgr->addARSceneNode("ardata/patt.kanji", node);
	//node for the fairy and water, attached the the sample1 pattern
	armgr->addARSceneNode("ardata/patt.sample1", fairy);
	//node for the dwarf, attached to a multi-pattern
	armgr->addARMultiSceneNode("ardata/multi/marker.dat", dwarf);
	//now fix our camera to display right
	armgr->fixCamera(camera);

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		//make movements and bring in new image
		armgr->run();

		//draw the background image centered and scaled
		armgr->drawBackground();

		smgr->drawAll();

		driver->endScene();
		device->sleep(10,0);
	}

	device->drop();

	return 0;
}
