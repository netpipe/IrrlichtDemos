// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_PARTICLE_EXPLOSION_EMITTER_H_INCLUDED__
#define __C_PARTICLE_EXPLOSION_EMITTER_H_INCLUDED__

#include "IParticleEmitter.h"
#include "irrArray.h"
#include "aabbox3d.h"

namespace irr
{
namespace scene
{

#define EPET_EXPLOSION (irr::scene::E_PARTICLE_EMITTER_TYPE)(EPET_COUNT+23)

//! A default box emitter
class CParticleExplosionEmitter : public IParticleEmitter {
  protected:
    core::array<SParticle> Particles;

    core::vector3df	Center;
    f32 Radius;
    core::vector3df Direction;

    core::dimension2df MinStartSize, MaxStartSize;
    u32 MinParticlesPerSecond, MaxParticlesPerSecond;
    video::SColor MinStartColor, MaxStartColor;
    u32 MinLifeTime, MaxLifeTime;

    u32 Time;
    u32 Emitted;

  public:

    //! constructor
    CParticleExplosionEmitter(
      const core::vector3df& center, f32 radius, core::vector3df vDirection=core::vector3df(0.0f,0.03f,0.0f),
      u32 minParticlesPerSecond = 20,
      u32 maxParticlesPerSecond = 40,
      const video::SColor& minStartColor = video::SColor(255,0,0,0),
      const video::SColor& maxStartColor = video::SColor(255,255,255,255),
      u32 lifeTimeMin=2000,
      u32 lifeTimeMax=4000,
      const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
      const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) );

    //! Prepares an array with new particles to emitt into the system
    //! and returns how much new particles there are.
    virtual s32 emitt(u32 now, u32 timeSinceLastCall, SParticle*& outArray);

    //! Set direction the emitter emits particles
    virtual void setDirection( const core::vector3df& newDirection ) { Direction = newDirection; }

    //! Set minimum number of particles per second.
    virtual void setMinParticlesPerSecond( u32 minPPS ) { MinParticlesPerSecond = minPPS; }

    //! Set maximum number of particles per second.
    virtual void setMaxParticlesPerSecond( u32 maxPPS ) { MaxParticlesPerSecond = maxPPS; }

    //! Set minimum start color
    virtual void setMinStartColor( const video::SColor& color ) { MinStartColor = color; }

    //! Set maximum start color
    virtual void setMaxStartColor( const video::SColor& color ) { MaxStartColor = color; }

    //! Set the maximum starting size for particles
    virtual void setMaxStartSize( const core::dimension2df& size ) { MaxStartSize = size; };

    //! Set the minimum starting size for particles
    virtual void setMinStartSize( const core::dimension2df& size ) { MinStartSize = size; };

    //! Set the center of the sphere for particle emissions
    virtual void setCenter( const core::vector3df& center ) { Center = center; }

    //! Set the radius of the sphere for particle emissions
    virtual void setRadius( f32 radius ) { Radius = radius; }

    //! Gets direction the emitter emits particles
    virtual const core::vector3df& getDirection() const { return Direction; }

    //! Get minimum number of particles per second.
    virtual u32 getMinParticlesPerSecond() const { return MinParticlesPerSecond; }

    //! Get maximum number of particles per second.
    virtual u32 getMaxParticlesPerSecond() const { return MaxParticlesPerSecond; }

    //! Get minimum start color
    virtual const video::SColor& getMinStartColor() const { return MinStartColor; }

    //! Get maximum start color
    virtual const video::SColor& getMaxStartColor() const { return MaxStartColor; }

    //! Gets the maximum starting size for particles
    virtual const core::dimension2df& getMaxStartSize() const { return MaxStartSize; };

    //! Gets the minimum starting size for particles
    virtual const core::dimension2df& getMinStartSize() const { return MinStartSize; };

    //! Get the center of the sphere for particle emissions
    virtual const core::vector3df& getCenter() const { return Center; }

    //! Get the radius of the sphere for particle emissions
    virtual f32 getRadius() const { return Radius; }

    virtual irr::scene::E_PARTICLE_EMITTER_TYPE getType() const {
      return EPET_EXPLOSION;
    }

    void restart() { Emitted=0; }
};

} // end namespace scene
} // end namespace irr


#endif


