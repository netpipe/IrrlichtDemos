// Copyright (C) Pazystamo
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#ifndef __IWeatherManagerAtmosphere_H_INCLUDED__
#define __IWeatherManagerAtmosphere_H_INCLUDED__

#include "IAtmosphereSkySceneNode.h"
#include "IAtmosphereStarSceneNode.h"
#include "IAtmosphereRainSceneNode.h"
#include "IAtmosphereCloudSceneNode.h"

namespace irr
{

class IWeatherManagerAtmosphere : public scene::ISceneNode
{
    public:
        //! Constructors.
        IWeatherManagerAtmosphere(f32 dayspeed, f32 phi, f32 theta, f32 rotation,
            scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id);

        //! Destructors.
        ~IWeatherManagerAtmosphere();

        virtual void OnRegisterSceneNode();

        //! Animates the node.
        virtual void OnAnimate(u32 timeMs);

        //! Renders the node.
		virtual void render();

        //! Returns the axis aligned bounding box of the Sky node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

        //! Sets the Sky texture.
        void setSkyImage(video::IImage* image);

		//! Returns the time to make a complete day/night cycle (in seconds)
		f32 getDaySpeed() const;

		//! Sets the time to make a complete day/night cycle (in seconds)
		void setDaySpeed(f32 daySpeed);

		//! Returns the angle of Phi
		f32 getPhi() const;

		//! Set the angle of Phi
		void setPhi(f32 phi);

        //! Returns the angle of Theta
		f32 getTheta() const;

		//! Set the angle of Theta
		void setTheta(f32 theta);

        //! Returns the Sky
		IAtmosphereSkySceneNode *getSky() const { return Sky; };

        //! Returns the Star
		IAtmosphereStarSceneNode *getStar() const { return Star; };

        //! Returns the Rain
		IAtmosphereRainSceneNode *getRain() const { return Rain; };

        //! Returns the Cloud
		IAtmosphereCloudSceneNode *getCloud() const { return Cloud; };

    private:
        //! Sets up the Sky and the Sun/Moon.
        void CreateSkyPalette();

        //! Rounds the angle to fit 360 degrees.
        void modulo360();

        scene::ISceneManager* smgr;
        video::IVideoDriver* driver;
        video::IImage* SkyImage;    // Sky texture

        f32 DaySpeed;   // How long goes day in minutes
        f32 Phi, Theta;
        f32 Rotation;

        video::SColorf SkyPixel;
        u32 LastAnimationTime;

        IAtmosphereSkySceneNode *Sky;
        IAtmosphereStarSceneNode* Star;
        IAtmosphereRainSceneNode* Rain;
        IAtmosphereCloudSceneNode* Cloud;
        //scene::ILightSceneNode *SunLight;
};

} // namespace

#endif // __IWeatherManagerAtmosphere_H_INCLUDED__
