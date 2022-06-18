#ifndef _WaterNode_H_
#define _WaterNode_H_

#include "../Globals.hpp"

class WaterNode
{
  private:

   // vector3df playerPos;
   // ISceneNode *messageHpPlus;

    IrrlichtDevice *irrDevice;

  public:

    static WaterNode *Instance();

 //   void Add(vector3df position);
 //   void CheckCollisionWithPlayer(void);
  //  s32 GetTotalHealthCells(void);
 //   void Clear(void);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:
     static WaterNode *m_WaterNode;
};

#endif
