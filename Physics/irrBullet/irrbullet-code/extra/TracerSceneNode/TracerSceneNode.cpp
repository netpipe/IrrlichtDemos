// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see copyright notice in irrbullet.h

#include "TracerSceneNode.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <RigidBody.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;


ITracerSceneNode::ITracerSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id)
{
    smgr = mgr;
    hasRigidBody = false;

    Material.Wireframe = false;
    Material.Lighting = false;

    Box.reset(getPosition());
    Box.MaxEdge = vector3df(1,1,1);
    Box.MinEdge = vector3df(-1,-1,-1);

    multiplier = 0.05f;
}

void ITracerSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void ITracerSceneNode::render()
{
    if(hasRigidBody)
    {
        IVideoDriver* driver = smgr->getVideoDriver();
        const vector3df& velocity = body->getLinearVelocity();
        const vector3df& pos = body->getWorldTransform().getTranslation();
        //printf("POS: %.2f, %.2f, %.2f\n", pos.X, pos.Y, pos.Z);

        driver->setMaterial(Material);
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);
        driver->draw3DLine(vector3df(0,0,0), velocity*multiplier, SColor(255, 255, 201, 14));
        //printf("DRAW");
    }
}

const aabbox3d<f32>& ITracerSceneNode::getBoundingBox() const
{
    return Box;
}

u32 ITracerSceneNode::getMaterialCount() const
{
    return 1;
}

SMaterial& ITracerSceneNode::getMaterial(u32 i)
{
    return Material;
}

ITracerSceneNode::~ITracerSceneNode()
{
    //dtor
}
