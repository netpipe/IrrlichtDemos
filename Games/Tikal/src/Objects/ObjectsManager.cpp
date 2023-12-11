#include <irrlicht.h>
#include "ObjectsManager.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


ObjectsManager::ObjectsManager(IrrlichtDevice *idevice)
{
    device=idevice;
    smgr=device->getSceneManager();

    ICameraSceneNode* originalCam = smgr->getActiveCamera();

    RTTCamera=smgr->addCameraSceneNode();

    smgr->setActiveCamera(originalCam);

    mc = new MyShaderCallBack(device);

    //Light
    light = smgr->addLightSceneNode(0,vector3df(5000,5000,0),SColorf(1.0f, 1.0f, 1.0f),10000.0f, -1);
    smgr->setAmbientLight(SColorf(0.5,0.5,0.5,1));

    oceanReflectionTexture=NULL;
}

ObjectsManager::~ObjectsManager()
{
}

TKFoodResource* ObjectsManager::createFoodResource(vector3df pos)
{
    TKFoodResource* temp=new TKFoodResource(smgr,pos);
    return temp;
}

TKStoneResource* ObjectsManager::createStoneResource(vector3df pos)
{
    TKStoneResource* temp=new TKStoneResource(smgr,pos);
    return temp;
}

TKSkySceneNode* ObjectsManager::createSky()
{
    sky=new TKSkySceneNode(device->getSceneManager()->getRootSceneNode(),smgr);
    return sky;
}

void ObjectsManager::loadAllResources()
{
    cout << "========================================LOAD SHADERS" << endl;
    materialTerrain=device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        "../media/Shaders/splat.vert", "vertexMain", video::EVST_VS_2_0,
        "../media/Shaders/splat.frag", "pixelMain", video::EPST_PS_2_0,
        mc, video::EMT_SOLID);

    materialOcean=device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        "../media/Shaders/Ocean.vert", "vertexMain", video::EVST_VS_2_0,
        "../media/Shaders/Ocean.frag", "pixelMain", video::EPST_PS_2_0,
        mc, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    fowTexture=device->getVideoDriver()->addTexture(dimension2du(32,32),"fowTexture",ECF_A8R8G8B8);

    u32 *p = (u32*)fowTexture->lock();

    SColor color(255,0,255,0);
    SColor color2(255,0,0,0);

    for(u32 j=0; j<fowTexture->getSize().Height; j++)
    for(u32 i=0; i<fowTexture->getSize().Width; i++)
    {// you need some simple math to calculate position in simple array from 2D coordinates
        if(rand()%2==0)
            p[j * fowTexture->getSize().Width + i] = color.color;
        else
            p[j * fowTexture->getSize().Width + i] = color2.color;
    }

    // apply changes
    fowTexture->unlock();

    miniMapTexture=device->getVideoDriver()->addTexture(dimension2du(128,128),"miniMapTextureObjects",ECF_A8R8G8B8);

    cout << "========================================SHADERS LOADED!" << endl;
}

TKOceanSceneNode* ObjectsManager::createOcean(stringc heightMapFile)
{
    ocean=new TKOceanSceneNode(device->getSceneManager()->getRootSceneNode(),smgr,-1);

    ///TODO: resolver bug do removeTexture e colocar a textura dentro da classe ocean
    if(oceanReflectionTexture==NULL)
    {
        cout << "RTT Ocean Created!" << endl;
        oceanReflectionTexture=smgr->getVideoDriver()->addRenderTargetTexture(dimension2du(256,256));
    }

    ocean->setMaterialType((E_MATERIAL_TYPE)materialOcean);///TODO: mover para dentro do construtor de Ocean quando criar ShaderCB

    ocean->setMaterialTexture(0,device->getVideoDriver()->getTexture("../media/Ocean/waveNM.png"));//NormalMap
    ocean->setMaterialTexture(1,device->getVideoDriver()->getTexture(heightMapFile.c_str()));//Heightmap (AlphaMap)
    ocean->setMaterialTexture(2,oceanReflectionTexture);//Reflection

    return ocean;
}

TKTreeSceneNode* ObjectsManager::createTree(ISceneNode* parent, vector3df pos)
{
    if(parent == 0) parent = device->getSceneManager()->getRootSceneNode();
    TKTreeSceneNode* tree=new TKTreeSceneNode(parent, smgr, -1, DRAW_DISTANCE);
    tree->setPosition(pos);

    vectorTree.push_back(tree);//armazenar arvore no vetor

    return tree;
}

void ObjectsManager::clearWorld()
{
    smgr->clear();

    for(int i=0;i<vectorGroundVegetation.size();i++)
    {
        delete vectorGroundVegetation[i];
    }
    vectorGroundVegetation.clear();

    for(int i=0;i<vectorRock.size();i++)
    {
        delete vectorRock[i];
    }
    vectorRock.clear();

    for(int i=0;i<vectorTree.size();i++)
    {
        delete vectorTree[i];
    }
    vectorTree.clear();

    delete terrain;

    delete ocean;

    delete sky;
}

TKGroundVegetationSceneNode* ObjectsManager::createGroundVegetation(ISceneNode* parent, vector3df pos)
{
    if(parent == 0) parent = device->getSceneManager()->getRootSceneNode();
    TKGroundVegetationSceneNode* temp=new TKGroundVegetationSceneNode(parent, smgr, -1, DRAW_DISTANCE);
    temp->setPosition(pos);

    vectorGroundVegetation.push_back(temp);

    return temp;
}

TKRockSceneNode* ObjectsManager::createRock(ISceneNode* parent, vector3df pos)
{
    if(parent == 0) parent = device->getSceneManager()->getRootSceneNode();
    TKRockSceneNode* temp=new TKRockSceneNode(parent, smgr, -1, DRAW_DISTANCE);
    temp->setPosition(pos);

    vectorRock.push_back(temp);

    return temp;
}

TKTerrainSceneNode* ObjectsManager::createTerrain(stringc heightMapFile)
{
    terrain = new TKTerrainSceneNode(device->getSceneManager()->getRootSceneNode(),device->getSceneManager(),-1,heightMapFile);

    terrain->setMaterialType((E_MATERIAL_TYPE)materialTerrain);
    stringc baseFile=heightMapFile.subString(0,heightMapFile.size()-4);
    terrain->setMaterialTexture(0,device->getVideoDriver()->getTexture(baseFile+"_Alpha.png"));
    terrain->setMaterialTexture(1,device->getVideoDriver()->getTexture("../media/Terrain/0.jpg"));///TODO carregar do XML
    terrain->setMaterialTexture(2,device->getVideoDriver()->getTexture("../media/Terrain/1.jpg"));
    terrain->setMaterialTexture(3,device->getVideoDriver()->getTexture("../media/Terrain/2.jpg"));
    terrain->setMaterialTexture(4,device->getVideoDriver()->getTexture("../media/Terrain/3.jpg"));
    terrain->setMaterialTexture(5,fowTexture);

    std::cout << "Terrain: " << heightMapFile.c_str() << std::endl;

    stringc grassMapFile = heightMapFile;
    IImage* grassMap=device->getVideoDriver()->createImageFromFile(grassMapFile.c_str());

    ///TODO: colocar todos os resources em uma unica imagem, ler RGB ao inves de luminance
    stringc treeMapFile = baseFile+"_Tree.png";
    IImage* treeMap=device->getVideoDriver()->createImageFromFile(treeMapFile.c_str());

    stringc wayMapFile = baseFile+"_Ways.png";
    IImage* wayMap=device->getVideoDriver()->createImageFromFile(wayMapFile.c_str());

    stringc resourcesMapFile = baseFile+"_Resources.png";
    IImage* resourcesMap=device->getVideoDriver()->createImageFromFile(resourcesMapFile.c_str());

    int offset=18;
    f32 mapx,mapz,scale,rxz,altura;

    for(int z=0;z < grassMap->getDimension().Height;z+=10)
    {
        for(int x=0;x < grassMap->getDimension().Width;x+=10)
        {
            if(treeMap->getPixel(x,z).getLuminance() > 127 )
            {
                rxz=(rand()%10)*0.1;

                //Create trees and ground vegetation
                for(int i=0;i<3;i++)
                {
                    mapx=((x+rxz)*MAPSIZE + (rand()%offset) )/MAPSIZE;
                    mapz=((z+rxz)*MAPSIZE+(rand()%offset))/MAPSIZE;

                    altura = grassMap->getPixel(mapx,mapz).getLuminance()/255.0 * terrain->TERRAIN_HEIGHT;

                    TKTreeSceneNode* tree=createTree(0,vector3df(mapx*MAPSIZE ,altura ,mapz*MAPSIZE ));

                    ///TODO:Colisões que preste!
                    selectors.push_back(smgr->createOctTreeTriangleSelector(tree->getMesh(),tree,4));

                    tree->setRotation(vector3df(0,rand()%360,0));
                    scale=4+(rand()%10)*0.1;
                    tree->setScale(vector3df(scale,scale,scale));

                    woodResources.push_back(tree->getPosition());


                    mapx=((x+rxz)*MAPSIZE + (rand()%offset))/MAPSIZE;
                    mapz=((z+rxz)*MAPSIZE+(rand()%offset))/MAPSIZE;

                    altura = grassMap->getPixel(mapx,mapz).getLuminance()/255.0 * terrain->TERRAIN_HEIGHT;

                    TKGroundVegetationSceneNode* gtemp=createGroundVegetation(0,vector3df(mapx*MAPSIZE ,altura+1 ,mapz*MAPSIZE ));

                    gtemp->setRotation(vector3df(0,rand()%360,0));
                    scale=1+(rand()%10)*0.1;
                    gtemp->setScale(vector3df(scale,scale,scale));
                }

                //create rocks
                mapx=((x+rxz)*MAPSIZE + (rand()%offset))/MAPSIZE;
                mapz=((z+rxz)*MAPSIZE+(rand()%offset))/MAPSIZE;

                altura = grassMap->getPixel(mapx,mapz).getLuminance()/255.0 * terrain->TERRAIN_HEIGHT;

                TKRockSceneNode* rock=createRock(0,vector3df(mapx*MAPSIZE ,altura+1 ,mapz*MAPSIZE ) );
            }
        }
    }


    u32 *pixelsMiniMap = (u32*)miniMapTexture->lock();

    SColor colorGreen(120,0,150,0);
    SColor colorBlue(100,0,0,255);

    ///Create MiniMap Base Texture
    for(int z=0;z < grassMap->getDimension().Height;z++)
    {
        for(int x=0;x < grassMap->getDimension().Width;x++)
        {
            if(grassMap->getPixel(x,z).getLuminance() > 50)
                pixelsMiniMap[(miniMapTexture->getSize().Width-z)*miniMapTexture->getSize().Width + (x)] = colorGreen.color;
            else
                pixelsMiniMap[(miniMapTexture->getSize().Width-z)*miniMapTexture->getSize().Width + (x)] = colorBlue.color;
        }
    }
    //apply changes
    miniMapTexture->unlock();

    ///Add Resources to cenario
    for(int z=0;z < resourcesMap->getDimension().Height;z++)
    {
        for(int x=0;x < resourcesMap->getDimension().Width;x++)
        {
            if(resourcesMap->getPixel(x,z).getGreen() > 127 )//Stone Source
            {
                altura = grassMap->getPixel(x,z).getLuminance()/255.0 * terrain->TERRAIN_HEIGHT;
                TKStoneResource* tempResource=createStoneResource(vector3df(x*MAPSIZE ,altura+1 ,z*MAPSIZE));
                stoneResources.push_back(tempResource->getPosition());
            }
            if(resourcesMap->getPixel(x,z).getRed() > 127 )//Food Source
            {
                altura = grassMap->getPixel(x,z).getLuminance()/255.0 * terrain->TERRAIN_HEIGHT;
                TKFoodResource* tempResource=createFoodResource(vector3df(x*MAPSIZE ,altura+1 ,z*MAPSIZE));
                foodResources.push_back(tempResource->getPosition());
            }
        }
    }

    ITriangleSelector* selector = smgr->createTriangleSelector(terrain->getMesh(), terrain);
    terrain->setTriangleSelector(selector);

    treeMap->drop();
    grassMap->drop();
    wayMap->drop();
    resourcesMap->drop();

    return terrain;
}

void ObjectsManager::update()
{
    ///===========================================OCEANO
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
    ///===========================================OCEANO
}

E_MATERIAL_TYPE ObjectsManager::getTerrainMaterial()
{
    return (E_MATERIAL_TYPE)materialTerrain;
}

ITriangleSelector* ObjectsManager::getTerrainTriangleSelector()
{
    return terrain->getTriangleSelector();
}

ITexture* ObjectsManager::getMiniMapTexture()
{
    return miniMapTexture;
}

vector<ITriangleSelector*> ObjectsManager::getTriangleSelectors()
{
    return selectors;
}

vector<vector3df> ObjectsManager::getResourcesPos()
{
    vector<vector3df> posVet;
    for(int i=0;i<foodResources.size();i++)
    {
        posVet.push_back(foodResources[i]);
    }
    for(int i=0;i<stoneResources.size();i++)
    {
        posVet.push_back(stoneResources[i]);
    }
    for(int i=0;i<woodResources.size();i++)
    {
        posVet.push_back(woodResources[i]);
    }

    return posVet;
}

vector<stringc> ObjectsManager::getResourcesType()
{
    vector<stringc> strVet;
    for(int i=0;i<foodResources.size();i++)
    {
        strVet.push_back("FOOD");
    }
    for(int i=0;i<stoneResources.size();i++)
    {
        strVet.push_back("STONE");
    }
    for(int i=0;i<woodResources.size();i++)
    {
        strVet.push_back("WOOD");
    }

    return strVet;
}
