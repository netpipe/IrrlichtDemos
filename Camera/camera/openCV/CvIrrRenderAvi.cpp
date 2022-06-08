#include "CvIrrRenderAvi.h"

CvIrrRenderAvi::CvIrrRenderAvi(IVideoDriver* driver, char *filename, int width, int height, int desired_fps)
{
	//set 
	this->driver = driver;
	
	//make out pointer
	//this->vidp = cvCreateVideoWriter(filename, CV_FOURCC('I', 'V', '4', '1'), desired_fps, cvSize(width, height), 1);
	
	//create our cv_img
	this->cv_img = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 3);

}

CvIrrRenderAvi::~CvIrrRenderAvi()
{
	//unload img
	cvReleaseImage(&this->cv_img);
	
	//unload vid writer
	cvReleaseVideoWriter(&this->vidp); 
}

IplImage* CvIrrRenderAvi::ITexture_to_IplImage(ITexture *src)
{
	// read the pixels directly into the texture
	u8* pixels = (u8*)(src->lock());
	u8* ardata = (u8*)cv_img->imageData;
	u8* final_loc = (u8*)(this->cv_img->imageSize + this->cv_img->imageData);

	printf("pixels:%d src:%d w:%d h:%d\n",pixels,src, src->getOriginalSize().Width, src->getOriginalSize().Height);
	printf("w:%d h:%d w*h:%d s:%d\n", this->cv_img->width, this->cv_img->height, this->cv_img->width * this->cv_img->height, this->cv_img->imageSize);

	while(ardata < final_loc)
	{
		*ardata = *pixels;
		pixels++; ardata++;
		*ardata = *pixels;
		pixels++; ardata++;
		*ardata = *pixels;
		pixels++; ardata++;
		
		pixels++;
	}
	
	src->unlock();
	
	return this->cv_img;
}
