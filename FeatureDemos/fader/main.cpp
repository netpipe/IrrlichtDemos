#include "CFadeAnimator.h"
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice* device;
IAnimatedMeshSceneNode* node;
// The vector to hold your fade textures //
std::vector<ITexture*> FadeTextures;

class MyEventReceiver : public IEventReceiver{
public:
  virtual bool OnEvent(const SEvent& event){
    if (event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown){
      switch(event.KeyInput.Key){
        case KEY_KEY_1:{ // fade out the node within 2 seconds (2000 ms)
          node->removeAnimators();
          CFadeAnimator* anim = new CFadeAnimator(CFA_FADEOUT, (u32)2000, device->getTimer()->getTime(), &FadeTextures);
          node->addAnimator(anim);
          anim->drop();
          return true;
          }
        case KEY_KEY_2:{ // fade in the node within 2 seconds (2000 ms)
          node->removeAnimators();
          CFadeAnimator* anim = new CFadeAnimator(CFA_FADEIN, (u32)2000, device->getTimer()->getTime(), &FadeTextures);
          node->addAnimator(anim);
          anim->drop();
          return true;
          }
        case KEY_ESCAPE:{ // close the program
          device->closeDevice();
          return true;
        }
      }
    }
    return false;
  }
};

int main(){
  MyEventReceiver receiver;
  device = createDevice(EDT_OPENGL, dimension2du(640, 480), 16, false, false, false, &receiver);
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  // add some info text
  guienv->addStaticText(L"1 - fade out\n2 - fade in\nEscape - quit", rect<int>(10,10,200,220));

  // Get your texture and some necessary attributes
  ITexture *source = driver->getTexture("../../media/sydney.bmp");
  dimension2d<u32> dim = source->getSize();
  ECOLOR_FORMAT format = source->getColorFormat();

  for (int i = 0; i <= 255; i+=5) {
    // Change alpha channel based on "i" //
    SColor* pixels = (SColor*)source->lock();
    for (int x = 0; x < dim.Width; x++) {
      for (int y = 0; y < dim.Height; y++) {
        pixels[y*dim.Width + x].setAlpha(i);
      }
    }
    source->unlock();

    // Create a new image and load it as a texture //
    IImage* image = driver->createImageFromData(format, dim, pixels, false);
    ITexture *texture = driver->addTexture("", image);
    FadeTextures.push_back(texture);
  }

  // Create the node
  IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
  node = smgr->addAnimatedMeshSceneNode(mesh);
  node->setMaterialTexture(0, source);
  node->setMaterialFlag(EMF_LIGHTING, false);
  node->setFrameLoop(1, 1);

  // Set the nodes parameters for blending on alpha channel
  node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
  node->getMaterial(0).MaterialTypeParam = 0.01;

  // run Irrlicht render loop
  smgr->addCameraSceneNode()->setPosition(vector3df(0,10,-50));
  while(device->run()){
    driver->beginScene(true, true, video::SColor(0,0,110,110));
    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
  }
  device->drop();
  return 0;
}
