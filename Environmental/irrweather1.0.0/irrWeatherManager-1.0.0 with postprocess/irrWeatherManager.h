/*
irrWeatherManager weather system for the Irrlicht rendering engine.
Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)

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

While it is not necessary to mention irrWeatherManager in the credits or documentation
of your project, it would be appreciated greatly. If you decide to give credit,
please don't forget to credit the other projects used in irrWeatherManager (see readme.txt).

Josiah Hartzell (fighterstw@hotmail.com)
*/

/*!
    @mainpage irrWeatherManager documentation


    @section intro Introduction

    Welcome to irrWeatherManager's documentation.

    Here you will find out how to use irrWeatherManager in your games and simulations.
    It is pretty straight-forward, so you shouldn't have any trouble.

    A good portion of this library's code is with credit due to Pazystamo (ATMOsphere)
    and Bitplane (original ICloudSceneNode, which I have majorly changed for irrWeatherManager).

    This library was originally created for Fighters Tactical Warfare, a Cold War combat simulation project
    by Josiah Hartzell (www.fighterstw.co.nr), and it is now released open-source for other C++ developers to use in
    their games and simulations.

    If you have any questions, suggestions, or bug reports, please email them to me at fighterstw@hotmail.com.
*/

/*!

    The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
    and mostly inspired of the original version of this project.

    I would like to thank Pazystamo (ATMOsphere), Josiah Hartzell (irrWeatherManager), tbw (CloudSceneNode)

    This code works on the version 1.8 of irrlicht.

*/

#ifndef __IRRWEATHERMANAGER_H__
#define __IRRWEATHERMANAGER_H__

#include "IWeatherManagerAtmosphere.h"
#include "PostProcessManager.h"
#include "ShaderPostProcess.h"

#define IRR_WEATHER_MANAGER_VER_MAJOR 1
#define IRR_WEATHER_MANAGER_VER_MINOR 0
#define IRR_WEATHER_MANAGER_VER_MICRO 0

namespace irr
{

class irrWeatherManager
{
    public:
        irrWeatherManager(IrrlichtDevice* device, CPostProcessManager* postProcessManager);

        ~irrWeatherManager();

        IWeatherManagerAtmosphere *getAtmosphere() const { return Atmosphere; };

        void updateWeather();

        //! Returns the nextWeatherSpeed.
        u32 getNextWeatherSpeed() const;

        //! Sets the nextWeatherSpeed.
        void setNextWeatherSpeed(u32 nextweatherspeed);

        E_POSTPROCESS_EFFECT getEPPE() const;

    private:

        IWeatherManagerAtmosphere *Atmosphere;
        IrrlichtDevice* Device;
        CPostProcessManager* PostProcessManager;
        scene::ISceneManager* Smgr;

        IAtmosphereRainSceneNode* Rain;
        IAtmosphereCloudSceneNode* Cloud;

        ITimer* timer;

        u32 nextWeatherSpeed;
        u32 nextWeatherUpdate;

        E_POSTPROCESS_EFFECT sampleEffect;
};

} // namespace

#endif // __IRRWEATHERMANAGER_H__
