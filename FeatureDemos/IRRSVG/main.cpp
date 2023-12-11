
#include <irrlicht.h>


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
To be able to use the Irrlicht.DLL file, we need to link with the Irrlicht.lib.
We could set this option in the project settings, but to make it easy, we use a
pragma comment lib for VisualStudio. On Windows platforms, we have to get rid
of the console window, which pops up when starting a program with main(). This
is done by the second pragma. We could also use the WinMain method, though
losing platform independence then.
*/
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include "irr_svg_agg.h"

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



/*
This is the main method. We can now use main() on every platform.
*/
int main()
{

	IrrlichtDevice *device =
		createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	/*
	Set the caption of the window to some nice text. Note that there is an
	'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	/*
	Get a pointer to the VideoDriver, the SceneManager and the graphical
	user interface environment, so that we do not always have to write
	device->getVideoDriver(), device->getSceneManager(), or
	device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	The text is placed at the position (10,10) as top left corner and
	(260,22) as lower right corner.
	*/
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);


	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	/*
	To let the mesh look a little bit nicer, we change its material. We
	disable lighting because we do not have a dynamic light in here, and
	the mesh would be totally black otherwise. Then we set the frame loop,
	such that the predefined STAND animation is used. And last, we apply a
	texture to the mesh. Without it the mesh would be drawn using only a
	color.
	*/
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}


	video_driver->addExternalImageLoader(new agg_svg_loader(video_driver));
	ITexture* tex = video_driver->getTexture("test.svg");



	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	/*
	Ok, now we have set up the scene, lets draw everything: We run the
	device in a while() loop, until the device does not want to run any
	more. This would be when the user closes the window or presses ALT+F4
	(or whatever keycode closes a window).
	*/
	while(device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	/*
	After we are done with the render loop, we have to delete the Irrlicht
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/
	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
