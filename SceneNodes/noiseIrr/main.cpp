#include "noise.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//------------------------------------------------------------------------------------------------
// Default device settings
//------------------------------------------------------------------------------------------------
E_DRIVER_TYPE driverType = EDT_OPENGL;
int deviceWidth = 512;
int deviceHeight = deviceWidth;
int bits = 32;
int AA = 0; // AntiAliasing off default
bool AnisotropicFilter = false;
bool StencilBuffer = false; // Shadows
bool windowMode = false; // false = windowed
bool vsync = false;
SIrrlichtCreationParameters Parameters;

int main()
{
   int NoiseSize = 512;
   int NoiseZoom = 40;
   int Octaves = 5;
   double Persistance = .60;
   bool Colored = 0;

   Parameters.AntiAlias = AA;
   Parameters.DriverType = driverType;
   Parameters.Bits = bits;
   Parameters.Vsync = vsync;
   Parameters.Fullscreen = windowMode;
   Parameters.WindowSize.Width = deviceWidth;
   Parameters.WindowSize.Height = deviceHeight;

   IrrlichtDevice* device =
      createDeviceEx(Parameters);
   if (device == 0)
      return 1;

   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();

   driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY, true);
   driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);


   Noise::Render_Clouds(Colored,NoiseSize,NoiseSize,NoiseZoom,Octaves,Persistance, 0, 0, 0,driver);
   ITexture* Map =
      driver->getTexture("NoiseMaps/noise.bmp");


   int lastFPS = -1;

   while(device->run())
   if (device->isWindowActive())
   {
      driver->beginScene(true, true, SColor(255,200,150,150));
      smgr->drawAll();

      driver->draw2DImage(Map,vector2d<s32>(0,0)); // display noise image

      driver->endScene();

      int fps = driver->getFPS();
      if (lastFPS != fps)
      {
         stringw str = L"Endless Engine [";
         str += driver->getName();
         str += "] FPS:";
         str += fps;

         device->setWindowCaption(str.c_str());
         lastFPS = fps;
      }
    }
   device->drop();

   return 0;
}
