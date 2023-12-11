// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#ifndef __IWeatherManagerAtmosphere_H_INCLUDED__
#define __IWeatherManagerAtmosphere_H_INCLUDED__

#include <irrTypes.h>
#include <vector3d.h>
#include <irrString.h>
#include "IAtmosphereSkySceneNode.h"
#include "ICloudLayerCone.h"
#include "ICompoundCelestialBodySceneNode.h"

class IWeatherManagerAtmosphere
{
    public:
        IWeatherManagerAtmosphere(irr::IrrlichtDevice* const irrDevice,irr::scene::ISceneNode* const parent, irr::s32 id);

        ~IWeatherManagerAtmosphere();

        void setDate(irr::u32 year, irr::u32 month, irr::u32 day, irr::u32 hour, irr::u32 minute);

        /// Rounds angle to fit 360 degrees
        irr::f32 round360(irr::f32 angle);

        irr::core::vector3df getInterpolated3df(irr::core::vector3df from,irr::core::vector3df to, irr::f32 d);

        /// Prepare sun position interpolation (find start and end positions)
        void prep_interpolation(irr::f64 Jdate, irr::f64 time);

        /// Calculate sun position
        void saule(irr::f64 pl,irr::f64 lw,irr::f64 J);

        void setSkyImage(const char *filename);

        void CreateSkyPallete();

        // Starts ATMOsphere and prepares for work
        //void start(irr::IrrlichtDevice* const irrDevice,irr::scene::ISceneNode* const parent, irr::s32 id);

        /// Starts timer
        void startTimer();

        /// Calculates delta time (time from last frame) for timer
        void updateTimer();

        /// If true, the atmosphere will automatically set the fog color to the clear color
        void setUpdateFog(bool b) { UpdateFog = b; };

        /// @return Sun's rotation about Y axis
        irr::f64 getSunXAngle();

        /// @return Sun's rotation about X axis (sun height above horizon)
        irr::f64 getSunYAngle();

        /// Sets simulation days per real day
        void setDaysPerDay(irr::f64 days);

        /// @return Simulation days per real day
        irr::f64 getDayspeed();

        /// Updates the entire atmosphere system
        void update();

        wchar_t getTextDate();

        /// Converts Roman calender date to Julian calendar date
        irr::f64 DateToJulian(irr::u16 y,irr::u16 m,irr::u16 d,irr::u16 h,irr::u16 min);

        /// Converts Julian calendar date to Roman calendar date
        void JulianToDate(irr::f64 x);

        /// Sets the ambient lighting of the entire scene
        void setAmbientLight2(irr::video::SColor color);

        const irr::video::SColor& getClearColor() const { return ClearColor; };

        bool getUpdateFog() { return UpdateFog; };

        ICompoundCelestialBodySceneNode* getCompoundCelestialBodySceneNode() const { return bill; };

    private:
        struct PIXEL
        {
            irr::u8  B,G,R,A;
        };

        ICompoundCelestialBodySceneNode* bill;
        irr::u32 kin,ilgis;
        irr::video::SColor ClearColor;
        bool UpdateFog;
        irr::u32 i;
        //u8 palete[256][3];
        //u8 prspalva[3];
        //u8 pbspalva[3];
        //u8 vspalva[3];
        float step[3];
        float pspalva[3];
        IAtmosphereSkySceneNode* Sky;
        ICloudLayerCone* CloudCone;
        irr::scene::ILightSceneNode* sunlight;
        irr::video::IImage* image;
        irr::s32 skyid;
        irr::video::ITexture* dangus;
        irr::IrrlichtDevice* device;
        irr::video::IVideoDriver* driver;
        irr::scene::ISceneManager* smgr;
        double rad;//radian
        irr::u32 tex_size;//texture size
        irr::u32 half_tex;//half texture size
        //--Timer----
        irr::ITimer *Atimer;//ATMOsphere timer
        irr::f32 currentTime, startTime, dTime;
        //how many virtual day per real day. 1440 = day per 1 min.
        //24*60=1440/dayspeed=1day-1min
        irr::f64 dayspeed; // how long goes day in minutes

        irr::u32 ptime,gtime;
        irr::f64 time_int_step;//sun place interpolation position (0-1)
        irr::f64 counter_time;//loop variable,count time from interpolation start in J
        irr::f64 J1minute;
        //--Sun position---
        double J;//julias date
        double sun_angle[2];//sun pos in angles
        irr::f32 sun_interpolation_speed;//how minutes in virtual time takes interpolate sun from start to end position
        double J1;
        irr::core::vector3df sun_pos_from,sun_pos_to;//interpolations points
        irr::f64 sun_angle_from,sun_angle_to;//sun height angle for interpolation
        //--Sky texture----
        //setSkyImage
        irr::video::IImage* skyimage;//sky texture
        PIXEL *pixels;//palete
        irr::f32 uvX;
        //---Date conversions
        irr::u16 Ndate[5];//stores date converted from Julian calendar
};

#endif // __IWeatherManagerAtmosphere_H_INCLUDED__
