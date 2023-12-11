// This file was part of the Irrlicht Engine's particle system, Copyright (C) 2002-2005 Nikolaus Gebhardt
// Adapted by Gaz Davidson - gaz at bitplane dot net

////////////////////////////////////////////////////////////////////////
// This node has been vastly improved from an old                     //
// scene node (by Bitplane) for use in the irrWeatherManager library. //
////////////////////////////////////////////////////////////////////////
// Josiah Hartzell - fighterstw@hotmail.com                           //
////////////////////////////////////////////////////////////////////////

#ifndef __C_CLOUD_NODE_INCLUDED__
#define __C_CLOUD_NODE_INCLUDED__

//#include "irrlicht.h"
#include <irrTypes.h>
#include <vector3d.h>
#include <irrArray.h>
#include <irrList.h>
#include <irrString.h>
#include <ISceneNode.h>
#include <S3DVertex.h>
#include "ICloudLayer.h"

#define CLOUD_PARTICLE_LIMIT 2000

namespace irr
{

class ITimer;

namespace scene
{

class IParticleAffector;
class ICameraSceneNode;


//! Struct for holding particle data
struct SCloudParticle
{
    //! Distance from camera
    u32 distance;

    //! Position of the particle, or position relative to parent cloud
    core::vector3d<f32> pos;

    //! Cloud size
    core::dimension2d<f32> size;

    //! Cloud colour - (tsk tsk, AmEn)
    video::SColor color;

    //! Number of children
    s16 children;

    //! Position of first child
    s16 firstchild;

    //! How much children will be scaled
    f32 childscale;
};



//! A particle system scene node.
class ICloudSceneNode : public ISceneNode
{
public:


    //! constructor
    ICloudSceneNode(ICloudLayer* cloudLayer,
        ISceneNode* parent, ISceneManager* mgr, ITimer *devicetimer, s32 id,
        const core::vector3df& position,
        const core::vector3df& rotation,
        const core::vector3df& scale,
        f32 globParticleSize = 2000.0f);

    //! destructor
    virtual ~ICloudSceneNode();

    //! Adds new particle affector to the particle system.
    void addAffector(IParticleAffector* affector);

    //! Removes all particle affectors in the particle system.
    void removeAllAffectors();

    //! pre render
    void OnRegisterSceneNode();

    void OnAnimate(irr::u32 timeMs);

    //! render to texture for imposter effect
    void renderToTexture(const irr::core::vector3df& relativeCamPos);

    //! render
    void render();

    //! cloud creation functions...
    void makeRandomCloud(s16 count);

    //! turns z sorting on or off.
    //! speed increase for alpha blend clouds, or clouds that are one colour
    void setZOrdering(bool zo);

    //! decides whether child nodes are the same colour as their parents
    void setIgnoreChildColor(bool b);

    //! sets the maximum detail depth to draw
    //void setMaxDepth(s16 maxdepth);

    //! sets the level of detail ("distance/size" - scale of 1 to 10 works well)
    //void setLOD(f32 detail);

    //! sets if these clouds are sky clouds or local clouds
    //! if they are distant, factor says how they move in the distance (ie 0.1 = 10x)
    void setSkyClouds(bool sky,f32 factor);

    //! sets the maximum time allowed to draw the screen.
    void setMaxTimePerFrame(u32 t);

    //! Added for irrWeatherManager; Sets the scale of the entire cloud.
    void setScale(const irr::core::vector3df &scale);

    /// Only to be used by irrWeatherManager
    void setLightningHost(irr::f32 time) { isLightningHost = true; lightningEndTime = time; };

    /// Sets the color of the cloud when it is not a lightning host
    void setDefaultColor(irr::video::SColor color) { DefaultColor = color; getMaterial(0).AmbientColor = DefaultColor; };

    void setCloudParticleSize(irr::f32 newSize) { cloudParticleSize = newSize; }


    //! Returns the material based on the zero based index i.
    video::SMaterial& getMaterial(u32 i);

    //! Returns amount of materials used by this scene node.
    u32 getMaterialCount() const;

    /// @return The current number of particles on screen
    s16 getCurrentParticleCount();

    //! returns the axis aligned bounding box of this node
    const core::aabbox3d<f32>& getBoundingBox() const;

    int rand2(int lim, int seed);

    ICloudLayer* getCloudLayer() const { return CloudLayer; };


private:
    // functions
    void setParticleTextureCoordinates();
    void reallocateBuffers();
    void sortParticles();
    void makeParticles();
    s16 makeChildClouds(SCloudParticle &parent,s16 start,core::matrix4 &m, core::vector3df &campos );

    ICloudLayer* CloudLayer;

    // shared variables
    core::array<SCloudParticle>      Particles; //
    core::array<video::S3DVertex>    Vertices; // 4 vertices per particle
    core::array<u16>                 Indices; // 6 indices per particle

    // instance variables
    core::list<IParticleAffector*>   AffectorList;


    core::array<SCloudParticle>      ParticleData;
    u32                              ParticleCount;
    u32                              ParticlesToDraw;

    u32                             depth; // for max depth

    bool                            ZOrdering;  // performance gain for fugly cloud
    bool                            SkyClouds; //
    f32                             SkyMoveFactor; //
    bool                            IgnoreChildColor;
    s16                             MaxDepth;
    f32                             LOD;

    ITimer                          *timer;
    u32                             MaxTimePerFrame;    // maximum time allowed to render
    u32                             lasttime;           // last time to draw frame
    u32                             lastcount;          // last number of polys

    video::SMaterial                Material;
    core::aabbox3d<f32>             Box;

    core::array<SCloudParticle*>    tmppointers;
    core::array<SCloudParticle>     tmpclouds;

    irr::f32 cloudParticleSize;

    bool isLightningHost;
    irr::f32 lightningEndTime;

    irr::video::SColor DefaultColor;

    irr::u16 CoarseIndices[6];
    irr::video::S3DVertex CoarseVertices[4];

    irr::u32 CurrentTime;
    irr::u32 LastImposterUpdateTime;

    irr::core::vector3df LastCameraPosition;
    irr::core::vector3df LastCameraTarget;

    bool FirstRun;

};

} // end namespace scene
} // end namespace irr


#endif
