#ifndef _HealthCell_HPP_
#define _HealthCell_HPP_

#include "Globals.hpp"

class HealthCell
{
  private:

    irr::core::array<ISceneNode*> healthCellArray;
    irr::core::array<IParticleSystemSceneNode*> particleNodeArray;
    s32 healthCellCount;
    vector3df playerPos;
    ISceneNode *messageHpPlus;

    IrrlichtDevice *irrDevice;

  public:

    static HealthCell *Instance();

    void Add(vector3df position);
    void CheckCollisionWithPlayer(void);
    s32 GetTotalHealthCells(void);
    void Clear(void);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static HealthCell *m_healthcell;
    static HealthCell m_default_healthcell_buffer;

};

#endif
