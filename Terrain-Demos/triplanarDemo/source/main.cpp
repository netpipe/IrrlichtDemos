/**Example of triplanar mapping with 4 textures (2 diffuse textures and 2 normal maps)
I have limited this to 2 textures and 2 normal maps so it doesn't need the library to be recompiled
And... How many devices out there don't support more than 4 textures at a time, anyway?...
*/

/**Thanks to Hendu for helping me on how to access more than 1 texture in a GL shader*/

#include <iostream>
#include <irrlicht.h>

/**
Class for the constants set callback of the triplanar mapping shader.

The texture setup is as follows:
slots 0 and 1 are diffuse textures
slots 2 and 3 are the normal maps
0 non horizontal texture
1 horizontal texture
2 non horizontal normal map
3 horizontal normal map
*/

//Simple event receiver class that exits the program when the key ESC is pressed.
class escEventReceiver : public irr::IEventReceiver
{
	virtual bool OnEvent (const irr::SEvent &event)
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if(event.KeyInput.Key == irr::KEY_ESCAPE)
			{
				exit(0);//We exit
			}
		}
		return false;
	}
};

class triplanarMappingShader : public irr::video::IShaderConstantSetCallBack
{
	//All these constants are picked up from the material
	irr::video::SColorf ambient;
	irr::video::SColorf diffuse;
	irr::video::SColorf specular;
	irr::f32 specularPower;
	irr::f32 tileSize; //This tileSize parameter is taken from the material parameter, which is a float.
	irr::core::vector3df lightDirection;

public:
	irr::scene::ICameraSceneNode* camera;

	triplanarMappingShader()
	{
		//An initial light direction, it can be switched at will later.
		setLightDirection(irr::core::vector3df(1,1,1));
		//If no camera... default values, can be changed later.
		camera = 0;
	}


	virtual void OnSetConstants (irr::video::IMaterialRendererServices *services, irr::s32 userData)
	{
		//We set the constants here. nothing too odd to explain.

		irr::core::matrix4 mat;
		irr::core::vector3df viewPos = irr::core::vector3df(0,0,0);
		irr::video::IVideoDriver* drv = services->getVideoDriver();

		mat = drv->getTransform(irr::video::ETS_PROJECTION);
		mat *= drv->getTransform(irr::video::ETS_VIEW);
		mat *= drv->getTransform(irr::video::ETS_WORLD);

		if(userData==1) //If user data is 1, the driver is directX, else, it is openGL and this variable can be omitted
		{
			services->setVertexShaderConstant("matWorldViewProjection",mat.pointer(),16);
		}

		mat = drv->getTransform(irr::video::ETS_WORLD);

		services->setVertexShaderConstant("matWorld",mat.pointer(),16);

		if(camera)
		{
			viewPos = camera->getPosition();
		}

		services->setVertexShaderConstant("viewPos",&viewPos.X,3);
		services->setVertexShaderConstant("tileSize",&tileSize,1);

		services->setPixelShaderConstant("ambient",&ambient.r,4);
		services->setPixelShaderConstant("diffuse",&diffuse.r,4);
		services->setPixelShaderConstant("specular",&specular.r,4);
		services->setPixelShaderConstant("specularPower",&specularPower,1);
		services->setPixelShaderConstant("lightDirection",&lightDirection.X,3);

		if(userData == -1)
		{
			irr::s32 textureSampler;
			textureSampler = 0;
			services->setPixelShaderConstant("baseX",&textureSampler,1);
			textureSampler = 1;
			services->setPixelShaderConstant("baseY",&textureSampler,1);
			textureSampler = 2;
			services->setPixelShaderConstant("normalX",&textureSampler,1);
			textureSampler = 3;
			services->setPixelShaderConstant("normalY",&textureSampler,1);

			if(camera)
			{
				irr::f32 depth = camera->getFarValue();
				services->setPixelShaderConstant("camDepth",&depth,1);
			}
		}
	}

	virtual void OnSetMaterial (const irr::video::SMaterial &material)
	{
		//Nothing too complex to explain here, it simply grabs the colors and parameters for the onSetConstants,
		//previously converting them to float values

		ambient = material.AmbientColor;
		diffuse = material.DiffuseColor;
		specular = material.SpecularColor;
		specularPower = material.Shininess;
		tileSize = material.MaterialTypeParam;
	}

	void setLightDirection(irr::core::vector3df lightDir)
	{
		lightDirection = lightDir;
		lightDirection.normalize();
	}
};

int main(int argc,char* argv[])
{
	irr::IrrlichtDevice* dev;
	irr::video::IVideoDriver* drv;
	irr::scene::ISceneManager* smgr;
	irr::video::IGPUProgrammingServices* gpu;
	irr::s32 shaderIndex;
	escEventReceiver* receiver;

	receiver = new escEventReceiver();

/**Engine initialization*/
	irr::SIrrlichtCreationParameters prm;
	int choice,choice2,choice3,choice4;
	std::cout<<"When running the demo, please, press ESC to exit"<<std::endl;
	std::cout<<"1. DirectX 9C\n2. OpenGL"<<std::endl;
	std::cin>>choice;

	prm.EventReceiver = receiver;
	prm.DriverType = choice == 1 ? irr::video::EDT_DIRECT3D9 : irr::video::EDT_OPENGL;
	prm.WindowSize = irr::core::dimension2du(1280,720);
	prm.Bits = 32;
	prm.DriverMultithreaded = true;

	std::cout<<"1. Fullscreen\n2. No FullScreen"<<std::endl;
	std::cin>>choice2;

	prm.Fullscreen = choice2 == 1;

	std::cout<<"1. Antialiasing\n2. No Antialiasing"<<std::endl;
	std::cin>>choice3;

	prm.AntiAlias = (choice3 == 1 ? 15 : 0);

	std::cout<<"1. Game Terrain\n2. Mountain Terrain"<<std::endl;
	std::cin>>choice4;

	dev = createDeviceEx(prm);

	if(dev){
//variable startup
		if(choice2 == 1)//If we are running in fullscreen...
		{
			dev->getCursorControl()->setVisible(false);
		}
		drv = dev->getVideoDriver();
		smgr = dev->getSceneManager();
		gpu = drv->getGPUProgrammingServices();

//shader compilation
		triplanarMappingShader* triplanarConstants = new triplanarMappingShader();
		irr::scene::ICameraSceneNode* camera;

	//The constants need a camera...
		camera = smgr->addCameraSceneNodeFPS(smgr->getRootSceneNode(),90.0f,0.15f);
		triplanarConstants->camera = camera;

	//The actual shader compilation
		irr::core::stringc filenameV,filenameP;
		filenameV = "../data/triplanarMappingV";
		filenameP = "../data/triplanarMappingP";

		if(choice == 1) //direct X
		{
			filenameV += irr::core::stringc(".hlsl");
			filenameP += irr::core::stringc(".hlsl");
		}
		else			//Open GL
		{
			filenameV += irr::core::stringc(".glsl");
			filenameP += irr::core::stringc(".glsl");
		}

		shaderIndex = gpu->addHighLevelShaderMaterialFromFiles(
			filenameV,
			choice == 1? "vs_main" : "main",
			irr::video::EVST_VS_2_0,
			filenameP,
			choice == 1? "ps_main" : "main",
			irr::video::EPST_PS_2_0,
			triplanarConstants,
			irr::video::EMT_SOLID,
			choice == 1? 1 : -1);

		std::cout<<"Triplanar shader compilation result is "<< (shaderIndex == -1? "Incorrect" : "Correct") <<std::endl;

//Terrain creation.
		irr::scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
			(choice4 == 1 ? "../data/game.png":"../data/mountain.png"),
			smgr->getRootSceneNode(),
			-1,
			irr::core::vector3df(),
			irr::core::vector3df(),
			irr::core::vector3df(10.0f,2.5f,10.0f),
			irr::video::SColor(255,255,255,255),
			5,
			irr::scene::ETPS_17,
			2);

//We set the shader, the textures and the colors...
		terrain->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE) shaderIndex;

		terrain->getMaterial(0).AmbientColor = irr::video::SColor(255,118,86,86);
		//terrain->getMaterial(0).AmbientColor = irr::video::SColor(255,0,0,0);

		terrain->getMaterial(0).DiffuseColor = irr::video::SColor(255,118,132,204);
		//terrain->getMaterial(0).DiffuseColor = irr::video::SColor(255,0,0,0);

		terrain->getMaterial(0).SpecularColor = irr::video::SColor(255,205,209,86);
		//terrain->getMaterial(0).SpecularColor = irr::video::SColor(255,0,0,0);

		terrain->getMaterial(0).Shininess = 4.0f;

		terrain->getMaterial(0).setTexture(0,drv->getTexture("../data/rock.png"));
		terrain->getMaterial(0).setTexture(1,drv->getTexture("../data/grass.png"));
		terrain->getMaterial(0).setTexture(2,drv->getTexture("../data/rockn.png"));
		terrain->getMaterial(0).setTexture(3,drv->getTexture("../data/grassn.png"));

		terrain->getMaterial(0).MaterialTypeParam = 100.0f;//tile size!
		//terrain->getMaterial(0).NormalizeNormals = true;
		terrain->getMaterial(0).FogEnable = true;

    // create triangle selector for the terrain
		irr::scene::ITriangleSelector* selector	= smgr->createTerrainTriangleSelector(terrain, 0);
		terrain->setTriangleSelector(selector);

    // create collision response animator and attach it to the camera
		irr::scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camera, irr::core::vector3df(.06f,0.1f,.06f),
			irr::core::vector3df(0,0,0),
			irr::core::vector3df(0,3,0));
		selector->drop();
		camera->addAnimator(anim);
		anim->drop();

//We enable some fog...
		drv->setFog(
			irr::video::SColor(255,128,160,192),
			irr::video::EFT_FOG_LINEAR,
			10.0f,
			camera->getFarValue(),
			0.01f,
			true,
			false);

//And we are done.
		irr::core::vector3df cameraFall = terrain->getBoundingBox().getCenter();
		cameraFall.Y += terrain->getBoundingBox().getExtent().Y/2.0f;
		camera->setPosition(cameraFall);

//main loop
		triplanarConstants->setLightDirection(irr::core::vector3df(-0.1f,1.0f,0.1f));

		while(dev->run())
		{
			drv->beginScene(true,true,irr::video::SColor(255,128,160,192));
			smgr->drawAll();
			drv->endScene();
		}

		triplanarConstants->drop();

	}
	else
	{
		printf("Uoops! Nothing happened!? Did you try to get the DirectX driver in Linux?\n");
	}

	return 0;
}
