//! The class was written by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut).

#include "IAtmosphereCloudSceneNode.h"

namespace irr
{

//! Constructor.
IAtmosphereCloudSceneNode::IAtmosphereCloudSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
        video::ITexture* cloud1, video::ITexture* cloud2, video::ITexture* cloud3)
	: Parent(parent), smgr(mgr), CloudTexture1(cloud1), CloudTexture2(cloud2), CloudTexture3(cloud3)
{
    // add 1st cloud layer
	CloudLayer1 = new CCloudSceneNode(smgr->getRootSceneNode(), smgr);
	CloudLayer1->setTranslation(core::vector2d<f32>(0.008f, 0.0f));
	CloudLayer1->getMaterial(0).setTexture(0, CloudTexture1);
	CloudLayer1->setCloudHeight(0.5f, 0.1f, -0.05f);

	// add 2nd cloud layer
	CloudLayer2 = new CCloudSceneNode(smgr->getRootSceneNode(), smgr);
	CloudLayer2->setTranslation(core::vector2d<f32>(0.006f, 0.003f));
	CloudLayer2->getMaterial(0).setTexture(0, CloudTexture2);
	CloudLayer2->setCloudHeight(0.4f, 0.05f, -0.1f);
	CloudLayer2->setTextureScale(0.5f);

	// add 3rd cloud layer
	CloudLayer3 = new CCloudSceneNode(smgr->getRootSceneNode(), smgr);
	CloudLayer3->setTranslation(core::vector2d<f32>(0.006f, 0.003f));
	CloudLayer3->getMaterial(0).setTexture(0, CloudTexture3);
	CloudLayer3->setCloudHeight(0.35f, 0.0f, -0.15f);
	CloudLayer3->setTextureScale(0.4f);
}

IAtmosphereCloudSceneNode::~IAtmosphereCloudSceneNode()
{
    if (CloudLayer1)
		CloudLayer1->drop();
	if (CloudLayer2)
		CloudLayer2->drop();
	if (CloudLayer3)
		CloudLayer3->drop();
}

void IAtmosphereCloudSceneNode::ShowClouds(bool cloud1, bool cloud2, bool cloud3)
{
    CloudLayer1->setVisible(cloud1);
    CloudLayer2->setVisible(cloud2);
    CloudLayer3->setVisible(cloud3);
}

} // namespace irr
