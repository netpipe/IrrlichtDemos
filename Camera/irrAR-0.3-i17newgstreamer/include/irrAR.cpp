#include "irrAR.h"

IARManager::IARManager(IrrlichtDevice *device)
{
	//defaults
	this->patt_loaded = 0;
	this->multi_loaded = 0;
	this->cam_texture = 0;

	//set
	this->smgr = device->getSceneManager();
	this->driver = device->getVideoDriver();
}

IARManager::~IARManager()
{

}

dimension2di IARManager::beginCamera(char* cparam_filename, char* win32_vconf_filename, char* unix_config_string)
{
	dimension2di window_dimensions;

	window_dimensions.Width = 0;
	window_dimensions.Height = 0;

	/* open the video path */
#ifdef _WIN32
	if(arVideoOpen(win32_vconf_filename) < 0) return window_dimensions;
#else
	if(arVideoOpen(unix_config_string) < 0) return window_dimensions;
#endif

	/* load camera param file */
	if(arParamLoad(cparam_filename, 1, &(this->wparam)) < 0) return window_dimensions;

	/* find the size of the window */
	if(arVideoInqSize(&(window_dimensions.Width), &(window_dimensions.Height)) < 0) return window_dimensions;

	/* set the initial camera parameters */
	arParamChangeSize(&(this->wparam), window_dimensions.Width, window_dimensions.Height, &(this->cparam));
	arInitCparam(&(this->cparam));

	//create gl_cpara for setting the proper camera projection later on
	this->create_projection_matrix(&(this->cparam), 1.0, 0, 0, 0, 0, this->gl_cpara);

	arVideoCapStart();

	//make the texture
	this->create_camera_texture((dimension2du)window_dimensions);

	//debug stuff
	printf("Image size (x,y) = (%d,%d)\n", window_dimensions.Width, window_dimensions.Height);
	printf("*** Camera Parameter ***\n");
	arParamDisp(&(this->cparam));

	return window_dimensions;
}

ISceneNode* IARManager::addARSceneNode(char* patt_filename, ISceneNode *initial_child, s32 id)
{
	//already at the max?
	if(this->patt_loaded == MAX_PATTERNS) return 0;

	//load the pattern
	if((this->patt_id[this->patt_loaded] = arLoadPatt(patt_filename)) < 0) return 0;

	//make the node
	this->patt_node[this->patt_loaded] = this->smgr->addEmptySceneNode(0, id);

	//make it inversed (solves a problem)
	this->patt_node[this->patt_loaded]->setScale(vector3df(-1,-1,-1));

	//set its child
	if(initial_child) this->patt_node[this->patt_loaded]->addChild(initial_child);

	//set new max and return the node
	this->patt_loaded++;
	return this->patt_node[this->patt_loaded - 1];

}

ISceneNode* IARManager::addARMultiSceneNode(char* config_filename, ISceneNode *initial_child, s32 id)
{
	//already at the max?
	if(this->multi_loaded == MAX_PATTERNS) return 0;

	//load the pattern
	if((this->multi_config[this->multi_loaded] = arMultiReadConfigFile(config_filename)) < 0) return 0;

	//make the node
	this->multi_node[this->multi_loaded] = this->smgr->addEmptySceneNode(0, id);

	//make it inversed (solves a problem)
	this->multi_node[this->multi_loaded]->setScale(vector3df(-1,-1,-1));

	//set its child
	if(initial_child) this->multi_node[this->multi_loaded]->addChild(initial_child);

	//set new max and return the node
	this->multi_loaded++;
	return this->patt_node[this->multi_loaded - 1];
}

ITexture* IARManager::getCameraTexture()
{
	return this->cam_texture;
}

int IARManager::run()
{
	ARUint8 *dataPtr;

	//pull our next cam image
	dataPtr = (ARUint8 *)arVideoGetImage();
	arVideoCapNext();

	if(!dataPtr) return 0;

	//make image readable by irrlicht
	this->update_ITexture(this->cam_texture, dataPtr);

	//find markers and make appropriate changes to the nodes attached to them
	this->translate_nodes(dataPtr);

	return 1;
}

void IARManager::drawBackground()
{
	dimension2du scrn_size = this->driver->getScreenSize();
	dimension2du img_size = this->cam_texture->getSize();

	this->driver->draw2DImage(this->cam_texture,
                           rect<s32>(0,0,scrn_size.Width,scrn_size.Height),
                           rect<s32>(0,0,img_size.Width,img_size.Height));
//UPSIDEDOWNCODE
//driver->draw2DImage(this->cam_texture,
                      //rect<s32>(0,0,scrn_size.Width,scrn_size.Height),
                      //rect<s32>(0,img_size.Height,img_size.Width,0));

}

void IARManager::fixCamera(ICameraSceneNode* camera)
{
	matrix4 projection_matrix;

	projection_matrix.setM(this->gl_cpara);

	camera->setProjectionMatrix(projection_matrix);
	camera->setPosition(vector3df(0,0,0));
	camera->setTarget(vector3df(0,0,1));
}

void IARManager::translate_nodes(ARUint8 *dataPtr)
{
	const int thresh = 100;
	ARMarkerInfo *marker_info;
	int marker_num;
	int p;

	//check
	if(!this->patt_loaded) return;

	//run ARTK's detection function
	arDetectMarker(dataPtr, thresh, &marker_info, &marker_num);

	//check each mutli-pattern
	for(p=0;p<this->multi_loaded;p++)
	{
		int i;
		double err;
		double gl_para[16];
		float glf_para[16];
		CMatrix4<float> mat;

		//do the work
		err = arMultiGetTransMat(marker_info, marker_num, this->multi_config[p]);

		//was it found?
		if(err < 0 || err > 100.0)
		{
			this->multi_node[p]->setVisible(0);
			continue;
		}

		//being the matrix process
		/*
		//incase you want to take advantage of this (ripped from multiTest.c)...
		for( i = 0; i < config->marker_num; i++ ) {
			if( config->marker[i].visible >= 0 ) draw( config->trans, config->marker[i].trans, 0 );
			else                                 draw( config->trans, config->marker[i].trans, 1 );
		}*/

		this->our_convert_trans_para(this->multi_config[p]->trans, gl_para);
		for(i=0;i<16;i++) glf_para[i] = (float)gl_para[i];

		mat.setM(glf_para);

		//vector3d<f32> scale_vec = mat.getScale();
		vector3d<f32> rot_vec = mat.getRotationDegrees();
		vector3d<f32> pos_vec = mat.getTranslation();

		rot_vec.X -= 90;

		multi_node[p]->setRotation(rot_vec);
		multi_node[p]->setPosition(pos_vec);

		multi_node[p]->setVisible(1);
	}

	//check each pattern
	for(p=0;p<this->patt_loaded;p++)
	{
		int i,j,k;
		double gl_para[16];
		float glf_para[16];
		CMatrix4<float> mat;
		double patt_width     = 80.0;
		double patt_center[2] = {0.0, 0.0};
		double patt_trans[3][4];

		//find most visible detection of this pattern
		for(k=-1,j=0; j < marker_num; j++ )
			if( patt_id[p] == marker_info[j].id )
			{
				if(k==-1) k = j;
				else if(marker_info[k].cf < marker_info[j].cf) k = j;
			}

		//was it found?
		if(k == -1)
		{
			patt_node[p]->setVisible(0);
			continue;
		}

		//begin the matrix process
		arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);

		this->our_convert_trans_para(patt_trans, gl_para);
		for(i=0;i<16;i++) glf_para[i] = (float)gl_para[i];

		mat.setM(glf_para);

		//vector3d<f32> scale_vec = mat.getScale();
		vector3d<f32> rot_vec = mat.getRotationDegrees();
		vector3d<f32> pos_vec = mat.getTranslation();

		rot_vec.X -= 90;

		patt_node[p]->setRotation(rot_vec);
		patt_node[p]->setPosition(pos_vec);

		patt_node[p]->setVisible(1);
	}
}

void IARManager::our_convert_trans_para(double para[3][4], double gl_para[16])
{
	int i, j;

	for( j = 0; j < 3; j++ ) {
		for( i = 0; i < 4; i++ ) {
			gl_para[i*4+j] = para[j][i];
		}
	}
	gl_para[0*4+3] = gl_para[1*4+3] = gl_para[2*4+3] = 0.0;
	gl_para[3*4+3] = 1.0;
}

ITexture* IARManager::update_ITexture(ITexture *dest, ARUint8 *ardata){
	u8* pixels;
	pixels = (u8*)(dest->lock());
	if( pixels )
	{
		int max_pixels = dest->getSize().Width * dest->getSize().Height;

#if (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_BGR)
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

#elif (AR_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_BGRA)
		for(int i=0;i<max_pixels;i++)
		{
			*pixels = *ardata;
			pixels++; ardata++;
			*pixels = *ardata;
			pixels++; ardata++;
			*pixels = *ardata;
			pixels++; ardata++;
			*pixels = *ardata;
			pixels++; ardata++;

		}

#else
		printf("Your camera images are coming in a format that hasn't been coded for yet\n");
		printf("If you would like the help make support for this image format please visit irrAR.cpp at the update_ITexture function\n");
		printf("and get in contact with freaknigh@gmail.com (project leader)\n");

		printf("\n\n");

		printf("AR_DEFAULT_PIXEL_FORMAT:%d\n", AR_DEFAULT_PIXEL_FORMAT);
		printf("AR_PIXEL_FORMAT_RGB:%d\n", AR_PIXEL_FORMAT_RGB);
		printf("AR_PIXEL_FORMAT_BGR:%d\n", AR_PIXEL_FORMAT_BGR);
		printf("AR_PIXEL_FORMAT_RGBA:%d\n", AR_PIXEL_FORMAT_RGBA);
		printf("AR_PIXEL_FORMAT_BGRA:%d\n", AR_PIXEL_FORMAT_BGRA);

		printf("\n\n");

		//may be useful too: AR_PIX_SIZE_DEFAULT
#endif

		dest->unlock();
	}
	else
	{
		printf("Camera's ITexture could not be written?\n");
	}

	return dest;
}

void IARManager::create_camera_texture(dimension2du dimensions)
{
	if(!this->cam_texture && dimensions.Width && dimensions.Height)
		this->cam_texture = this->driver->addTexture(dimensions, "IrrAR_text", video::ECF_A8R8G8B8);
}

#define   MINIWIN_MAX    8
#define   GMINI          2

void IARManager::create_projection_matrix(ARParam *cparam, double zoom, int fullFlag, int xwin, int ywin, int hmd_flag, float m[16])
{
	int i;
	static int      gl_hmd_flag = 0;
	static double   gZoom;
	static int      gImXsize, gImYsize;
	static int      gXsize, gYsize;
	static int      gMiniXsize, gMiniYsize;
	static int      gMiniXnum,  gMiniYnum;
	static int      gWinXsize, gWinYsize;
	static ARParam  gCparam;

	gl_hmd_flag = hmd_flag;
	gZoom  = zoom;
	gImXsize = cparam->xsize;
	gImYsize = cparam->ysize;
	if( gl_hmd_flag == 0 )
	{
		gXsize = (int)((double)cparam->xsize * gZoom);
		gYsize = (int)((double)cparam->ysize * gZoom);
	}
	else
	{
		gXsize = AR_HMD_XSIZE;
		gYsize = AR_HMD_YSIZE;
	}
	gMiniXsize = (int)((double)cparam->xsize * gZoom / GMINI);
	gMiniYsize = (int)((double)cparam->ysize * gZoom / GMINI);

	if( xwin * ywin > MINIWIN_MAX )
	{
		if( xwin > MINIWIN_MAX ) xwin = MINIWIN_MAX;
		ywin = MINIWIN_MAX / xwin;
	}
	gMiniXnum = xwin;
	gMiniYnum = ywin;
	gWinXsize = (gMiniXsize*gMiniXnum > gXsize)? gMiniXsize*gMiniXnum: gXsize;
	gWinYsize = gYsize + gMiniYsize*gMiniYnum;

	gCparam = *cparam;
	for( i = 0; i < 4; i++ )
		gCparam.mat[1][i] = (gCparam.ysize-1)*(gCparam.mat[2][i]) - gCparam.mat[1][i];

	//argConvGLcpara( &gCparam, AR_GL_CLIP_NEAR, AR_GL_CLIP_FAR, gl_cpara );
	//argConvGLcpara2( param->mat, param->xsize, param->ysize, gnear, gfar, m );
	this->our_argConvGLcpara2( gCparam.mat, gCparam.xsize, gCparam.ysize, AR_GL_CLIP_NEAR, AR_GL_CLIP_FAR, gl_cpara );
}

void IARManager::our_argConvGLcpara2(double cparam[3][4], int width, int height, double gnear, double gfar, float m[16])
{
	double   icpara[3][4];
	double   trans[3][4];
	double   p[3][3], q[4][4];
	int      i, j;


	if( arParamDecompMat(cparam, icpara, trans) < 0 )
	{
		printf("gConvGLcpara: Parameter error!!\n");
		exit(0);
	}

  for(i = 0; i < 4; i++)///
      icpara[1][i] = height*icpara[2][i] - icpara[1][i];

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			p[i][j] = icpara[i][j] / icpara[2][2];
		}
	}
	m[0] = q[0][0] = (2.0 * p[0][0] / width);
	m[1] = q[0][1] = (2.0 * p[0][1] / width);
	m[2] = q[0][2] = ((2.0 * p[0][2] / width)  - 1.0);
	m[3] = q[0][3] = 0.0;

	m[4] = q[1][0] = 0.0;
	m[5] = q[1][1] = (2.0 * p[1][1] / height);
	m[6] = q[1][2] = ((2.0 * p[1][2] / height) - 1.0);
	m[7] = q[1][3] = 0.0;

	m[8] = q[2][0] = 0.0;
	m[9] = q[2][1] = 0.0;
	m[10] = q[2][2] = (gfar + gnear)/(gfar - gnear); //-(gfar + gnear)/(gfar - gnear);
	m[11] = q[2][3] = -2.0 * gfar * gnear / (gfar - gnear);

	m[12] = q[3][0] = 0.0;
	m[13] = q[3][1] = 0.0;
	m[14] = q[3][2] = 1.0; //-1.0;
	m[15] = q[3][3] = 0.0;


	for( i = 0; i < 4; i++ ) {
		for( j = 0; j < 3; j++ ) {
			m[i+j*4] = q[i][0] * trans[0][j]
					+ q[i][1] * trans[1][j]
					+ q[i][2] * trans[2][j];
		}
		m[i+3*4] = q[i][0] * trans[0][3]
				+ q[i][1] * trans[1][3]
				+ q[i][2] * trans[2][3]
				+ q[i][3];
	}
}
