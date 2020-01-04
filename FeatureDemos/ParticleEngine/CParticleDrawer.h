#ifndef CPARTICLEDRAWER_H
#define CPARTICLEDRAWER_H

#include "Irrlicht_Extras/ParticleEngine/Particle.h"
#include "Irrlicht_Extras/ParticleEngine/IParticleDrawer.h"
#include <irrList.h>
#include <SMaterial.h>
#include <CMeshBuffer.h>

class CParticleDrawer : public irr::scene::particle::IParticleDrawer
{
public:
    CParticleDrawer(void);
    virtual ~CParticleDrawer(void);

    irr::scene::particle::IParticleEmitter* addStandardEmitter(const irr::core::vector3df& rotation, const irr::core::vector3df& rotationspeed, const irr::core::vector3df& speed, const irr::core::vector3di& maxAngle, const irr::u32& minPpS, const irr::u32& maxPpS, const irr::u32& maxEmitt, const irr::u32& minlifetime, const irr::u32& maxlifetime, const irr::core::vector2df& minsize, const irr::core::vector2df& maxsize, const irr::core::vector2df& sizespeed, const irr::video::SColor& minColor, const irr::video::SColor& maxColor);

    irr::video::SMaterial& getMaterial(void);

    void addUVCoords(const irr::scene::particle::SParticleUV& coord);

    virtual void doParticles(bool localParticles, const irr::scene::ICameraSceneNode* camera, const irr::core::matrix4& transform, irr::u32 timeMs, irr::f32 diff);

    virtual void createParticle(const irr::u32& id, const irr::scene::particle::Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform) = 0;

    virtual void drawParticles(const irr::core::matrix4& transform, irr::video::IVideoDriver* driver, bool useOneMaterial);

    void setEmitter(irr::scene::particle::IParticleEmitter* emitter);

    irr::scene::particle::IParticleEmitter* getEmitter(void);

    void addAffector(irr::scene::particle::IParticleAffector* affector);

    static irr::core::list<irr::scene::SMeshBuffer*>* BufferPool;
    //static irr::core::list<Particle*> ParticlePool;

    const irr::core::aabbox3df& getBBox(void) const;

protected:
    irr::scene::SMeshBuffer* Buffer;
    irr::video::SMaterial Material;
    irr::scene::particle::IParticleEmitter* Emitter;

    irr::core::list<irr::scene::particle::Particle*> Particles;
    irr::core::list<irr::scene::particle::Particle*> ParticlePool;

    irr::core::list<irr::scene::particle::IParticleAffector*> Affectors;
    irr::core::array<irr::scene::particle::SParticleUV> UVCoords;

    virtual void rellocateBuffer(irr::u32 particleCount = 0);
    irr::u32 ParticleCount;

    irr::core::aabbox3df Box;
};

#endif // CPARTICLEDRAWER_H
