//============================================================================//
//
// date: 17.12.2008
//============================================================================//
//////////////////////////////////////////////////h
#define _IRR_COMPILE_WITH_MDL7_MESH_LOADER_
#ifndef __C_MDL7_MESH_LOADER_H_INCLUDED__
#define __C_MDL7_MESH_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "IAnimatedMesh.h"
#include "CMeshBuffer.h"
#include "IVideoDriver.h"
#include "../M8M/CM8MMeshLoader.h"

//////////////////////////////////////////////////h
namespace irr
{
namespace scene
{
    class IMeshBuffer;
    class ISceneManager;

    class IMDL7Mesh:public IAnimatedMesh
    {
    public:typedef core::vector3d<f32> v3f;typedef core::vector2d<f32> v2f;

        struct SFDatax{
            core::stringc name;
            u16 start;
            u16 end;
            u16 fps;
        };
        struct STCoordx{
            f32 s;
            f32 t;
        };
        struct STriangx{
            u16 vertexIndices[3];
            u16 textureIndices[3];
            s32 null;
        };

        IMDL7Mesh(scene::ISceneManager*smgr);

        virtual ~IMDL7Mesh();

        virtual bool loadFile(io::IReadFile*file);

        virtual u32 getFrameCount()const;

        virtual IMesh*getMesh(s32 frame,s32 detailLevel=255,s32 startFrame=-1,s32 endFrame=-1);

        virtual u32 getMeshBufferCount()const;

        virtual IMeshBuffer*getMeshBuffer(u32 nr)const;

        virtual IMeshBuffer*getMeshBuffer(const video::SMaterial &material)const;

        virtual const core::aabbox3d<f32> &getBoundingBox()const;

        virtual void setBoundingBox(const core::aabbox3df &box);

        virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag,bool newvalue);

        virtual E_ANIMATED_MESH_TYPE getMeshType()const;

        virtual bool getFrameLoop(const c8*name,s32 &outStart,s32 &outEnd,s32 &outFps)const;

        virtual u32 getAnimationCount()const;

        virtual const c8*getAnimationName(u32 nr)const;

        virtual video::ITexture*getTexture(bool nr)const;
        virtual video::IImage*getSkin(bool nr)const;
        virtual void setSkin(bool nr,video::IImage*im);

        virtual bool WriteM8M(const c8*fname,bool nr,const c8*mnx=0,const u16*mvx=0,bool ni=0);
        virtual core::array<v3f>*getVertList()const;
        virtual core::array<SFDatax> getFData()const;
        virtual STCoordx*getMTCoords()const;
        virtual STriangx*getMTriangs()const;
        virtual s32 getnsk()const;
        virtual s32 getnvt()const;
        virtual s32 getntr()const;
        virtual s32 getnsv()const;
        virtual s32 getskt()const;
        virtual s32 getskw()const;
        virtual s32 getskh()const;

    private:

        virtual void updateIntpBfr(s32 frame,s32 startFrame,s32 endFrame);
        virtual void calculateBoundingBox();
        SMeshBuffer*IntpBfr;
        core::array<v3f>*VertList;core::array<v3f>*NormList;
        core::array<SFDatax> FData;

        //////////e
        STCoordx*MTCoords;
        STriangx*MTriangs;
        ISceneManager*SceneManager;
        s32 nsk;       //1
        s32 nvt;       //86
        s32 ntr;       //164
        s32 nfr;       //6
        s32 nsv;       //39
        s32 skt;       //88
        s32 skw;       //256
        s32 skh;       //256
        video::IImage*skbm[2];
        virtual void Trisx(STriangx*t0,STriangx*t1,s32 i);
        //////////e
    };
} // end namespace scene
} // end namespace irr

//////////////////////////////////////////////////h
namespace irr
{
namespace scene
{
    class CMDL7MeshLoader:public IMeshLoader
    {
    public:

       CMDL7MeshLoader(scene::ISceneManager*smgr);

       virtual bool isALoadableFileExtension(const c8*fileName)const;

       virtual IAnimatedMesh*createMesh(io::IReadFile*file);

    private:

       ISceneManager*SceneManager;
    };

} // end namespace scene
} // end namespace irr

#endif // __C_MDL7_MESH_LOADER_H_INCLUDED__
//////////////////////////////////////////////////h
