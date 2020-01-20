// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#include "CParticleExplosionEmitter.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CParticleExplosionEmitter::CParticleExplosionEmitter(
	const core::vector3df& center, f32 radius,
	core::vector3df vDirection, u32 minParticlesPerSecond,
	u32 maxParticlesPerSecond, const video::SColor& minStartColor,
	const video::SColor& maxStartColor, u32 lifeTimeMin, u32 lifeTimeMax,
	const core::dimension2df& minStartSize,
	const core::dimension2df& maxStartSize )
	: Center(center), Radius(radius),
	MinStartSize(minStartSize), MaxStartSize(maxStartSize),
	MinParticlesPerSecond(minParticlesPerSecond),
	MaxParticlesPerSecond(maxParticlesPerSecond),
	MinStartColor(minStartColor), MaxStartColor(maxStartColor),
	MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
	Time(0), Emitted(0)
{
  Direction=vDirection;
	#ifdef _DEBUG
	setDebugName("CParticleExplosionEmitter");
	#endif

}


//! Prepares an array with new particles to emitt into the system
//! and returns how much new particles there are.
s32 CParticleExplosionEmitter::emitt(u32 now, u32 timeSinceLastCall, SParticle*& outArray)
{
	Time += timeSinceLastCall;

	if (Emitted>0) return 0;

	const u32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
	const f32 particleCount = pps ? (f32)MinParticlesPerSecond + (os::Randomizer::rand() % pps) : MinParticlesPerSecond;

  Particles.set_used(0);
  Time = 0;
  SParticle p;

  for(u32 i=0; i<particleCount; ++i)
  {
    // Random distance from center
    f32 distance =fmodf( (f32)os::Randomizer::rand(), Radius * 1000.0f ) * 0.001f;

    // Random direction from center
    p.pos.set(Center + distance);

    /**/p.pos.rotateXYBy(  os::Randomizer::rand() % 360, Center );
    p.pos.rotateYZBy( os::Randomizer::rand() % 360, Center );
    p.pos.rotateXZBy( os::Randomizer::rand() % 360, Center );/**/

    p.startTime = now;
    p.vector = Direction;

    core::vector3df tgt = Direction;
    /**/tgt.rotateXYBy((os::Randomizer::rand()%(360*2)) - 360);
    tgt.rotateYZBy((os::Randomizer::rand()%(360*2)) - 360);
    tgt.rotateXZBy((os::Randomizer::rand()%(360*2)) - 360);/**/
    p.vector = tgt;

    if(MaxLifeTime - MinLifeTime == 0)
      p.endTime = now + MinLifeTime;
    else
      p.endTime = now + MinLifeTime + (os::Randomizer::rand() % (MaxLifeTime - MinLifeTime));

    p.color = MinStartColor.getInterpolated(
      MaxStartColor, (os::Randomizer::rand() % 100) / 100.0f);

    p.startColor = p.color;
    p.startVector = p.vector;

    if (MinStartSize==MaxStartSize)
      p.startSize = MinStartSize;
    else
      p.startSize = MinStartSize.getInterpolated(
        MaxStartSize, (os::Randomizer::rand() % 100) / 100.0f);
    p.size = p.startSize;

    Particles.push_back(p);
  }

  outArray = Particles.pointer();

  Emitted+=Particles.size();
  return Particles.size();
}

} // end namespace scene
} // end namespace irr


