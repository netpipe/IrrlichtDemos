#include "global.h"
#include "shader.h"

bool InitIrrlicht();

int main()
{
    //Startup irrlicht
    InitIrrlicht();
      //  return 0;
        InitShaders();
    //Setup Camera
    smgr->addCameraSceneNodeFPS();

    //Create CFurSceneNode
    IAnimatedMesh* meshFur = smgr->getMesh("./res/torus.b3d");
    CFurSceneNode* nodeFur = new CFurSceneNode(meshFur, smgr->getRootSceneNode(), smgr, 666);
    if (nodeFur)
    {
        nodeFur->setName("nodeFur");
	    nodeFur->setMaterialFlag(EMF_LIGHTING, false);
        nodeFur->setMaterialType((E_MATERIAL_TYPE)EMT_FUR);
	    nodeFur->setPosition( vector3df( 0, -50, 100) );
        nodeFur->setAnimationSpeed(20);
	    nodeFur->setLayers(20);
        nodeFur->setFurLength(1.0f);

	    nodeFur->setMaterialTexture( 0, driver->getTexture("./res/texture.tga") );       //FurTexture
        nodeFur->setMaterialTexture( 1, driver->getTexture("./res/Fur_Texture.bmp") ); //FurAlpha
    }

    while(device->run())
    {
        driver->beginScene(true, true, SColor(255, 255, 128, 64));
       // nodeFur->render();
            smgr->drawAll();
        driver->endScene();
        device->sleep(20,0);
    }


    device->drop();
    return 0;
}

bool InitIrrlicht()
{
    //*************************starting irrlicht********************************
    device = createDevice(EDT_OPENGL, dimension2du(800, 600), 32, false, false, false);

    if(!device)
        return 0;

    device->setWindowCaption(L"Fur Demo - by zeroZshadow");
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    gpu = driver->getGPUProgrammingServices();

    driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,false);
    smgr->setAmbientLight(SColor(SColor(0,146,75,56)));


}
