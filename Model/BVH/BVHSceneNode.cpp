#include "BVHSceneNode.h"

void BVHSceneNode::render() {
	video::IVideoDriver* pDriver = SceneManager->getVideoDriver();
	pDriver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	core::list<ISceneNode*> childs = getChildren();
	for (core::list<ISceneNode*>::Iterator itr = childs.begin(); itr != childs.end(); itr++) {
		ISceneNode* pSceneNode = *itr;
		BVHSceneNode* pBVHNode = dynamic_cast<BVHSceneNode*>(pSceneNode);
		if (pBVHNode == NULL) {
			printf("\nBVHSceneNode: error on casting bvh node");
			continue;
		}
		core::vector3df pos = pBVHNode->m_Offset;
		//pos.X *= 0.7f; pos.Y *= 0.7f; pos.Z *= 0.7f;
		pDriver->setMaterial(m_Material);
		
		
		pDriver->draw3DLine(core::vector3df(0.0f, 0.0f, 0.0f), pos);
	}
}

const core::aabbox3df& BVHSceneNode::getBoundingBox() const {
	m_Box.reset(0.0f, 0.0f, 0.0f);
	m_Box.addInternalPoint(getPosition());
	return m_Box;
}

