#include "CvIrrAviTexture.h"

CvIrrAviTexture::CvIrrAviTexture(IVideoDriver* driver, char *file_name, int use_rgba_texture)
{
	char unique_tex_name[50];
	IplImage* cv_img = 0;
	
	//assume no
	this->use_alpha = 0;
	
	//set this
	this->driver = driver;
	
	//start the capture
	this->cv_capture = cvCaptureFromFile(file_name);
	if(!this->cv_capture) return;

	//grab an image
	cv_img = cvQueryFrame(this->cv_capture);
	if(!cv_img) return;
	
	//set time
	this->last_time = this->current_time();
	
	//get the fps for this vid
	this->spf = 1.0 / cvGetCaptureProperty(this->cv_capture, CV_CAP_PROP_FPS);
	
	//for irrlicht scheme
	sprintf(unique_tex_name, "cv_image_AVI:%d", this);
	
	//make the texture
	if(use_rgba_texture)
		this->irr_texture = this->driver->addTexture(core::dimension2d<s32>(cv_img->width, cv_img->height), unique_tex_name, video::ECF_A8R8G8B8);
	else
		this->irr_texture = this->driver->addTexture(core::dimension2d<s32>(cv_img->width, cv_img->height), unique_tex_name, video::ECF_R8G8B8);
	
	//finally
	this->copy_over_image(cv_img);
}

CvIrrAviTexture::~CvIrrAviTexture()
{
	//release the capture and texture
	if(this->cv_capture);
		cvReleaseCapture(&this->cv_capture);
}

ITexture* CvIrrAviTexture::getTexture()
{
	return this->irr_texture;
}

int CvIrrAviTexture::UpdateTexture()
{
	IplImage* cv_img = 0;
	double time_since;

	//make time_since
	time_since = this->current_time() - this->last_time;

	//grab an image
	for( ;time_since > this->spf; time_since -= this->spf)
		cv_img = cvQueryFrame(this->cv_capture);
	
	if(!cv_img) return 0;
	
	//set time
	this->last_time = this->current_time() - time_since;
	
	this->copy_over_image(cv_img);
}

void CvIrrAviTexture::setAlphaKey(char r, char g, char b)
{
	this->r_key = r;
	this->g_key = g;
	this->b_key = b;
	
	//well
	this->use_alpha = 1;
}

void CvIrrAviTexture::copy_over_image(IplImage* cv_img)
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
			if(this->use_alpha)
			{
				while(ardata < final_loc)
				{
					*pixels = *ardata;
					pixels++; ardata++;
					*pixels = *ardata;
					pixels++; ardata++;
					*pixels = *ardata;
					pixels++; ardata++;
					
					*pixels = 255;
					if(ardata[-3] < this->r_key + 20 && ardata[-3] > this->r_key - 20)
						if(ardata[-2] < this->g_key + 20 && ardata[-2] > this->g_key - 20)
							if(ardata[-1] < this->b_key + 20 && ardata[-1] > this->b_key - 20)
								*pixels = 0;
						
					pixels++;
				}
			}
			else
			{
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
			}
			break;
	}


	this->irr_texture->unlock();
}

double CvIrrAviTexture::current_time()
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
	
	return (new_time.time - first_sec) + ((new_time.millitm - first_msec) * 0.001);
}
