#ifndef _PowerCell_HPP_
#define _PowerCell_HPP_

#include "Globals.hpp"

class PowerCell
{
  private:

    irr::core::array<ISceneNode*> powerCellArray;
    irr::core::array<IParticleSystemSceneNode*> particleNodeArray;
    s32 powerCellCount;
    vector3df playerPos;

    IrrlichtDevice *irrDevice;

  public:

    static PowerCell *Instance();

    void Add(vector3df position);
    void CheckCollisionWithPlayer(void);
    s32 GetPowerCellCount(void);
    void Clear(void);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static PowerCell *m_powercell;
    static PowerCell m_default_powercell_buffer;

};

#endif
