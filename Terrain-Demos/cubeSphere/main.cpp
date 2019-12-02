#include <irrlicht.h>
#include <string>
#include <iostream>
#include "CTerrainSphereNode.h"


using namespace irr;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

bool mars = true;

IrrlichtDevice* device;
SIrrlichtCreationParameters param;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* cam;

scene::ILightSceneNode* light;

IGUIEnvironment* env;
ICursorControl* cursor;
IGUIFont* fnt;

bool up=false,down=false,leftBool=false,rightBool=false,keyQ=false,keyE=false,
leftB=false,rightB=false,mouseW=false,upA=false,
downA=false,rightA=false,leftA=false,
key1=false,key2=false,key3=false,key4=false,key5=false,key6=false,key7=false,key8=false,key9=false,key0=false,
keyF1=false,keyF2=false,keyF3=false,key_shift=false,key_ctrl=false,key_p=false,editB=false,burnB=false,keyM=false,
key_hyphen=false,keyT=false,KeyF=false,KeyI=false;

core::position2d<f32> CursorPos;
f64 camSpeedr = 0;
f64 camSpeed = 0;
u32 Width;
u32 Height;

scene::CTerrainSphereNode* planet;

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();

		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);

		// set clip matrix

		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		core::vector3df posLight2 = light->getAbsolutePosition();
		services->setVertexShaderConstant("mLight2Pos", reinterpret_cast<f32*>(&posLight2), 3);

		// set camera position

		core::vector3df pos = device->getSceneManager()->getActiveCamera()->getAbsolutePosition();
		services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);


		// set light color

		video::SColorf col(0.0f,1.0f,1.0f,0.0f);
		services->setVertexShaderConstant("mLightColor", reinterpret_cast<f32*>(&col), 4);
		video::SColorf fvAmbient(0.368f,0.368f,0.368f,1.0f);
		services->setVertexShaderConstant("fvAmbient", reinterpret_cast<f32*>(&fvAmbient), 4);
		video::SColorf fvSpecular(0.790f,0.488f,0.488f,1.0f);
		services->setVertexShaderConstant("fvSpecular", reinterpret_cast<f32*>(&fvSpecular), 4);
		video::SColorf fvDiffuse(0.886f,0.885f,0.885f,1.0f);
		services->setVertexShaderConstant("fvDiffuse", reinterpret_cast<f32*>(&fvDiffuse), 4);
		f32 fSpecularPower = 50.0f;
		services->setVertexShaderConstant("fSpecularPower", reinterpret_cast<f32*>(&fSpecularPower), 1);

		// set transposed world matrix

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();

		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

		//irr::core::vector3df camPos = camera->getAbsolutePosition();
		//services->setVertexShaderConstant("camPos", reinterpret_cast<f32*>(&camPos), 3);
	}
};

class MyeventReceiver:public IEventReceiver
{
public:

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			if(event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED)
			{

			}
			if(event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
			{

			}

		}
		else if(event.EventType == EET_KEY_INPUT_EVENT)
		{
			if(event.KeyInput.PressedDown == true)
			{
				if(event.KeyInput.Key==KEY_KEY_A)
				{
					leftBool=true;
					//printf("A pressed\n");
				}
				if(event.KeyInput.Key==KEY_KEY_D)
					rightBool=true;
				if(event.KeyInput.Key==KEY_KEY_W)
					up=true;
				if(event.KeyInput.Key==KEY_KEY_S)
					down=true;
				if(event.KeyInput.Key==KEY_KEY_Q)
					keyQ=true;
				if(event.KeyInput.Key==KEY_KEY_E)
					keyE=true;
				if(event.KeyInput.Key==KEY_UP)
					upA=true;
				if(event.KeyInput.Key==KEY_DOWN)
					downA=true;
				if(event.KeyInput.Key==KEY_RIGHT)
					rightA=true;
				if(event.KeyInput.Key==KEY_LEFT)
					leftA=true;
				if(event.KeyInput.Key==KEY_KEY_1)
					key1=true;
				if(event.KeyInput.Key==KEY_KEY_2)
					key2=true;
				if(event.KeyInput.Key==KEY_KEY_3)
					key3=true;
				if(event.KeyInput.Key==KEY_KEY_4)
					key4=true;
				if(event.KeyInput.Key==KEY_KEY_5)
					key5=true;
				if(event.KeyInput.Key==KEY_KEY_6)
					key6=true;
				if(event.KeyInput.Key==KEY_KEY_7)
					key7=true;
				if(event.KeyInput.Key==KEY_KEY_8)
					key8=true;
				if(event.KeyInput.Key==KEY_KEY_9)
					key9=true;
				if(event.KeyInput.Key==KEY_KEY_0)
					key0=true;
				if(event.KeyInput.Key==KEY_F1)
					keyF1=true;
				if(event.KeyInput.Key==KEY_F2)
					keyF2=true;
				if(event.KeyInput.Key==KEY_F3)
					keyF3=true;
				if(event.KeyInput.Key==KEY_KEY_M)
					keyM=true;
				if(event.KeyInput.Key==KEY_KEY_P)
				{

				}
				if(event.KeyInput.Key==KEY_KEY_T)
				{

				}
				if(event.KeyInput.Key==KEY_KEY_O)
				{

				}
				if(event.KeyInput.Key==KEY_KEY_F)
				{

				}
				if(event.KeyInput.Key==KEY_KEY_I)
				{

				}
				if(event.KeyInput.Key==KEY_COMMA)
				{

				}
				if(event.KeyInput.Key==KEY_PERIOD)
				{


				}
				if(event.KeyInput.Key==KEY_F8)
				{

				}
				if(event.KeyInput.Key==KEY_F9)
				{

				}

			}

			if(event.KeyInput.PressedDown == false) {
				if(event.KeyInput.Key==KEY_KEY_A)
					leftBool=false;
				if(event.KeyInput.Key==KEY_KEY_D)
					rightBool=false;
				if(event.KeyInput.Key==KEY_KEY_W)
					up=false;
				if(event.KeyInput.Key==KEY_KEY_S)
					down=false;
				if(event.KeyInput.Key==KEY_KEY_Q)
					keyQ=false;
				if(event.KeyInput.Key==KEY_KEY_E)
					keyE=false;
				if(event.KeyInput.Key==KEY_UP)
					upA=false;
				if(event.KeyInput.Key==KEY_DOWN)
					downA=false;
				if(event.KeyInput.Key==KEY_RIGHT)
					rightA=false;
				if(event.KeyInput.Key==KEY_LEFT)
					leftA=false;
				if(event.KeyInput.Key==KEY_KEY_1)
					key1=false;
				if(event.KeyInput.Key==KEY_KEY_2)
					key2=false;
				if(event.KeyInput.Key==KEY_KEY_3)
					key3=false;
				if(event.KeyInput.Key==KEY_KEY_4)
					key4=false;
				if(event.KeyInput.Key==KEY_KEY_5)
					key5=false;
				if(event.KeyInput.Key==KEY_KEY_6)
					key6=false;
				if(event.KeyInput.Key==KEY_KEY_7)
					key7=false;
				if(event.KeyInput.Key==KEY_KEY_8)
					key8=false;
				if(event.KeyInput.Key==KEY_KEY_9)
					key9=false;
				if(event.KeyInput.Key==KEY_KEY_0)
					key0=false;
				if(event.KeyInput.Key==KEY_F1)
					keyF1=false;
				if(event.KeyInput.Key==KEY_F2)
					keyF2=false;
				if(event.KeyInput.Key==KEY_F3)
					keyF3=false;
				if(event.KeyInput.Key==KEY_KEY_M)
					keyM=false;

			}
		}if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			// leftBool mouse button state check
			if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				leftB = true;
			}
			if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			{
				leftB = false;
			}
			if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
			{
				rightB = true;
			}
			if(event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
			{
				rightB = false;
			}
			if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
			{

			}
			if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
			{
				CursorPos = cursor->getRelativePosition();
			}
		}
		return false;
	}

}my_event_receiver;

void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;
	min = core::min_( r, g, b );
	max = core::min_( r, g, b );
	*v = max;				// v
	delta = max - min;
	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}

void renderCubeMaps(video::ITexture* input, core::dimension2d<u32> dim, core::stringw outname)
{
	scene::ISceneNode* sphere = smgr->addSphereSceneNode(400,2400);
	sphere->setMaterialTexture(0,input);
	sphere->setPosition(core::vector3df(0,0,0));
	sphere->setScale(core::vector3df(-1,1,1));

	sphere->setMaterialFlag(video::EMF_LIGHTING, false);
	sphere->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

	/*
	sphere->setMaterialFlag(video::EMF_BILINEAR_FILTER,false);
	sphere->setMaterialFlag(video::EMF_ANTI_ALIASING,false);
	sphere->setMaterialFlag(video::EMF_USE_MIP_MAPS,false);
	*/

	scene::ICameraSceneNode* sphereCam = smgr->addCameraSceneNode();
    sphereCam->setPosition(core::vector3df(0,0,0));
	sphereCam->setFarValue(100000.0f);
    sphereCam->setAspectRatio(1.0f);
    sphereCam->setFOV(1.573f);
	//sphereCam->setFOV(core::PI/2.0);
	sphereCam->bindTargetAndRotation(true);

	video::ITexture* rt[6];
	video::IImage* img[6];
	io::IWriteFile* outWF[6];

	for (s32 n=0; n<6; ++n)
		rt[n] = smgr->getVideoDriver()->addRenderTargetTexture(dim);

	// render left
    smgr->getVideoDriver()->setRenderTarget(rt[0], true, true, video::SColor(0,0,0,0));
    //sphereCam->setTarget(core::vector3df(10,0,0));
    sphereCam->setRotation(core::vector3df(0,270,0));
	sphereCam->updateAbsolutePosition();
    smgr->drawAll();

	// render back
	smgr->getVideoDriver()->setRenderTarget(rt[1], true, true, video::SColor(0,0,0,0));
    //sphereCam->setTarget(core::vector3df(0,0,10));
	sphereCam->setRotation(core::vector3df(0,180,0));
    sphereCam->updateAbsolutePosition();
    smgr->drawAll();

	// render right
    smgr->getVideoDriver()->setRenderTarget(rt[2], true, true, video::SColor(0,0,0,0));
    //sphereCam->setTarget(core::vector3df(-10,0,0));
    sphereCam->setRotation(core::vector3df(0,90,0));
	sphereCam->updateAbsolutePosition();
    smgr->drawAll();

	 // render front
    smgr->getVideoDriver()->setRenderTarget(rt[3], true, true, video::SColor(0,0,0,0));
    //sphereCam->setTarget(core::vector3df(0,0,-10));
	sphereCam->setRotation(core::vector3df(0,0,0));
    sphereCam->updateAbsolutePosition();
    smgr->drawAll();

	sphere->setScale(core::vector3df(1,1,-1));
	sphere->setRotation(core::vector3df(0,90,0));

    // render down
    smgr->getVideoDriver()->setRenderTarget(rt[4], true, true, video::SColor(0,0,0,0));
    //sphereCam->setTarget(core::vector3df(0,-10,0));
    sphereCam->setRotation(core::vector3df(-90,0,0));
	sphereCam->updateAbsolutePosition();
    smgr->drawAll();

	// render up
    smgr->getVideoDriver()->setRenderTarget(rt[5], true, true, video::SColor(0,0,0,0));
    //sphereCam->setTarget(core::vector3df(0,10,0));
    sphereCam->setRotation(core::vector3df(90,0,0));
	sphereCam->updateAbsolutePosition();
    smgr->drawAll();

	video::IImageWriter* writer = device->getVideoDriver()->getImageWriter(1);


	for (s32 n=0; n<6; ++n)
	{
		core::stringw out = "../";
		out +=  outname;
		out += n+1;
		out += ".pcx";

		irr::io::path outp = out;

		outWF[n] = smgr->getFileSystem()->createAndWriteFile(outp,false);
		img[n] = driver->createImage(rt[n],core::vector2d<s32>(0,0),dim);
		writer->writeImage(outWF[n],img[n]);
		img[n]->drop();
	}

	sphere->remove();
	sphereCam->remove();

}

void rotateVectorAroundAxis(core::vector3df & vector, const core::vector3df & axis, f64 radians)
{
	core::quaternion MrQuaternion;
	core::matrix4 MrMatrix;
	(void)MrQuaternion.fromAngleAxis(radians, axis);
	MrQuaternion.getMatrix(MrMatrix,core::vector3df(0,0,0));
	MrMatrix.rotateVect(vector);
}

void handleCam()
{
	core::position2d<f32> deltaC = (CursorPos - core::vector2df(0.5,0.5));
	cursor->setPosition(core::vector2d<s32>(Width/2,Height/2));

	if(leftBool)
		camSpeedr += 0.00055;
	if(rightBool)
		camSpeedr -= 0.00055;

	if(!leftBool && !rightBool)
	{
		if(camSpeedr > 0)
			camSpeedr -= 0.001;
		if(camSpeedr < 0)
			camSpeedr += 0.001;
		if(camSpeedr < 0.0001 && camSpeedr > -0.0001)
			camSpeedr = 0;
	}

	//Rotation--------------------------------------------------------------------------------------
	//Work out the 3 Axis of the Camera
	core::vector3df forwardD = (cam->getTarget() - cam->getAbsolutePosition()).normalize();
	core::vector3df upD = cam->getUpVector();
	core::vector3df rightD = forwardD.crossProduct(upD);

	//yaw Around the up axis
	rotateVectorAroundAxis(forwardD, upD, deltaC.X);

	// pitch around the right axis (we need to change both forward AND up)
	//Non Windows fullscreen cursor bug ?
	rotateVectorAroundAxis(forwardD, rightD, deltaC.Y);
	rotateVectorAroundAxis(upD, rightD, deltaC.Y);

	// roll around the forward axis
	rotateVectorAroundAxis(upD, forwardD, camSpeedr);

	// And re-orient the flyCam to face along the foward and up axes.
	core::vector3df targetD = cam->getAbsolutePosition() + forwardD;
	cam->setTarget(targetD);
	cam->setUpVector(upD);

	core::vector3df thrusters = core::vector3df(0,0,0);

	camSpeed = 0;

	core::vector3df thrusteradd = forwardD;

	if(up)
	{
		camSpeed += 0.9;
		thrusters += thrusteradd*camSpeed;
		//printf("Thrustes Length forward %f\n",thrusters.getLength());
		//printf("CamSpeed %f\n",camSpeed);
	}
	if(down)
	{
		camSpeed -= 0.9;
		thrusters += thrusteradd*camSpeed;
		//printf("Thrustes Length backward %f\n",thrusters.getLength());
		//printf("CamSpeed %f\n",camSpeed);
	}

	cam->setPosition(cam->getAbsolutePosition()+thrusters*5);
	cam->updateAbsolutePosition();
	cam->setTarget(cam->getAbsolutePosition()+forwardD);
}

int main()
{

	param.DriverType = video::EDT_OPENGL;
	param.Vsync = false;
	param.AntiAlias = true;
	param.Bits = 32;
	param.WindowSize = core::dimension2d<u32>(800,600);
	param.Fullscreen = false;

	//create pointers to device, scene manager, GUI env., sound engine, cursor--------------------------------
	device=createDeviceEx(param);

	device->setEventReceiver(&my_event_receiver);
	driver=device->getVideoDriver();
	smgr=device->getSceneManager();
	device->getTimer()->setTime( 0 );
	env = device->getGUIEnvironment();
	cursor = device->getCursorControl();
	cursor->setVisible(false);

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(video::ETCF_ALLOW_NON_POWER_2, true);

	Width = driver->getScreenSize().Width;
	Height = driver->getScreenSize().Height;
	//cursor->setPosition(s32(Width/2),s32(Height/2));

	scene::ISceneNodeAnimator* dayNight = smgr->createFlyCircleAnimator(core::vector3df(0,0,0),12000,0.00001);

	light = smgr->addLightSceneNode();
	light->setRadius(9000.);

	//video::SLight ldata = light->getLightData();
	//ldata.Attenuation.set(0.0f,1.0f,0.0f);
	//ldata.AmbientColor.set(0.156f,0.156f,0.156f);
	//light->setLightData(ldata);

	light->addAnimator(dayNight);

/*
	//Failed Attempt to use Heightmaps with Spectral Colors

	io::IReadFile* Hfile = smgr->getFileSystem()->createAndOpenFile("../16TSPEC.BMP");

	if(Hfile)
	{
		video::IImage* map = driver->createImageFromFile(Hfile);
		core::vector2d<u32> dim = map->getDimension();
		const u32 xSize = dim.X;
		const u32 ySize = dim.Y;

		f32 *data[5762][2880];

		for(u32 x=0; x<dim.X; x++)
		{
			for(u32 y=0; x<dim.Y; y++)
			{
				f32* h,*s,*v;
				video::SColor col = map->getPixel(x,y);
				RGBtoHSV(col.getRed(),col.getBlue(),col.getGreen(),h,s,v);
				data[x][y] = h;
			}
		}

		//io::IReadFile* dataF = smgr->getFileSystem()->createMemoryReadFile(data,dsize,"read.img",true);
	}
	*/

	//cam = smgr->addCameraSceneNodeFPS(0,100,0.2);

	cam = smgr->addCameraSceneNode();

	//debug
	/*
	scene::ISceneNode* ts = smgr->addSphereSceneNode(1000,800);
	ts->setMaterialFlag(video::EMF_LIGHTING, false);
	ts->setMaterialFlag(video::EMF_WIREFRAME, true);
	ts->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	*/

	scene::ISceneNode * parent = smgr->addEmptySceneNode();

	char i;

	printf("\n\nPLEASE BE VERY PATIENT WITH THIS APP\n"\
		"ALSO MAKE SURE TO HAVE AT LEAST 1.1 GB FREE RAM\n\n"\
		"Please select Mars or Earth:\n\n"\
		" (a) Mars\n"\
		" (b) Earth\n");
	std::cin >> i;
	switch(i)
	{
		case 'a':
			mars = true;
			break;
		case 'b':
			mars = false;
			break;
	}


	if(!mars)
	{
		//RTT Method to make 6 cubemap textures from a single file
		//video::ITexture* rttMap = driver->getTexture("../earthTex/earthbump.jpg");
		//renderCubeMaps(rttMap,core::dimension2d<u32>(2048,2048),"../earthTex/outmars_c");

		planet = new irr::scene::CTerrainSphereNode(parent,
													smgr,
													smgr->getFileSystem(),
													-1,
													4,
													scene::ETPS_129,
													core::vector3df(0,0,0),
													core::vector3df(0,0,0),
													core::vector3df(1,1,1));


		io::IReadFile* file[6];

		file[0] = smgr->getFileSystem()->createAndOpenFile("../earthTex/out_5.jpg");
		file[1] = smgr->getFileSystem()->createAndOpenFile("../earthTex/out_4.jpg");
		file[2] = smgr->getFileSystem()->createAndOpenFile("../earthTex/out_2.jpg");
		file[3] = smgr->getFileSystem()->createAndOpenFile("../earthTex/out_1.jpg");
		file[4] = smgr->getFileSystem()->createAndOpenFile("../earthTex/out_3.jpg");
		file[5] = smgr->getFileSystem()->createAndOpenFile("../earthTex/out_6.jpg");

		planet->loadHeightMap(file,video::SColor(255,255,255,255),0,0.9);

		file[0]->drop();
		file[1]->drop();
		file[2]->drop();
		file[3]->drop();
		file[4]->drop();
		file[5]->drop();

		//planet->setMaterialFlag(video::EMF_LIGHTING, false);
		//planet->setMaterialFlag(video::EMF_WIREFRAME, true);
		//planet->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

		video::ITexture* back = driver->getTexture("../earthTex/earthfcm001.jpg");
		video::ITexture* right = driver->getTexture("../earthTex/earthfcm002.jpg");
		video::ITexture* front = driver->getTexture("../earthTex/earthfcm003.jpg");
		video::ITexture* left = driver->getTexture("../earthTex/earthfcm000.jpg");
		video::ITexture* top = driver->getTexture("../earthTex/earthfcm004.jpg");
		video::ITexture* bottom = driver->getTexture("../earthTex/earthfcm005.jpg");

		planet->setTextures(top,front,back,left,right,bottom,video::SColor(255,30,30,30),video::SColor(255,30,30,30),0);
	}
	else
	{
		//RTT Method to make 6 cubemap textures from a single file
		//video::ITexture* rttMap = driver->getTexture("../marsTex/Mars_m46_Level2.jpg");
		//renderCubeMaps(rttMap,core::dimension2d<u32>(2048,2048),"../marsTex/outmars_c");

		planet = new irr::scene::CTerrainSphereNode(parent,
													smgr,
													smgr->getFileSystem(),
													-1,
													4,
													scene::ETPS_129,
													core::vector3df(0,0,0),
													core::vector3df(0,0,0),
													core::vector3df(1,1,1));


		io::IReadFile* file[6];

		file[0] = smgr->getFileSystem()->createAndOpenFile("../marsTex/outmars5.jpg");
		file[1] = smgr->getFileSystem()->createAndOpenFile("../marsTex/outmars4.jpg");
		file[2] = smgr->getFileSystem()->createAndOpenFile("../marsTex/outmars2.jpg");
		file[3] = smgr->getFileSystem()->createAndOpenFile("../marsTex/outmars1.jpg");
		file[4] = smgr->getFileSystem()->createAndOpenFile("../marsTex/outmars3.jpg");
		file[5] = smgr->getFileSystem()->createAndOpenFile("../marsTex/outmars6.jpg");

		planet->loadHeightMap(file,video::SColor(255,255,255,255),0,1.4);

		file[0]->drop();
		file[1]->drop();
		file[2]->drop();
		file[3]->drop();
		file[4]->drop();
		file[5]->drop();

		//planet->setMaterialFlag(video::EMF_LIGHTING, false);
		//planet->setMaterialFlag(video::EMF_WIREFRAME, true);
		//planet->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);


		video::ITexture* back = driver->getTexture("../marsTex/outmars_c2.jpg");
		video::ITexture* right = driver->getTexture("../marsTex/outmars_c3.jpg");
		video::ITexture* front = driver->getTexture("../marsTex/outmars_c4.jpg");
		video::ITexture* left = driver->getTexture("../marsTex/outmars_c1.jpg");
		video::ITexture* top = driver->getTexture("../marsTex/outmars_c5.jpg");
		video::ITexture* bottom = driver->getTexture("../marsTex/outmars_c6.jpg");

		planet->setTextures(top,front,back,left,right,bottom,video::SColor(255,30,30,30),video::SColor(255,30,30,30),0);

	}

	//test Terrain. Some bugs with rotate and pivot above >90°
	/*
	io::IReadFile* testFile = smgr->getFileSystem()->createAndOpenFile("../out_1.jpg");
	scene::ITerrainSceneNode* testter = smgr->addTerrainSceneNode(testFile,0,-1);
	testFile->drop();
	*/

	f64 rot = 0.0;

	//core::vector3df pSize = planet->getBoundingBox().getExtent();
	cam->setPosition(core::vector3df(0,1200,1200));
	cam->updateAbsolutePosition();

	while(device->run())
	{
		//rot += 0.1;
		//printf("%f\n",rot);
		//parent->setRotation(core::vector3df(0,(f32)rot,0));
		//testter->setRotation(core::vector3df(0,(f32)rot,0));
		driver->beginScene(true, true, video::SColor(0,100,100,100));
		handleCam();
		smgr->drawAll();
		driver->endScene();
	}

	device->drop();
	//planet->drop();
	dayNight->drop();

	return 0;
}
