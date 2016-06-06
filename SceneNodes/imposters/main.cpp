
#include <irrlicht.h>


#include "CImpostorSceneNode.h"

//#define GUI

// for stats display
#ifdef GUI
#include "../../tools/GUIEditor/CGUIEditFactory.h"
#include "../../tools/GUIEditor/CGUIAttributeEditor.h"
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

class myevents : public IEventReceiver
{
	public: myevents() : enable(true) { }
	bool OnEvent(const SEvent& e)
	{
		if (e.EventType == EET_KEY_INPUT_EVENT && e.KeyInput.PressedDown && e.KeyInput.Key == KEY_SPACE)
			{ enable = !enable; cam->setInputReceiverEnabled(enable); }
		return false;
	}
	ICameraSceneNode* cam;
	bool enable;
} events;

int main()
{
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2du(800, 600), 32,
		//createDevice( video::EDT_BURNINGSVIDEO, dimension2d<s32>(800, 600), 32,
		//createDevice( video::EDT_OPENGL, dimension2d<s32>(800, 600), 32,
		//createDevice( video::EDT_SOFTWARE, dimension2d<s32>(800, 600), 32,
			false, false, false, &events);

	device->setWindowCaption(L"Impostors");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* env = device->getGUIEnvironment();

	CImpostorSceneNode* imp = new CImpostorSceneNode(smgr->getRootSceneNode(), smgr, -1, device->getTimer());





	#define GRID_SIZE 5
	#define OBJ_SIZE 300.0f
	#define PARENT imp
	//#define PARENT 0
	//imp->setDebugDataVisible(EDS_FULL);
	//imp->setDebugDataVisible(EDS_BBOX_BUFFERS);
	//imp->setDebugDataVisible(EDS_MESH_WIRE_OVERLAY);
	imp->setDebugDataVisible(E_DEBUG_SCENE_TYPE( EDS_BBOX_BUFFERS | EDS_MESH_WIRE_OVERLAY ));





	IGUIFont *font = env->getFont("../../media/lucida.xml");

	env->getSkin()->setFont(font);
	imp->setDebugFont(font);
	device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");


	imp->drop();

	IMesh* meshes[3];
	meshes[0] = smgr->getMesh("../../media/earth.x")->getMesh(0);
	meshes[1] = smgr->getMesh("20kdm2.bsp")->getMesh(0);
	meshes[2] = smgr->getMesh("../../media/dwarf.x")->getMesh(0);
	meshes[1]->getMeshBuffer(0)->setHardwareMappingHint(EHM_STATIC);

	for (s32 x=0; x< GRID_SIZE; ++x)
		for (s32 z=0; z< GRID_SIZE; ++z)
	{
		u32 num=((x+1*z+1)+z) % 3;
		//num = 1;

		ISceneNode* node =
			smgr->addMeshSceneNode( meshes[num], PARENT );
			//smgr->addOctTreeSceneNode( smgr->getMesh("20kdm2.bsp"), PARENT );

		node->setMaterialFlag(EMF_LIGHTING, false);

		vector3df p(x * OBJ_SIZE * 1.0, 0.0, z * OBJ_SIZE * 1.5);
		node->setPosition(p);

		// scale them to fit size
		core::vector3df s = meshes[num]->getBoundingBox().getExtent();
		f32 max = core::max_<f32>(s.Y, core::max_<f32>(s.X, s.Z));
		f32 scale = OBJ_SIZE / max;
		node->setScale( vector3df(scale,scale,scale));

	}

	events.cam = smgr->addCameraSceneNodeFPS();

#ifdef GUI
	// gui attribute editors
	IGUIElementFactory* factory = new CGUIEditFactory(env);
	env->registerGUIElementFactory(factory);
	factory->drop();

	CGUIAttributeEditor* PerfStats = (CGUIAttributeEditor*) env->addGUIElement("attributeEditor", env->getRootGUIElement());
	s32 h = device->getVideoDriver()->getScreenSize().Height / 2;
	s32 w = 160;
	PerfStats->setRelativePosition(core::rect<s32>(0,0,w,h));
	PerfStats->setBorder(true);

	CGUIAttributeEditor* Attribs = (CGUIAttributeEditor*) env->addGUIElement("attributeEditor", env->getRootGUIElement());
	Attribs->setRelativePosition(core::rect<s32>(0,h-1,w,h*2));
	Attribs->setBorder(true);

	io::IAttributes *PerfStatsAttribs = PerfStats->getAttribs();
	io::IAttributes *NodeInfo = Attribs->getAttribs();
	imp->serializeAttributes(NodeInfo);
	Attribs->refreshAttribs();
	// bug
	Attribs->draw();
	Attribs->setRelativePosition(core::rect<s32>(0,h-1,w,h*2));

	IGUIImage* img = env->addImage(imp->WorkTexture, core::position2di(600,0));
	img->setRelativePosition(core::rect<s32>(600,0,800,200));
	img->setScaleImage(true);


#endif

	while(device->run())
		//if (device->isWindowActive())
		{
			#ifdef GUI
			imp->getStats(PerfStatsAttribs);
			PerfStats->refreshAttribs();
			#endif

			stringw display = L"Impostors! FPS: ";
			display += driver->getFPS();
			display += L", Polys: ";
			display += driver->getPrimitiveCountDrawn();

			driver->beginScene(true, true,SColor(0,0,0,100));//SColor(255,100,101,140));
			device->setWindowCaption(display.c_str());

			smgr->drawAll();
			env->drawAll();

			//imp->doRenderQueue();

			driver->endScene();
		}

	device->drop();

	return 0;
}

