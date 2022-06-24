#ifndef __SHADER_H_INCLUDED__
#define __SHADER_H_INCLUDED__

#include <ISceneManager.h>
#include <IQ3Shader.h>
#include <ITexture.h>
#include <SMaterial.h>
#include <irrString.h>
#include <irrList.h>
#include "iparser.h"
#include "StageShader.h"

namespace irr
{
namespace scene
{
namespace quake3
{

class Shader : public IReferenceCounted
{
public:
    Shader( const quake3::SShader *shader, io::IFileSystem * fileSystem, scene::ISceneManager * SceneManager );

    Shader( scene::ISceneManager * sceneManager, const video::SMaterial & material );

    ~Shader();

    void parse(const quake3::SShader *shader, io::IFileSystem * fileSystem);

    void clear();

    bool isTransparent() const { return IsTransparent; }

    video::SMaterial &getMaterial() { return Material; }

    const video::SMaterial &getMaterial() const { return Material; }

    core::list<IStageShader*>& getStages( video::S3DVertex2TCoords *vsrc, video::S3DVertex2TCoords *vdst, u32 vsize );

    const core::list<IStageShader*>& getStages() const { return Stages; }

    core::list<IStageShader*>& getStages() { return Stages; }

    video::SMaterial &getStageMaterial() { return State.material; }

    bool isNull() const { return IsNull; }

protected:
    friend class StageShader;
    friend class NullStageShader;

    SStateCommadShaderStage &getState() { return State; }

private:
    core::list<IStageShader*> Stages;
    bool IsTransparent, BackfaceCulling;
    SStateCommadShaderStage State;
    video::SMaterial Material;
    bool IsNull;
};

typedef core::array<quake3::SShader> shaders_t;


class ShadersParser : public IParser <quake3::SShader>
{
public:
    ShadersParser(Script & script) : IParser<quake3::SShader>(&script) { parse(); }

    ShadersParser(Script * script) : IParser<quake3::SShader>(script) { parse(); }

    ShadersParser(io::IReadFile* file, u32 size=0, s32 offset = 0)
          : IParser<quake3::SShader>( new Script(file, size, offset) ) { parse(); }

    ShadersParser(const c8* name, const c8* script, u32 size=0, u32 offset = 0)
          : IParser<quake3::SShader>( new Script(name, script, size, offset) ) { parse(); }

    ~ShadersParser() {   }

    virtual const quake3::SShader * get(u32 index) const
    {
        return ( index < Shader.size() ? &Shader[index] : NULL );
    }

    virtual s32 find(const c8 * name) const
    {
        quake3::SShader sh;
        sh.name = name;
        return Shader.linear_search( sh );
    }

    virtual s32 findShaderByTexture(const core::stringc textureName);

    virtual s32 findShaderByTexture(const c8 * textureName) {
        return (textureName ? findShaderByTexture(core::stringc(textureName)) : -1);
    }

    virtual s32 findShaderByTexture(video::ITexture * texture)
    {
        return (texture ? findShaderByTexture(texture->getName()) : -1);
    }

    virtual u32 size() const { return Shader.size(); }

protected:
    virtual void parse();

private:
    shaders_t Shader;
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr

#endif
