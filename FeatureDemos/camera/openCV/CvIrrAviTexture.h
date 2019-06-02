#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <irrlicht.h>
#include <time.h>
#include <sys/timeb.h>

//I'm lazy
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CvIrrAviTexture
{
	public:
		CvIrrAviTexture(IVideoDriver* driver, char *file_name, int use_rgba_texture = 0);
		~CvIrrAviTexture();

		ITexture* getTexture();
		int UpdateTexture();
		void setAlphaKey(char r, char g, char b);
	private:
		//needed to manipulate irr textures
		IVideoDriver* driver;

		//our texture
		ITexture* irr_texture;

		//cv capture
		CvCapture* cv_capture;

		//seconds per frame
		double spf;
		double last_time;

		//alpha key
		char r_key,g_key,b_key;
		int use_alpha;

		void copy_over_image(IplImage* cv_img);
		double current_time();
};
