/*
  Copyright (C) 2011 Thijs Ferket (RdR)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "DecalSceneNode.h"

using namespace irr;
using namespace scene;

DecalSceneNode::DecalSceneNode(scene::ISceneNode* parent, scene::ISceneManager* smgr, scene::IMesh* mesh, video::ITexture* texture, const core::vector3df position) :
	scene::ISceneNode(parent, smgr, -1, position, irr::core::vector3df(0, 0, 0), (irr::core::vector3df(1, 1, 1) / parent->getScale())) {

	this->mesh = mesh;
	this->texture = texture;
	this->box = mesh->getBoundingBox();

	fadeOut = true;
	fadeOutTime = 3;
	startFadeOutTimeMs = 0;

	distance = 0;
	lifetime = 0;
	time(&creationTime);

	// Material
	material.Wireframe = false;
	material.Lighting = false;
	material.BackfaceCulling = false;
	material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
///	material.MaterialTypeParam = irr::video::pack_texureBlendFunc(irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_TEXTURE | irr::video::EAS_VERTEX_COLOR);

	material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_BORDER;
	material.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_BORDER;

	material.setTexture(0, texture);

	setAutomaticCulling(scene::EAC_FRUSTUM_BOX);
}

DecalSceneNode::~DecalSceneNode() {
	if (mesh) {
		mesh->drop();
	}
}

void DecalSceneNode::OnRegisterSceneNode() {
	if (lifetime > 0 && (difftime(time(NULL), creationTime) > lifetime)) {
		// If fading out enabled
		if (fadeOut) {
			// Delete if decal is fade out
			if (difftime(time(NULL), creationTime) > lifetime + fadeOutTime) {
				SceneManager->addToDeletionQueue(this);
			}
		} else {
			SceneManager->addToDeletionQueue(this);
		}
	}
	if (IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}

void DecalSceneNode::render() {
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver || !IsVisible) {
		return;
	}

	// Check if node is out of range
	if (distance > 0) {
		f32 d = camera->getAbsolutePosition().getDistanceFrom(getAbsolutePosition());
		if (d > distance) {
			return;
		}
	}

	// Draw mesh
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	if (!material.Wireframe) {
		material.setTexture(0, texture);
	}
	driver->setMaterial(material);
	driver->drawMeshBuffer(mesh->getMeshBuffer(0));

	// Debug only
	if (SHOW_DEBUG_BOX) {
		material.setTexture(0, 0);
		driver->setMaterial(material);

		driver->draw3DBox(box, video::SColor(255, 255, 0, 0));
	}
}

void DecalSceneNode::OnAnimate(u32 timeMs) {
	if (lifetime > 0 && fadeOut) {

		// Lifetime done, start fading out
		if (difftime(time(NULL), creationTime) > lifetime) {
			if (startFadeOutTimeMs > 0) {
				u32 timeDiff = timeMs - startFadeOutTimeMs;
				u32 alpha = 255 - ((timeDiff * 255) / (fadeOutTime * 1000));

				SceneManager->getMeshManipulator()->setVertexColorAlpha(mesh, alpha);
			} else {
				material.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
				startFadeOutTimeMs = timeMs;
			}
		}
	}
	ISceneNode::OnAnimate(timeMs);
}

const core::aabbox3d<f32>& DecalSceneNode::getBoundingBox() const {
	return box;
}

scene::ESCENE_NODE_TYPE DecalSceneNode::getType() const {
	return (scene::ESCENE_NODE_TYPE) DECAL_SCENE_NODE_ID;
}

u32 DecalSceneNode::getMaterialCount() const {
	return 1;
}

video::SMaterial& DecalSceneNode::getMaterial(u32 i) {
	return material;
}

scene::IMesh* DecalSceneNode::getMesh() {
	return mesh;
}

video::ITexture* DecalSceneNode::getTexture() {
	return texture;
}

f32 DecalSceneNode::getLifetime() {
	return lifetime;
}

void DecalSceneNode::setLifetime(const f32 lifetime) {
	this->lifetime = lifetime;
}

f32 DecalSceneNode::getDistance() {
	return distance;
}

void DecalSceneNode::setDistance(const f32 distance) {
	this->distance = distance;
}

void DecalSceneNode::setFadeOut(const bool fadeOut, const irr::f32 time) {
	this->fadeOut = fadeOut;
	if (time > 0) {
		this->fadeOutTime = time;
	}
}
