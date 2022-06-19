#include "PowerCell.hpp"

PowerCell *PowerCell::m_powercell = NULL;
PowerCell PowerCell::m_default_powercell_buffer;

PowerCell *PowerCell::Instance()
{
  if(!m_powercell)
    m_powercell = &m_default_powercell_buffer;

  return m_powercell;
}

void PowerCell::Add(vector3df position)
{
  ISceneNode *sphere = irrDevice->getSceneManager()->addSphereSceneNode(10, 16, NULL, -1, position);
  sphere->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("StageData/cell_power.tga"));
  sphere->setMaterialTexture(1, irrDevice->getVideoDriver()->getTexture("StageData/glass.tga"));

  sphere->setMaterialFlag(video::EMF_LIGHTING, false);
  sphere->getMaterial(0).Shininess = 0.050f;
  sphere->getMaterial(0).DiffuseColor = SColor(255,0,220,250);
  sphere->getMaterial(0).EmissiveColor = SColor(255,0,220,250);
  sphere->getMaterial(0).AmbientColor = SColor(255,0,220,250);
  sphere->getMaterial(0).SpecularColor = SColor(255,0,220,250);
  sphere->getMaterial(0).MaterialType = EMT_SPHERE_MAP;//video::EMT_SPHERE_MAP;//EMT_TRANSPARENT_REFLECTION_2_LAYER;
  sphere->getMaterial(0).MaterialTypeParam = 0.01;
///		node->setMaterialFlag(video::EMF_LIGHTING, false);
		sphere->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
  u32 particleLifeTimeMax = 800;
  u32 particleMin = 100;
  u32 particleMax = 200;

  //! Trail

  IParticleSystemSceneNode *p = irrDevice->getSceneManager()->addParticleSystemSceneNode(
          false, sphere, -1, vector3df(0,0,0));

  p->setParticleSize(core::dimension2d<f32>(8.0f, 12.0f));

  scene::IParticleEmitter* em = p->createBoxEmitter(
          core::aabbox3d<f32>(-1,-1,-1,1,1,1),
          core::vector3df(0,0.0,0.0)*0.025,
          particleMin,particleMax,
          video::SColor(0, 64, 155, 255),
          video::SColor(0, 13, 30, 53),
          100,
          particleLifeTimeMax);

  p->setEmitter(em);
  em->drop();

  scene::IParticleAffector* paf = p->createFadeOutParticleAffector(video::SColor(0,0,0,0), particleLifeTimeMax);
  p->addAffector(paf);
  paf->drop();
  p->setMaterialFlag(video::EMF_LIGHTING, false);
  p->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
  p->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("FXTextures/Dee_FX_64x64_24.tga"));
  p->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR); //vertex_alpha

  //! Animator
  ISceneNodeAnimator *anim = irrDevice->getSceneManager()->createFlyCircleAnimator(vector3df(0,0,0), 20, 0.005f);
  p->addAnimator(anim);
  anim->drop();

  powerCellCount++;
  powerCellArray.push_back(sphere);
  particleNodeArray.push_back(p);
}

void PowerCell::CheckCollisionWithPlayer(void)
{
  playerPos = Player::Instance()->getPlayerNode()->getPosition();
  for(u32 i = 0; i < powerCellArray.size(); i++)
  {
    if(powerCellArray[i]->getPosition().getDistanceFrom(playerPos) < 50.0f)
    {
      //  Sound::Instance()->PlayCellPickupSample(powerCellArray[i]->getPosition());

 //     Output::Instance()->w("Player is colliding with power cell %d\n", i);

      powerCellArray[i]->remove();
      //particleNodeArray[i]->drop();
      powerCellArray.erase(i);

      PlayerHelper::Instance()->IncreaseHealth(5);
      PlayerHelper::Instance()->IncreaseAquiredPowerCells(1);
      PlayerHelper::Instance()->ShowPickupEffect(EFFECT_POWERCELL_PICKUP);
      GameGUI::Instance()->updatePowerCellInfo(
        PlayerHelper::Instance()->GetPowerCellsAquired(),
        PlayerHelper::Instance()->GetPowerCellsTotal());
    }
  }
}

s32 PowerCell::GetPowerCellCount(void)
{
  return powerCellCount;
}

void PowerCell::Clear(void)
{
  Output::Instance()->w("Clearing power cells.\n");

  if(powerCellArray.size() > 0)
  {
    for(u32 i = 0; i < powerCellArray.size()-1; i++)
    {
      powerCellArray[i]->remove();
      powerCellArray.erase(i);
    }
    powerCellArray.clear();
  }

  if(particleNodeArray.size() > 0)
  {
    for(u32 i = 0; i < particleNodeArray.size()-1; i++)
    {
      //particleNodeArray[i]->drop();
      particleNodeArray.erase(i);
    }
    particleNodeArray.clear();
  }
}
