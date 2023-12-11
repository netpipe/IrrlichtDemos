#ifndef INTRO_H
#define INTRO_H
#include "players.h"
#pragma comment(lib, "Irrlicht.lib")
/*================================================
FOR LATER IMPROVEMENT
================================================*/
joystick* joystick1;
joystick* joystick2;
//=================================================
int frameH;
int frameW;
int fps;


irr::video::ITexture* create_ITexture_from_CvCapture(CvCapture* capture)
{
   //grab an image
   IplImage* img = NULL;
   if(!cvGrabFrame(capture))
   {
      printf("Could not grab a frame\n");
      return 0;
   }
   img=cvRetrieveFrame(capture);

   //get these variables about the video
   frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
   frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
   fps       = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
   
   //make our texture
   irr::video::ITexture* m_poTileTexture = device->getVideoDriver()->addTexture(irr::core::dimension2d<irr::s32>(frameW, frameH), "fucker", irr::video::ECF_A1R5G5B5);

   /// read the pixels directly into the texture
   irr::u8* pixels = (irr::u8*)m_poTileTexture->lock();
   irr::u8* ardata = (irr::u8*)img->imageData;
   int max_pixels = frameW * frameH;

   for(int i=0;i<max_pixels;i++)
   {
      *pixels = *ardata;
      pixels++; ardata++;
      *pixels = *ardata;
      pixels++; ardata++;
      *pixels = *ardata;
      pixels++; ardata++;
   
      pixels++;
   }

   m_poTileTexture->unlock();

   return m_poTileTexture;
}

double current_time();

bool intro() {
   
   //choose your own avi's
   //refer to this if needed to convert your vid to cv usable...
   //mencoder in.avi -ovc raw -vf format=i420 -o out.avi
   CvCapture* capture = cvCaptureFromFile("./intro.avi");
   
   if (!capture) {
      std::cout << "Error 23: Could not initialize intro...\n";
      return 0;
   }

   //get the initial image
   irr::video::ITexture* CvImage = create_ITexture_from_CvCapture(capture);
   if (!CvImage) {
      std::cout << "Error 24: Could not create a texture..\n";
      return 0;
   }
   
   //fps stuff with a bit more precision
   double last_time = current_time();
   double add_on_time = 0;

   double begin_time = current_time();
   
   //set current time
   bool first_run=true;
   clock_t startintro = std::clock();
   irr::video::IVideoDriver* driver = device->getVideoDriver();
   enginesound->play2D("./Game Data/music/smile pretty for the devil.mp3", false, false, true);
   int frames_to_capture;
   IplImage* img;
   irr::u8* pixels;
   irr::u8* ardata;
   while (device->run()&&((( std::clock() - startintro ) / (double)CLOCKS_PER_SEC )<210))
   {
      driver->beginScene(false, false, irr::video::SColor(255,0,0,0));
      driver->draw2DImage(CvImage, irr::core::rect<irr::s32>(0,0,res.x,res.y), irr::core::rect<irr::s32>(0,0,frameW,frameH));
      driver->endScene();

      if (EventRec.IsKeyDown(irr::KEY_ESCAPE))
         break;
      //alittle code to try and keep it all at the right speed
      frames_to_capture = fps * (current_time() - last_time + add_on_time);
      if (frames_to_capture) {
         add_on_time = current_time() - last_time - (frames_to_capture / fps);
      }
      else {
         add_on_time = add_on_time + current_time() - last_time;
      }
      //grab an image and potentially skip some frames
      if (!cvGrabFrame(capture)) {
         std::cout << "Could not grab a frame\n";
         break;
      }
      img = cvRetrieveFrame(capture);
   
      //now put the img data into the texture data
      pixels = (irr::u8*) CvImage->lock();
      ardata = (irr::u8*) img->imageData;
      int max_pixels = frameH * frameW;
      for(int i=0;i<max_pixels;i++) {
         *pixels = *ardata;
         pixels++; ardata++;
         *pixels = *ardata;
         pixels++; ardata++;
         *pixels = *ardata;
         pixels++; ardata++;
   
         pixels++;
      }
      irr::core::stringw str = L"Project Ninja Star - Nexuiz Map example FPS:";
      str += driver->getFPS();
      device->setWindowCaption(str.c_str());
      CvImage->unlock();
      last_time = current_time();
   }
   enginesound->removeSoundSource("./Game Data/music/smile pretty for the devil.mp3");
   device->getVideoDriver()->removeTexture(CvImage);
   return 1;
}



//this function brings back seconds with milliseconds since it was first called
double current_time()
{
   static int first_sec = 0;
   static int first_msec = 0;
   struct timeb new_time;
   
   //set current time
   ftime(&new_time);

   //set if not set
   if(!first_sec)
   {
      first_sec = new_time.time;
      first_msec = new_time.millitm;
   }
   
   return ((new_time.time - first_sec) + ((new_time.millitm - first_msec) * 0.001)*1.55);
}
#endif
