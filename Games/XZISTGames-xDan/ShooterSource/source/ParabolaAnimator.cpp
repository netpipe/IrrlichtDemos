
#include "ParabolaAnimator.h"

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
///        smgr->addToDeletionQueue(node);
    }
}

