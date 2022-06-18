#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Globals.hpp"

class Player
{

  private:

    triangle3df tri;
    vector3df hitPos;
    vector3df playerRot;
    vector3df facing;
    vector3df newPos;

//    const irr::scene::ISceneNode nodeHit;

    IAnimatedMeshSceneNode* playerNode;
    IAnimatedMeshMD2 *md2Node;

    IrrlichtDevice *irrDevice;
    IMetaTriangleSelector *metaSelector;

  public:

    bool  isFalling,
          isRunning,
          isDead;

    float moveSpeed;
    static Player *Instance();

    void SetDefaultValues(void);
    void setIsRunning(bool run);
    bool getIsRunning(void);
    void moveForward();
    void strafeLeft();
    void strafeRight();
    void moveBackwards();
    void jump();
    void CheckFalling(void);
    void createShadow(void);
    bool getIsDead(void);
    void setIsDead(bool val);
        void setSpeed(float val);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

    __inline void setPlayerNode(IAnimatedMeshSceneNode *node)
    {
      playerNode = node;
    }

    __inline IAnimatedMeshSceneNode *getPlayerNode(void)
    {
      return playerNode;
    }

  protected:

    static Player *m_player;
    static Player m_default_player_buffer;

};

#endif
