/*
Copyright (C) 2008   Christian Reiser, Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "graphics/CameraSceneNode.h"
#include "graphics/GraphicsEngine.h"
#include "core/SettingsManager.h"

#include "Horde3D.h"

#include <deque>

namespace peak
{
	CameraSceneNode::CameraSceneNode(SceneNode *parent)
	{
		ResHandle pipeline = Horde3D::addResource(ResourceTypes::Pipeline, SettingsManager::get()->getString("device.pipeline").c_str(), 0);
		node = Horde3D::addCameraNode(RootNode, "Camera", pipeline);
		
		if (parent)
		{
			setParent(parent);
		}
		cameras.push_back(this);
	}
	
	CameraSceneNode::~CameraSceneNode()
	{
		for (unsigned int i = 0; i < cameras.size(); i++)
		{
			if (cameras[i] == this)
			{
				cameras.erase(cameras.begin() + i);
				break;
			}
		}
		if (node)
		{
			//int stdcam = (scene::ICameraSceneNode*)GraphicsEngine::get()->getStandardCamera()->getSceneNode();
			//GraphicsEngine::get()->getSceneManager()->setActiveCamera(stdcam);
		}
	}
	
	void CameraSceneNode::setNearValue(float nearvalue)
	{
		//((scene::ICameraSceneNode*)node)->setNearValue(nearvalue);
	}
	float CameraSceneNode::getNearValue(void)
	{
		//return ((scene::ICameraSceneNode*)node)->getNearValue();
	}
	void CameraSceneNode::setFarValue(float nearvalue)
	{
		//((scene::ICameraSceneNode*)node)->setFarValue(nearvalue);
	}
	float CameraSceneNode::getFarValue(void)
	{
		//return ((scene::ICameraSceneNode*)node)->getFarValue();
	}
	
	void CameraSceneNode::setOrthogonal(bool orthogonal)
	{
		//((scene::ICameraSceneNode*)node)->setIsOrthogonal(orthogonal);
	}
	bool CameraSceneNode::isOrthogonal(void)
	{
		//return ((scene::ICameraSceneNode*)node)->isOrthogonal();
	}
	
	void CameraSceneNode::setFOV(float fov)
	{
		//((scene::ICameraSceneNode*)node)->setFOV(fov);
	}
	float CameraSceneNode::getFOV(void)
	{
		//return ((scene::ICameraSceneNode*)node)->getFOV();
	}
	
	void CameraSceneNode::setAspectRatio(float ratio)
	{
		//((scene::ICameraSceneNode*)node)->setAspectRatio(ratio);
	}
	float CameraSceneNode::getAspectRatio(void)
	{
		//return ((scene::ICameraSceneNode*)node)->getAspectRatio();
	}
	
	void CameraSceneNode::render(void)
	{
		Horde3D::render(node);
	}
	
	void CameraSceneNode::updateAllCameras(void)
	{
		for (unsigned int i = 0; i < cameras.size(); i++)
		{
			cameras[i]->updateCamera();
		}
	}
	void CameraSceneNode::updateCamera(void)
	{
		// Update global positions
		/*std::deque<scene::ISceneNode*> parents;
		scene::ISceneNode *parent = node->getParent();
		while (parent)
		{
			parents.push_back(parent);
			parent = parent->getParent();
		}
		std::deque<scene::ISceneNode*>::reverse_iterator it = parents.rbegin();
		core::vector3df rotation = node->getRotation();
		while (it != parents.rend())
		{
			(*it)->updateAbsolutePosition();
			rotation += (*it)->getRotation();
			it++;
		}
		
		// Get the new vectors
		core::matrix4 direction;
		direction.setRotationDegrees(rotation);
		core::vector3df lookvector(0, 0, 1);
		direction.rotateVect(lookvector);
		core::vector3df upvector(0, 1, 0);
		direction.rotateVect(upvector);

		// Set camera
		((scene::ICameraSceneNode*)node)->updateAbsolutePosition();
		((scene::ICameraSceneNode*)node)->setTarget(node->getAbsolutePosition() + lookvector);
		((scene::ICameraSceneNode*)node)->setUpVector(upvector);*/
		
	}
	
	std::vector<CameraSceneNode*> CameraSceneNode::cameras;
}
	
