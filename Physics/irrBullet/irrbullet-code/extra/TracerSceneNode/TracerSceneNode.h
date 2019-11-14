// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see copyright notice in irrbullet.h

#ifndef __TRACER_SCENE_NODE_H_INCLUDED__
#define __TRACER_SCENE_NODE_H_INCLUDED__

#include <ISceneNode.h>
#include <irrTypes.h>

namespace irr
{
    namespace scene
    {
        class ISceneManager;
    }
}

class IRigidBody;


class ITracerSceneNode : public irr::scene::ISceneNode
{
    public:
        ITracerSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id=-1);
        ~ITracerSceneNode();

        virtual void OnRegisterSceneNode();

        virtual void render();

        virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

        virtual irr::u32 getMaterialCount() const;

        virtual irr::video::SMaterial& getMaterial(irr::u32 i);

        void setLengthMultiplier(irr::f32 newMultiplier) { multiplier = newMultiplier; };

        void setRigidBody(IRigidBody* const rigidBody) { body = rigidBody; hasRigidBody=true; };

    private:
        irr::core::aabbox3d<irr::f32> Box;
        IRigidBody* body;
        irr::video::SMaterial Material;
        irr::scene::ISceneManager* smgr;
        bool hasRigidBody;
        irr::f32 multiplier;
};

#endif // __TRACER_SCENE_NODE_H_INCLUDED__
