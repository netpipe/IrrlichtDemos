#include <irrlicht.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
#include <sys/timeb.h>

#include "CvIrrCamController.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;

//our cam controller object
CvIrrCamController* cam_control;

double current_time();
void do_fade(int face_found, int win_w, int win_h, ITexture *logo);

class MyEventReceiver : public IEventReceiver
{
	public:
		virtual bool OnEvent(const SEvent &event)
		{
			if (event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
			{
				vector3df cur_travel;
				vector2df cur_win_size;

				switch(event.KeyInput.Key)
				{
					case KEY_ESCAPE:
						device->closeDevice();
						return true;
					case KEY_KEY_Q:
						cur_travel = cam_control->getMaxTravel() + vector3df(10,0,0);
						cam_control->setMaxTravel(cur_travel);
						printf("Max Travel Now at: %f %f %f\n", cur_travel.X, cur_travel.Y, cur_travel.Z);
						return true;
					case KEY_KEY_A:
						cur_travel = cam_control->getMaxTravel() - vector3df(10,0,0);
						cam_control->setMaxTravel(cur_travel);
						printf("Max Travel Now at: %f %f %f\n", cur_travel.X, cur_travel.Y, cur_travel.Z);
						return true;
					case KEY_KEY_W:
						cur_travel = cam_control->getMaxTravel() + vector3df(0,10,0);
						cam_control->setMaxTravel(cur_travel);
						printf("Max Travel Now at: %f %f %f\n", cur_travel.X, cur_travel.Y, cur_travel.Z);
						return true;
					case KEY_KEY_S:
						cur_travel = cam_control->getMaxTravel() - vector3df(0,10,0);
						cam_control->setMaxTravel(cur_travel);
						printf("Max Travel Now at: %f %f %f\n", cur_travel.X, cur_travel.Y, cur_travel.Z);
						return true;
					case KEY_KEY_E:
						cur_travel = cam_control->getMaxTravel() + vector3df(0,0,10);
						cam_control->setMaxTravel(cur_travel);
						printf("Max Travel Now at: %f %f %f\n", cur_travel.X, cur_travel.Y, cur_travel.Z);
						return true;
					case KEY_KEY_D:
						cur_travel = cam_control->getMaxTravel() - vector3df(0,0,10);
						cam_control->setMaxTravel(cur_travel);
						printf("Max Travel Now at: %f %f %f\n", cur_travel.X, cur_travel.Y, cur_travel.Z);
						return true;
					case KEY_KEY_R:
						cur_win_size = cam_control->getViewWindowSizel() + vector2df(5,0);
						cam_control->setViewWindowSize(cur_win_size);
						printf("Window Size Now at: %f %f\n", cur_win_size.X, cur_win_size.Y);
						return true;
					case KEY_KEY_F:
						cur_win_size = cam_control->getViewWindowSizel() - vector2df(5,0);
						cam_control->setViewWindowSize(cur_win_size);
						printf("Window Size Now at: %f %f\n", cur_win_size.X, cur_win_size.Y);
						return true;
					case KEY_KEY_T:
						cur_win_size = cam_control->getViewWindowSizel() + vector2df(0,5);
						cam_control->setViewWindowSize(cur_win_size);
						printf("Window Size Now at: %f %f\n", cur_win_size.X, cur_win_size.Y);
						return true;
					case KEY_KEY_G:
						cur_win_size = cam_control->getViewWindowSizel() - vector2df(0,5);
						cam_control->setViewWindowSize(cur_win_size);
						printf("Window Size Now at: %f %f\n", cur_win_size.X, cur_win_size.Y);
						return true;
				}
			}
			return false;
		}
};

int main()
{
	bool fullscreen = false;
	MyEventReceiver receiver;


	device = createDevice( video::EDT_OPENGL, dimension2d<s32>(WINDOW_WIDTH, WINDOW_HEIGHT), 16, fullscreen, false, false, &receiver);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	device->setWindowCaption(L"CvIrrCamController Demo");

	//something for 3d reference
	IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("media/sydney.bmp") );
		node->setPosition(vector3df(-100,-30,80));
		node->setScale(vector3df(2,2,2));
	}

	//and some more
	device->getFileSystem()->addZipFileArchive("media/map-20kdm2.pk3");
	IAnimatedMesh* mesh2 = smgr->getMesh("20kdm2.bsp");
	ISceneNode* node2 = 0;
	if (mesh2)
		node2 = smgr->addOctTreeSceneNode(mesh2->getMesh(0), 0, -1, 128);
	if (node2)
		node2->setPosition(vector3df(-1300,-144,-1249));

	//our camera farie
	IAnimatedMeshSceneNode* fairy = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/faerie.md2"));
	if (fairy)
	{
		fairy->setMaterialTexture(0, driver->getTexture("media/faerie2.bmp")); // set diffuse texture
		fairy->setMaterialFlag(video::EMF_LIGHTING, false); // enable dynamic lighting
		fairy->setRotation(vector3df(0,180,0));
		fairy->setScale(vector3df(0.5,0.5,0.5));
		fairy->setMD2Animation(scene::EMAT_POINT);

	}

	//camera stuff
	vector3df camera_pos = vector3df(160,20,80);
	vector3df camera_target = vector3df(100,20,80);
	ICameraSceneNode* camera = smgr->addCameraSceneNode(0, camera_pos, camera_target);
	fairy->setParent(camera); //this is the "camera"

	//use this if you want to see the camera from a 3rd person view, otherwise comment out
	//ICameraSceneNode* debug_fps_camera = smgr->addCameraSceneNodeFPS();
	//debug_fps_camera->setPosition(camera_pos + vector3df(0,200,0));
	//debug_fps_camera->setTarget(camera_target);
	//smgr->setActiveCamera(debug_fps_camera);

	//load our controller class
	cam_control = new CvIrrCamController("haarcascade/haarcascade_frontalface_alt.xml");
	//set it to control our camera
	cam_control->setCamera(camera);
	//start the webcam
	cam_control->startWebcam();
	//helps the effect in this scenerio
	cam_control->setScaleTravel(vector3df(3,3,3));
	//and this unless you dont want it
	cam_control->ShowDebugWindow();

	float cam_mat[3][3];

	cam_mat[0][0] = 455.54370;
	cam_mat[0][1] = 0.0;
	cam_mat[0][2] = 155.05815;

	cam_mat[1][0] = 0.0;
	cam_mat[1][1] = 457.77859;
	cam_mat[1][2] = 114.30121;

	cam_mat[2][0] = 0.0;
	cam_mat[2][1] = 0.0;
	cam_mat[2][2] = 1.0;

	float lens_dist[4];

	lens_dist[0] = -0.01109;
	lens_dist[1] = -0.02224;
	lens_dist[2] = -0.00830;
	lens_dist[3] = -0.00075;

	cam_control->setCalib(cam_mat, lens_dist);
	cam_control->setCalibExtrinsic(vector3df(-6.5,-1,-5), vector3df(0,8,0));

	//simiosys logo
	ITexture *logo_img = 0;
	//ITexture *logo_img = driver->getTexture("media/filename");

	while(device->run())
	{
		//update the camera position based on head tracking
		int face_found = cam_control->UpdateCamPosition();

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();

        //for vanity
		//driver->draw2DRectangle(video::SColor(60,255,255,255), rect<s32>(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

		do_fade(face_found, WINDOW_WIDTH, WINDOW_HEIGHT, logo_img);

		driver->endScene();
	//	device->sleep(10,0);
	}

	device->drop();

	return 0;
}


void do_fade(int face_found, int win_w, int win_h, ITexture *logo)
{
     static double fade_i = 0;
     static double last_time = current_time();
     const int fade_max = 200;
     const double fade_ps = 40;
     const double unfade_ps = fade_ps * 2;


     if(face_found)
     {
         if(fade_i > 0)
         {
             double time_difference = current_time() - last_time;

             fade_i -= time_difference * unfade_ps;

             if(fade_i > 0)
             {
                 driver->draw2DRectangle(video::SColor((int)fade_i,0,0,0), rect<s32>(0, 0, win_w, win_h));
             }
             else
                 fade_i = 0;
         }
     }

     if(!face_found)
     {
         double time_difference = current_time() - last_time;

         if(fade_i < fade_max)
             fade_i += time_difference * fade_ps;

         if(fade_i > fade_max)
             fade_i = fade_max;

         driver->draw2DRectangle(video::SColor((int)fade_i,0,0,0), rect<s32>(0, 0, win_w, win_h));

         //logo
	 if(logo)
	 {
		dimension2di image_size = logo->getSize();
		int center_x = win_w / 2;
		int center_y = win_h / 2;
		int x = center_x - (image_size.Width / 2);
		int y = center_y - (image_size.Height / 2);
		rect<int> somerect = rect<int>(0, 0, image_size.Width, image_size.Height);
		driver->draw2DImage(logo,
				core::position2d<s32>(x,y),
				somerect,
				NULL,
				SColor((int)fade_i, 255, 255, 255),
				true
				);
	 }
     }

     last_time = current_time();
}

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

	return (new_time.time - first_sec) + ((new_time.millitm - first_msec) * 0.001);
}
