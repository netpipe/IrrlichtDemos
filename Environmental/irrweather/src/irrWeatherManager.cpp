// Copyright (C) 2009-2011 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// For conditions of distribution and use, see copyright notice in irrWeatherManager.h

#include "irrWeatherManager.h"
#include "IBoltSceneNode.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

irrWeatherManager::irrWeatherManager(irr::IrrlichtDevice* const irrDevice)
{
    if(!irrDevice)
        return;

    device = irrDevice;

    atmosphere = new IWeatherManagerAtmosphere(device,device->getSceneManager()->getRootSceneNode(),-1);

    if(!atmosphere)
    {
        printf("-- irrWeatherManager: ERROR: could not create atmosphere --\n");
        return;
    }

    else
        printf("irrWeatherManager %i.%i.%i\n", IRR_WEATHER_MANAGER_VER_MAJOR, IRR_WEATHER_MANAGER_VER_MINOR, IRR_WEATHER_MANAGER_VER_MICRO);

    lightningFrequency = 300.0f;

    nextLightningUpdate = device->getTimer()->getTime() + lightningFrequency;

    CloudLayerCount = 0;
}

int irrWeatherManager::getRandom(int lim, int seed) const
{
    static long a = seed;

    a = (a * 32719 + 3) % 32749;

    return ((a % lim) + 1);
}

/*void irrWeatherManager::addCloud(const SCloudCreationInfo &info, u32 extraSeed)
{
    // create a cloud node
    ICloudSceneNode* clouds = new ICloudSceneNode(
            device->getSceneManager()->getRootSceneNode(), device->getSceneManager(),
                device->getTimer(), info.ID, info.location, info.rotation, vector3df(1,1,1), 6000.0f);

    video::ITexture* txture = 0;
    if(info.textureNames.size() > 0)
    {
        int num = clouds->rand2(info.textureNames.size(), device->getTimer()->getTime()*extraSeed);

        txture = device->getVideoDriver()->getTexture(info.textureNames[num-1].c_str());
    }

    else
        txture = device->getVideoDriver()->getTexture(info.textureName.c_str());

    // set any old random seed
    srand(device->getTimer()->getRealTime() + extraSeed);

    // set the level of details to draw at what distance
    clouds->setLOD(info.LOD);

    // set the maximum detail level when recursing
    clouds->setMaxDepth(info.maxDepth);

    // we dont use lighting yet. all the normals face the camera and it looks wierd
    // we could edit the colour of the particles in the list in realtime though!
    clouds->setMaterialFlag(video::EMF_LIGHTING, true);


    clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // set any old random seed
    srand(rand());

    // call a "here's one I made earlier" function
    clouds->makeRandomCloud(info.numParticles); //60

    clouds->setScale(info.scale);

    clouds->setMaterialTexture(0, txture );
}*/

/*void irrWeatherManager::addCloud(const vector3df &pos, const vector3df &scale, stringc textureName, u32 extraSeed)
{
    // create a cloud node
    ICloudSceneNode* clouds = new ICloudSceneNode(
            device->getSceneManager()->getRootSceneNode(), device->getSceneManager(),
                device->getTimer(), -1, pos, core::vector3df(0,0,0), core::vector3df(1,1,1), 6000.0f);

    // load the texture
    video::ITexture * txture = device->getVideoDriver()->getTexture(textureName.c_str());

    // set any old random seed
    srand(device->getTimer()->getRealTime() + extraSeed);

    // set the level of details to draw at what distance
    clouds->setLOD(10.0f);

    // set the maximum detail level when recursing
    clouds->setMaxDepth(3);

    // we dont use lighting yet. all the normals face the camera and it looks wierd
    // we could edit the colour of the particles in the list in realtime though!
    clouds->setMaterialFlag(video::EMF_LIGHTING, true);


    clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // set any old random seed
    srand(rand());

    // call a "here's one I made earlier" function
    clouds->makeRandomCloud(10); //60

    clouds->setLOD(10.0f);
    clouds->setMaxDepth(1);


    clouds->setScale(scale);

    clouds->setMaterialTexture(0, txture );


    // cheap movement just so the clouds are not static
    scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyStraightAnimator(clouds->getPosition(), clouds->getPosition() + vector3df(0,0,80000), 456400, true, true);
	clouds->addAnimator(anim);
	anim->drop();
}*/

void irrWeatherManager::updateWeather()
{
    atmosphere->update();

    if(device->getTimer()->getTime() >= nextLightningUpdate)
    {
        for(u32 i = 0; i < CloudLayerCount; ++i)
        {
            //printf("IS STORM: %s\n", getCloudLayerByIndex(i)->getIsStormCloudLayer() ? "True":"False");
            if(getCloudLayerByIndex(i)->getIsStormCloudLayer()==true)
            {
                srand(device->getTimer()->getRealTime());

                //u32 randomCloudCount = rand()%((getCloudLayerByIndex(i)->getCloudCount()-1)/2)+0;
                for(u32 k=0; k < 2; ++k)
                {
                    //u32 randomCloudIndex = getRandom(getCloudLayerByIndex(i)->getCloudCount(), device->getTimer()->getRealTime());
                    u32 randomCloudIndex = rand()%(getCloudLayerByIndex(i)->getCloudCount()-1)+0;

                    //if(randomCloudIndex > getCloudLayerByIndex(i)->getCloudCount())
                        //randomCloudIndex = getCloudLayerByIndex(i)->getCloudCount();

                    // Since getRandom() only returns values greater than zero, we subtract 1 from the random value.
                    ICloudSceneNode* cloud = getCloudLayerByIndex(i)->getCloudByIndex(randomCloudIndex);

                    u32 randInt = getRandom(2, device->getTimer()->getRealTime());

                    u32 randHeight = rand()%100+40;
                    u32 randBolts = rand()%3+1;
                    u32 randScaleX = rand()%30+1;
                    u32 randScaleZ = rand()%30+1;
                    u32 randLifeTime = rand()%200+20;

                    u32 dirX = rand()%800+10;
                    u32 dirZ = rand()%800+10;
                    u32 neg = rand()%2+1;

                    if(randInt == 1)
                    {
                        irr::scene::IBoltSceneNode* beam = 0;
                        beam = new irr::scene::IBoltSceneNode(
                            cloud, device->getSceneManager(), -1);
                        beam->setLine(vector3df(0,0,0), vector3df((neg=1)?dirX:-dirX,-14020,(neg=1)?dirZ:-dirZ),
                        50, randHeight, 10, randBolts, false, irr::video::SColor(255,185,243,255));
                        beam->drop();
                        beam->setScale(vector3df(randScaleX,1,randScaleZ));

                        //void setLine(core::vector3df start, core::vector3df end, u32 updateTime = 300, u32 height = 10,
                        //u32 parts = 10, u32 bolts = 1, bool steddyend = true, video::SColor color = video::SColor(255,255,0,0));

                        if(beam)
                        {
                            ISceneNodeAnimator* anim = 0;
                            anim = device->getSceneManager()->createDeleteAnimator(randLifeTime);
                            beam->addAnimator(anim);
                            anim->drop();
                        }

                        cloud->getMaterial(0).AmbientColor = irr::video::SColor(255,28,216,255);
                        cloud->setLightningHost(device->getTimer()->getTime() + randLifeTime);


                        /*ILightSceneNode* light1 =
                        device->getSceneManager()->addLightSceneNode(beam, core::vector3df(0.0f,0.0f,0.0f),
                        irr::video::SColor(255,185,243,255), 40200.0);

                        if(light1)
                        {
                            ISceneNodeAnimator* anim = 0;
                            anim = device->getSceneManager()->createDeleteAnimator(randLifeTime);
                            light1->addAnimator(anim);
                            anim->drop();
                        }*/

                        //printf("CREATE LIGHTNING\n");
                    }
                }
            }
        }
        nextLightningUpdate = device->getTimer()->getTime() + lightningFrequency;
    }
}

/*void irrWeatherManager::addCloudLayer(const vector3df &center, const vector3df &cloudScale, irr::core::stringc cloudTextureName,
    u32 numClouds, f32 randomness)
{
    for(u32 i=0; i < numClouds; ++i)
    {
        srand(device->getTimer()->getRealTime() + i*33);
        f32 x = rand() / randomness - (rand() / randomness);
        f32 z = rand() / randomness - (rand() / randomness); // 50.0

        // Altitude will be set from the midpoint of the world on the Y axis (sea level)
        createCloud(center + vector3df(x, 0, z), cloudScale, cloudTextureName.c_str(), i*22);
    }
}*/

ICloudLayer* irrWeatherManager::addCloudLayer(const vector3df &center, SCloudCreationInfo &info,
    u32 numClouds, bool isStormCloudLayer, f32 randomness)
{
    ICloudLayer* layer = 0;
    layer = new ICloudLayer(device, center);
    for(u32 i=0; i < numClouds; ++i)
    {
        srand(device->getTimer()->getRealTime());
        f32 x = rand() / randomness - (rand() / randomness);
        srand(device->getTimer()->getRealTime() * rand());
        f32 z = rand() / randomness - (rand() / randomness); // 50.0

        // Altitude will be set from "center"
        info.location = center + vector3df(x,0,z);
        layer->addCloud(info, i*rand());
    }

    layer->setIsStormCloudLayer(isStormCloudLayer);

    CloudLayers.push_back(layer);
    ++CloudLayerCount;

    if(layer)
        return layer;

    return 0;
}

ICloudLayer* irrWeatherManager::getCloudLayerByIndex(u32 index) const
{
    list<ICloudLayer*>::ConstIterator it = CloudLayers.begin();

    it += index;
    ICloudLayer* layer = 0;
    layer = (*it);
    if(layer != 0)
    {
        return layer;
    }

    return 0;
}


void irrWeatherManager::removeCloudLayer(ICloudLayer* layer, bool removeFromScene)
{
    if(layer)
    {
        list<ICloudLayer*>::Iterator it = CloudLayers.begin();

        for(; it != CloudLayers.end(); )
        {
            ICloudLayer* cloudLayer = (*it);
            if(cloudLayer == layer)
            {
                if(removeFromScene)
                    cloudLayer->removeAllClouds(removeFromScene);
                it = CloudLayers.erase(it);
                CloudLayerCount--;
            }

            else
                ++it;
        }
    }
}


void irrWeatherManager::removeAllCloudLayers(bool removeFromScene)
{
    list<ICloudLayer*>::Iterator it = CloudLayers.begin();

    for(; it != CloudLayers.end(); )
    {
        ICloudLayer* cloudLayer = (*it);
        if(removeFromScene)
            cloudLayer->removeAllClouds(removeFromScene);
        it = CloudLayers.erase(it);
        CloudLayerCount--;
    }
}

u32 irrWeatherManager::getCloudLayerCount() const
{
    return CloudLayerCount;
}

irrWeatherManager::~irrWeatherManager()
{
    removeAllCloudLayers();
}
