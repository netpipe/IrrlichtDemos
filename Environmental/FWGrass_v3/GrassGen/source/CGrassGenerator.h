#ifndef _C_GRASS_GENERATOR_H_
#define _C_GRASS_GENERATOR_H_

#include "irrlicht.h"

//enclose the class in a namespace, things inside the
//namespace become accessible only with GrassGenerator::
//in front or if "using namespace GrassGenerator;" is
//declared. A bit like irr::io::path
namespace GrassGenerator
{

//The Vegetation Type Struct
struct VegetationTypeParams
{
    irr::u32           colorMaks;
    irr::s32           ratio;
    irr::f32           scale;
    irr::f32           position_var;
    irr::f32           scale_var;
    irr::scene::IMesh* pMesh;
    irr::core::stringc texturePath;

    bool equals(VegetationTypeParams& other)
    {
        return other.colorMaks == colorMaks && other.ratio == ratio && other.scale == scale
                && other.position_var == position_var && other.scale_var == scale_var
                && pMesh == other.pMesh && other.texturePath == texturePath;
    }
};

//The class definition, the actual function code is
//contained in the implementation file (source, .cpp)
class CGrassGenerator
{
    //These functions can be accessed from the outside
    //i.e. the main() function. The load() and writeOut()
    //functions take const parameters because they are
    //references to avoid copying memory around and are
    //not allowed to change the original value.
    public:
        //The constructor sets up the access to irrlicht's
        //functions
        CGrassGenerator(irr::IrrlichtDevice* Irrlicht);
        ~CGrassGenerator();

        void removeChannelTextures();
        void clearChannelArrays();
        bool load(const irr::io::path& xmlFilename);
        void process();
        bool writeOut(const irr::io::path& Filename);
        irr::scene::ISceneNode* getTerrainNode() {return pTerrainNode;}
        irr::video::IImage* getVegetationMap() {return pVegetationMap;}

    //The following are private, therefore unique to each
    //instance of CGrassGenerator. They are also declared
    //constant so the data cannot be modified once set.
    private:
        int                                    m_mb2tAlloc;
        int                                    m_mbtanAlloc;
        float                                  density;
        irr::c8                                up_axis;
        irr::scene::ITriangleSelector*         pTerrainModel;
        irr::scene::ISceneNode*                pTerrainNode;
        irr::video::IImage*                    pVegetationMap;
        irr::core::array<VegetationTypeParams> allVTs;
        irr::core::array<VegetationTypeParams> channelSortedVTs[3];
        irr::core::array<irr::u32>             channelSortedVTsLimits[3];
        irr::u32                               channelSortedVTsRatioSums[3];

        //processing values
        irr::core::vector2df                               scale;
        irr::f32*                                          mb2tQuadXCount;
        irr::f32*                                          mb2tQuadScale;
        irr::core::array<irr::scene::SMeshBufferLightMap*> mb2tVec;
        irr::f32*                                          mbtanQuadXCount;
        irr::f32*                                          mbtanQuadScale;
        irr::core::array<irr::scene::SMeshBufferTangents*> mbtanVec;
        irr::u32                                           mbtanSize;

        //we need to store some irrlicht subsystem's references
        irr::io::IFileSystem*      fileSystem;
        irr::video::IVideoDriver*  video;
        irr::scene::ISceneManager* smgr;
        irr::IrrlichtDevice*       irrlicht;
};

}

#endif
