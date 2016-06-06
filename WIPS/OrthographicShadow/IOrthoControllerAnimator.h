#include <irrlicht.h>

class IOrthoControllerAnimator
	: public irr::scene::ISceneNodeAnimator
{
public:
	IOrthoControllerAnimator(irr::scene::ISceneManager* smgrIn);
	virtual bool irr::IEventReceiver::OnEvent(const irr::SEvent& event);
	virtual void irr::scene::ISceneNodeAnimator::animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);
private:
	irr::core::vector3df targetPosition;
	irr::scene::ISceneManager* smgr;
	irr::u32 lastTime;
	bool justClicked;
};
