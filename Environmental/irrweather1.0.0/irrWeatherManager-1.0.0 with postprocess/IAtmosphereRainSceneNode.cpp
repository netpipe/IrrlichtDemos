//! The class was written by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut).

#include "IAtmosphereRainSceneNode.h"

namespace irr
{

//! Constructor.
IAtmosphereRainSceneNode::IAtmosphereRainSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
        video::ITexture* rain, video::ITexture* snow)
	: Parent(parent), smgr(mgr), Rain(rain), Snow(snow)
{
    Driver = smgr->getVideoDriver();

    ps = smgr->addParticleSystemSceneNode(false);
    ps->setPosition(core::vector3df(0.f, 0.f, 0.f));
    ps->setScale(core::vector3df(1.f, 1.f, 1.f));

    em = ps->createBoxEmitter(core::aabbox3d<f32>(-128.f, -128.f, -128.f, 128.f, 128.f, 128.f),
    core::vector3df(0.f, 0.f, 0.f), 1000, 2000, video::SColor(255, 255, 255, 255),
    video::SColor(255, 255, 255, 255), 500, 1000);

    ps->setEmitter(em);

    scene::IParticleAffector* paf = ps->createGravityAffector(core::vector3df(0.f, -1.5f, 0.f), 1000);

    ps->addAffector(paf);
    paf->drop();

    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialTexture(0, Rain);
    ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

    ps->setParent(Parent);
}

//! Destructor.
IAtmosphereRainSceneNode::~IAtmosphereRainSceneNode()
{

}

//! Changes the inclemency's effect.
void IAtmosphereRainSceneNode::changeInclemency(E_WEATHER_EFFECT effect)
{
    if ( effect == EWE_NONE )
    {
        ps->clearParticles();
        ps->setVisible(false);
    }

    if ( effect == EWE_RAIN )
    {
        ps->setMaterialTexture(0, Rain);
        ps->setVisible(true);
        em->setMinStartSize(core::dimension2df(0.1f, 0.5f));
        em->setMaxStartSize(core::dimension2df(0.4f, 2.f));
    }

    if ( effect == EWE_SNOW )
    {
        ps->setMaterialTexture(0, Snow);
        ps->setVisible(true);
        em->setMinStartSize(core::dimension2df(2.f, 2.f));
        em->setMaxStartSize(core::dimension2df(1.f, 1.f));
    }
}

} // namespace irr
