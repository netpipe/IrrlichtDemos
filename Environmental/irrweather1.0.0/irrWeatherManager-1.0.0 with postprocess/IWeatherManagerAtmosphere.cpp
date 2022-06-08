// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// For conditions of distribution and use, see copyright notice in irrWeatherManager.h

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#include "IWeatherManagerAtmoSphere.h"

#define Radius 100.f

namespace irr
{

//! Constructor.
IWeatherManagerAtmosphere::IWeatherManagerAtmosphere(f32 dayspeed, f32 phi, f32 theta, f32 rotation,
    scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) : scene::ISceneNode(parent, mgr, id),
    DaySpeed(dayspeed), Phi(phi), Theta(theta), Rotation(rotation), LastAnimationTime(0)
{
    if(Phi < 0.f)
        Phi = -Phi;
    if(Theta < 0.f)
        Theta = -Theta;

    // Default values
    smgr = mgr;
    driver = smgr->getVideoDriver();

    // Start the sky
    setSkyImage(driver->createImageFromFile("../../Data/skycolor.png"));
    CreateSkyPalette();
    smgr->setAmbientLight(video::SColorf(1.f, 1.f, 1.f, 1.f));
}

//! Destructors.
IWeatherManagerAtmosphere::~IWeatherManagerAtmosphere()
{
    if ( Sky )
        Sky->remove();

    if ( Star )
        Star->remove();
}

//! Sets up the Sky and the Sun/Moon.
void IWeatherManagerAtmosphere::CreateSkyPalette()
{
    // Stars box
    scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
        driver->getTexture("../../Data/stars_top.png"),
        driver->getTexture("../../Data/stars_bottom.png"),
        driver->getTexture("../../Data/stars.png"),
        driver->getTexture("../../Data/stars.png"),
        driver->getTexture("../../Data/stars.png"),
        driver->getTexture("../../Data/stars.png"));

    skybox->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    Sky = new IAtmosphereSkySceneNode(smgr->getRootSceneNode(), smgr, -1, driver->getTexture("../../Data/sky.png"), 16, Radius * 10.f);

    // Sun billboard
    video::ITexture* sun = driver->getTexture("../../Data/sun.tga");
    video::ITexture* moon = driver->getTexture("../../Data/moon.tga");
    Star = new IAtmosphereStarSceneNode(smgr->getRootSceneNode(), smgr, -1, sun, moon, core::dimension2df(128.f, 128.f));

    Cloud = new IAtmosphereCloudSceneNode(smgr->getRootSceneNode(), smgr, driver->getTexture("../../Data/cloud01.png"), driver->getTexture("../../Data/cloud02.png"), driver->getTexture("../../Data/cloud03.png"));

//    Sky->setDebugDataVisible(scene::EDS_FULL);
//    Star->setDebugDataVisible(scene::EDS_FULL);

//    Sunlight = smgr->addLightSceneNode(Star, core::vector3df(0.f, 0.f, 0.f),
//        video::SColor(255, 255, 255, 255), 1000.f);
//    Sunlight->setRadius(256.f * 10.f / 2.f);
}

void IWeatherManagerAtmosphere::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering( this, scene::ESNRP_SKY_BOX );
	}

	ISceneNode::OnRegisterSceneNode();
}

//! Animates the node.
void IWeatherManagerAtmosphere::OnAnimate(u32 timeMs)
{
    // call base OnAnimate
	ISceneNode::OnAnimate(timeMs);

    // get time
	f32 timeDiff = static_cast<f32>( timeMs - LastAnimationTime );

    Theta += (180.f * 100.f) / (86400.f * DaySpeed) * timeDiff;

    LastAnimationTime = timeMs;
}

//! Renders the node.
void IWeatherManagerAtmosphere::render()
{
    modulo360();
    f32 sinOfPhi = sinf(Phi * core::DEGTORAD);
    f32 cosOfPhi = cosf(Phi * core::DEGTORAD);

    f32 sinOfTheta = sinf(Theta * core::DEGTORAD);
    f32 cosOfTheta = cosf(Theta * core::DEGTORAD);

    f32 x = Radius * cosOfTheta * cosOfPhi;
    f32 y = Radius * sinOfTheta;
    f32 z = Radius * cosOfTheta * sinOfPhi;

    core::vector3df sun_place = core::vector3df(x, y, z);
    if ( Rotation != 0.f )
        sun_place.rotateYZBy(Rotation, smgr->getRootSceneNode()->getAbsolutePosition());

    core::vector3df billboardPosition;
    billboardPosition = sun_place;
    Star->setSunPosition(billboardPosition);

    billboardPosition = -sun_place;
    Star->setMoonPosition(billboardPosition);

//    Sunlight->setPosition(sun_place * 10.f + core::vector3df(256.f*10.f/2.f, 0.f, 256.f*10.f/2.f));

    SkyPixel = SkyImage->getPixel(static_cast<u32>(Theta), 0); // Y = 123

    smgr->setAmbientLight(video::SColorf(SkyPixel.getRed(), SkyPixel.getGreen(), SkyPixel.getBlue(), 1.f));

    //! STILL BUGING A BIT !!!
//    smgr->setShadowColor(video::SColor(255, SkyPixel.getRed(), SkyPixel.getGreen(), SkyPixel.getBlue()));
//    Sunlight->getLightData().DiffuseColor = video::SColorf(video::SColor(SkyPixel.getRed(), SkyPixel.getGreen(), SkyPixel.getBlue(), 1.f));

    Sky->setUvX(Theta / 360.f); //! Control the color of the sky
}

//! Returns the axis aligned bounding box of the Sky node
const core::aabbox3d<f32>& IWeatherManagerAtmosphere::getBoundingBox() const
{
    return Sky->getBoundingBox();
}

//! Sets the Sky texture.
void IWeatherManagerAtmosphere::setSkyImage(video::IImage* image)
{
    SkyImage = image;
}

//! Gets the time to make a complete day/night cycle (in seconds)
f32 IWeatherManagerAtmosphere::getDaySpeed() const
{
	return DaySpeed;
}

//! Sets the time to make a complete day/night cycle (in seconds)
void IWeatherManagerAtmosphere::setDaySpeed(f32 dayspeed)
{
	DaySpeed = dayspeed;
}

//! Gets the angle of Phi
f32 IWeatherManagerAtmosphere::getPhi() const
{
	return Phi;
}

//! Sets the angle of Phi
void IWeatherManagerAtmosphere::setPhi(f32 phi)
{
	Phi = phi;
}

//! Gets the angle of Theta
f32 IWeatherManagerAtmosphere::getTheta() const
{
	return Theta;
}

//! Sets the angle of Theta
void IWeatherManagerAtmosphere::setTheta(f32 theta)
{
	Theta = theta;
}

//! Rounds the angle to fit 360 degrees.
void IWeatherManagerAtmosphere::modulo360()
{
    if(Phi > 360.f)
        Phi = 0.f;
    if(Theta > 360.f)
        Theta = 0.f;
}

} // namespace irr
