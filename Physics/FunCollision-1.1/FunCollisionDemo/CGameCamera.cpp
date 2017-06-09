// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#include <irrlicht.h>
using namespace irr;

#include "CGameCamera.h"
#include "CGame.h"
#include "CGameInterface.h"

#include "FunCollision.h"
using namespace COLL;


namespace GAME
{

//! constructor
CGameCamera::CGameCamera(CGame* Game)
{
    // instances
    game = Game;
    device = game->getDevice();
    smgr = game->getSmgr();
    driver = game->getDriver();
	cmgr = game->getCmgr();
    iface = game->getIFace();

    // create camera scene node:
    cam = smgr->addCameraSceneNode();
	smgr->setActiveCamera(cam);
	cam->bindTargetAndRotation(true);
	camPos = cam->getAbsolutePosition();
	camRot = cam->getRotation();

    // Init our variables:
    curSampler = 0;
    runSpeedMulti = 0;
    enableRayCasting = true;
    gravityStrength = 2;
    enableDrawDebug = false;
    enableShowInfo = true;
    enableBouncing = false;
    canUsePortals = true;

    // Load the assets:
    loadAssets();
}

//! loadAssest
void CGameCamera::loadAssets()
{
    for (int i = 0; i < 10; ++i) textNode[i] = NULL;

	// ------ Load up the level scene node:
    scene::IAnimatedMesh* levelMesh = smgr->getMesh("../media/level/testlab.x");
    if (levelMesh)
    {
        scene::ISceneNode *levelNode = smgr->addOctTreeSceneNode(levelMesh->getMesh(0), 0, -1, 512);
        levelNode->setMaterialFlag(video::EMF_LIGHTING, false);
        levelNode->setMaterialType(video::EMT_LIGHTMAP_M4);
        levelNode->setScale(core::vector3df(8));
        /* Now add an octree selector to the manager for the level geometry, simplify it
           if it's possible and finally add the level collision-object itself. As you can see,
           the level object will be reading it's geomerty data from the selector, and it's
           binded to the scene node, so it'll be exactly where you've put your level scene node. */
        ICollOctreeSelector* levelSelector = cmgr->addOctreeSelector(levelMesh->getMesh(0));
        levelSelector->simplify(); // simplify it a little bit if possible
        levelSelector->build(128); // don't forget to call build()
        cmgr->addObject(levelSelector, levelNode);
    }


	/**/ // ------ Create portals:
	core::array<video::ITexture*> textures;
	for (s32 g = 1; g < 8; ++g)
	{
		core::stringc tmp("../media/objects/portal");
		tmp += g;
		tmp += ".bmp";
		video::ITexture* t = driver->getTexture( tmp );
		if (t) textures.push_back(t);
	}
	scene::ISceneNodeAnimator* anim = NULL;
	if (textures.size() > 0) anim = smgr->createTextureAnimator(textures, 100);

    // Add a billboard scene node for portal A
	portalA = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(100, 100), core::vector3df(130, 180, -990));
    portalA->setMaterialFlag(video::EMF_LIGHTING, false);
    portalA->setMaterialTexture(0, driver->getTexture("../media/objects/portal1.bmp"));
    portalA->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    if (anim) portalA->addAnimator(anim);
    // And add a text scene node to it
    video::SColor textColor(255, 255, 255, 0);
    textNode[0] = smgr->addTextSceneNode(game->getIFace()->getFont(),
        core::stringw("portal A object (demonstrates teleportation)").c_str(), textColor, portalA);

    // Add a billboard scene node for portal B
	portalB = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(100, 100), core::vector3df(-1260, 320, -1280));
    portalB->setMaterialFlag(video::EMF_LIGHTING, false);
    portalB->setMaterialTexture(0, driver->getTexture("../media/objects/portal1.bmp"));
    portalB->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    if (anim) portalB->addAnimator(anim);
	if (anim) anim->drop();
    // And add a text scene node
    textNode[1] = smgr->addTextSceneNode(game->getIFace()->getFont(),
        core::stringw("portal B object").c_str(), textColor, portalB);


	// ----- Load jumppad 0:
    scene::IAnimatedMesh* jumppadMesh = smgr->getMesh("../media/objects/jumppad.obj");
    jumppad0 = jumppad1 = NULL;
    if (jumppadMesh)
    {
        jumppad0 = smgr->addMeshSceneNode(jumppadMesh->getMesh(0));
        jumppad0->setPosition(core::vector3df(640, -30, 70));
        jumppad0->setScale(core::vector3df(40));
        jumppad0->setMaterialTexture(0, driver->getTexture("../media/objects/jumppad.bmp"));
        jumppad0->setMaterialFlag(video::EMF_LIGHTING, false);
        // Add a bounding box selector for the jumppads
        ICollBBoxSelector* jumppadSelector = cmgr->addBBoxSelector(jumppadMesh);
        jumppadSelector->build(); // you also have to call build() for bbox selectors
        // And finally add the jumppad object
        cmgr->addObject(jumppadSelector, jumppad0);

        textNode[2] = smgr->addTextSceneNode(game->getIFace()->getFont(),
            core::stringw("jumppad 0 object (shows what's an impulse)").c_str(), textColor, jumppad0);

        // ----- Add jumppad 1:
        // Like as we re-use the jumppad mesh here to create an other jumppad scene node, ...
        jumppad1 = smgr->addMeshSceneNode(jumppadMesh->getMesh(0));
        jumppad1->setPosition(core::vector3df(1150, -690, 1570));
        jumppad1->setScale(core::vector3df(40));
        jumppad1->setMaterialTexture(0, driver->getTexture("../media/objects/jumppad.bmp"));
        jumppad1->setMaterialFlag(video::EMF_LIGHTING, false);
        // ... we'll do the same with the jumppad selector: use the same as before.
        cmgr->addObject(jumppadSelector, jumppad1);

        textNode[3] = smgr->addTextSceneNode(game->getIFace()->getFont(),
            core::stringw("jumppad 1 object").c_str(), textColor, jumppad1);
    }

	// ----- Add terrain scene node:
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("../media/objects/terrain-heightmap.bmp", 0, -1,
		core::vector3df(-4000, -700, -4000), core::vector3df(0.f, 0.f, 0.f),	core::vector3df(30, 4, 30),
		video::SColor ( 255, 255, 255, 255 ), 3, scene::ETPS_9, 4);

    if (terrain)
    {
        terrain->setMaterialFlag(video::EMF_LIGHTING, false);
        terrain->setMaterialTexture(0, driver->getTexture("../media/objects/terrain-texture.jpg"));

        // And add a terrain selector to the manager
        ICollTerrainSelector* terrainSelector = cmgr->addTerrainSelector();
        // In most cases, you won't need the finest detail (LOD 0); a little bit less detailed
        // terrain will do the trick quicker, the artifacts are barely noticable...
        terrainSelector->addFromTerrainNode(terrain, 2);
        terrainSelector->build(); // terrains also need to be built
        // And add the terrain object itself
        cmgr->addObject(terrainSelector, terrain);
    }


	// ------ Add bomb scene node:
    scene::IAnimatedMesh* bombMesh = smgr->getMesh("../media/objects/bomb.obj");
    if (bombMesh)
    {
        scene::IMeshSceneNode* bombNode = smgr->addMeshSceneNode(bombMesh->getMesh(0));
        bombNode->setPosition(core::vector3df(-180,-150,-500));
        bombNode->setScale(core::vector3df(30));
        bombNode->setRotation(core::vector3df(30,60,80));
        bombNode->setMaterialTexture(0, driver->getTexture("../media/objects/bombsd.png"));
        bombNode->setMaterialFlag(video::EMF_LIGHTING, true);
        bombNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        // Add a box selector to store it, and add the collision object
        ICollBBoxSelector* bombSelector = cmgr->addBBoxSelector(bombMesh->getMesh(0));
        bombSelector->build(); // !!!
        cmgr->addObject(bombSelector, bombNode);

        textNode[4] = smgr->addTextSceneNode(game->getIFace()->getFont(),
            core::stringw("bomb object (bbox selector)").c_str(), textColor, bombNode);

        // Add another bomb, but this one will use a simple selector:
        scene::IMeshSceneNode* bombNode2 = smgr->addMeshSceneNode(bombMesh->getMesh(0));
        bombNode2->setPosition(core::vector3df(180,-120,-500));
        bombNode2->setScale(core::vector3df(50));
        bombNode2->setRotation(core::vector3df(80,60,120));
        bombNode2->setMaterialTexture(0, driver->getTexture("../media/objects/bombsd.png"));
        bombNode2->setMaterialFlag(video::EMF_LIGHTING, true);
        bombNode2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        // Simple selectors don't need to be built
        ICollSelector* bombSelector2 = cmgr->addSelector(bombMesh->getMesh(0));
        cmgr->addObject(bombSelector2, bombNode2);

        textNode[5] = smgr->addTextSceneNode(game->getIFace()->getFont(),
            core::stringw("bomb2 object (simple selector)").c_str(), textColor, bombNode2);
    }


	// ----- Now let's add the dwarves:
    scene::IAnimatedMesh* dwarfMesh = smgr->getMesh("../media/objects/dwarf.x");
    if (dwarfMesh)
    {
        scene::IMeshSceneNode* dwarfNode = smgr->addMeshSceneNode(dwarfMesh->getMesh(0));
        // This one is relocated, rotated, and scaled
        dwarfNode->setPosition(core::vector3df(0,-200,-500));
        dwarfNode->setScale(core::vector3df(5,4,6));
        dwarfNode->setRotation(core::vector3df(0,160,0));
        dwarfNode->setMaterialFlag(video::EMF_LIGHTING, true);
        dwarfNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        /* We're gonna set up a simple selector for this (larger) dwarf, which is exactly
           the >> worst << choice, because the dwarf model has about 1500 polygons, so it
           definately will be slow. However, currently we don't have any better alternatives. */
        ICollSelector* dwarfSelector = cmgr->addSelector(dwarfMesh->getMesh(0));
        cmgr->addObject(dwarfSelector, dwarfNode);

        textNode[6] = smgr->addTextSceneNode(game->getIFace()->getFont(),
            core::stringw("dwarf object (moved, rotated, scaled)").c_str(), textColor, dwarfNode);

        // Add another (smaller) dwarf, which has it's original rotation and size:
        scene::IMeshSceneNode* dwarfNode2 = smgr->addMeshSceneNode(dwarfMesh->getMesh(0));
        dwarfNode2->setPosition(core::vector3df(-700, -110, -190));
        dwarfNode2->setMaterialFlag(video::EMF_LIGHTING, true);
        dwarfNode2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        // Let's say it's small enough to put it into a box selector:
        ICollBBoxSelector* dwarfSelector2 = cmgr->addBBoxSelector();
        dwarfSelector2->addMesh(dwarfMesh->getMesh(0));
        dwarfSelector2->build();
        cmgr->addObject(dwarfSelector2, dwarfNode2);

        textNode[7] = smgr->addTextSceneNode(game->getIFace()->getFont(),
            core::stringw("dwarf2 object (bbox selector)").c_str(), textColor, dwarfNode2);
    }

    // ----- Add the player entity, which we bind to the camera:
    // Add to samplers because we want an option to change shapes
    sphereSampler = cmgr->addSphereSampler(core::vector3df(40), core::vector3df(), 45, 60, 0.8f, 0.0f);
    ellipsoidSampler = cmgr->addEllipsoidSampler(core::vector3df(25, 50, 25), core::vector3df(0, 30, 0), 45, 60, 0.8f, 0.0f);
    ent = cmgr->addEntity(core::vector3df(0, 0, 40), sphereSampler, cam);


    // ----- Let's add some scene nodes for our fellow entities:
    // The sphere
    sphereNode = smgr->addSphereSceneNode(1, 20, 0, 0);
    sphereNode->setScale(sphereSampler->getScale());
    sphereNode->setMaterialTexture(0, driver->getTexture("../media/objects/wall.bmp"));
    sphereNode->setMaterialFlag(video::EMF_LIGHTING, true);
    sphereNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    // It does not have an own sampler, but uses one of the players' sampler.
    sphereEnt = cmgr->addEntity(core::vector3df(-100, 0, 80), sphereSampler, sphereNode);

    textNode[8] = smgr->addTextSceneNode(game->getIFace()->getFont(),
        core::stringw("sphere entity (reused player's sampler)").c_str(), textColor, sphereNode);

    // And the ellipsoid
    // First add the sampler (so that we'll know how to scale the ellipsoid node):
    ellipsoidSampler2 = cmgr->addEllipsoidSampler(core::vector3df(30, 60, 30), core::vector3df(), 45, 60, 0.2f, 0.0f);
    ellipsoidNode = smgr->addSphereSceneNode(1, 20, 0, 0);
    ellipsoidNode->setScale(ellipsoidSampler2->getScale());
    ellipsoidNode->setMaterialTexture(0, driver->getTexture("../media/objects/wall.bmp"));
    ellipsoidNode->setMaterialFlag(video::EMF_LIGHTING, true);
    ellipsoidNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// Then, add the object to the manager:
    ellipsoidEnt = cmgr->addEntity(core::vector3df(-100, 0, 0), ellipsoidSampler2, ellipsoidNode);

    textNode[9] = smgr->addTextSceneNode(game->getIFace()->getFont(),
        core::stringw("ellipsoid entity (heavier than you)").c_str(), textColor, ellipsoidNode);
    /**/

	// ------ Add a light to the scene
	smgr->addLightSceneNode(0, core::vector3df(-1000, 1000, -1000), video::SColor(255, 255, 255, 255), 3000);


    // ----- Finally, create a sprite for aiming (like what the Predator has :))
    aimNode = smgr->addBillboardSceneNode(0, core::dimension2df(15, 15));
    aimNode->setMaterialTexture(0, driver->getTexture("../media/gui/aim.png"));
    aimNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    aimNode->setMaterialFlag(video::EMF_LIGHTING, false);
    aimNode->setMaterialFlag(video::EMF_ZBUFFER, false);
    aimNode->setVisible(false);
}

//! updateMovement
void CGameCamera::updateMovement()
{
    // ----- Camera variables:
    float runSpeed = 0.3f + 1.0f * float(runSpeedMulti);
    float jumpStrength = 10.0f;
    float mouseSpeed = 0.1f;
    camPos = cam->getAbsolutePosition();

    // ----- Camera rotation:
    s32 rmx = 0, rmy = 0;
    game->getInput()->getRelMousePos(rmx, rmy);
    camRot.Y -= rmx * mouseSpeed;
    camRot.X += rmy * mouseSpeed;
    // Trim camera rotation values, not to flip over
    camRot.Y = (camRot.Y < 0) ? camRot.Y + 360 : camRot.Y;
    camRot.Y = (camRot.Y >= 360) ? camRot.Y - 360 : camRot.Y;
    camRot.X = (camRot.X < -89.9f) ? -89.9f : camRot.X;
    camRot.X = (camRot.X > 89.9f) ? 89.9f : camRot.X;
    // Set camera target
    core::vector3df camVect = core::vector3df(0, 0, 1);
    camVect.rotateYZBy(camRot.X, core::vector3df(0, 0, 0));
    camVect.rotateXZBy(camRot.Y, core::vector3df(0, 0, 0));
    cam->setTarget(camPos + camVect);
    // Update camera scene node
    cam->updateAbsolutePosition();

    // Define camera up, left and forward directions
    core::vector3df camUp(0, 1, 0);
    core::vector3df camLeft = core::vector3df(camVect.X, 0, camVect.Z).normalize();
    camLeft.rotateXZBy(90, core::vector3df(0, 0, 0));
    core::vector3df camForward = (gravityStrength) ? core::vector3df(camVect.X, 0, camVect.Z).normalize() : camVect;

    // ----- Camera movement:
    /* We're going to use forces and impulses to move our character in the world. Why? To move them
       framerate independently, and smoothly.

       There's a difference between force and impulse. Forces are continuous, while impulses happen
       in very brief moment.

       We're gonne use forces for running around, because if the game stops for a second, and the player
       were holding UP key (running forward), we suppose that the character should continue running forward. */
    if (game->getInput()->getKeyDown(irr::KEY_KEY_W) || game->getInput()->getKeyDown(irr::KEY_UP))      ent->addForce(camForward * runSpeed);
    if (game->getInput()->getKeyDown(irr::KEY_KEY_S) || game->getInput()->getKeyDown(irr::KEY_DOWN))    ent->addForce(-camForward * runSpeed);
    if (game->getInput()->getKeyDown(irr::KEY_KEY_D) || game->getInput()->getKeyDown(irr::KEY_RIGHT))   ent->addForce(-camLeft * runSpeed);
    if (game->getInput()->getKeyDown(irr::KEY_KEY_A) || game->getInput()->getKeyDown(irr::KEY_LEFT))    ent->addForce(camLeft * runSpeed);

    // For jumping, we need to check if: A) There is gravity at all; B) If the entity is not falling
    if (game->getInput()->getKeyDown(irr::KEY_SPACE) &&  gravityStrength && ent->getState() != ECES_FALLING)
    {
        // The power of a jump is a sudden impulse: once we've jumped up, we cannot push ourselves upper and upper.
        ent->addImpulse(camUp * jumpStrength);
        // Jumping is against the gravity, so we should ignore gravity here.
        ent->resetGravity();
    }
}

//! updateAssets
void CGameCamera::updateAssets()
{
    /**/

    // ----- Jumppads and teleporting:
    // Create a "sensor" bounding box for the entity, and extend it a little bit.
    // This will help us to know wheather we've touched a jumppad or a teleport.
    core::aabbox3df entSensorBB = ent->getBBoxWS();
    entSensorBB.MinEdge -= core::vector3df(10);
    entSensorBB.MaxEdge += core::vector3df(10);

    // Handle jumppads: (shows how to use impulses)
    core::vector3df jumppadSensor(0, 40, 0);
    // Check if we are near enough to a sensor of jumppad
    if (jumppad0 && entSensorBB.isPointInside(jumppad0->getAbsolutePosition() + jumppadSensor))
    {
        // Using a jumppad is similar to jumping: we'll use a sudden force (= impulse) and ignore gravity.
        ent->addImpulse(core::vector3df(0, 50, 0));
        ent->resetGravity();
    }
    if (jumppad1 && entSensorBB.isPointInside(jumppad1->getAbsolutePosition() + jumppadSensor))
    {
        ent->addImpulse(core::vector3df(0, 150, 0));
        ent->resetGravity();
    }

    // Handle teleporting: (demonstrates the difference between teleport() and spawn() functions)
    /* Using the teleport() function will keep the entity's actual movement, and changes it's position.
       And it won't keep the entity's previous position, so that it can be teleported through walls.

       The spawn() function is pretty much the same, but it sets all the entity's movement to zero, and
       then repositions it. It should be used when your game is started, or when a new round starts, etc. */
    bool inPortal = false;
    core::vector3df portalCenter(-50, 0, -50);
    portalA->setVisible(true);
    portalB->setVisible(true);
    // If we're teleported from a portal A into portal B, we're suddenly being in portal B, and then we
    // will be teleported back to portal A ... The following piece of code avoids this anomaly:
    if (entSensorBB.isPointInside(portalA->getAbsolutePosition()))
    {
        if (canUsePortals)
        {
            ent->teleport(portalB->getAbsolutePosition() + portalCenter + ent->getScale().Y);
            canUsePortals = false;
        }
        inPortal = true;
        portalA->setVisible(false);
    }
    if (entSensorBB.isPointInside(portalB->getAbsolutePosition()))
    {
        if (canUsePortals)
        {
            ent->teleport(portalA->getAbsolutePosition() + portalCenter + ent->getScale().Y);
            canUsePortals = false;
        }
        inPortal = true;
        portalB->setVisible(false);
    }
    if (!inPortal) canUsePortals = true;


    // ----- Show or hide the info text nodes we've made, if the player is close enough:
    for (int i = 0; i < 10; ++i) if (textNode[i]) textNode[i]->setVisible(
        camPos.getDistanceFrom(textNode[i]->getParent()->getAbsolutePosition()) < 300);


    // ----- Move fellow entites
    core::vector3df spherePos = sphereNode->getAbsolutePosition();
    float sphereSpeed = 0.2f;
    core::vector3df sphereVect;
    // They should be following you, if you're too far away
    if (camPos.getDistanceFrom(spherePos) > 180)
    {
        sphereVect = camPos - spherePos;
        if (gravityStrength) sphereVect.Y = 0;
        sphereVect.normalize();
        sphereVect *= sphereSpeed;
    }
    sphereEnt->addForce(sphereVect);

    core::vector3df ellipsoidPos = ellipsoidNode->getAbsolutePosition();
    float ellipsoidSpeed = 0.2f;
    core::vector3df ellipsoidVect;
    if (spherePos.getDistanceFrom(ellipsoidPos) > 160)
    {
        ellipsoidVect = spherePos - ellipsoidPos;
        if (gravityStrength) ellipsoidVect.Y = 0;
        ellipsoidVect.normalize();
        ellipsoidVect *= ellipsoidSpeed;
    }
    ellipsoidEnt->addForce(ellipsoidVect);
    /**/

    // ----- Deal with our aim node:
    // If ray casting is enabled and the ray hits something, except our character
    if (enableRayCasting && cmgr->rayIntersect(cam->getAbsolutePosition(),
        cam->getTarget() - cam->getAbsolutePosition(), rayContact, 3000, 20, ent))
    {
        // Make it visible and place it where the impact was
        aimNode->setVisible(true);
        aimNode->setPosition(rayContact.impact);
    }
    else aimNode->setVisible(false);
}

//! updateInput
void CGameCamera::updateInput()
{
    // ----- Deal with other input:
    if (game->getInput()->getKeyReleased(KEY_KEY_E)) ent->spawn(core::vector3df(0));
    if (game->getInput()->getKeyReleased(KEY_KEY_R)) enableRayCasting = 1 - enableRayCasting;
    if (game->getInput()->getKeyReleased(KEY_KEY_G))
    {
        gravityStrength = ++gravityStrength > 2 ? 0 : gravityStrength;
        switch(gravityStrength)
        {
            case 0: cmgr->setGravityStrength(0.0f); break;   // no gravity
            case 1: cmgr->setGravityStrength(0.001f); break; // weak gravity
            case 2: cmgr->setGravityStrength(0.01f); break;  // normal gravity
        }
    }
    if (game->getInput()->getKeyReleased(KEY_KEY_N)) enableDrawDebug = 1 - enableDrawDebug;
    if (game->getInput()->getKeyReleased(KEY_KEY_I)) enableShowInfo = 1 - enableShowInfo;
    if (game->getInput()->getKeyReleased(KEY_KEY_H) && ent)
    {
        /* By changing the sampler (say: the shape) of an entity, the spawn() command is
           automatically called. These changes are not being tracked by the collision system,
           so we're gonna cheat a little bit, see below. */
        curSampler = 1 - curSampler;
        switch (curSampler)
        {
            case 0: ent->setSampler(sphereSampler); break;
            case 1: ent->setSampler(ellipsoidSampler); break;
        }
        /* Since we've used getKeyReleased() here, which happens in at certain momentum,
           (not like when pressing a key, which takes more time) and we want to ensure
           that our player jumps up when changing shape (not to sink into the ground by
           the gravity; we're going to set WaitSync to false. */
        ent->addImpulse(core::vector3df(0, 10.0f, 0), false);
    }
    if (game->getInput()->getKeyReleased(KEY_KEY_Q)) runSpeedMulti = ++runSpeedMulti > 2 ? 0 : runSpeedMulti;
    if (game->getInput()->getKeyReleased(KEY_KEY_B))
    {
        // Bouncing values must range from 0.0f to 1.0f, where 0.0f is no bouncing,
        // 1.0f is reflecting the whole velocity our character has.
        enableBouncing = 1 - enableBouncing;
        if (sphereSampler) sphereSampler->setBounceFactor(enableBouncing ? 0.999f : 0.0f);
        if (ellipsoidSampler) ellipsoidSampler->setBounceFactor(enableBouncing ? 0.999f : 0.0f);
        if (ellipsoidSampler2) ellipsoidSampler2->setBounceFactor(enableBouncing ? 0.999f : 0.0f);
    }
}

//! drawDebugData
void CGameCamera::drawDebugData()
{
    // ----- Create the collision info:
    u32 simTime = 0;
    int numSelTris = 0, numIters = 0;
    u32 numContacts = 0;
    core::stringw info = L"";
    info += "Simulation time: ";
    cmgr->getDebugData(simTime);
    info += simTime;
    info += "\n\n[I] Show info: ";
    info += enableShowInfo ? "ON" : "OFF";
    if (enableShowInfo)
    {
        info += "\n\n[WASD] Move";
        info += "\n[Space] Jump";
        info += "\n[Escape] Quit";
        info += "\n[E] Respawn";
        info += "\n[N] Draw debug data: ";
        info += enableDrawDebug ? "ON" : "OFF";
        info += "\n[H] Collision shape: ";
        switch (curSampler)
        {
            case 0: info += "Sphere"; break;
            case 1: info += "Ellipsoid"; break;
        }
        info += "\n[G] Gravity: ";
        switch (gravityStrength)
        {
            case 0: info += "None"; break;
            case 1: info += "Weak"; break;
            case 2: info += "Normal"; break;
        }
        info += "\n[R] Ray casting: ";
        info += enableRayCasting ? "ON" : "OFF";
        info += "\n[Q] Run speed multi: x";
        info += runSpeedMulti;
        info += "\n[B] Bouncing: ";
        info += enableBouncing ? "ON" : "OFF";
        core::vector3df camPos = cam->getAbsolutePosition();
        info += "\n\nPosition X: "; info += camPos.X;
        info += "\nPosition Y: "; info += camPos.Y;
        info += "\nPosition Z: "; info += camPos.Z;
        ent->getDebugData(numSelTris, numIters);
        info += "\n\nSelected tris: ";
        info += numSelTris;
        info += "\nIterations: ";
        info += numIters;
        info += "\nContacts: ";
        numContacts = ent->getNumContacts();
        info += numContacts;
        info += "\nPicked tri ID: ";
        info += rayContact.tri.getID();
        info += "\n\nState: ";
        switch (ent->getState())
        {
            case ECES_FALLING: info += "FALLING"; break;
            case ECES_SLIDING: info += "SLIDING"; break;
            case ECES_STANDING: info += "STANDING"; break;
        }
        info += "\nSleeping: ";
        info += ent->getSleeping() ? "SLEEPING" : "AWAKE";
    }
    iface->setDebugInfo(info);

    // Draw debug data for the player entity
    if (enableDrawDebug && driver) ent->drawDebugData(driver);

    // And also draw the raycast result
    if (enableDrawDebug && aimNode->isVisible())
    {
        driver->draw3DTriangle(rayContact.tri.getTri(), video::SColor(255, 255, 255, 0));
        driver->draw3DLine(rayContact.impact, rayContact.impact + (rayContact.normal * 20), video::SColor(255, 0, 0, 255));

        core::aabbox3df selNodeBox;
        if (rayContact.object) selNodeBox = rayContact.object->getBBoxWS();
        if (rayContact.entity) selNodeBox = rayContact.entity->getBBoxWS();
        driver->draw3DBox(selNodeBox, video::SColor(255, 255, 255, 255));
    }
}

} // end namespace GAME
