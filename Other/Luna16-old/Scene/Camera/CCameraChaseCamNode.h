#ifndef CCAMERACHASECAMNODE_H
#define CCAMERACHASECAMNODE_H

#include <irrlicht.h>
#include "CCameraSceneNode.h"

namespace irr
{
        namespace scene
        {
            class CCameraChaseCamNode : public CCameraSceneNode
            {
                public:
                    CCameraChaseCamNode(ISceneNode* parent, ISceneManager* mgr, s32 id, f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f, f32 translationSpeed = 100.0f);

                    ~CCameraChaseCamNode();

                    bool OnEvent(const SEvent &event);

                    void setTarget(const core::vector3df &newtarget);

                    void setRotation(const core::vector3df &rotation);

                    virtual void OnAnimate(u32 timeMs);

                    bool isButtonPressed(const s32 &id);

                    void toggleInput() { inputEnabled = !inputEnabled; };

                    void setCamDistance(const f32 &newDistance);
                private:
                    bool inputEnabled;

                    bool invertView;
                    bool invertX;
                    bool invertY;

                    f32 cameraDistance;

                    f32 yaw;
                    f32 currentDegree;

                    core::vector3df rotation;
                    core::vector3df position;
            };
        }
}

#endif
