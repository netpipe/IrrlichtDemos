//============================================================================//
//
// date: 17.12.2008
//============================================================================//
//////////////////////////////////////////////////h
#define _IRR_COMPILE_WITH_M8M_MESH_LOADER_
#ifndef __C_M8M_MESH_LOADER_H_INCLUDED__
#define __C_M8M_MESH_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "IAnimatedMesh.h"
#include "CMeshBuffer.h"
#include "IVideoDriver.h"

//////////////////////////////////////////////////h
namespace irr
{
namespace scene
{
    class IMeshBuffer;
    class ISceneManager;

    class IM8MMesh:public IAnimatedMesh
    {
    public:typedef core::vector3d<f32> v3f;typedef core::vector2d<f32> v2f;

        struct SFDatax{
            core::stringc name;
            u16 start;
            u16 end;
            u16 fps;
        };
        struct STCoordx{
            s16 s;
            s16 t;
        };
        struct STriangx{
            u16 vertexIndices[3];
            u16 textureIndices[3];
        };

        IM8MMesh(scene::ISceneManager*smgr);

        virtual ~IM8MMesh();

        virtual bool loadFile(io::IReadFile*file);

        virtual bool createMesh(core::array<v3f>*VertListx,
         const core::array<SFDatax> &FDatax,STCoordx*MTCoordsx,STriangx*MTriangsx,
         const s32 &nvtx,const s32 &ntrx,const u32 &nfrx,const s32 &nsvx,video::IImage*im);

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

        //M8M = WriteM8M(fname,0) MDL5 = WriteM8M(fname,1)
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

        virtual u8 getFileType()const;
        virtual const c8*getMName()const;
        virtual const u16*getMv()const;

    private:

        virtual void updateIntpBfr(s32 frame,s32 startFrame,s32 endFrame);
        virtual void calculateBoundingBox();
        SMeshBuffer*IntpBfr;
        core::array<v3f>*VertList;
        core::array<SFDatax> FData;
        
        //////////e
        u8  FTYPE;
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
        c8  mn[10];
        u16 mv[11];
        virtual void Trisx(STriangx*t0,STriangx*t1,s32 i);
        //////////e
    };
    namespace IM8M{
        struct CMRWFile{
         public:
          CMRWFile(const c8*fileName,bool append):File(0),FileSize(0){
           Filename=fileName;openFile(append);}virtual ~CMRWFile(){drop();}
          virtual s32 read(void*bfr,u32 size){if(File){
           return (s32)fread(bfr,1,size,File);}return 0;}
          virtual s32 write(const void*bfr,u32 size){if(File){
           return fwrite(bfr,1,size,File);}return 0;}
          virtual bool seek(long final,bool relative=0){if(File){
           return !fseek(File,final,relative?SEEK_CUR:SEEK_SET);}return 0;}
          virtual long getSize()const{return FileSize;}
          virtual bool isOpen()const{return !!File;}
          virtual long getPos()const{return ftell(File);}
          virtual const c8*getFileName()const{return Filename.c_str();}
          virtual void drop(){if(File){fclose(File);}}
          CMRWFile*createMRWFile(const c8*fileName,bool append){
           CMRWFile*file=new CMRWFile(fileName,append);if(file->isOpen()){
            return file;}delete file;return 0;}
         private:
          virtual void openFile(bool append){if(!Filename.size()){File=0;return;}
           File=fopen(Filename.c_str(),"ab");append*=!!File;if(File){fclose(File);}
           File=fopen(Filename.c_str(),append?"r+b":"w+b");
           if(!File){File=fopen(Filename.c_str(),"rb");}
           if(File){fseek(File,0,SEEK_END);FileSize=getPos();fseek(File,0,SEEK_SET);}}
          core::stringc Filename;FILE*File;long FileSize;
        };}
} // end namespace scene
} // end namespace irr

//////////////////////////////////////////////////h
namespace irr
{
namespace scene
{
    class CM8MMeshLoader:public IMeshLoader
    {
    public:

       CM8MMeshLoader(scene::ISceneManager*smgr);

       virtual bool isALoadableFileExtension(const c8*fileName)const;

       virtual IAnimatedMesh*createMesh(io::IReadFile*file);
    
    private:

       ISceneManager*SceneManager;
    };

} // end namespace scene
} // end namespace irr

#endif // __C_M8M_MESH_LOADER_H_INCLUDED__
//////////////////////////////////////////////////h
