
#ifndef __Effects_h
#define __Effects_h

#include <irrlicht.h>
using namespace irr;

class SprayAffector : public scene::IParticleAffector
{
public:
    SprayAffector(scene::IParticleSystemSceneNode *ps, scene::ISceneManager *smgr, u32 when)
    : ParticleSystem(ps), SceneManager(smgr), When(when)
    {
        //decal_added = false;
    }
    virtual scene::E_PARTICLE_AFFECTOR_TYPE getType()
    {
        return scene::EPAT_NONE;
    }
    
    virtual void affect(u32 now, scene::SParticle *particlearray, u32 count)
    {
        /*
        if (count > 0 && !decal_added)
        {
            if (particlearray[0].pos.Y < 0)
            {
                core::vector3df pos = particlearray[0].pos;
                pos.Y = 0;        
                bloodSplatter(pos, core::vector3df(0,1,0), 5, 2.0);
                decal_added = true;
            }
        }
        */
        if (now < When) return;
        // specified time has been reached
        // clear the particle emitter so no new particles are generated
        ParticleSystem->setEmitter(0);
        // if the particle count every goes to zero, delete the particle system
        if (!count) SceneManager->addToDeletionQueue(ParticleSystem);
    }
    virtual scene::E_PARTICLE_AFFECTOR_TYPE getType() const
    {
        return scene::EPAT_NONE;
    }
private:
    scene::IParticleSystemSceneNode *ParticleSystem;
    scene::ISceneManager *SceneManager;
    u32 When; // time that we clear the particle system emitter
};

class Effects
{
public:
    Effects(IrrlichtDevice *);
    scene::IParticleSystemSceneNode *particleSpray(
        video::ITexture *,
        core::vector3df, core::vector3df, u16, u32, u32,
        f32 = 0.4f
        );
protected:
    scene::ISceneManager *smgr;
    video::IVideoDriver *driver;
    scene::ISceneNode *sn;  // scene node to attach any effects to (parent)
private:
    IrrlichtDevice *device;
};

#endif
