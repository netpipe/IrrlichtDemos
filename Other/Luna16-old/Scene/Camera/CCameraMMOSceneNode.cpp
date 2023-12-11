#include "CCameraMMOSceneNode.h"

#ifndef PIOVER180
#define PIOVER180 0.0174532925f
#endif

namespace irr
{
	namespace scene
	{
		CCameraMMOSceneNode::CCameraMMOSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				f32 rotateSpeed, f32 zoomSpeed, f32 translationSpeed)
		: CCameraSceneNode(parent, mgr, id)
		{
			Target.set(0, 0, 0);
			cameraDistance = 100;
			mouseSpeed = 3.0f;

			resetButtons();
			recalculateViewArea();
		}

		CCameraMMOSceneNode::~CCameraMMOSceneNode()
		{

		}

		bool CCameraMMOSceneNode::OnEvent(const SEvent &event)
		{

			if (event.EventType == EET_MOUSE_INPUT_EVENT)
			{
				switch (event.MouseInput.Event)
				{
					case EMIE_LMOUSE_PRESSED_DOWN:
						buttonPressed[0] = true;
						break;
					case EMIE_RMOUSE_PRESSED_DOWN:
						buttonPressed[1] = true;
						break;
					case EMIE_MMOUSE_PRESSED_DOWN:
						buttonPressed[2] = true;
						break;
					case EMIE_LMOUSE_LEFT_UP:
						buttonPressed[0] = false;
						break;
					case EMIE_RMOUSE_LEFT_UP:
						buttonPressed[1] = false;
						break;
					case EMIE_MMOUSE_LEFT_UP:
						buttonPressed[1] = false;
						break;
					case EMIE_MOUSE_WHEEL:
                        {
                            cameraDistance = 100;
                        }
						break;
                    case EMIE_MOUSE_MOVED:
                        {
                            video::IVideoDriver* driver = SceneManager->getVideoDriver();
                            if (driver)
                            {
                                core::dimension2du ssize = SceneManager->getVideoDriver()->getScreenSize();
                                MousePos.X = event.MouseInput.X / (f32)ssize.Width;
                                MousePos.Y = event.MouseInput.Y / (f32)ssize.Height;
                            }
                        }
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

		void CCameraMMOSceneNode::OnAnimate(u32 timeMs)
		{
		    animate();

			ISceneNode::setPosition(Pos);
			updateAbsolutePosition();
		}

		bool CCameraMMOSceneNode::isButtonPressed(const s32 &id)
		{
			return buttonPressed[id];
		}

		void CCameraMMOSceneNode::setTarget(const core::vector3df &newtarget)
		{
			Target = newtarget;
		}

		void CCameraMMOSceneNode::setRotation(const core::vector3df &newRotation)
		{
            Rot = newRotation;
		}

		void CCameraMMOSceneNode::resetButtons()
		{
			for (unsigned short i = 0; i < 5; ++i)
				buttonPressed[i] = false;
		}

		void CCameraMMOSceneNode::setSensitivity(const f32 &sen)
		{
            mouseSpeed = sen;
            if (mouseSpeed < 1.0f)
                mouseSpeed = 1.0f;

            if (mouseSpeed > 9.0f)
                mouseSpeed = 9.0f;
		}

		void CCameraMMOSceneNode::animate()
		{
		    nRotX = RotX;
		    nRotY = RotY;
            if (buttonPressed[1])
            {
                if (!rotating)
                {
                    RotateStartX = MousePos.X;
                    RotateStartY = MousePos.Y;
                    rotating = true;
                    nRotX = RotX;
                    nRotY = RotY;
                }
                else
                {
                    nRotX += (RotateStartX - MousePos.X) * (1000.0f / (10.0f - mouseSpeed));
                    nRotY += (RotateStartY - MousePos.Y) * (1000.0f / (10.0f - mouseSpeed));
                }
            }
            else
            {
                if (rotating)
                {
                    RotX = RotX + (RotateStartX - MousePos.X) * (1000.0f / (10.0f - mouseSpeed));
                    RotY = RotY + (RotateStartY - MousePos.Y) * (1000.0f / (10.0f - mouseSpeed));
                    nRotX = RotY;
                    nRotY = RotY;
                }

                rotating = false;
            }

            Pos.X = Target.X + cameraDistance;
            Pos.Y = Target.Y;
            Pos.Z = Target.Z;

            Pos.rotateXYBy(nRotY, Target);
            Pos.rotateXZBy(-nRotX, Target);
		}
	}
}

