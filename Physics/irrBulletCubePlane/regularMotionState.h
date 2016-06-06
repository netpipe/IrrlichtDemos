#ifndef __regularMotionState__
#define __regularMotionState__
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
using namespace irr;
class regularMotionState : public  btMotionState
{
public:
	regularMotionState(const btTransform& initalpos, scene::ISceneNode* node);
	//virtual ~regularMotionState();
	void setWorldTransform(const btTransform &worldTrans);
	void getWorldTransform(btTransform &worldTrans) const;

	bool m_placed;
	btTransform m_pos;
	scene::ISceneNode* m_node;

};

#endif