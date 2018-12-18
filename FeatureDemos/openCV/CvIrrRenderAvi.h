#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <irrlicht.h>

//This class has stopped development because of a bug in Irrlicht

//I'm lazy
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CvIrrRenderAvi
{
	public:
		CvIrrRenderAvi(IVideoDriver* driver, char *filename, int width, int height, int desired_fps = 10);
		~CvIrrRenderAvi();

		IplImage* ITexture_to_IplImage(ITexture *src);
	private:
		//needed
		IVideoDriver* driver;

		//opnecv
		CvVideoWriter* vidp;
		IplImage* cv_img;
};
