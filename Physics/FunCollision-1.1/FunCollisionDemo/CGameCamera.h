// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#ifndef CGAMECAMERA_H_INCLUDED
#define CGAMECAMERA_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "FunCollision.h"
using namespace COLL;

namespace GAME
{

//! forward decl.
class CGame;
class CGameInterface;


//! CGameCamera class
class CGameCamera
{
    public:
        //! constructor
        CGameCamera(CGame* Game);

        //! update
        void update()
        {
            updateMovement();
            updateAssets();
            updateInput();
        }

        //! drawDebugData
        void drawDebugData();

    private:
        //! loadAssest
        void loadAssets();

        //! updateMovement
        void updateMovement();

        //! updateAssets
        void updateAssets();

        //! updateInput
        void updateInput();

        // instances
        CGame *game;
        CGameInterface* iface;
        IrrlichtDevice* device;
        scene::ISceneManager* smgr;
        video::IVideoDriver* driver;

        // camera
        scene::ICameraSceneNode* cam;
        core::vector3df camPos, camRot;

        // collision related
        ICollManager *cmgr;
        ICollSphereSampler* sphereSampler;
        ICollEllipsoidSampler* ellipsoidSampler, *ellipsoidSampler2;
        ICollEntity *ent, *sphereEnt, *ellipsoidEnt;
        SCollContact rayContact;

        // scene nodes
        scene::ISceneNode* sphereNode, *ellipsoidNode;
        scene::IMeshSceneNode *jumppad0, *jumppad1;
        scene::ISceneNode *textNode[10];
        scene::IBillboardSceneNode* aimNode, *portalA, *portalB;

        // vars:
        int curSampler;
        int runSpeedMulti;
        bool canUsePortals;
        bool enableRayCasting;
        int gravityStrength;
        bool enableDrawDebug;
        bool enableShowInfo;
        bool enableBouncing;

}; // end class CGameCamera

} // end namespace GAME

#endif // CGAMECAMERA_H_INCLUDED
