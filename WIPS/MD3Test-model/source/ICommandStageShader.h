#ifndef __I_COMMAND_STAGE_SHADER_H_INCLUDED__
#define __I_COMMAND_STAGE_SHADER_H_INCLUDED__

#include <ISceneManager.h>
#include <IQ3Shader.h>
#include <ITexture.h>
#include <SMaterial.h>
#include <irrString.h>

namespace irr
{
namespace scene
{
namespace quake3
{

enum E_SHADER_COMMAND
{
    ECM_TCMOD = 0,
    ECM_DEFORMVERTEXES,
    ECM_RGBGEN,
    ECM_TCGEN,
    ECM_MAP,
    ECM_COUNT
};

enum E_SHADER_FUNCTION
{
    SFN_NONE = -1,
    SFN_SCROLL = 0,
    SFN_SCALE,
    SFN_ROTATE,
    SFN_STRETCH,
    SFN_TURB,
    SFN_WAVE,
    SFN_IDENTITY,
    SFN_VERTEX,
    SFN_TEXTURE,
    SFN_LIGHTMAP,
    SFN_ENVIRONMENT,
    SFN_SLIGHTMAP,
    SFN_BULGE,
    SFN_AUTOSPRITE,
    SFN_AUTOSPRITE2,
    SFN_LIGHTINGDIFFUSE,
    SFN_COUNT
};

//
// Vertex processing functions.
//

struct SStateCommadShaderStage;

typedef void (*VetexTransformShader_t) (SStateCommadShaderStage &state);

void cmd_Wave( SStateCommadShaderStage &state );
void cmd_Bulge( SStateCommadShaderStage &state );
void cmd_AutoSprite( SStateCommadShaderStage &state );
void cmd_RgbGenIdentity( SStateCommadShaderStage &state );
void cmd_RgbGenVertex( SStateCommadShaderStage &state );
void cmd_RgbGenWave( SStateCommadShaderStage &state );
void cmd_TcGenTurb( SStateCommadShaderStage &state );
void cmd_TcGenTexture( SStateCommadShaderStage &state );
void cmd_TcGenLightmap( SStateCommadShaderStage &state );
void cmd_TcGenEnvironment( SStateCommadShaderStage &state );

struct SStateCommadShaderStage
{
    SStateCommadShaderStage()
        : dt(0.f),
          vsrc(NULL), vdst(NULL), vsize(0),
          SceneManager(NULL),
          tcgen(NULL)
    {
    }

    f32 dt;
    core::matrix4 m2, texture;
    quake3::SModifierFunction function;
    video::S3DVertex2TCoords *vsrc;
    video::S3DVertex2TCoords *vdst;
    u32 vsize;
    scene::ISceneManager * SceneManager;
    VetexTransformShader_t tcgen;
    video::SMaterial material;
};

class ICommandStageShader
{
public:
    ICommandStageShader(E_SHADER_COMMAND command) : Command(command) {}

    virtual ~ICommandStageShader() {}

    virtual void parse(const quake3::SVariable &v, u32 &pos) = 0;

    virtual void operate(SStateCommadShaderStage &state) = 0;

    virtual void execute(SStateCommadShaderStage &state)
    {
        this->operate(state);
    }

    virtual E_SHADER_COMMAND getCommand() const { return Command; }

    virtual E_SHADER_FUNCTION getFunction() const = 0;

    static ICommandStageShader* newInstance(const quake3::SVariable &v);

private:
    E_SHADER_COMMAND Command;
};

class ITcmodCommandStageShader : public ICommandStageShader
{
public:
    ITcmodCommandStageShader() : ICommandStageShader(ECM_TCMOD) {}

    virtual ~ITcmodCommandStageShader() {}

    virtual void execute(SStateCommadShaderStage &state)
    {
        state.m2.makeIdentity();
        this->operate(state);
        state.texture *= state.m2;
    }
};



} // end namespace quake3
} // end namespace scene
} // end namespace irr


#endif

