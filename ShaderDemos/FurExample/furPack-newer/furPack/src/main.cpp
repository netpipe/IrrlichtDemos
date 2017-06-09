#include "global.h"
#include "shader.h"

bool InitIrrlicht();

int main()
{
    //Startup irrlicht
    if(!InitIrrlicht())
        return 0;

    //Setup Camera
    //smgr->addCameraSceneNodeFPS();
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.5f);
	cam->setPosition(vector3df(0.0f, 100.0f, -100.0f));

    //Create CFurSceneNode
    IAnimatedMesh* meshFur = smgr->getMesh("../res/eekhoorn2.b3d");
    //IAnimatedMesh* meshFur = smgr->getMesh("../res/teapot.3ds");
	//IAnimatedMesh* meshFur = smgr->addHillPlaneMesh("plane", core::dimension2d<irr::f32>(10,10), core::dimension2d<irr::u32>(10,10));
    CFurSceneNode* nodeFur = new CFurSceneNode(meshFur, smgr->getRootSceneNode(), smgr, 666);
    if (nodeFur)
    {
        nodeFur->setName("nodeFur");
	    nodeFur->setMaterialFlag(EMF_LIGHTING, false);
		nodeFur->setMaterialFlag(EMF_BLEND_OPERATION,true);
        nodeFur->setMaterialType((E_MATERIAL_TYPE)EMT_FUR);
	    nodeFur->setPosition( vector3df( 0, -50, 100) );
        nodeFur->setAnimationSpeed(20);
	    nodeFur->setLayers(40);
        nodeFur->setFurLength(0.3f);

	    nodeFur->setMaterialTexture( 1, driver->getTexture("../res/texture.tga") );       //FurTexture
        nodeFur->setMaterialTexture( 0, driver->getTexture("../res/Fur_Texture.bmp") ); //FurAlpha
        //nodeFur->setMaterialTexture( 1, driver->getTexture("../res/FurTexColor.jpg") );       //FurTexture
        //nodeFur->setMaterialTexture( 0, driver->getTexture("../res/uberFur.tga") ); //FurAlpha
    }

    while(device->run())if (device->isWindowActive())
    {
        driver->beginScene(true, true, SColor(255, 255, 128, 64));
        smgr->drawAll();
        driver->endScene();
    }

    device->drop();
    return 0;
}

bool InitIrrlicht()
{
    //*************************starting irrlicht********************************
    device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, false);

    if(!device)
        return 0;

    device->setWindowCaption(L"Fur Demo - by zeroZshadow");
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    gpu = driver->getGPUProgrammingServices();

    driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,false);
    smgr->setAmbientLight(SColor(0,255,255,255));

    InitShaders();

	return 1;
}
