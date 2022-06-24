#ifndef __STAGE_SHADER_H_INCLUDED__
#define __STAGE_SHADER_H_INCLUDED__

#include <IQ3Shader.h>
#include <ITexture.h>
#include <SMaterial.h>

#include "ICommandStageShader.h"

namespace irr
{
namespace scene
{
namespace quake3
{

class Shader;

class IStageShader
{
public:
    IStageShader(Shader &shader) : Owner(shader) {}

    virtual ~IStageShader() {}

    virtual void parse(const quake3::SVarGroup *group, io::IFileSystem * fileSystem) = 0;

    virtual bool animate(f32 timeMs) = 0;

    virtual bool isDrawable() const = 0;

    virtual bool isTransparent() const = 0;

    virtual core::array<ICommandStageShader*>& commands() { return Commands; }

    virtual quake3::tTexArray& textures() { return Texture; }

protected:
    Shader& Owner;
    core::array<ICommandStageShader*> Commands;
    quake3::tTexArray Texture;
};

class StageShader : public IStageShader
{
public:
    StageShader(Shader &shader);

    virtual ~StageShader();

    virtual void parse(const quake3::SVarGroup *group, io::IFileSystem * fileSystem);

    virtual bool animate(f32 timeMs);

    virtual bool isDrawable() const { return Texture.size() > 0; }

    virtual bool isTransparent() const { return IsTransparent; }

private:
    void free();

    void loadTextures(const quake3::SVarGroup *group, io::IFileSystem * fileSystem);

    void loadCommand(const quake3::SVarGroup *group);

    void loadMaterial(const quake3::SVarGroup *group);

    void getBlendMaterial(const quake3::SVarGroup *group);

    void getDepthMaterial(const quake3::SVarGroup *group);

    video::ITexture *Lightmap;
	u32 TextureIndex;
	f32 TextureFrequency;
	video::E_TEXTURE_CLAMP TextureAddressMode;

	char ZBuffer;
	bool ZWriteEnable;
	video::E_MATERIAL_TYPE MaterialType;
	f32 MaterialTypeParam;
	bool IsTransparent;
};

class NullStageShader : public IStageShader
{
public:
    NullStageShader(Shader &shader) : IStageShader(shader) {}

    virtual ~NullStageShader() {}

    virtual void parse(const quake3::SVarGroup *group, io::IFileSystem * fileSystem) {}

    virtual bool animate(f32 timeMs);

    virtual bool isDrawable() const { return true; }

    virtual bool isTransparent() const;
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr


#endif
