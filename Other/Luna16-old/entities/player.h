#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <irrlicht.h>
#include "../Equipment/Rifle.h"

using namespace irr;
using namespace core;

class Player
{
	public:

		Player();
		~Player();

		void initialize(void);
		void registerIrrlichtDevice(IrrlichtDevice &device);

		void setParent(ISceneNode *parent);

		std::string name;
		std::string guildName;

		unsigned int level;
		unsigned int health;

		bool duelling;
		bool attackable;

		bool inParty;
		bool inGuild;

		vector3df pos;


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


	private:

		IrrlichtDevice *m_irrDevice;

		Rifle *m_cWeapon;

    triangle3df tri;
    vector3df hitPos;
    vector3df playerRot;
    vector3df facing;
    vector3df newPos;
    const irr::scene::ISceneNode* outNode;
    //const irr::scene::ISceneNode* nodeHit;

    IAnimatedMeshSceneNode* playerNode;
    IAnimatedMeshMD2 *md2Node;

    IrrlichtDevice *irrDevice;
    IMetaTriangleSelector *metaSelector;
};

#endif
