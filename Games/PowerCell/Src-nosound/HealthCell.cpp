#include "HealthCell.hpp"

HealthCell *HealthCell::m_healthcell = NULL;
HealthCell HealthCell::m_default_healthcell_buffer;

HealthCell *HealthCell::Instance()
{
  if(!m_healthcell)
    m_healthcell = &m_default_healthcell_buffer;

  return m_healthcell;
}

void HealthCell::Add(vector3df position)
{
  ISceneNode *sphere = irrDevice->getSceneManager()->addSphereSceneNode(10, 16, NULL, -1, position);
  sphere->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("StageData/cell_heart.tga"));
  sphere->setMaterialTexture(1, irrDevice->getVideoDriver()->getTexture("StageData/glass.tga"));

  sphere->setMaterialFlag(video::EMF_LIGHTING, false);
  sphere->getMaterial(0).Shininess = 100.0f;
  sphere->getMaterial(0).DiffuseColor = SColor(255,250,128,0);
  sphere->getMaterial(0).EmissiveColor = SColor(255,250,128,0);
  sphere->getMaterial(0).AmbientColor = SColor(255,250,128,0);
  sphere->getMaterial(0).SpecularColor = SColor(255,250,128,0);
  sphere->getMaterial(0).MaterialType = EMT_TRANSPARENT_REFLECTION_2_LAYER;
  sphere->getMaterial(0).MaterialTypeParam = 0.01;

  u32 particleLifeTimeMax = 800;
  u32 particleMin = 100;
  u32 particleMax = 200;

  //! Trail

  IParticleSystemSceneNode *p = irrDevice->getSceneManager()->addParticleSystemSceneNode(
          false, sphere, -1, vector3df(0,0,0));

  p->setParticleSize(core::dimension2d<f32>(8.0f, 12.0f));

  scene::IParticleEmitter* em = p->createBoxEmitter(
          core::aabbox3d<f32>(-1,-1,-1,1,1,1),
          core::vector3df(0.2,0.2,0.2)*0.025,
          particleMin,particleMax,
          video::SColor(0, 53, 30, 13),
          video::SColor(0, 255, 155, 64),
          100,
          particleLifeTimeMax,
          180);

  p->setEmitter(em);
  em->drop();

  scene::IParticleAffector* paf = p->createFadeOutParticleAffector(video::SColor(0,0,0,0), particleLifeTimeMax);
  p->addAffector(paf);
  paf->drop();

  p->setMaterialFlag(video::EMF_LIGHTING, false);
  p->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("FXTextures/Dee_FX_64x64_24.tga"));
  p->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

  //! Animator
  ISceneNodeAnimator *anim = irrDevice->getSceneManager()->createFlyCircleAnimator(vector3df(0,0,0), 20, 0.005f, vector3df(1,1,1));
  p->addAnimator(anim);
  anim->drop();

  healthCellCount++;
  healthCellArray.push_back(sphere);
  particleNodeArray.push_back(p);
}

void HealthCell::CheckCollisionWithPlayer(void)
{
  playerPos = Player::Instance()->getPlayerNode()->getPosition();
  for(u32 i = 0; i < healthCellArray.size(); i++)
  {
    if(healthCellArray[i]->getPosition().getDistanceFrom(playerPos) < 50.0f)
    {
      //Sound::PlaySound("sound30.mp3");

      healthCellArray[i]->remove();
      healthCellArray.erase(i);

      PlayerHelper::Instance()->IncreaseMaxHealth(25);
      PlayerHelper::Instance()->ShowPickupEffect(EFFECT_HEALTHCELL_PICKUP);
      messageHpPlus = irrDevice->getSceneManager()->addTextSceneNode(
        GameGUI::Instance()->GetFont(), L"Max Power +25", SColor(200,255,255,255), Player::Instance()->getPlayerNode());
      ISceneNodeAnimator *anim = irrDevice->getSceneManager()->createDeleteAnimator(3000);
      messageHpPlus->addAnimator(anim);
      anim->drop();
    }
  }
}

s32 HealthCell::GetTotalHealthCells(void)
{
  return healthCellCount;
}

void HealthCell::Clear(void)
{
  Output::Instance()->w("Clearing health cells.\n");

  if(healthCellArray.size() > 0)
  {
    for(u32 i = 0; i < healthCellArray.size()-1; i++)
    {
      healthCellArray[i]->remove();
      healthCellArray.erase(i);
    }
    healthCellArray.clear();
  }

  if(particleNodeArray.size() > 0)
  {
    for(u32 i = 0; i < particleNodeArray.size()-1; i++)
    {
      particleNodeArray[i]->drop();
      particleNodeArray.erase(i);
    }
    particleNodeArray.clear();
  }
}
