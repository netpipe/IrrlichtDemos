
#include "Weather.h"

Weather::Weather(WorldBase *worldBase)
    : WorldBase(worldBase)
{
    psRain = NULL;
    
    // Some default values
    rainSetup(40.0,5.0,20.0,core::vector3df(0,-0.01,0),200,8000);
}

void Weather::rainSetup(
	f32 rainHeight,
	f32 rainOffset,
	f32 rainArea,
	core::vector3df rainVel,
	int rainDropsPerSecond,
	int rainDropLifeTime
    )
{
	rainPos = core::vector3df(0,rainHeight,0);
	this->rainOffset = rainOffset;
	this->rainArea = rainArea;
	this->rainVel = rainVel;
	this->rainDropsPerSecond = rainDropsPerSecond;
	this->rainDropLifeTime = rainDropLifeTime;
	
	// If raining, then restart
	if (psRain)
	{
        psRain->remove();
        psRain = NULL;
        setRaining(true);
    }
}

void Weather::setRaining(bool rain)
{
    // Not raining
    if (!rain)
    {
        if (psRain)
        {
            psRain->remove();
            psRain = NULL;
        }
        return;
    }
    
    // Raining
    
    // If already raining, do nothing.
    if (psRain) return;
    
	psRain = smgr->addParticleSystemSceneNode(
        false,
        smgr->getActiveCamera(),
        -1,
        rainPos
        );
    
    psRain->setParticleSize( core::dimension2d<f32>(1.0,1.0) * 0.1);
    psRain->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    psRain->setMaterialFlag(video::EMF_LIGHTING, false);
    psRain->setMaterialTexture(0, driver->getTexture("raindrop.tga"));
    
    scene::IParticleEmitter *pemRain = psRain->createBoxEmitter(
        core::aabbox3df(-rainArea*0.5,0,-rainArea*0.5, rainArea*0.5,1,rainArea*0.5),
        rainVel, // direction
        rainDropsPerSecond, // min per second
        rainDropsPerSecond, // max per second
        video::SColor(255,255,255,255), // min start colour
        video::SColor(255,255,255,255), // max start colour
        rainDropLifeTime, // min life
        rainDropLifeTime, // max life
        0 // max angle
        );
    
    psRain->setEmitter(pemRain);
    pemRain->drop();
}


