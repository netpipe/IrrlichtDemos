/** Example 012 Terrain Rendering

This tutorial will briefly show how to use the terrain renderer of Irrlicht. It
will also show the terrain renderer triangle selector to be able to do
collision detection with terrain.

Note that the Terrain Renderer in Irrlicht is based on Spintz'
GeoMipMapSceneNode, lots of thanks go to him. DeusXL provided a new elegant
simple solution for building larger area on small heightmaps -> terrain
smoothing.

In the beginning there is nothing special. We include the needed header files
and create an event listener to listen if the user presses certain keys.
*/
#include <irrlicht.h>
//#include "driverChoice.h"
//#include "exampleHelper.h"
#include "BitBuffer.hpp"
#include "QrCode.hpp"
#include "./IrrAssimp/IrrAssimp.h"

//maze generator
#include <mazegenerator/mainMaze.h>



#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>


#include <bits/stdc++.h>
using namespace std;

//#include "irr_svg_agg.h"

//

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

scene::ITerrainSceneNode* terrain;

	video::IVideoDriver* driver;
	scene::ISceneManager* smgr ;
	gui::IGUIEnvironment* env ;


	IGUIEditBox * qrcodeinput;



	int create3dQR();


#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//void save(IVideoDriver* driver, ITerrainSceneNode* terrain);


enum
{
	GUI_ID_CREATE_BUTTON = 101,

};

struct SAppContext
{
	IrrlichtDevice *device;
	s32				counter;
	//IGUIListBox*	listbox;
};


class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	//MyEventReceiver(scene::ISceneNode* terrain, scene::ISceneNode* skybox, scene::ISceneNode* skydome) :
	//	Terrain(terrain), Skybox(skybox), Skydome(skydome), showBox(true), showDebug(false)
	//{
	//	Skybox->setVisible(showBox);
	//	Skydome->setVisible(!showBox);
	//}

	virtual bool OnEvent(const SEvent& event)
	{


		if (event.EventType == EET_GUI_EVENT)
		{
						s32 id = event.GUIEvent.Caller->getID();
						//IGUIEnvironment* env = Context.device->getGUIEnvironment();
			switch(event.GUIEvent.EventType)
			{
				case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_CREATE_BUTTON:
					create3dQR();
					//Context.device->closeDevice();
					return true;
				}

			}
		}

//		// check if user presses the key 'W' or 'D'
//		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
//		{
//			switch (event.KeyInput.Key)
//			{
//			case irr::KEY_KEY_W: // switch wire frame mode
//				Terrain->setMaterialFlag(video::EMF_WIREFRAME,
//						!Terrain->getMaterial(0).Wireframe);
//				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
//				return true;
//			case irr::KEY_KEY_P: // switch point cloud mode
//				Terrain->setMaterialFlag(video::EMF_POINTCLOUD,
//						!Terrain->getMaterial(0).PointCloud);
//				Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
//				return true;
//			case irr::KEY_KEY_D: // toggle detail map
//				Terrain->setMaterialType(
//					Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
//					video::EMT_DETAIL_MAP : video::EMT_SOLID);
//				return true;
//			case irr::KEY_KEY_S: // toggle skies
//				showBox=!showBox;
//				Skybox->setVisible(showBox);
//				Skydome->setVisible(!showBox);
//				return true;
//			case irr::KEY_KEY_X: // toggle debug information
//				showDebug=!showDebug;
//				Terrain->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
//				return true;
//			default:
//				break;
//			}
//
//		}


		return false;
	}

private:
	scene::ISceneNode* Terrain;
	scene::ISceneNode* Skybox;
	scene::ISceneNode* Skydome;
	bool showBox;
	bool showDebug;
	SAppContext & Context;
};


//class agg_svg_loader : public irr::video::IImageLoader
//{
//public:
//   agg_svg_loader(IVideoDriver* driver)
//   {
//      video_driver = driver;
//   }
//   ~agg_svg_loader()
//   {
//      video_driver = 0;
//   }
//   virtual bool isALoadableFileExtension(const io::path& filename) const
//   {
//      return core::hasFileExtension ( filename, "svg" );
//   }
//   virtual bool isALoadableFileFormat(irr::io::IReadFile* file) const
//   {
//      return (false);
//   }
//   virtual irr::video::IImage* loadImage(irr::io::IReadFile* file) const
//   {
//      agg::svg::path_renderer m_path;
//      agg::svg::parser p(m_path);
//      p.parse(file->getFileName().c_str());
//      double m_min_x = 0.0;
//      double m_min_y = 0.0;
//      double m_max_x = 0.0;
//      double m_max_y = 0.0;
//      m_path.bounding_rect(&m_min_x, &m_min_y, &m_max_x, &m_max_y);
//      const dimension2d<u32>& image_size = dimension2d<u32>((irr::u32)m_max_x, (irr::u32)m_max_y);
//      IImage* image = video_driver->createImage(ECF_A8R8G8B8, image_size);
//      typedef row_accessor<irr::u32> rendering_buffer_u32;
//      rendering_buffer_u32 rbuf((irr::u32*)image->lock(), image_size.Width, image_size.Height, image_size.Width);
//      agg_pixel_type pixf(rbuf);
//      agg::renderer_base<agg_pixel_type> renb(pixf);
//      agg::renderer_scanline_aa_solid<agg::renderer_base<agg_pixel_type>> ren(renb);
//      renb.clear(agg::rgba8(255, 255, 255, 0));
//      agg::rasterizer_scanline_aa<> ras;
//      agg::scanline_p8 sl;
//      agg::trans_affine mtx;
//      agg::render_scanlines(ras, sl, ren);
//      m_path.render(ras, sl, ren, mtx, renb.clip_box(), 1.0);
//      image->unlock();
//      return image;
//   }
//   protected:
//      IVideoDriver* video_driver;
//};
//

static void doBasicDemo() {

std::wstring text ( qrcodeinput->getText() );

//char *text2 = text.c_str();
const wchar_t* wstr = text.c_str() ;
    char mbstr[4000];
    std::wcstombs(mbstr, wstr, 4000);


//	const char *text = //"Hello, world!";              // User-supplied text
//"tes1234567ajsl;dkfjasdlk;fj;lsadjf;lsdakjf;lsdkajf;alsdkjfs;dlakjfsdal;kjfsadlkjfsdaljfl;sdkaja;lskd \
//fjlskdjflskdajflksa;djflksjdaf;lkjsda;lfkjsdalkfjsd;lkfjsda;ljf;lsdakjfl;skadjf;lksdjfl;ksajdfl;ksdjlk\
//fj";

	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

	// Make and print the QR Code symbol
	//const QrCode qr = QrCode::encodeText( text.c_str() , errCorLvl);
		const QrCode qr = QrCode::encodeText( mbstr , errCorLvl);
	//printQr(qr);
	std::cout << qr.toSvgString(4) << std::endl;

	//system ( "" );
	ofstream write;

	std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << qr.toSvgString(4);
}

// borrowed fromn terrain-paint example)
int save (IVideoDriver* driver, ITerrainSceneNode* terrain)
{
   core::dimension2d<u32> dim (256,256);
   video::IImage *img = driver->createImage(ECF_R8G8B8, dim);

   u32 VertexCount = terrain->getMesh()->getMeshBuffer(0)->getVertexCount();
   S3DVertex2TCoords* verts = (S3DVertex2TCoords*)terrain->getMesh()->getMeshBuffer(0)->getVertices();

   for (u32 i=0; i<VertexCount; i++)
   {
     S3DVertex2TCoords* vertex = verts + i;
     u8 y = (u8)vertex->Pos.Y;
     img->setPixel((u32)vertex->Pos.X, (u32)vertex->Pos.Z, video::SColor(0, y,y,y));
   }

   driver->writeImageToFile(img, "qrcode.png", 0);
   img->drop();
}

int create3dQR(){



	doBasicDemo();

	//system("inkscape -z -e tmp.png -w 1000 -h 1000 tmp.svg");
	system("inkscape -z -e tmp.png -w 1000 -h 1000 maze.svg");


	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		//mediaPath + "terrain-heightmap.bmp", //
		"tmp.png", //terrain-heightmap.bmp
		0,					// parent node
		-1,					// node id
		core::vector3df(0.f, 0.f, 0.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		//core::vector3df(40.f, 4.4f, 40.f),	// scale
		core::vector3df(14.f, -1.51f, 14.f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		3,					// maxLOD
		scene::ETPS_17,				// patchSize
		1					// smoothFactor
		);



//		 smgr->getGeometryCreator()->createTerrainMesh(video::IImage* texture,
//            video::IImage* heightmap,
//            const core::dimension2d<f32>& stretchSize,
//            f32 maxHeight, video::IVideoDriver* driver,
//            const core::dimension2d<u32>& defaultVertexBlockSize,
//            bool debugBorders=false) const =0;


// inhouse svg support rather than external inkscape command
// todo export itexture
//	video_driver->addExternalImageLoader(new agg_svg_loader(video_driver));
//	ITexture* tex = video_driver->getTexture("test.svg");

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0,
			driver->getTexture( "tmp.png"));
	terrain->setMaterialTexture(1,
			driver->getTexture( "detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);

 irr::scene::IMeshSceneNode* const cube = smgr->addCubeSceneNode(50.0f, 0, -1, irr::core::vector3df(0, 100, 0.0f));

	//printf ("export stl mesh \n");
	//assimp.exportMesh(pMesh, "stl", "./qrcode.stl");

	/*
	To be able to do collision with the terrain, we create a triangle selector.
	If you want to know what triangle selectors do, just take a look into the
	collision tutorial. The terrain triangle selector works together with the
	terrain. To demonstrate this, we create a collision response animator
	and attach it to the camera, so that the camera will not be able to fly
	through the terrain.
	*/

	// create triangle selector for the terrain
	scene::ITriangleSelector* selector
		= smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, smgr->getActiveCamera(), core::vector3df(60,100,60),
		core::vector3df(0,-500,0),
		core::vector3df(0,50,0));
	selector->drop();
	smgr->getActiveCamera()->addAnimator(anim);
	anim->drop();

	/* If you need access to the terrain data you can also do this directly via the following code fragment.
	*/
	scene::CDynamicMeshBuffer* buffer = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
	terrain->getMeshBufferForLOD(*buffer, 0);
	video::S3DVertex2TCoords* data = (video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();
	// Work on data or get the IndexBuffer with a similar call.





//   btVector3 vertices[3];
//   s32 j,k;
//  // btTriangleMesh *  mTriMesh = new btTriangleMesh();
//
//   const irr::u32 vertexCount = buffer->getVertexCount();
//   const irr::u32 indexCount = buffer->getIndexCount();
//
//            irr::video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();
//
//   u16* mb_indices = buffer->getIndices();
//
//   for(j=0;j<indexCount;j+=3)
//   {
//      for (k=0;k<3;k++)
//      {
//         s32 index = mb_indices[j+k];
//         vertices[k] = btVector3(
//            mb_vertices[index].X,
//            mb_vertices[index].Y,
//            mb_vertices[index].Z);
//      }
//      mTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
//   }
	buffer->drop(); // When done drop the buffer again.

	// for some reason mesh not exporting properly yet.
	IMesh* pMesh = terrain->getMesh();
	//IMeshBuffer *mb=mesh->getMeshBuffer(0);

	printf ("export mesh");
	IrrAssimp assimp(smgr);
	assimp.exportMesh(pMesh, "obj", "./qrcode.obj");





}

/*
The start of the main function starts like in most other example. We ask the
user for the desired renderer and start it up. This time with the advanced
parameter handling.
*/
int main()
{
	// ask user for driver
//	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
//	if (driverType==video::EDT_COUNT)
	//	return 1;

	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr::SIrrlichtCreationParameters
	irr::SIrrlichtCreationParameters params;
	params.DriverType=EDT_OPENGL;
	params.WindowSize=core::dimension2d<u32>(640, 480);
	IrrlichtDevice* device = createDeviceEx(params);
	if (device == 0)
		return 1; // could not create selected driver.
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	env = device->getGUIEnvironment();

	//int generateMaze( );
	//-m 0 -w 40 -h 40 -a 2 -o -t 1 rectangular_BFS
	//../src/mazegen -m 4 -s 20 -a 0 -o circularhexagon_Kruskal
	char *argv1[]={"appname","-m","0","-w","40","-h","40","-a" ,"2","-o" ,"maze","-t","0","test"};//rectangular_BFS
//	char *argv1[]={"appname","-m","1","-s","20","-a", "0","-o" ,"maze","-t","0","hexagonal_Kruskal"};//hexagonal_Kruskal
	//char *argv1[]={"appname","-m","2","-s","20","-a", "1","-o" ,"maze","-t","0","honeycomb_DFS"};//honeycomb_DFS
	//char *argv1[]={"appname","-m","3","-s","20","-a", "1","-o" ,"maze","-t","0","circular_DFS"};//circular_DFS
	//	char *argv1[]={"appname","-m","4","-s","20","-a", "0","-o" ,"maze","-t","0","circularhexagon_Kruskal"};//circularhexagon_Kruskal
	//  char *argv1[]={"test","--help"};
	   int argc1 = sizeof(argv1) / sizeof(char*) - 1;
	//../src/mazegen -m 0 -w 40 -h 40 -a 2 -o -t 1 rectangular_BFS
	//../src/mazegen -m 1 -s 20 -a 0 -o -t 0 hexagonal_Kruskal
	//../src/mazegen -m 2 -s 20 -a 1 -o -t 1 honeycomb_DFS
	//../src/mazegen -m 3 -s 20 -a 1 -o -t 0 circular_DFS
	//../src/mazegen -m 4 -s 20 -a 0 -o circularhexagon_Kruskal
	generateMaze( argc1, argv1 );

	//"-m 4 -s 20 -a 0 -o circularhexagon_Kruskal"

	//generateMaze( );

	qrcodeinput = env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	IGUIButton * create = env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_CREATE_BUTTON,
			L"Create QR Code", L"created new 3d qr code");

	//env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
////	gui::IGUIStaticText* text = 0;
////	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
////	char *sometext = "hello";
////	text->setText(sometext);
////	guienv->addStaticText(text,core::rect<s32>(500,10,600,80),false,true);
////
////	IGUIStaticText* blah = guienv->addStaticText(L"",core::rect<s32>(500,10,600,80),false,true);
////
////	blah->setText(L"NewTextHere");

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

//	const io::path mediaPath = getExampleMediaPath();

	// add irrlicht logo
//	env->addImage(driver->getTexture(mediaPath + "irrlichtlogo2.png"),
//		core::position2d<s32>(10,10));

	//set other font
	//env->getSkin()->setFont(env->getFont( + "fontlucida.png"));

	// add some help text (let's ignore 'P' and 'X' which are more about debugging)
//	env->addStaticText(
//		L"Press 'W' to change wireframe mode\nPress 'D' to toggle detail map\nPress 'S' to toggle skybox/skydome",
//		core::rect<s32>(10,421,250,475), true, true, 0, -1, true);



	// add camera
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0,1000.0f,1.2f);

	camera->setPosition(core::vector3df(2700*2,255*2,2600*2));
	camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(42000.0f);


	device->getCursorControl()->setVisible(false);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
		driver->getTexture( "../../media/irrlicht2_up.jpg"),
		driver->getTexture( "../../media/irrlicht2_dn.jpg"),
		driver->getTexture( "../../media/irrlicht2_lf.jpg"),
		driver->getTexture( "../../media/irrlicht2_rt.jpg"),
		driver->getTexture( "../../media/irrlicht2_ft.jpg"),
		driver->getTexture( "../../media/irrlicht2_bk.jpg"));
	//scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"),16,8,0.95f,2.0f);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
//	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);

	/*
	That's it, draw everything.
	*/
create3dQR();
	int lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, video::SColor(0));

		smgr->drawAll();
		env->drawAll();

		driver->endScene();

		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;
			// Also print terrain height of current camera position
			// We can use camera position because terrain is located at coordinate origin
//			str += " Height: ";
//			str += terrain->getHeight(camera->getAbsolutePosition().X,
//					camera->getAbsolutePosition().Z);

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}

/*
Now you know how to use terrain in Irrlicht.
**/
