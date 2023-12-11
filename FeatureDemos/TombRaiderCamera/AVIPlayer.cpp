// 17.12.2006 - IAVIPlayer - Demo by A. Buschhüter (Acki)
//                    A_Buschhueter@gmx.de
//                 http://abusoft.g0dsoft.com

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(){
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<s32>(800, 600), 16, false, false, false, 0);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	device->setWindowCaption(L"AVIPlayer-Demo");

	// Create movie
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	IAVIPlayer* MyAvi = smgr->createAVIPlayer();
	MyAvi->OpenAVIStream("gfx/intro.avi", driver, true);
  MyAvi->SetLooped(true);
	// get the dimension of the movie
  dimension2d<u16> dimMov = MyAvi->getMovieDimension();
  // get the texture of the movie
  ITexture* txt = MyAvi->getVideoTexture();
  // get the rect to draw the movie in center of the screen
  rect<s32> posMovie = rect<s32>(
    (800 - dimMov.Width) / 2,                    // upper left corner X
    (600 - dimMov.Height) / 2,                   // upper left corner Y
    (800 - dimMov.Width) / 2 + dimMov.Width,     // lower right corner X
    (600 - dimMov.Height) / 2 + dimMov.Height);  // lower right corner Y

	// setup and run Irrlicht render loop
  smgr->addCameraSceneNode();
	while(device->run()){
		driver->beginScene(true, true, video::SColor(0,0,100,100));
		smgr->drawAll();
    // get the next frame and draw the texture
		if(MyAvi->IsRunning()){
		  // since Irrlicht v1.3 it seems you'll have to call this twice !?!?!
		  MyAvi ->DrawMovie();
		  MyAvi ->DrawMovie();
		  driver->draw2DImage(txt, posMovie, rect<s32>(0, 0, dimMov.Width, dimMov.Height));
		}
		driver->endScene();

		if(!MyAvi->IsRunning())
      device->closeDevice();
	}
	MyAvi->drop();
	device->drop();
	return 0;
}
