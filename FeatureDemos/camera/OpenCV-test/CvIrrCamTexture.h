#include <cv.h>
#include <highgui.h>
#include <irrlicht/irrlicht.h>
#include <math.h>

//I'm lazy
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CvIrrCamTexture
{
   public:
      CvIrrCamTexture(IVideoDriver* driver, int cvflags = 0, int use_rgba_texture = 0);
      ~CvIrrCamTexture();

      ITexture* getTexture();
      int UpdateTexture();
   private:
      //needed to manipulate irr textures
      IVideoDriver* driver;

      //our texture
      ITexture* irr_texture;

      //cv capture
      CvCapture* cv_capture;

      void copy_over_image(IplImage* cv_img);
};

