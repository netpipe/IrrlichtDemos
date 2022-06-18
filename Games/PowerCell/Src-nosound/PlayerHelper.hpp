#ifndef _PlayerHelper_HPP_
#define _PlayerHelper_HPP_

#include "Globals.hpp"

class PlayerHelper
{
  private:

    IParticleSystemSceneNode* p;
    IParticleEmitter* em;
    IParticleAffector* paf;
    ISceneNodeAnimator* anim;

    u32 powerCellsAquired;
    u32 powerCellsTotal;
    u32 timeTotal;
    f32 health;
    f32 maxHealth;

    IrrlichtDevice *irrDevice;

  public:

    static PlayerHelper *Instance();

    void SetDefaultValues(void);
    void SetPowerCellInfo(u32 a, u32 t);
    void IncreaseAquiredPowerCells(u32 num);
    u32 GetPowerCellsAquired(void);
    u32 GetPowerCellsTotal(void);
    void DecreaseHealth(f32 amount);
    void IncreaseHealth(f32 amount);
    void IncreaseMaxHealth(f32 amount);
    u32 GetHealth(void);
    void ShowPickupEffect(int EFFECT);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static PlayerHelper *m_playerhelper;
    static PlayerHelper m_default_playerhelper_buffer;

};

#endif
