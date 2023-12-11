#include "IOrthoControllerAnimator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

IOrthoControllerAnimator::IOrthoControllerAnimator(irr::scene::ISceneManager* smgrIn)
	: smgr(smgrIn), lastTime(0), justClicked(false)
{
};

bool IOrthoControllerAnimator::OnEvent(const irr::SEvent& event)
{
	if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			position2di clickScreenPos(event.MouseInput.X, event.MouseInput.Y);
			line3df screenRay = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(clickScreenPos, smgr->getActiveCamera());
			plane3df groundPlane = plane3df(0,0,0,0,1,0);
			groundPlane.getIntersectionWithLimitedLine(screenRay.start, screenRay.end, targetPosition);
			targetPosition.Y = 8.0f;
			justClicked = true;
			return true;
		}
	}

	return false;
}

void IOrthoControllerAnimator::animateNode(ISceneNode* node, u32 timeMs)
{
	vector3df offsetVector = targetPosition - node->getPosition();

	if(offsetVector.getLengthSQ() > 5.0f)
	{
		vector3df nodeRotation = (-offsetVector).getHorizontalAngle();

		// Ignore the X axis for rotation.
		nodeRotation.X = 0.0f;

		node->setRotation(nodeRotation);

		f32 deltaTime = core::min_<f32>(f32(timeMs - lastTime) / 5.0f, 1.5f);

		node->setPosition(node->getPosition() + offsetVector.normalize() * deltaTime);

		if(justClicked)
		{
			// These animation charasteristics are specific to the dwarf model.
			((IAnimatedMeshSceneNode*)node)->setLoopMode(true);
			((IAnimatedMeshSceneNode*)node)->setFrameLoop(2, 14);
			((IAnimatedMeshSceneNode*)node)->setAnimationSpeed(15.0f);
			justClicked = false;
		}
	}
	else
	{
		((IAnimatedMeshSceneNode*)node)->setLoopMode(false);
		((IAnimatedMeshSceneNode*)node)->setFrameLoop(0, 0);
	}

	lastTime = timeMs;
}
