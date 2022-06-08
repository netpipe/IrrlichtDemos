#ifndef __IRRAR_H_INCLUDED__
#define __IRRAR_H_INCLUDED__

#include <irrlicht.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/arMulti.h>

//I'm lazy
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define MAX_PATTERNS 100
#define MAX_MULTIS 20

class IARManager
{
	public:
		IARManager(IrrlichtDevice* device);
		~IARManager();

		dimension2di beginCamera(char* cparam_filename, char* win32_vconf_filename = 0, char* unix_config_string = "-dev=/dev/video0");
		ISceneNode* addARSceneNode(char* patt_filename, ISceneNode *initial_child = 0, s32 id = -1);
		ISceneNode* addARMultiSceneNode(char* config_filename, ISceneNode *initial_child = 0, s32 id = -1);
		ITexture* getCameraTexture();
		int run();
		void drawBackground();
		void fixCamera(ICameraSceneNode* camera);

	private:
		int initCamera(char* cparam_filename, char* vconf_filename, int *xsize, int *ysize);
		void create_camera_texture(dimension2du dimensions);
		void create_projection_matrix(ARParam *cparam, double zoom, int fullFlag, int xwin, int ywin, int hmd_flag, float m[16]);
		void our_argConvGLcpara2(double cparam[3][4], int width, int height, double gnear, double gfar, float m[16]);
		void our_convert_trans_para(double para[3][4], double gl_para[16]);
		ITexture* update_ITexture(ITexture *dest, ARUint8 *dataPtr);
		void translate_nodes(ARUint8 *ardata);


		//Irrlicht
		IrrlichtDevice* device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		ITexture* cam_texture;

		//ARToolkit variables
		ARParam wparam;
		ARParam cparam;
		float gl_cpara[16];

		//patterns
		int patt_id[MAX_PATTERNS];
		ISceneNode* patt_node[MAX_PATTERNS];
		ARMultiMarkerInfoT* multi_config[MAX_MULTIS];
		ISceneNode* multi_node[MAX_MULTIS];
		int patt_loaded;
		int multi_loaded;
};

#endif
