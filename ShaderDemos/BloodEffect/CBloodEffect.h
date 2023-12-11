#ifndef CLASS_BLOOD_EFFECT_H_INC
#define CLASS_BLOOD_EFFECT_H_INC

#include <irrlicht.h>

class CDeleteParticleAffector;

enum E_GORE_LEVEL
{
  EGL_MILD,
  EGL_MEDIUM,
  EGL_BRUTAL,
  EGL_INSANE
};

class CBloodEffect : public irr::scene::ISceneNode
{
private :

  irr::core::array<irr::scene::IParticleSystemSceneNode*> particleSystems;
  irr::core::array<CDeleteParticleAffector*>              deleteAffectors;

public :

  CBloodEffect( irr::scene::ISceneManager* smgr,
                E_GORE_LEVEL               goreLevel       = EGL_MEDIUM,
                irr::core::vector3df       sprayFrom       = irr::core::vector3df(0, 0, 0),
                irr::core::vector3df       sprayDirection  = irr::core::vector3df(0, 0, 1.0f),
                irr::u32                   sprayDurationMs = 100,
                irr::f32                   spraySpeed      = 0.075f,
                irr::f32                   gravity         = 0.05f,
                irr::scene::ISceneNode*    parent          = 0,
                irr::s32                   id              = -1
              );

  ~CBloodEffect();

  const irr::core::aabbox3df& getBoundingBox() const;
  void OnRegisterSceneNode();
  void render() { }
};

#endif
