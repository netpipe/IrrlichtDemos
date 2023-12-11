#include "regularMotionState.h"

regularMotionState::regularMotionState(const btTransform &initialpos, scene::ISceneNode* node)
{
	m_pos = initialpos; //store the pos
	m_node = node;

}

//Bullet Calls this every stepsimulation (or at least when it thinks it needs to be called.
//An inactive object doesn't have this called
void regularMotionState::setWorldTransform(const btTransform& worldTrans) 
{

	using namespace core;
	//get the rotation
	btQuaternion rot = worldTrans.getRotation();
	quaternion quat(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
	vector3df irrRot;
	//convert a quaternion to a regular angle.
	quat.toEuler(irrRot);
	this->m_node->setRotation(vector3df(irrRot.X*180/PI, irrRot.Y*180/PI, irrRot.Z*180/PI)); //Convert into degrees and set the position
	btVector3 pos = worldTrans.getOrigin(); //same as above but with positions
	this->m_node->setPosition(vector3df(pos.x(), pos.y(), pos.z()));

	this->m_pos = worldTrans; //store a position
}

void regularMotionState::getWorldTransform(btTransform& worldTrans) const 
{
	worldTrans = this->m_pos; //get the position
}