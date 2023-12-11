#ifndef CCAMERA_MMO_SCENE_NODE_H
#define CCAMERA_MMO_SCENE_NODE_H

#include <irrlicht.h>
#include <CCameraSceneNode.h>

namespace irr
{
	namespace scene
	{
		class CCameraMMOSceneNode : public CCameraSceneNode
		{
			public:
				CCameraMMOSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
					f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f, f32 translationSpeed = 100.0f);

				~CCameraMMOSceneNode();

				bool OnEvent(const SEvent &event);

				void OnAnimate(u32 timeMs);

				void setTarget(const core::vector3df &newtarget);

				void setRotation(const core::vector3df &newpos);

				bool isButtonPressed(const s32 &id);

				void setSensitivity(const f32 &sen);
			private:
                void animate();
				void resetButtons();

				bool buttonPressed[5];
				bool keyPressed[5];

				bool rotating;

				core::vector3df Pos;
				core::vector3df Rot;

				f32 cameraDistance;

                f32 RotateStartX;
                f32 RotateStartY;
                f32 nRotX, RotX;
                f32 nRotY, RotY;
                f32 mouseSpeed;

                core::vector2df MousePos;
		};
	}
}

#endif

