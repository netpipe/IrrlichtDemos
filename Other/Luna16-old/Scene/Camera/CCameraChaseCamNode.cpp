#include "CCameraChaseCamNode.h"

#ifndef PIOVER180
#define PIOVER180 0.0174532925f
#endif

namespace irr
{
	namespace scene
	{
        CCameraChaseCamNode::CCameraChaseCamNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				f32 rotateSpeed, f32 zoomSpeed, f32 translationSpeed)
		: CCameraSceneNode(parent, mgr, id)
		{
            cameraDistance = 100.0f;
            rotation.X = rotation.Y = rotation.Z = 0.0f;
            position.X = position.Y = position.Z = 0.0f;
		}

        CCameraChaseCamNode::~CCameraChaseCamNode()
        {

        }

        bool CCameraChaseCamNode::OnEvent(const SEvent &event)
		{

			if (event.EventType == EET_MOUSE_INPUT_EVENT)
			{
				switch (event.MouseInput.Event)
				{
				    default:
                        break;
				}
				return true;
			}
			else if (event.EventType == EET_KEY_INPUT_EVENT)
			{
				if (event.KeyInput.PressedDown)
				{
					switch (event.KeyInput.Key)
					{
					    default:
                            break;
					}
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}

		void CCameraChaseCamNode::OnAnimate(u32 ms)
		{
            position.X = Target.X + cameraDistance;
            position.Y = Target.Y;
            position.Z = Target.Z;

            position.rotateXYBy(rotation.Y, Target);
            position.rotateXZBy(rotation.X, Target);
            position.rotateYZBy(rotation.Z, Target);

            ICameraSceneNode::setPosition(position);
            updateAbsolutePosition();
		}

		bool CCameraChaseCamNode::isButtonPressed(const s32 &id)
		{
            return false;
		}

		void CCameraChaseCamNode::setTarget(const core::vector3df &newtarget)
		{
			Target = newtarget;
		}

		void CCameraChaseCamNode::setRotation(const core::vector3df &rot)
		{
            rotation = rot;
		}

		void CCameraChaseCamNode::setCamDistance(const f32 &newdistance)
		{
            cameraDistance = newdistance;
		}
	}
}
