#include "CBloodEffect.h"
#include "CBloodShader.h"
#include "CDeleteParticleAffector.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;

CBloodEffect::CBloodEffect( irr::scene::ISceneManager* smgr,
                            E_GORE_LEVEL               goreLevel,
                            irr::core::vector3df       sprayFrom,
                            irr::core::vector3df       sprayDirection,
                            irr::u32                   sprayDurationMs,
                            irr::f32                   spraySpeed,
                            irr::f32                   gravity,
                            irr::scene::ISceneNode*    parent,
                            irr::s32                   id
                          ) : ISceneNode(parent, smgr, id)
{
if (!Parent)
  setParent(SceneManager->getRootSceneNode());

IVideoDriver *driver = SceneManager->getVideoDriver();

s32 matBlood = CBloodShader::instance().getMaterial();

driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

vector3df    direction = sprayDirection.normalize() * spraySpeed;
u32          minParticles;
u32          maxParticles;
u32          minLife;
u32          maxLife;
s32          maxAngle;
dimension2df minSize;
dimension2df maxSize;

switch (goreLevel)
{
  case EGL_MILD :
  {
    minParticles = 20;
    maxParticles = 30;
    minLife      = 550;
    maxLife      = 600;
    maxAngle     = 5;
    minSize      = dimension2df(2.0f, 2.0f);
    maxSize      = dimension2df(2.5f, 2.5f);
  } break;

  case EGL_MEDIUM :
  {
    minParticles = 45;
    maxParticles = 50;
    minLife      = 650;
    maxLife      = 700;
    maxAngle     = 7;
    minSize      = dimension2df(3.0f, 3.0f);
    maxSize      = dimension2df(3.5f, 3.5f);
  } break;

  case EGL_BRUTAL :
  {
    minParticles = 100;
    maxParticles = 150;
    minLife      = 750;
    maxLife      = 750;
    maxAngle     = 10;
    minSize      = dimension2df(4.0f, 4.0f);
    maxSize      = dimension2df(4.5f, 4.5f);
  } break;

  case EGL_INSANE :
  {
    minParticles = 500;
    maxParticles = 550;
    minLife      = 900;
    maxLife      = 900;
    maxAngle     = 12;
    minSize      = dimension2df(5.5f, 5.5f);
    maxSize      = dimension2df(5.5f, 5.5f);
  } break;
}

IParticleSystemSceneNode* ps1;
ps1 = SceneManager->addParticleSystemSceneNode(false, parent, -1, sprayFrom);
ps1->setMaterialType((E_MATERIAL_TYPE) matBlood );
ps1->setMaterialTexture(0, driver->getTexture("blood1.png"));
ps1->setMaterialFlag(EMF_LIGHTING,      false);
ps1->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
ps1->setMaterialFlag(EMF_ZBUFFER,       true);

IParticleSystemSceneNode* ps2;
ps2 = SceneManager->addParticleSystemSceneNode(false, parent, -1, sprayFrom);
ps2->setMaterialType((E_MATERIAL_TYPE) matBlood );
ps2->setMaterialTexture(0, driver->getTexture("blood2.png"));
ps2->setMaterialFlag(EMF_LIGHTING,      false);
ps2->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
ps2->setMaterialFlag(EMF_ZBUFFER,       true);

IParticleSystemSceneNode* ps3;
ps3 = SceneManager->addParticleSystemSceneNode(false, parent, -1, sprayFrom);
ps3->setMaterialType((E_MATERIAL_TYPE) matBlood );
ps3->setMaterialTexture(0, driver->getTexture("blood3.png"));
ps3->setMaterialFlag(EMF_LIGHTING,      false);
ps3->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
ps3->setMaterialFlag(EMF_ZBUFFER,       true);

IParticleSystemSceneNode* ps4;
ps4 = SceneManager->addParticleSystemSceneNode(false, parent, -1, sprayFrom);
ps4->setMaterialType((E_MATERIAL_TYPE) matBlood );
ps4->setMaterialTexture(0, driver->getTexture("blood4.png"));
ps4->setMaterialFlag(EMF_LIGHTING,      false);
ps4->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
ps4->setMaterialFlag(EMF_ZBUFFER,       true);

IParticleSystemSceneNode* ps5;
ps5 = SceneManager->addParticleSystemSceneNode(false, parent, -1, sprayFrom);
ps5->setMaterialType((E_MATERIAL_TYPE) matBlood );
ps5->setMaterialTexture(0, driver->getTexture("blood5.png"));
ps5->setMaterialFlag(EMF_LIGHTING,      false);
ps5->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
ps5->setMaterialFlag(EMF_ZBUFFER,       true);

particleSystems.push_back(ps1);
particleSystems.push_back(ps2);
particleSystems.push_back(ps3);
particleSystems.push_back(ps4);
particleSystems.push_back(ps5);

IParticleEmitter* em1 = ps1->createPointEmitter(
0.9f * direction, minParticles, maxParticles, SColor(255,255,255,255), SColor(255,255,255,255),
minLife, maxLife, maxAngle, minSize, maxSize);

IParticleEmitter* em2 = ps2->createPointEmitter(
1.05f * direction, minParticles, maxParticles, SColor(255,255,255,255), SColor(255,255,255,255),
minLife, maxLife, maxAngle, minSize, maxSize);

IParticleEmitter* em3 = ps3->createPointEmitter(
0.95f * direction, minParticles, maxParticles, SColor(255,255,255,255), SColor(255,255,255,255),
minLife, maxLife, maxAngle, minSize, maxSize);

IParticleEmitter* em4 = ps4->createPointEmitter(
1.0f * direction, minParticles, maxParticles, SColor(255,255,255,255), SColor(255,255,255,255),
minLife, maxLife, maxAngle, minSize, maxSize);

IParticleEmitter* em5 = ps5->createPointEmitter(
1.1f * direction, minParticles, maxParticles, SColor(255,255,255,255), SColor(255,255,255,255),
minLife, maxLife, maxAngle, minSize, maxSize);

ps1->setEmitter(em1); em1->drop();
ps2->setEmitter(em2); em2->drop();
ps3->setEmitter(em3); em3->drop();
ps4->setEmitter(em4); em4->drop();
ps5->setEmitter(em5); em5->drop();

IParticleAffector* pa1  = ps1->createScaleParticleAffector(minSize * -1.0f);                                          ps1->addAffector(pa1);  pa1->drop();
IParticleAffector* pa11 = ps1->createGravityAffector(vector3df(0.0f, -gravity, 0.0f), sprayDurationMs + 2 * maxLife); ps1->addAffector(pa11); pa11->drop();

IParticleAffector* pa2  = ps2->createScaleParticleAffector(minSize * -1.0f);                                          ps2->addAffector(pa2);  pa2->drop();
IParticleAffector* pa22 = ps2->createGravityAffector(vector3df(0.0f, -gravity, 0.0f), sprayDurationMs + 2 * maxLife); ps2->addAffector(pa22); pa22->drop();

IParticleAffector* pa3  = ps3->createScaleParticleAffector(minSize * -1.0f);                                          ps3->addAffector(pa3);  pa3->drop();
IParticleAffector* pa33 = ps3->createGravityAffector(vector3df(0.0f, -gravity, 0.0f), sprayDurationMs + 2 * maxLife); ps3->addAffector(pa33); pa33->drop();

IParticleAffector* pa4  = ps4->createScaleParticleAffector(minSize * -1.0f);                                          ps4->addAffector(pa4);  pa4->drop();
IParticleAffector* pa44 = ps4->createGravityAffector(vector3df(0.0f, -gravity, 0.0f), sprayDurationMs + 2 * maxLife); ps4->addAffector(pa44); pa44->drop();

IParticleAffector* pa5  = ps5->createScaleParticleAffector(minSize * -1.0f);                                          ps5->addAffector(pa5);  pa5->drop();
IParticleAffector* pa55 = ps5->createGravityAffector(vector3df(0.0f, -gravity, 0.0f), sprayDurationMs + 2 * maxLife); ps5->addAffector(pa55); pa55->drop();

deleteAffectors.push_back( new CDeleteParticleAffector(ps1, sprayDurationMs) );
deleteAffectors.push_back( new CDeleteParticleAffector(ps2, sprayDurationMs) );
deleteAffectors.push_back( new CDeleteParticleAffector(ps3, sprayDurationMs) );
deleteAffectors.push_back( new CDeleteParticleAffector(ps4, sprayDurationMs) );
deleteAffectors.push_back( new CDeleteParticleAffector(ps5, sprayDurationMs) );

driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

this->drop();
}

CBloodEffect::~CBloodEffect()
{
  for (u32 i = 0; i < particleSystems.size(); ++i)
  {
    particleSystems[i]->removeAllAffectors();
    particleSystems[i]->remove();
  }

  particleSystems.clear();
  deleteAffectors.clear();
}

const irr::core::aabbox3df& CBloodEffect::getBoundingBox() const
{
  return particleSystems[0]->getBoundingBox();
}

void CBloodEffect::OnRegisterSceneNode()
{
  u32 ready = 0;

  for (u32 i = 0; i < deleteAffectors.size(); ++i)
  {
    if (deleteAffectors[i]->hasFinished())
    {
      ++ready;
    }
  }

  if (ready == deleteAffectors.size())
  {
    SceneManager->addToDeletionQueue(this);
  }
}