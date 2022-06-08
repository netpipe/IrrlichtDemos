#include "CvIrrCamTexture.h"

CvIrrCamTexture::CvIrrCamTexture(IVideoDriver* driver, int cvflags, int use_rgba_texture)
{
   char unique_tex_name[50];
   IplImage* cv_img = 0;

   //set this
   this->driver = driver;

   //start the capture
   this->cv_capture = cvCaptureFromCAM(cvflags);
   if(!this->cv_capture) return;

   //grab an image
   cv_img = cvQueryFrame(this->cv_capture);
   if(!cv_img) return;

   //for irrlicht scheme
   sprintf(unique_tex_name, "cv_image_CAM:%d", this);

   //make the texture
   if(use_rgba_texture)
      this->irr_texture = this->driver->addTexture(core::dimension2du(cv_img->width, cv_img->height), unique_tex_name, video::ECF_A8R8G8B8);
   else
      this->irr_texture = this->driver->addTexture(core::dimension2du(cv_img->width, cv_img->height), unique_tex_name, video::ECF_R8G8B8);

   //finally
   this->copy_over_image(cv_img);
}

CvIrrCamTexture::~CvIrrCamTexture()
{

}

ITexture* CvIrrCamTexture::getTexture()
{
   return this->irr_texture;
}

int CvIrrCamTexture::UpdateTexture()
{
   IplImage* cv_img = 0;

   //grab an image
   cv_img = cvQueryFrame(this->cv_capture);
   if(!cv_img) return 0;

   this->copy_over_image(cv_img);
}

void CvIrrCamTexture::copy_over_image(IplImage* cv_img)
{
   // read the pixels directly into the texture
   char* pixels = (char*)(this->irr_texture->lock());
   char* ardata = (char*)cv_img->imageData;
   char* final_loc = cv_img->imageSize + cv_img->imageData;

   switch(this->irr_texture->getColorFormat())
   {
      case ECF_R8G8B8:
         while(ardata < final_loc)
         {
            *pixels = *ardata;
            pixels++; ardata++;
            *pixels = *ardata;
            pixels++; ardata++;
            *pixels = *ardata;
            pixels++; ardata++;

         }
         break;

      case ECF_A8R8G8B8:
         while(ardata < final_loc)
         {
            *pixels = *ardata;
            pixels++; ardata++;
            *pixels = *ardata;
            pixels++; ardata++;
            *pixels = *ardata;
            pixels++; ardata++;

            pixels++;
         }
         break;
   }


   this->irr_texture->unlock();
}
