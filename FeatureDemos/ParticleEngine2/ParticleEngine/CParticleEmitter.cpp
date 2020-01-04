/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "CParticleEmitter.h"
#include "IEmitterRegion.h"

CParticleEmitter::CParticleEmitter(void)
{
    MaxParticleEmitt = 0;
    ParticlesEmitted = 0;
    Time = 0;
    MaxParticlesPerSecond = 0;
    MinParticlesPerSecond = 0;

    Region = new CPointRegion();
}

CParticleEmitter::CParticleEmitter(const irr::core::vector3df& rotation, const irr::core::vector3df& rotationspeed, const irr::core::vector3df& speed, const irr::core::vector3di& maxAngle, const irr::u32& minPpS, const irr::u32& maxPpS, const irr::u32& maxEmitt, const irr::u32& minlifetime, const irr::u32& maxlifetime, const irr::core::vector2df& minsize, const irr::core::vector2df& maxsize, const irr::core::vector2df& sizespeed, const irr::video::SColor& minColor, const irr::video::SColor& maxColor)
{
    Region = new CPointRegion();

    Time = 0;

    Speed = speed;

    Rotation = rotation;
    RotationSpeed = rotationspeed;

    MaxParticlesPerSecond = maxPpS;
    MinParticlesPerSecond = minPpS;

    MaxAngleDegrees = maxAngle;

    MinLifeTime = minlifetime;
    MaxLifeTime = maxlifetime;

    MinSize = minsize;
    MaxSize = maxsize;
    SizeSpeed = sizespeed;

    MinColor = minColor;
    MaxColor = maxColor;

    MaxParticleEmitt = maxEmitt;
    ParticlesEmitted = 0;
}

CParticleEmitter::~CParticleEmitter(void)
{
    //dtor
}

void CParticleEmitter::emitt(const irr::core::matrix4& transform, irr::core::list<irr::scene::particle::Particle*>& particles, irr::core::list<irr::scene::particle::Particle*>& particlepool, irr::u32 timeMs, irr::f32 diff, const irr::core::array<irr::scene::particle::SParticleUV>& coords)
{
    if (doEmitt(diff))
    {
        if (getRegion())
            emittParticleStandard(getRegion()->getPoint(transform), transform, particles, particlepool, timeMs, coords);
    }
}



irr::scene::particle::Particle* CParticleEmitter::create(irr::core::list<irr::scene::particle::Particle*>& particlepool, const irr::core::vector3df& pos, const irr::core::vector3df& speed, const irr::core::vector3df& rot, const irr::core::vector3df& rotSpeed, const irr::core::vector2df& size, const irr::core::vector2df& sizespeed, const irr::video::SColor& color, const irr::u32& start, const irr::u32& end)
{
    irr::core::list<irr::scene::particle::Particle*>::Iterator it = particlepool.begin();
    if (it != particlepool.end())
    {
        irr::scene::particle::Particle* p = *it;
        particlepool.erase(it);
        p->reset(pos, speed, rot, rotSpeed, size, sizespeed, color, start, end);
        return p;
    }
    return new irr::scene::particle::Particle(pos, speed, rot, rotSpeed, size, sizespeed, color, start, end);
}

irr::core::vector3df CParticleEmitter::rotateVec(const irr::core::vector3df& vec)
{
    irr::core::vector3df tgt = vec;
    if (MaxAngleDegrees.X != 0 || MaxAngleDegrees.Y != 0)
    {
        irr::core::matrix4 mat;
        irr::f32 x = (MaxAngleDegrees.X != 0 ? ((rand()%((int)(MaxAngleDegrees.X*2)))-MaxAngleDegrees.X) : 0);
        irr::f32 y = (MaxAngleDegrees.Y != 0 ? ((rand()%((int)(MaxAngleDegrees.Y*2)))-MaxAngleDegrees.Y) : 0);
        irr::f32 z = (MaxAngleDegrees.Z != 0 ? ((rand()%((int)(MaxAngleDegrees.Z*2)))-MaxAngleDegrees.Z) : 0);
        mat.setRotationDegrees(irr::core::vector3df(x, y, z));
        mat.rotateVect(tgt);
    }
    return tgt;
}

bool CParticleEmitter::doEmitt(irr::f32 diff)
{
    if ((MaxParticleEmitt > 0 && ParticlesEmitted == MaxParticleEmitt) || (MaxParticleEmitt == 0 && MinParticlesPerSecond == 0))
        return false;

    if (MinParticlesPerSecond == 0 && ParticlesEmitted < MaxParticleEmitt)
    {
        ParticlesEmitted++;
        return true;
    }

    Time += diff;

    const irr::u32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
    const irr::f32 perSecond = pps ? (irr::f32)MinParticlesPerSecond + (rand() % pps) : MinParticlesPerSecond;
    const irr::f32 everyWhatMillisecond = 1.0f / perSecond;
    if (Time > everyWhatMillisecond)
    {
        Time = 0;
        if (MaxParticleEmitt > 0)
            ParticlesEmitted++;
        return true;
    }
    return false;
}

void CParticleEmitter::emittParticleStandard(const irr::core::vector3df& pos, const irr::core::matrix4& transform, irr::core::list<irr::scene::particle::Particle*>& particles, irr::core::list<irr::scene::particle::Particle*>& particlepool, irr::u32 timeMs, const irr::core::array<irr::scene::particle::SParticleUV>& coords)
{
    irr::u32 lifetime = MinLifeTime;
    if (MaxLifeTime-MinLifeTime > 0)
        lifetime+=rand()%(MaxLifeTime-MinLifeTime);

    irr::f32 d = (rand()%100)/100.f;
    irr::core::vector2df size = MinSize+(MaxSize-MinSize)*d;
    size *= transform.getScale().getLength();

    irr::core::vector2df sizeSpeed = SizeSpeed*transform.getScale().getLength();

    irr::video::SColor color = MaxColor.getInterpolated(MinColor, d);

    irr::core::matrix4 mat;
    mat.setRotationDegrees(transform.getRotationDegrees());
    mat.setScale(transform.getScale());

    irr::core::vector3df s;
    if (Region && Region->isForceField())
        s = rotateVec(Region->getVector()*Speed.getLength());
    else
        s = rotateVec(Speed);
    mat.transformVect(s);
    irr::core::vector3df r = Rotation;
    if (r.getLengthSQ() > 0)
        mat.rotateVect(r);
    else
        r = transform.getRotationDegrees();
    irr::core::vector3df rs = RotationSpeed;
    mat.rotateVect(rs);

    irr::scene::particle::Particle* p = create(particlepool, pos, s, r, rs, size, sizeSpeed, color, timeMs, timeMs+lifetime);
    if (coords.size() > 0)
    {
        irr::u32 u = rand()%(coords.size());
        p->UVs = coords[u];
    }
    particles.push_back(p);
}
