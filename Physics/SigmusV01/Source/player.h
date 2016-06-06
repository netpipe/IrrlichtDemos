#ifndef _PLAYER_H
#define _PLAYER_H

#include "globals.h"
#include "main.h"
#include "map.h"
#include "physics.h"
#include "HUD.h"
#include "effectsmanager.h"

#include "BulletDynamics/Character/btKinematicCharacterController.h"

enum CCMovement
{
    CM_NONE = 0,
    CM_LEFT = 1,
    CM_RIGHT = 2,
    CM_FORWARD = 3,
    CM_BACKWARD = 4,
    CM_JUMP = 5
};

enum CCSpeed
{
    CS_CROUCH = 4,
    CS_NORMAL = 7,
    CS_RUN = 9
};

enum CCHeight
{
    CH_NORMAL = 0,
    CH_CROUCH = 1
};

class CCharacterController
{
    public:
    CCharacterController(CPhysics* Physics, ICameraSceneNode* Camera, ITimer* Timer, ISceneManager* smgr);
    void Update(bool Left, bool Right, bool Forwards, bool Backwards, bool Jump);

    void setSpeed(int Speed);
    void setHeight(int Height);
    int getSpeed();
    int getHeight();

    private:
    ITimer* Timer;
    CPhysics* Physics;
    ISceneManager* smgr;
    ICameraSceneNode* Camera;

    btPairCachingGhostObject* Ghost;
    btKinematicCharacterController* Character;
    btRigidBody* JumpBody;

    bool isJumping;
    float jumpStart;
    int JumpSpeed;

    int Speed;
    int Height;
};

class CPlayer
{
    public:
    CPlayer(vector3df Position, vector3df Target, float MovSpeed, float RotSpeed, float JumpSpeed, ISceneManager* smgr, IVideoDriver* driver, IrrlichtDevice* device, ITimer* timer, CMap* Map, CEventReceiver* receiver, CEffectsManager* EffectsManager);
    ICameraSceneNode* getCamera();
    IAnimatedMeshSceneNode* getWeapon();
    CEventReceiver* getReceiver();
    void addAmmo(int Type, int Value);
    void subAmmo(int Type, int Value);
    void setAmmo(int Type, int Value);
    void setHealth(int Health);
    int getHealth();
    int getCurrentWeapon();
    bool isActive();
    void setActive(bool Active);
    void setPhysics(CPhysics* Physics);
    void setPlayerHUD(CHUD* HUD);
    void setPlayerWeapon(int type);
    void setupPlayerPhysics();
    void Update();

    void remove();

    int CurrentWeapon;
    int Ammo[3];

    bool BloomActive;
    bool InvertActive;

    struct SProjectiles
	{
	    btGhostObject* ShotChecker;
	    int type;
		ISceneNode* Node;
		btRigidBody* rigidBody;
		vector3df oldPos;
		vector3df newPos;
		bool used;
		float Tick;
		bool Kill;
	};

	list<SProjectiles *> Projectiles;

    bool isDead;
    bool FinishedLevel;

    private:
    CCharacterController* CController;
    int Active;
    int FiringMode;
    int FarValue;
    vector3df StartPos;
    vector3df StartTar;
    ICameraSceneNode* Camera;
    float RotSpeed;
    float MovSpeed;
    float JumpSpeed;
    float shotForce;
    int Health;
    bool ChangedFireMode;
    bool shoot;
    bool shoot2;
    int checkEffect;
    CEventReceiver* receiver;
    IrrlichtDevice* device;

	void FireProjectile();
	void RemoveProjectile(CPlayer::SProjectiles* Projectiles);
	void CheckProjectilesStart(CPlayer::SProjectiles* Projectiles);
	void CheckProjectilesEnd(CPlayer::SProjectiles* Projectiles);
    IAnimatedMeshSceneNode* Weapon;
    IMeshSceneNode* AimNear;
    IMeshSceneNode* AimFar;

    ITimer* timer;
    IVideoDriver* driver;
    ISceneManager* smgr;
    CPhysics* Physics;
    CHUD* HUD;
    CEffectsManager* EffectsManager;
};

////
////
////
////


#endif
