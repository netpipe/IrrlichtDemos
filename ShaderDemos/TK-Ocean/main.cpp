#include <irrlicht.h>
#include "src/TKOceanSceneNode.h"
#include "src/TKSkySceneNode.h"
#include "src/TKOceanSceneNodeShaderCallBack.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


int main(int argc, char** argv)
{
    IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, true, 0);

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();




    ///Create sky...
    TKSkySceneNode* sky = new TKSkySceneNode(device->getSceneManager()->getRootSceneNode(),smgr,-1);
    smgr->addCubeSceneNode(10)->setMaterialFlag(EMF_LIGHTING,false);



    ///OCEAN SETUP
    TKOceanSceneNodeShaderCallBack* shaderCB = new TKOceanSceneNodeShaderCallBack(device);

    s32 materialOcean=device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
    "../media/Shaders/Ocean.vert", "vertexMain", video::EVST_VS_2_0,
    "../media/Shaders/Ocean.frag", "pixelMain", video::EPST_PS_2_0,
    shaderCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    TKOceanSceneNode* ocean = new TKOceanSceneNode(smgr->getRootSceneNode(),smgr,-1);
    ocean->setPosition(vector3df(-128,0,-128));

    ///Create an texture to reflection map.
    ITexture* oceanReflectionTexture=smgr->getVideoDriver()->addRenderTargetTexture(dimension2d<u32>(256,256));

    ///Set the first layer with the normalmap...
    ocean->setMaterialTexture(0,device->getVideoDriver()->getTexture("../media/Ocean/waveNM.png"));//NormalMap

    ///The second layer is the Ocean Alpha Map, you can use your HeightMap texture, then the higher areas will be
    ///transparent and the lower areas (black) painted with ocean material.
    ocean->setMaterialTexture(1,device->getVideoDriver()->getTexture("../media/Terrain/hmap.png"));//Heightmap (AlphaMap)

    ///The last layer is the reflection map
    ocean->setMaterialTexture(2,oceanReflectionTexture);

    ///And assign the shader material...
    ocean->setMaterialType((E_MATERIAL_TYPE)materialOcean);

    ///Create an camera to render reflection texture
    ICameraSceneNode* RTTCamera = smgr->addCameraSceneNode();

    ///END OCEAN SETUP


    //create simple terrain...
    IImage *heightMap=smgr->getVideoDriver()->createImageFromFile("../media/Terrain/hmap.png");
    stringc name = "terrain";
    IMesh* terrainMesh=smgr->addTerrainMesh(name.c_str(),heightMap,heightMap,dimension2d<f32>(2,2),5,dimension2d<u32>(128,128));
    IMeshSceneNode* terrain = smgr->addMeshSceneNode(terrainMesh);
    terrain->setMaterialTexture(0,driver->getTexture("../media/Terrain/grass.jpg"));
    terrain->setMaterialFlag(EMF_LIGHTING,false);
    terrain->setPosition(vector3df(-128,0,-128));
    terrain->getMaterial(0).getTextureMatrix(0).setScale(30);

    //real camera..
    ICameraSceneNode* realCamera=smgr->addCameraSceneNodeFPS(0,100,0.1);



    while(device->run())
    {
        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        sky->setPosition(realCamera->getPosition());//update sky position


        ///====================UPDATE REFLECTION TEXTURE
        ///TODO: put this on a separeted function
        IVideoDriver* driver=device->getVideoDriver();
        scene::ICameraSceneNode *camera = smgr->getActiveCamera();
        RTTCamera->setFarValue(camera->getFarValue());

        core::vector3df Position=ocean->getPosition() + vector3df(0,1.8,0);

        core::vector3df campos = camera->getPosition();
        if (campos.Y >= Position.Y)
        {
            RTTCamera->setPosition(core::vector3df(campos.X, 2* Position.Y - campos.Y, campos.Z));
            core::vector3df target = camera->getTarget() - campos;
            target.normalize();
            target.Y *= -1;

            RTTCamera->setTarget(RTTCamera->getPosition() + target*20000);
            RTTCamera->setUpVector(camera->getUpVector());
        }
        else
        {
            RTTCamera->setPosition(camera->getPosition());

            core::vector3df target = camera->getTarget() - camera->getPosition();
            target.normalize();
            target *= 200000;
            RTTCamera->setTarget(RTTCamera->getPosition() + target);
            RTTCamera->setUpVector(camera->getUpVector());
        }

        sky->setPosition(RTTCamera->getPosition());

        driver->setRenderTarget(oceanReflectionTexture, true, true, video::SColor(255,255,255,255));
        smgr->setActiveCamera(RTTCamera);
        ocean->setVisible(false);
        terrain->setVisible(false);

        smgr->drawAll();

        driver->setRenderTarget(0,false,false);//sets the render to real screen

        terrain->setVisible(true);
        ocean->setVisible(true);
        smgr->setActiveCamera(camera);

        sky->setPosition(camera->getPosition());
        ///===============================+END UPDATE REFLECTION TEXTURE

device->sleep(5,0);
        driver->endScene();
    }


    device->drop();
    return 0;
}

