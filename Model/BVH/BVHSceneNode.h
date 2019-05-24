#ifndef BVH_SCENE_NODE_H
#define BVH_SCENE_NODE_H

#include <irrlicht.h>
#include <ISceneNode.h>
#include "bvh.h"
#include <vector>

using namespace irr;
using namespace scene;

class BVHSceneNode : public ISceneNode {
public:
	BVHSceneNode(ISceneNode* pParent, scene::ISceneManager* mgr, Joint* pJoint, vector<float*>* frames, f32 interval)
		: ISceneNode(pParent, mgr)
		, m_AnimationInterval(interval)
		, m_pFrames(frames)
		, m_AnimationTime(0)
		, m_LastTime(0){
		
		m_Offset.X = pJoint->m_Offset[0]; m_Offset.Y = pJoint->m_Offset[1]; m_Offset.Z = pJoint->m_Offset[2];
		setPosition(m_Offset);
		m_Name = pJoint->m_Name;
		m_Channels = pJoint->m_Channels;
		m_ChannelIndex = pJoint->m_ChanelIndex;
		for (u32 i = 0; i < pJoint->m_Childrens.size(); i++) {
			this->addChild(new BVHSceneNode(0, mgr, pJoint->m_Childrens[i], frames, interval));
		}
	}

	virtual void OnRegisterSceneNode() {
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	void OnAnimate(u32 timeMs) {
		ISceneNode::OnAnimate(timeMs);
		u32 currTime = timeMs;
		float  delta = (currTime - m_LastTime) * 0.001f;
		if (delta > 0.03f)
			delta = 0.03f;
		m_LastTime = currTime;
		m_AnimationTime += delta;

		u32 frameNo = int(m_AnimationTime / m_AnimationInterval);
		frameNo = frameNo %  m_pFrames->size();

		printf("\nAnimating for frame: %d", frameNo);
		core::vector3df position = m_Offset;
		core::vector3df rotation(0.0f, 0.0f, 0.0f);
		f32* frames = m_pFrames->at(frameNo) + m_ChannelIndex;
		for (u32 i = 0; i < m_Channels.size(); i++) {
			Channel ch = m_Channels[i];
			switch (ch) {
			case XPosition:
				position.X += frames[i];
				break;
			case YPosition:
				position.Y += frames[i];
				break;
			case ZPosition:
				position.Z += frames[i];
				break;
			case XRotation:
				rotation.X = frames[i];
				break;
			case YRotation:
				rotation.Y = frames[i];
				break;
			case ZRotation:
				rotation.Z = frames[i];
				break;
			}
		}
		setPosition(position);
		setRotation(rotation);
	}

	void render();
	const core::aabbox3df& getBoundingBox() const;


	f32 getAnimationInterval() {
		return m_AnimationInterval;
	}
private:
	video::SMaterial m_Material;
	vector<Channel> m_Channels;
	core::vector3df m_Offset;
	u32 m_ChannelIndex;
	std::string m_Name;
	f32 m_AnimationInterval;
	vector<float*>* m_pFrames;

	vector<core::matrix4> m_MatrixStack;
	mutable core::aabbox3df m_Box;

	mutable f32 m_AnimationTime;
	mutable u32 m_LastTime;
};

#endif