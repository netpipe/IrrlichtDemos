#ifndef CPARTICLEEMITTER_H
#define CPARTICLEEMITTER_H

#include "Irrlicht_Extras/ParticleEngine/Particle.h"
#include "Irrlicht_Extras/ParticleEngine/IParticleEmitter.h"

class CParticleEmitter : public irr::scene::particle::IParticleEmitter
{
public:
    CParticleEmitter(const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0), const irr::core::vector3df& rotationspeed = irr::core::vector3df(0,0,0), const irr::core::vector3df& speed = irr::core::vector3df(0,1,0), const irr::core::vector3di& maxAngle = irr::core::vector3di(45,0,45), const irr::u32& minPpS = 50, const irr::u32& maxPpS = 100, const irr::u32& maxEmitt = 100, const irr::u32& minlifetime = 100, const irr::u32& maxlifetime = 1000, const irr::core::vector2df& minsize = irr::core::vector2df(1,1), const irr::core::vector2df& maxsize = irr::core::vector2df(1,1), const irr::core::vector2df& sizespeed = irr::core::vector2df(0,0), const irr::video::SColor& minColor = irr::video::SColor(255,255,255,255), const irr::video::SColor& maxColor = irr::video::SColor(255,255,255,255));
    CParticleEmitter(void);
    virtual ~CParticleEmitter(void);

    virtual void emitt(const irr::core::matrix4& transform, irr::core::list<irr::scene::particle::Particle*>& particles, irr::core::list<irr::scene::particle::Particle*>& particlepool, irr::u32 timeMs, irr::f32 diff, const irr::core::array<irr::scene::particle::SParticleUV>& coords);
protected:

    irr::scene::particle::Particle* create(irr::core::list<irr::scene::particle::Particle*>& particlepool, const irr::core::vector3df& pos, const irr::core::vector3df& speed, const irr::core::vector3df& rot, const irr::core::vector3df& rotSpeed, const irr::core::vector2df& size, const irr::core::vector2df& sizespeed, const irr::video::SColor& color, const irr::u32& start, const irr::u32& end);
    irr::core::vector3df rotateVec(const irr::core::vector3df& vec);
    bool doEmitt(irr::f32 diff);
    virtual void emittParticleStandard(const irr::core::vector3df& pos, const irr::core::matrix4& transform, irr::core::list<irr::scene::particle::Particle*>& particles, irr::core::list<irr::scene::particle::Particle*>& particlepool, irr::u32 timeMs, const irr::core::array<irr::scene::particle::SParticleUV>& coords);
};

#endif // CPARTICLEEMITTER_H
