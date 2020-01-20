#ifndef CLASS_DELETE_PARTICLE_AFFECTOR_H_INC
#define CLASS_DELETE_PARTICLE_AFFECTOR_H_INC

#include <Irrlicht.h>

class CDeleteParticleAffector : public irr::scene::IParticleAffector
{
private :

  irr::scene::IParticleSystemSceneNode *ParticleSystem;
  irr::u32                             time;
  bool                                 finished;
  bool                                 firstTime;

public :

  CDeleteParticleAffector(irr::scene::IParticleSystemSceneNode* pParticleSystem, irr::u32 timeMs) : ParticleSystem(pParticleSystem), finished(false), firstTime(true)
  {
    time = timeMs;
    ParticleSystem->addAffector(this);
    this->drop();
  }

  void affect(irr::u32 now, irr::scene::SParticle* particlearray, irr::u32 count)
  {
    if (firstTime)  { time += now; firstTime = false; }
    if (now < time) { return; }
    ParticleSystem->setEmitter(0);
    if (!count)     { finished = true; }
  }

  irr::scene::E_PARTICLE_AFFECTOR_TYPE getType()     const { return irr::scene::EPAT_NONE; }
  bool                                 hasFinished() const { return finished; }
};

#endif