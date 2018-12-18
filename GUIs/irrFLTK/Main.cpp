//author hendu

#include <irrlicht.h>
#include <cstdio>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
   #include <unistd.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

void btn_pressed(Fl_Widget*, void *data) {

   ISceneNode *ptr = (ISceneNode *) data;

   vector3df pos = ptr->getPosition();
   pos.Y++;
   ptr->setPosition(pos);
}

int main() {
   IrrlichtDevice *device =
      createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
         false, false, false, 0);

   if (!device)
      return 1;

   IVideoDriver *driver = device->getVideoDriver();
   ISceneManager *smgr = device->getSceneManager();
   ITimer *timer = device->getTimer();

   IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
   if (!mesh) {
      device->drop();
      return 1;
   }
   IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

   if (node) {
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setMD2Animation(scene::EMAT_STAND);
      node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
   }

   ISceneNode *cam = smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

   int width, a;
   Fl::screen_xywh(a,a,width,a);
   // Yes, we could get screen width via Irrlicht too.

   Fl_Window *window = new Fl_Window(width-300,0,300,180,"Edit properties");
   Fl_Button *btn = new Fl_Button(20,40,260,100,"Go higher!");
   btn->user_data(cam);
   btn->callback(btn_pressed);
   window->end();
   window->show();

   int fps=-1, lastfps= -1;
   long long time, lasttime = timer->getTime(), timediff;
   wchar_t cfps[7];


   while(device->run()) {
      driver->beginScene(true, true, SColor(255,100,101,140));
      smgr->drawAll();
      driver->endScene();

      fps = driver->getFPS();
      if (fps != lastfps) {
         swprintf(cfps,7,L"%d",fps);
         cfps[6] = L'\0';
         // Why this and not stringw + whatever?
         // Think of it the next time you meditate.
         device->setWindowCaption(cfps);
         lastfps = fps;
      }

      Fl::check(); // This keeps our FLTK windows interactive.

      time = timer->getTime();
      timediff = 16-(time-lasttime);
      if (timediff > 0) device->sleep(timediff);
      // Yes, this is unrelated to FLTK.
      // I just prefer all my projects to be efficient.

      lasttime = timer->getTime();
      usleep(1000);
         }

   device->drop();

   return 0;
}
