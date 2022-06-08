// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// For conditions of distribution and use, see copyright notice in irrWeatherManager.h

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#include "irrWeatherManager.h"

namespace irr
{

irrWeatherManager::irrWeatherManager(IrrlichtDevice* device)
{
    if( !device )
        return;

    Device = device;

    Smgr = Device->getSceneManager();

    f32 timeInSeconds = 20.f;
    f32 correctionOfTheVSync = (20.f / 34.f); // I get this value when i calculate the diffTime in onAnimate fonction. Generally eguals to 1 / 60 (fps).

    Atmosphere = new IWeatherManagerAtmosphere((timeInSeconds * correctionOfTheVSync), 0.f, 0.f, 0.f, Smgr->getRootSceneNode(), Smgr, 0);

    if( !Atmosphere )
    {
        printf("-- irrWeatherManager: ERROR: could not create atmosphere --\n");
        return;
    }

    else
        printf("irrWeatherManager %i.%i.%i\n", IRR_WEATHER_MANAGER_VER_MAJOR, IRR_WEATHER_MANAGER_VER_MINOR, IRR_WEATHER_MANAGER_VER_MICRO);

    Rain = Atmosphere->getRain();
    Cloud = Atmosphere->getCloud();

    nextWeatherSpeed = 5 * 1000; //Every 5 seconds, there is an update of the weather
    timer = Device->getTimer();
    timer->start();
    nextWeatherUpdate = nextWeatherSpeed + timer->getTime();

    Rain->changeInclemency(EWE_NONE);
}

irrWeatherManager::~irrWeatherManager()
{
    if ( Atmosphere)
        Atmosphere->remove();
}

void irrWeatherManager::updateWeather()
{
    if ( timer->getTime() > nextWeatherUpdate )
    {
        u32 randInt = rand() % 3;

        switch ( randInt )
        {
            case 0:
            {
                Rain->changeInclemency(EWE_NONE);

                u32 randomNumberOfClouds = rand() % 4; // 0 or 1 clouds shown

                switch ( randomNumberOfClouds )
                {
                    case 0:
                        Cloud->ShowClouds(false, false, false);
                        break;
                    case 1:
                        Cloud->ShowClouds(false, false, true);
                        break;
                    case 2:
                        Cloud->ShowClouds(false, true, false);
                        break;
                    case 3:
                        Cloud->ShowClouds(true, false, false);
                        break;
                }
            }

            break;

            case 1:
            {
                Rain->changeInclemency(EWE_RAIN);

                u32 randomNumberOfClouds = rand() % 3; // 2 or 3 clouds shown

                switch ( randomNumberOfClouds )
                {
                    case 0:
                        Cloud->ShowClouds(true, true, false);
                        break;
                    case 1:
                        Cloud->ShowClouds(true, false, true);
                        break;
                    case 2:
                        Cloud->ShowClouds(false, true, true);
                        break;
                    case 3:
                        Cloud->ShowClouds(true, true, true);
                        break;
                }
            }

            break;

            case 2:
                Rain->changeInclemency(EWE_SNOW);  // 3 clouds shown
                Cloud->ShowClouds(true, true, true);
            break;

            default:
                Rain->changeInclemency(EWE_NONE);
            break;
        }

        nextWeatherUpdate += nextWeatherSpeed;
    }
}

u32 irrWeatherManager::getNextWeatherSpeed() const
{
    return nextWeatherSpeed;
}

void irrWeatherManager::setNextWeatherSpeed(u32 nextweatherspeed)
{
    nextWeatherSpeed = nextweatherspeed;
}

} // namespace irr
