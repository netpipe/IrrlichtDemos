#ifndef scroller_h
#define scroller_h
// Wave based back view 3d scroller game
// code by G Davidson

#include "irrlicht.h"
#include <cstdlib>

using namespace irr;

void loadResources();
void dropResources();
void makeWaves();

// types of movement / collision
#define GM_FLYING   0
#define GM_ROLLING  1
#define GM_STATIC   2
#define GM_MOVEONLY 3

// ai types
#define AI_LAME    0   // static object
#define AI_TURRET  1   // turrets are static, but rotate and fire
#define AI_MISSILE 2   // missiles fly towards the target and explode
#define AI_TANK    3   // tanks roll along the floor, rotate and fire
#define AI_SHIP    4   // ships fly through the air, shoot ahead
#define AI_CAR     5   // just rolls along the floor on a set path


// modes that an enemy can be in
#define EM_OUT_OF_RANGE  0
#define EM_ACTIVE        1
#define EM_DYING         2
#define EM_DEAD          3

// full enemy list
#define EN_SCENERY            0
#define EN_NUKE               1
#define EN_NUKE_LAUNCHER      2
#define EN_GUN_TURRET         3
#define EN_         4

// meshes and textures
#define MAX_TEXTURES    32
#define MAX_MESHES      5

char *texturenames[] = {
            /*  0 */    "sfx0.bmp","sfx1.bmp","sfx2.bmp","sfx3.bmp","sfx4.bmp",
            /*  5 */    "sfx5.bmp","sfx6.bmp","sfx7.bmp","sfx8.bmp","sfx9.bmp",
            /* 10 */    "sfx10.bmp","playership.bmp","target.bmp", "gun.base.bmp","gun.dome.bmp",
            /* 15 */    "gun.gun.bmp","nuke.bmp","nuke launcher.bmp","tank.base.jpg","tank.head.jpg",
            /* 20 */    "tank.gun.jpg","balltype1.jpg","smoke.jpg","explosion1.bmp","explosion2.bmp",
            /* 25 */    "explosion3.bmp","explosion4.bmp","bullet1.bmp","bullet2.bmp","bullet3.bmp",
            /* 30 */    "bullet4.bmp","decal.bmp","","",""
                       };

char *meshnames[]    = {
            /*  0 */    "shieldsphere.x","playership.obj","gun.base.obj","gun.dome.obj","gun.gun.obj",
            /*  5 */    "nuke.obj","nuke launcher.md2","tank.base.obj","tank.head.obj","tank.gun.obj",
            /* 10 */    ""
            /* 15 */
                        };

int screenwidth      = 800;
int screenheight     = 600;

IrrlichtDevice              *device;
video::IVideoDriver         *driver;

core::array<video::ITexture*>      gametextures;
core::array<scene::IAnimatedMesh*> gamemeshes;

template<typename X, typename T> bool isA(T &obj) { return dynamic_cast<X *>(obj); }

core::position2d<f32> cpos,mousesensitivity;  // cursor position

struct SEnemy
{
    s32             enemytype;
    s32             enemydata;
    core::vector3df pos;
    core::vector3df rotation;
};

core::array< core::array<SEnemy> > waves;

#include "CParticleSystem2SceneNode.h"

// Textures and meshes

//       ______________________________
//     /-----------------------. ---- /|
//    /---     INPUT      - --. ---- / /
//   /- ------------------- -. ---- / /
//  /-- -- ----------- -- --. ---- / /
//  |______________________________|/

// bool keys for reading the keyboard
bool keys[irr::KEY_KEY_CODES_COUNT];
bool lmouse, rmouse, mmouse;

class InputReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
        // deal with keyboard
        if(event.EventType == irr::EET_KEY_INPUT_EVENT){
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            return true;
        }
        // deal with mouse

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
                 lmouse = true;
            else if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
                 lmouse = false;
            else if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
                 rmouse = true;
            else if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
                 rmouse = false;

        }

		return false;
	}
};


//
//  GAME NODE
//

class CGameSceneNode : public scene::ISceneNode
{

  public:

        core::aabbox3d<f32> BBox;

        // public variables that every game node has
        bool                 hasshield;             // does it have a shield?
        CGameSceneNode      *shield;                // pointer to the shield
        f32                  size;                  // size, sphere around the node
        f32                  scale;                 // size/scale is relative size

        bool                 hasweapon;             // does it have a weapon?
        CGameSceneNode      *weapon;                // pointer to the weapon

        bool                 hasenergy;             // can this be blown up?
        f32                  energy;                // how much energy it has
        f32                  maxenergy;             // how much energy it starts with
        scene::IBillboardSceneNode      *energybar; // energybar
        bool                 dead;                  // is this object dead?
        s32                  movement;              // for collision - GM_FLYING GM_ROLLING or GM_STATIC

        core::vector3df      velocity;

        // the code

        CGameSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id);
        virtual void OnPostRender(u32 t);
        virtual void OnPreRender();
		virtual void render();

		virtual const core::aabbox3d<f32>& getBoundingBox() const;

        // move the node depending on its velocity
        virtual void Move(u32 t);
        // hits the game node from a direction for a duration
		virtual f32 Hit(f32 strength, s32 speed, core::vector3df attackerpos);

		// causes smoke, damage etc
        virtual void Smoke(f32 strength, s32 speed, core::vector3df attackerpos);

		// kills the node
		virtual void Die(f32 strength, s32 speed, core::vector3df attackerpos);

		// destroys the node - (player node overrides it)
		virtual void Destroy(f32 strength, s32 speed, core::vector3df attackerpos);
};

//       _____
//     /      \
//    | /\ /\  |
//    | \/ \/  |     enemy nodes
//    |        |
//    \_/\_/\_/
//

class CEnemySceneNode : public CGameSceneNode
{
    public:
        CEnemySceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id, core::vector3df pos, s32 enemytype);

        virtual void OnPostRender(u32 t);

        virtual void Aim();

        virtual ~CEnemySceneNode();
        virtual void Destroy(f32 strength, s32 speed, core::vector3df attackerpos);

        s32  mode;     // if this enemy is active, etc
        s32  type;     // the type of enemy
        f32  accuracy; // how accurate this thing is
        s32  aiclass;  // the type of object that this is

        bool falling;  // if this has just dropped out the sky

        ISceneNode  *h_aim;   // horizontal turret part
        ISceneNode  *v_aim;   //   vertical turret part

        f32 maxrotation; // maximum rotation speed
        f32 speed;       // enemy speed

        u32 reloadspeed;
        u32 reloadtime;
        u32 nextfiretime;
        f32 range;       // they don't aim/shoot til they're in range (z)
};


//           _____
//    _____ | |   \
//   ====== |_|___/    Bullets
//

class CBulletSceneNode : public CGameSceneNode
{
    public:
        CBulletSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id,
                                           core::vector3df position, core::vector3df vel,
                                           s32 meshno, s32 spriteno, s32 spritecount, f32 impactenergy);

        virtual void makeBullet();

        bool              impact;       // will this hit the floor?
        core::vector3df   endpoint;     // the point it hits
        u32               endtime;      // when this gets deleted
        u32               lasttime;     // last render time
        CGameSceneNode   *parent;       // cant harm daddy node
        s32               spritenumber; // sprite no
        s32               sprites;      // sprite count
        virtual void OnPostRender(u32 t);
};


//
//  /\/\/\/\/\/\
// < Explosions >
//  \/\/\/\/\/\/
//

class CExplosionSceneNode : public CGameSceneNode
{
    public:
        CExplosionSceneNode( CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id,
                                           core::vector3df position, core::vector3df vel,
                                           s32 spriteno, s32 spritecount, f32 e, u32 lifespan);

        virtual scene::IParticleSystemSceneNode*  CreateExplosionParticles();

        u32               endtime;      // when this gets deleted
        u32               lasttime;     // last render time
        s32               spritenumber; // sprites!

        virtual void OnPostRender(u32 t);
};




//
//  /\/\/\       ______
//  \/\/\/\/\/\ |      \ Boosters
//       \/\/\/ |______/


class CBoosterSceneNode : public scene::ISceneNode
{
public:

	//! constructor

    CBoosterSceneNode(scene::ISceneNode *parent, scene::ISceneManager*   smgr,  s32 id,
                    scene::CParticleSystem2SceneNode *ps, CGameSceneNode *follows,
                    core::aabbox3d<f32> box,  core::vector3df direction, u32 minParticlesPerSecond,
                    u32 maxParticlePerSecond,	video::SColor minStartColor,
                    video::SColor maxStartColor, u32 lifeTimeMin, u32 lifeTimeMax,
                	s32 maxAngleDegrees);


	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual s32 emitt(u32 now, u32 timeSinceLastCall,  scene::SParticle2*& outArray);

    virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual void OnPostRender(u32 t);

	core::vector3df Direction;
	u32 MinParticlesPerSecond, MaxParticlesPerSecond;
	video::SColor MinStartColor, MaxStartColor;
	u32 MinLifeTime, MaxLifeTime;

private:

	core::aabbox3d<f32> Box;
	core::array<scene::SParticle2> Particles;
    u32 lastEmitTime;
	scene::CParticleSystem2SceneNode *ps;

	CGameSceneNode *gameobject;

	u32 Time;
	u32 Emitted;
	s32 MaxAngleDegrees;
};



//    ____
//   /    \
//  |      |   --- Shield
//   \____/

class CShieldSceneNode : public CGameSceneNode
 {

    private:
        core::array<video::ITexture*>    textures;
        scene::IAnimatedMesh            *mesh;
        f32                              rechargerate;
        f32                              size;   // scale
        scene::IParticleSystemSceneNode *ps;
        scene::ILightSceneNode          *light;
        video::SLight                   *lightdata;
        u32                              lightstarttime;
        u32                              emitterdeltime;
        u32                              lasttime;

    public:
        CShieldSceneNode(CGameSceneNode* parent,         scene::ISceneManager*   smgr,
                         s32                id,             f32                     startenergy,
                         f32                RechargeRate,   f32                     scale,
                         s32                imagestart,     s32                     imagecount, s32 sparkle);
        ~CShieldSceneNode();

        virtual void OnPostRender(u32 t);
        virtual void OnPreRender();

		virtual void render();

		virtual const core::aabbox3d<f32>& getBoundingBox()  const;

        // returns the remainder
        virtual f32 Hit(f32 strength, s32 speed, core::vector3df attackerpos);
};





//             ___       _
//   /\   _  /    \    /  \
//  /  \/  \/      \/\/    \
// /        Terrain         \

s32 terrainorder[] = { 0,1, 0,1, 2,0, 0,1, 0,1,
//                       0,1, 0,1, 0,1, 0,1, 0,1,
//                       0,1, 0,1, 0,1, 0,1, 0,1,
                       0,1, 0,1, 0,1, 0,1, 0,2 };

class CScrollingWorldSceneNode : public CGameSceneNode
{
    public:
        scene::ISceneNode        *ground[2];   // the two visible ground nodes
        scene::IAnimatedMesh     *mesh[3];     // the all the mesh data
        scene::ITriangleSelector *selector[2]; // triangle selectors
        video::ITexture          *texture[3];  // the texture data
        s16                       current;     // the current floor num
        f32                       join;        // the place where the map joins
        f32                       farpoint;    // the place where the map ends

        CScrollingWorldSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id);
        ~CScrollingWorldSceneNode();

        virtual void                      Move(u32 t);
        virtual void                      OnPostRender(u32 time);

        // returns the triangle selector associated with a Z value
        virtual scene::ITriangleSelector* getSelector(f32 Z);
        virtual core::vector3df           getHeightAtXZ(f32 X, f32 Z);

        virtual bool checkGround();
};







//     _
//   _/_\_
// (______)    -- Player ship
//

class CPlayerSceneNode : public CGameSceneNode
{
    private:
		scene::ISceneNode                 *ship;
		CBoosterSceneNode                 *jets[3];

    public:
        f32                     maxvel;
        f32                     maxdistx;
        f32                     maxdisty;
        u32                     lasttime;
        u32                     nextfiretime;
        f32                     playerspeed;
        core::vector3d<f32>     shiprot;

        CPlayerSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id);

        ~CPlayerSceneNode();
        virtual void Destroy(f32 strength, s32 speed, core::vector3df attackerpos);
        virtual void Smoke(f32 strength, s32 speed, core::vector3df attackerpos);
        virtual void OnPostRender(u32 time);
};




//
//      Weapons
//

struct SWeaponParticle {
    core::vector3d<f32> pos;        // the current position of the projectile
    core::vector3d<f32> velocity;   // speed and direction
    core::vector3d<f32> size;       // used in the render

    s16                 spriteno;   // the sprite number
    f32                 maxdist;    // the point where it dissapears
    f32                 travelled;  // how far it has travelled

    // internal
    f32                 magnitude;  // sacrifice ram for cpu in dist travelled calc
};


//  ______________   _
// |              | / |
// |              |K  |  --- Camera node
// |______________| \_|
//


class CGameCamera : public CGameSceneNode
{
    public:
        scene::ICameraSceneNode *cam;
        CGameCamera(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id);
        virtual void OnPostRender(u32 t);
        core::vector3df shake;
        u32             lasttime;
        u32             lastframe;
};

#endif
