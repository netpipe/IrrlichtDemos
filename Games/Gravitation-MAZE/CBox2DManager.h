#ifndef __CBOX2DMANAGER
  #define __CBOX2DMANAGER

#include <irrlicht.h>
#include "CBox2DImageSceneNode.h"

class CBox2DManager {
  private:

    CBox2DManager();
    ~CBox2DManager();
  public:
    static CBox2DManager *getSharedInstance();

    void addBox2DStaticSceneNode(CBox2DSceneNode *pNode);
};

#endif
