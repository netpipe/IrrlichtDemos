#ifndef __MOTION_STATE_H_INCLUDED__
#define __MOTION_STATE_H_INCLUDED__

#include <iostream>
#include "common.h"


using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;


class IMotionState : public btDefaultMotionState
{
    public:
        IMotionState(const btTransform &initialPos);
        virtual ~IMotionState();

        //////////////////////////
        // The 'dos' and 'sets' //
        //////////////////////////
        virtual void setWorldTransform(const btTransform &worldTrans);

        void setNode(ISceneNode* const n) {node = n;};

        void applyTransformToVisualModel(const btTransform &transform);


        ////////////////
        // The 'gets' //
        ////////////////
        virtual void getWorldTransform(btTransform &worldTrans) const;

    private:
        ISceneNode *node;
        ITimer *timer;
        bool failed;

        btTransform worldTransform;
};

#endif // __MOTION_STATE_H_INCLUDED__

