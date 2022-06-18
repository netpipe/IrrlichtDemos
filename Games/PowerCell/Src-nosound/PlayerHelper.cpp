#include "PlayerHelper.hpp"

PlayerHelper *PlayerHelper::m_playerhelper = NULL;
PlayerHelper PlayerHelper::m_default_playerhelper_buffer;

void PlayerHelper::SetDefaultValues()
{
  health = 100;
  maxHealth = 100;
}

PlayerHelper *PlayerHelper::Instance()
{
  if(!m_playerhelper)
    m_playerhelper = &m_default_playerhelper_buffer;

  return m_playerhelper;
}

void PlayerHelper::SetPowerCellInfo(u32 a, u32 t)
{
  powerCellsAquired = a;
  powerCellsTotal = t;
}

void PlayerHelper::IncreaseAquiredPowerCells(u32 num)
{
  if(powerCellsAquired < powerCellsTotal)
    powerCellsAquired += num;

  if(powerCellsAquired == powerCellsTotal)
  {
    timeTotal = LocalTimer::Instance()->GetTime();
    if(Player::Instance()->getIsDead())
    {
      Sound::Instance()->PlayBackgroundMusic(MUSIC_DEAD);
    }
    else
    {
      Highscore::Instance()->CheckPlayerHighScore(timeTotal, powerCellsAquired);
      Sound::Instance()->PlayBackgroundMusic(MUSIC_CLEAR);
    }
    Scene::Instance()->EndGame();
    Scene::Instance()->GameOver(true); // mainly for music ?
    Scene::Instance()->SetState(STATE_GAME_OVER);
  }
}

u32 PlayerHelper::GetPowerCellsAquired(void)
{
  return powerCellsAquired;
}

u32 PlayerHelper::GetPowerCellsTotal(void)
{
  return powerCellsTotal;
}

void PlayerHelper::DecreaseHealth(f32 amount)
{
  health -= amount;
  if(health <= 0)
  {
    health = 0;
    Player::Instance()->setIsDead(true);
    GameGUI::Instance()->updateHealthInfo(health);
    timeTotal = LocalTimer::Instance()->GetTime();
 //   Highscore::Instance()->CheckPlayerHighScore(timeTotal, powerCellsAquired);
    if(Player::Instance()->getIsDead())
    {
//      Sound::Instance()->PlayBackgroundMusic(MUSIC_DEAD);
    }
    else
    {
//      Sound::Instance()->PlayBackgroundMusic(MUSIC_CLEAR);
    }
    Scene::Instance()->EndGame();
    Scene::Instance()->GameOver(true);
    Scene::Instance()->SetState(STATE_GAME_OVER);
  }
}

void PlayerHelper::IncreaseHealth(f32 amount)
{
  health += amount;
  if(health>maxHealth)
    health = maxHealth;
}

void PlayerHelper::IncreaseMaxHealth(f32 amount)
{
  maxHealth += amount;
  health = maxHealth;
}

u32 PlayerHelper::GetHealth(void)
{
  return health;
}

void PlayerHelper::ShowPickupEffect(int EFFECT)
{
  switch(EFFECT)
  {
    case EFFECT_POWERCELL_PICKUP:
    {
      //! Trail
      p = irrDevice->getSceneManager()->addParticleSystemSceneNode(
              false, Player::Instance()->getPlayerNode(), -1);

      p->setParticleSize(core::dimension2d<f32>(4.0f, 4.0f));

      em = p->createBoxEmitter(
              core::aabbox3d<f32>(-1,-1,-1,1,1,1),
              core::vector3df(0,1.0,0.0)*0.025,
              100,200,
              video::SColor(0, 13, 30, 53),
              video::SColor(0, 64, 155, 255),
              100,
              2000);

      p->setEmitter(em);
      em->drop();

      paf = p->createFadeOutParticleAffector(video::SColor(0,0,0,0), 2000);
      p->addAffector(paf);
      paf->drop();

      p->setMaterialFlag(video::EMF_LIGHTING, false);
      p->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("FXTextures/Dee_FX_64x64_24.tga"));
      p->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

      //! Animator
      anim = irrDevice->getSceneManager()->createFlyCircleAnimator(vector3df(0,0,0), 20, 0.01f);
      p->addAnimator(anim);
      anim->drop();

      anim = irrDevice->getSceneManager()->createDeleteAnimator(2000);
      p->addAnimator(anim);
      anim->drop();
    } break;
    case EFFECT_HEALTHCELL_PICKUP:
    {
      //! Trail
      p = irrDevice->getSceneManager()->addParticleSystemSceneNode(
              false, Player::Instance()->getPlayerNode(), -1);

      p->setParticleSize(core::dimension2d<f32>(4.0f, 4.0f));

      em = p->createBoxEmitter(
              core::aabbox3d<f32>(-1,-1,-1,1,1,1),
              core::vector3df(0,1.0,0.0)*0.025,
              100,200,
              video::SColor(0, 53, 30, 13),
              video::SColor(0, 255, 155, 64),
              100,
              2000);

      p->setEmitter(em);
      em->drop();

      paf = p->createFadeOutParticleAffector(video::SColor(0,0,0,0), 2000);
      p->addAffector(paf);
      paf->drop();

      p->setMaterialFlag(video::EMF_LIGHTING, false);
      p->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("FXTextures/Dee_FX_64x64_24.tga"));
      p->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);//EMT_TRANSPARENT_VERTEX_ALPHA

      //! Animator
      anim = irrDevice->getSceneManager()->createFlyCircleAnimator(vector3df(0,0,0), 20, 0.01f);
      p->addAnimator(anim);
      anim->drop();

      anim = irrDevice->getSceneManager()->createDeleteAnimator(2000);
      p->addAnimator(anim);
      anim->drop();
    } break;
  }
}
