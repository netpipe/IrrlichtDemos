// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// For conditions of distribution and use, see copyright notice in irrWeatherManager.h

#ifndef __CLOUD_LAYER_H_INCLUDED__
#define __CLOUD_LAYER_H_INCLUDED__

#include <vector3d.h>
#include <irrTypes.h>
#include <irrString.h>
#include <irrArray.h>
#include <irrList.h>

namespace irr
{
    class IrrlichtDevice;

    namespace scene
    {
        class ISceneNode;
        class ICloudSceneNode;
    }
}

struct SCloudCreationInfo
{
    void setDefaults()
    {
        location = irr::core::vector3df(0.0f,6000.0f,0.0f);
        rotation = irr::core::vector3df(0,0,0);
        scale = irr::core::vector3df(0.15f,0.2f,0.3f);
        textureName = "";
        seed = 2000;
        ID = -1;
        LOD = 8.0f;
        maxDepth = 3;
        numParticles = 10;
        globalParticleSize = 6000.0f;
    }

    irr::core::vector3df location;
    irr::core::vector3df rotation;
    irr::core::vector3df scale;
    irr::core::stringc textureName;
    irr::core::array<irr::core::stringc> textureNames;
    irr::u32 seed;
    irr::s32 ID;
    irr::f32 LOD;
    irr::u32 maxDepth;
    irr::u32 numParticles;
    irr::f32 globalParticleSize;
};


class ICloudLayer
{
    public:
        ICloudLayer(irr::IrrlichtDevice* const irrDevice, const irr::core::vector3df& layerCenter);
        ~ICloudLayer();

        /// Adds a single cloud to the layer
        void addCloud(const SCloudCreationInfo& info, irr::u32 extraSeed);

        void removeCloud(irr::scene::ICloudSceneNode* const cloud, bool removeFromScene=true);

        void removeAllClouds(bool removeFromScene=true);

        void setIsStormCloudLayer(bool b) { IsStormCloudLayer = b; };

        /// Sets the distance the camera must move before the particles in each cloud in this layer update
        void setCameraPositionThreshold(irr::f32 threshold)  { CameraPositionThreshold = threshold; };

        /// Sets the distance the camera's target must move before the particles in each cloud in this layer update
        void setCameraTargetThreshold(irr::f32 threshold)  { CameraTargetThreshold = threshold; };

        /// Sets whether or not the imposter system is enabled for this cloud layer
        void setImposterEnabled(bool b) { ImposterEnabled = b; };

        /*!
        Sets the frequency of checks to see if a cloud is far enough away from the currently
        active camera to be changed to an imposter.
        */
        void setImposterUpdateFrequency(irr::f32 frequency) { ImposterUpdateFrequency = frequency; };

        /// Sets the distance from the active camera to a cloud at which this cloud turns into an imposter
        void setImposterDistance(irr::f32 distance) { ImposterDistance = distance; };


        // Operators
        bool operator==(const ICloudLayer& other) const { return (this->uniqueID == other.getUniqueID()); };

        bool operator!=(const ICloudLayer& other) const { return !(*this == other); }


        irr::scene::ICloudSceneNode* getCloudByIndex(irr::u32 index) const;

        irr::u32 getCloudCount();

        irr::u32 getUniqueID() const { return uniqueID; };

        bool getIsStormCloudLayer() { return IsStormCloudLayer; };

        /// @return Whether or not the imposter system is enabled for this cloud layer
        bool getImposterEnabled() const { return ImposterEnabled; };

        /*!
            @return The frequency of checks to see if a cloud is far enough away from the currently
            active camera to be changed to an imposter.
        */
        irr::f32 getImposterUpdateFrequency() const { return ImposterUpdateFrequency; };

        /// @return The distance from the active camera to a cloud at which the cloud turns into an imposter
        irr::f32 getImposterDistance() const { return ImposterDistance; };

        /// @return The distance the camera must move before the particles in each cloud in this layer update
        irr::f32 getCameraPositionThreshold() const { return CameraPositionThreshold; };

        /// @return The distance the camera's target must move before the particles in each cloud in this layer update
        irr::f32 getCameraTargetThreshold() const { return CameraTargetThreshold; };


    private:
        irr::IrrlichtDevice* device;
        irr::core::list<irr::scene::ICloudSceneNode*> Clouds;
        irr::u32 cloudCount;
        irr::u32 uniqueID;
        bool IsStormCloudLayer;

        irr::u32 ImposterUpdateFrequency;
        irr::f32 ImposterDistance;

        bool ImposterEnabled;

        irr::f32 CameraPositionThreshold;
        irr::f32 CameraTargetThreshold;
};

#endif // __CLOUD_LAYER_H_INCLUDED__
