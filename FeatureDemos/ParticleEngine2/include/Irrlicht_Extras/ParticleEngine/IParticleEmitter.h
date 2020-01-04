#ifndef IPARTICLEEMITTER_H
#define IPARTICLEEMITTER_H

#include <IReferenceCounted.h>
#include "IEmitterRegion.h"
#include "Particle.h"
#include <irrList.h>

namespace irr
{
namespace scene
{
namespace particle
{
class IParticleEmitter : public irr::IReferenceCounted
{
public:
    IParticleEmitter()
    {
        MaxParticleEmitt = 0;
        ParticlesEmitted = 0;
        Time = 0;
        MaxParticlesPerSecond = 0;
        MinParticlesPerSecond = 0;

        Region = new CPointRegion();
    }
    virtual ~IParticleEmitter()
    {
        if (Region)
        {
            Region->drop();
            Region = 0;
        }
    }

    bool emittAll(void) const
    {
        if (MinParticlesPerSecond == 0 && ParticlesEmitted < MaxParticleEmitt)
            return true;
        return false;
    }

    virtual void emitt(const irr::core::matrix4& transform, irr::core::list<irr::scene::particle::Particle*>& particles, irr::core::list<irr::scene::particle::Particle*>& particlepool, irr::u32 timeMs, irr::f32 diff, const irr::core::array<irr::scene::particle::SParticleUV>& coords) = 0;

    void setRegion(IEmitterRegion* region)
    {
        if (region)
            region->grab();
        if (Region)
            Region->drop();
        Region = region;
    }

    IEmitterRegion* getRegion(void)
    {
        return Region;
    }

    const irr::u32& getMaxParticleEmitt(void) const
    {
        return MaxParticleEmitt;
    }

    const irr::u32& getParticleEmitted(void) const
    {
        return ParticlesEmitted;
    }

    const irr::u32& getMaxParticlesPerSecond(void) const
    {
        return MaxParticlesPerSecond;
    }

    const irr::u32& getMinParticlesPerSecond(void) const
    {
        return MinParticlesPerSecond;
    }

    const irr::u32& getMaxLifeTime(void) const
    {
        return MaxLifeTime;
    }

    const irr::u32& getMinLifeTime(void) const
    {
        return MinLifeTime;
    }

    void setColor(const irr::video::SColor& min, const irr::video::SColor& max)
    {
        MinColor = min;
        MaxColor = max;
    }

    const irr::video::SColor& getMinColor(void) const
    {
        return MinColor;
    }

    const irr::video::SColor& getMaxColor(void) const
    {
        return MaxColor;
    }

    void setMaxAngleDegrees(const irr::core::vector3di& angle)
    {
        MaxAngleDegrees = angle;
    }

    const irr::core::vector3di& getMaxAngleDegrees(void) const
    {
        return MaxAngleDegrees;
    }

    void setSpeed(const irr::core::vector3df& speed)
    {
        Speed = speed;
    }

    const irr::core::vector3df& getSpeed(void) const
    {
        return Speed;
    }


    void setRotation(const irr::core::vector3df& rot)
    {
        Rotation = rot;
    }

    const irr::core::vector3df& getRotation(void) const
    {
        return Rotation;
    }


    void setRotationSpeed(const irr::core::vector3df& rotspeed)
    {
        RotationSpeed = rotspeed;
    }

    const irr::core::vector3df& getRotationSpeed(void) const
    {
        return RotationSpeed;
    }


    void setSize(const irr::core::vector2df& min, const irr::core::vector2df& max)
    {
        MinSize = min;
        MaxSize = max;
    }

    const irr::core::vector2df& getMinSize(void) const
    {
        return MinSize;
    }

    const irr::core::vector2df& getMaxSize(void) const
    {
        return MaxSize;
    }

    void setSizeSpeed(const irr::core::vector2df& speed)
    {
        SizeSpeed = speed;
    }

    const irr::core::vector2df& getSizeSpeed(void) const
    {
        return SizeSpeed;
    }

protected:
    IEmitterRegion* Region;

    irr::f32 Time;

    irr::core::vector3di MaxAngleDegrees;

    irr::u32 MaxParticleEmitt;
    irr::u32 ParticlesEmitted;

    irr::u32 MinParticlesPerSecond;
    irr::u32 MaxParticlesPerSecond;

    irr::core::vector3df Speed;

    irr::core::vector3df Rotation;
    irr::core::vector3df RotationSpeed;

    irr::core::vector2df MinSize;
    irr::core::vector2df MaxSize;
    irr::core::vector2df SizeSpeed;

    irr::video::SColor MinColor;
    irr::video::SColor MaxColor;

    irr::u32 MinLifeTime;
    irr::u32 MaxLifeTime;
private:
};
}
}
}
#endif // IPARTICLEEMITTER_H
