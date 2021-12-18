/*
irrWeatherManager weather system for the Irrlicht rendering engine.
Copyright (C) 2009-2011 Josiah Hartzell (Skyreign Software)

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
of your project, it would be greatly appreciated. If you decide to give credit,
please don't forget to credit the other projects used in irrWeatherManager (see readme.txt).

Josiah Hartzell (fighterstw@hotmail.com)
*/

/*!
    @mainpage irrWeatherManager documentation


    @section intro Introduction

    Welcome to irrWeatherManager's documentation.

    Here you will find out how to use irrWeatherManager in your games and simulations.
    It is pretty straight-forward, so you shouldn't have any trouble using it.

    Some of this library's code is with credit due to Pazystamo (ATMOsphere),
    Bitplane (original ICloudSceneNode, which I have majorly changed for irrWeatherManager),
    and Sudi (IBoltSceneNode).

    This library was originally created for Fighters Tactical Warfare, a Cold War combat game project
    by Josiah Hartzell (Skyreign Software), and it is now released open-source for other C++ developers to use in
    their games and simulations.

    If you have any questions, suggestions, or bug reports, please email them to me at fighterstw@hotmail.com.
*/

#ifndef __IRRWEATHERMANAGER_H__
#define __IRRWEATHERMANAGER_H__

#include "IWeatherManagerAtmosphere.h"
#include "ICloudSceneNode.h"
#include "ICloudLayer.h"
#include <vector3d.h>
#include <irrTypes.h>
#include <irrString.h>

#define IRR_WEATHER_MANAGER_VER_MAJOR 0
#define IRR_WEATHER_MANAGER_VER_MINOR 1
#define IRR_WEATHER_MANAGER_VER_MICRO 58

namespace irr
{
    class IrrlichtDevice;

    namespace scene
    {
        class ISceneNode;
    }
}


class irrWeatherManager
{
    public:
        irrWeatherManager(irr::IrrlichtDevice* const irrDevice);
        ~irrWeatherManager();

        IWeatherManagerAtmosphere *getAtmosphere() const { return atmosphere; };

        /*void addCloud(const irr::core::vector3df &pos, const irr::core::vector3df &scale,
            irr::core::stringc textureName, irr::u32 extraSeed);*/

        //void addCloud(const SCloudCreationInfo &info, irr::u32 extraSeed);


        /*void addCloudLayer(const irr::core::vector3df &center,
            const irr::core::vector3df &cloudScale, irr::core::stringc cloudTextureName,
            irr::u32 numClouds=10, irr::f32 randomness=0.4);*/

        /*!
            @param center The center around which the cloud layer will be distributed (This includes the altitude on the Y axis.)
            @param info The cloud creation info for each cloud in the layer
            @param numClouds The number of clouds in this layer
            @param randomness How random the distribution will be (0.4 is a nice value)
        */
        ICloudLayer* addCloudLayer(const irr::core::vector3df &center, SCloudCreationInfo &info,
            irr::u32 numClouds, bool isStormCloudLayer=false, irr::f32 randomness=0.4f);

        /// Updates everything in the weather system
        void updateWeather();

        void removeCloudLayer(ICloudLayer* layer, bool removeFromScene=true);

        void removeAllCloudLayers(bool removeFromScene=true);

        void setLightningFrequency(irr::f32 newFreq) { lightningFrequency = newFreq; };


        ICloudLayer* getCloudLayerByIndex(irr::u32 index) const;

        irr::u32 getCloudLayerCount() const;

        int getRandom(int lim, int seed) const;

        irr::f32 getLightningFrequency() const { return lightningFrequency; };


    private:
        IWeatherManagerAtmosphere *atmosphere;
        irr::IrrlichtDevice* device;

        irr::core::list<ICloudLayer*> CloudLayers;
        irr::u32 CloudLayerCount;

        irr::f32 nextLightningUpdate;

        irr::f32 lightningFrequency;
};

#endif // __IRRWEATHERMANAGER_H__
