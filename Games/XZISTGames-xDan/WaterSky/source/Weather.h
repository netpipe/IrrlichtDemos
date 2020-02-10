
#ifndef __Weather_h
#define __Weather_h

#include "WorldBase.h"

class Weather : public WorldBase
{
public:
    Weather(WorldBase *);
    
    void rainSetup(f32,f32,f32,core::vector3df,int,int);
    
    void setRaining(bool);
    
private:
    
    scene::IParticleSystemSceneNode *psRain;
    
    // Rain
	core::vector3df rainPos;
	f32 rainOffset;
	f32 rainArea;
	core::vector3df rainVel;
	int rainDropsPerSecond;
	int rainDropLifeTime;
};

#endif
