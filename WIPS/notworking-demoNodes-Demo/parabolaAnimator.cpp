/*

I'm not actually sure if it is a parabola Razz But this is some simple code used to make "gib" meshes fly off from an explosion, and then disappear when they hit a ground plane.

Author : xDan

  Sample Usage
    for (int i = 0; i < gibMeshCount; i ++)
    {
        scene::IAnimatedMeshSceneNode *n = smgr->addAnimatedMeshSceneNode(gibMeshes[i]);

        core::vector3df rvel;
        rvel.X = rand()%100 * ((rand()%2) ? -1 : 1);
        rvel.Y = rand()%100 * ((rand()%2) ? -1 : 1);
        rvel.Z = rand()%100 * ((rand()%2) ? -1 : 1);

        scene::ISceneNodeAnimator *an = new ParabolaAnimator(
            smgr,
            device->getTimer()->getTime(),
            gibPos, // initial position
            core::vector3df(rand()%200-100,100.0,rand()%200-100) / 50.0, // initial velocity
            core::vector3df(0,-5.0,0), // acceleration
            rvel, // angular velocity
            core::plane3df(0,-1.0,0, 0,1,0) // ground plane
            );
        n->addAnimator(an);
        an->drop();
    }

*/

#include "parabolaAnimator.h"

ParabolaAnimator::ParabolaAnimator(
    scene::ISceneManager *smgr,
    u32 timeMs,
    const core::vector3df &start,
    const core::vector3df &vel,
    const core::vector3df &acc,
    const core::vector3df &rvel,
    const core::plane3df &groundPlane)
{
    this->smgr = smgr;
    this->lastTime = timeMs;
    this->pos = start;
    this->vel = vel;
    this->acc = acc;
    this->rvel = rvel;
    this->ground = groundPlane;
}

void ParabolaAnimator::animateNode(ISceneNode *node, u32 timeMs)
{
   if (!node) return;

   f32 dt = ((f32)(timeMs-lastTime)) / 1000.0;
   lastTime = timeMs;

   pos += vel*dt;
   vel += acc*dt;

   node->setPosition(pos);

   node->setRotation(node->getRotation() + rvel*dt);

   if (ground.classifyPointRelation(pos) == core::ISREL3D_FRONT)
   {
      //  smgr->addToDeletionQueue(node);
      node->drop();
    }
}

