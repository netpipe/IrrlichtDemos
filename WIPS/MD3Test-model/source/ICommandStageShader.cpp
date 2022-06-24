#include <os.h>
#include <irrMath.h>

#include <ICameraSceneNode.h>
#include <SViewFrustum.h>
#include "StageShader.h"
#include "shader.h"

namespace irr
{
namespace scene
{
namespace quake3
{

/*********************************************************************************************/
/**                 Vertex processing functions (VetexTransformShader_t).                   **/
/*********************************************************************************************/
void cmd_Wave(SStateCommadShaderStage &state)
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

	state.function.freq = core::reciprocal( state.function.freq );

	const f32 phase = state.function.phase;

	for ( u32 i = 0; i != state.vsize; ++i, ++src, ++dst )
	{
		const f32 wavephase = (src->Pos.X + src->Pos.Y + src->Pos.Z) * state.function.freq;
		state.function.phase = phase + wavephase;

		const f32 f = state.function.evaluate( state.dt );

		dst->Pos.X = src->Pos.X + f * src->Normal.X;
		dst->Pos.Y = src->Pos.Y + f * src->Normal.Y;
		dst->Pos.Z = src->Pos.Z + f * src->Normal.Z;
	}
}


void cmd_Bulge( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

	state.function.func = 0;
	state.function.wave = core::reciprocal( state.function.bulgewidth );

	state.dt *= state.function.bulgespeed * 0.1f;
	const f32 phase = state.function.phase;

	for ( u32 i = 0; i != state.vsize; ++i, ++src, ++dst )
	{
		const f32 wavephase = ( src->TCoords.X ) * state.function.wave;
		state.function.phase = phase + wavephase;

		const f32 f = state.function.evaluate( state.dt );

		dst->Pos.X = src->Pos.X + f * src->Normal.X;
		dst->Pos.Y = src->Pos.Y + f * src->Normal.Y;
		dst->Pos.Z = src->Pos.Z + f * src->Normal.Z;
	}
}


void cmd_AutoSprite( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

	const core::matrix4 &m = state.SceneManager->getActiveCamera()->getViewFrustum()->Matrices [ video::ETS_VIEW ];
	const core::vector3df view( -m[2], -m[6] , -m[10] );

	core::aabbox3df box;
	u32 g;

	for ( u32 i = 0; i < state.vsize; i += 4, dst += 4 )
	{
		// in pairs of 4
		box.reset( src->Pos );
		for ( g = 1, ++src; g != 4; ++g, ++src )
		{
			box.addInternalPoint( src->Pos );
		}

		core::vector3df c = box.getCenter();
		f32 sh = 0.5f * ( box.MaxEdge.Z - box.MinEdge.Z );
		f32 sv = 0.5f * ( box.MaxEdge.Y - box.MinEdge.Y );

		const core::vector3df h( m[0] * sh, m[4] * sh, m[8] * sh );
		const core::vector3df v( m[1] * sv, m[5] * sv, m[9] * sv );

		(dst + 0 )->Pos = c + h + v;
		(dst + 1 )->Pos = c - h - v;
		(dst + 2 )->Pos = c + h - v;
		(dst + 3 )->Pos = c - h + v;

		(dst + 0 )->Normal = view;
		(dst + 1 )->Normal = view;
		(dst + 2 )->Normal = view;
		(dst + 3 )->Normal = view;
	}
}

void cmd_RgbGenIdentity( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *dst = state.vdst;
	u32 i;
    for (i = 0; i != state.vsize; ++i, ++dst)
        dst->Color = 0xFFFFFFFF;
}

void cmd_RgbGenVertex( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;
	u32 i;
	for (i = 0; i != state.vsize; ++i, ++src, ++dst)
		dst->Color = src->Color;
}

void cmd_RgbGenWave( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *dst = state.vdst;

	f32 f = state.function.evaluate( state.dt ) * 255.f;
	s32 value = core::clamp( core::floor32(f), 0, 255 );
	value |= value << 8;
	value |= value << 16;

	u32 i;
    for ( i = 0; i != state.vsize; ++i, ++dst )
        dst->Color = value;
}

void cmd_TcGenTurb( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

    state.function.wave = core::reciprocal( state.function.wave );

    const f32 phase = state.function.phase;

    u32 i;
    for ( i = 0; i != state.vsize; ++i, ++src, ++dst )
    {
        const f32 wavephase = (src->Pos.X + src->Pos.Y + src->Pos.Z) * state.function.wave;
        state.function.phase = phase + wavephase;

        const f32 f = state.function.evaluate( state.dt );

        dst->TCoords.X = src->TCoords.X + f * src->Normal.X;
        dst->TCoords.Y = src->TCoords.Y + f * src->Normal.Y;
    }
}

void cmd_TcGenTexture( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

    u32 i;
    for ( i = 0; i != state.vsize; ++i, ++src, ++dst )
        dst->TCoords = src->TCoords;
}

void cmd_TcGenLightmap( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

    u32 i;
    for ( i = 0; i != state.vsize; ++i, ++src, ++dst )
        dst->TCoords = src->TCoords2;
}

void cmd_TcGenEnvironment( SStateCommadShaderStage &state )
{
	video::S3DVertex2TCoords *src = state.vsrc;
	video::S3DVertex2TCoords *dst = state.vdst;

 // using eye linear, sphere map may be cooler;-)
 // modelmatrix is identity
    const core::matrix4 &view = state.SceneManager->getActiveCamera()->getViewFrustum()->Matrices [ video::ETS_VIEW ];
    const core::matrix4 &viewinverse = state.SceneManager->getActiveCamera()->getViewFrustum()->Matrices [ SViewFrustum::ETS_VIEW_MODEL_INVERSE_3 ];

 // eyePlane
    core::vector3df eyePlaneS;
    core::vector3df eyePlaneT;

    viewinverse.transformVect( eyePlaneS, core::vector3df(1.f, 0.f, 0.f) );
    viewinverse.transformVect( eyePlaneT, core::vector3df(0.f, 1.f, 0.f) );

    eyePlaneS.normalize();
    eyePlaneT.normalize();

    core::vector3df v;

    u32 i;
    for ( i = 0; i != state.vsize; ++i, ++src, ++dst )
    {
     // vertex in eye space
        view.transformVect( v, src->Pos );
        v.normalize();

        dst->TCoords.X = (1.f + eyePlaneS.dotProduct(v) ) * 0.5f;
        dst->TCoords.Y = 1.f - ( (1.f + eyePlaneT.dotProduct(v) ) * 0.5f );
    }
}


void cmd_RgbGenLightingDiffuse( SStateCommadShaderStage &state )
{
    video::IVideoDriver  *driver;
    video::SColor         ambientLight;
    u32                   ambientLightInt;

    driver = state.SceneManager->getVideoDriver();

    ambientLight = state.SceneManager->getAmbientLight().toSColor();
    ambientLightInt = ambientLight.color;

    u32 l;
    for (l=0; l < driver->getDynamicLightCount(); ++l)
    {
        //if (state.p
    }
}

/*********************************************************************************************/
/**                                   Scroll                                                **/
/*********************************************************************************************/

class Scroll : public ITcmodCommandStageShader
{
public:
    Scroll() : F0(0.f), F1(0.f) {    }

    virtual ~Scroll() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        F0 = quake3::getAsFloat( v.content, pos );
        F1 = quake3::getAsFloat( v.content, pos );
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.m2.setTextureTranslate( F0 * state.dt, F1 * state.dt );
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_SCROLL;
    }

    f32 F0;
    f32 F1;
};

/*********************************************************************************************/
/**                                   Scale                                                 **/
/*********************************************************************************************/

class Scale : public ITcmodCommandStageShader
{
public:
    Scale() : F0(0.f), F1(0.f) {    }

    virtual ~Scale() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        F0 = quake3::getAsFloat( v.content, pos );
        F1 = quake3::getAsFloat( v.content, pos );
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.m2.setTextureTranslate( F0, F1 );
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_SCALE;
    }

    f32 F0;
    f32 F1;
};

/*********************************************************************************************/
/**                                   Rotate                                                **/
/*********************************************************************************************/

class Rotate : public ITcmodCommandStageShader
{
public:
    Rotate() : F0(0.f) {    }

    virtual ~Rotate() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        F0 = quake3::getAsFloat( v.content, pos ) * core::DEGTORAD;
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.m2.setTextureRotationCenter( F0 * state.dt );
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_ROTATE;
    }

    f32 F0;
};

/*********************************************************************************************/
/**                                   Stretch                                               **/
/*********************************************************************************************/

class Stretch : public ITcmodCommandStageShader
{
public:
    Stretch() : Func(0), Base(0.f), Amp(0.f), Phase(0.f), Freq(0.f) {    }

    virtual ~Stretch() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        SModifierFunction function;
        quake3::getModifierFunc( function, v.content, pos );
        Func  = function.func;
		Base  = function.base;
		Amp   = function.amp;
		Phase = function.phase;
		Freq  = function.freq;
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.func  = Func;
		state.function.base  = Base;
		state.function.amp   = Amp;
		state.function.phase = Phase;
		state.function.freq  = Freq;

        f32 f0;
        f0 = core::reciprocal( state.function.evaluate(state.dt) );

		state.m2.setTextureScaleCenter( f0, f0 );
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_STRETCH;
    }

    s32 Func;
    f32 Base;
    f32 Amp;
    f32 Phase;
    f32 Freq;
};

/*********************************************************************************************/
/**                                   Turb                                                  **/
/*********************************************************************************************/

class Turb : public ITcmodCommandStageShader
{
public:
    Turb() : Func(0), Base(0.f), Amp(0.f), Phase(0.f), Freq(0.f) {    }

    virtual ~Turb() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        SModifierFunction function;
        quake3::getModifierFunc( function, v.content, pos );
        Func  = function.func;
		Base  = function.base;
		Amp   = function.amp;
		Phase = function.phase;
		Freq  = function.freq;
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.func  = Func;
		state.function.base  = Base;
		state.function.amp   = Amp;
		state.function.phase = Phase;
		state.function.freq  = Freq;

        f32 f0, f1, f2;

        quake3::SModifierFunction ampFunction;
		ampFunction.func = 0;
		ampFunction.freq = state.function.freq;
		ampFunction.base = 1.f;
		ampFunction.amp = 0.2f;

        f2 = ampFunction.evaluate( state.dt );
		f0 = state.function.evaluate( state.dt );
        state.function.func = 1;
        f1 = state.function.evaluate( state.dt );

        state.m2.setTextureTranslate( f0, f1 );
		state.m2.setTextureScaleCenter( f2, f2 );
        state.m2.setTextureScale( f2, f2 );
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_TURB;
    }

    s32 Func;
    f32 Base;
    f32 Amp;
    f32 Phase;
    f32 Freq;
};

/*********************************************************************************************/
/**                                   Wave                                                  **/
/*********************************************************************************************/

class Wave : public ICommandStageShader
{
public:
    Wave(E_SHADER_COMMAND command)
       : ICommandStageShader(command),
         Func(0), Base(0.f), Amp(0.f), Phase(0.f), Freq(0.f)
    {    }

    virtual ~Wave() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        SModifierFunction function;

        if (getCommand() == ECM_DEFORMVERTEXES)
        {
            function.wave = quake3::getAsFloat( v.content, pos );
            Modifier = &cmd_Wave;
        }
        else
            Modifier = &cmd_RgbGenWave;

        quake3::getModifierFunc( function, v.content, pos );
        Func  = function.func;
		Base  = function.base;
		Amp   = function.amp;
		Phase = function.phase;
		Freq  = function.freq;
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.func  = Func;
		state.function.base  = Base;
		state.function.amp   = Amp;
		state.function.phase = Phase;
		state.function.freq  = Freq;

		Modifier(state);
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_WAVE;
    }

    s32 Func;
    f32 Base;
    f32 Amp;
    f32 Phase;
    f32 Freq;
    VetexTransformShader_t Modifier;
};

/*********************************************************************************************/
/**                                   Identity                                              **/
/*********************************************************************************************/

class Identity : public ICommandStageShader
{
public:
    Identity(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~Identity() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.func = 0;
        cmd_RgbGenIdentity(state);
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_IDENTITY;
    }
};

/*********************************************************************************************/
/**                                   Vertex                                                **/
/*********************************************************************************************/

class Vertex : public ICommandStageShader
{
public:
    Vertex(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~Vertex() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.func = 0;
        cmd_RgbGenVertex(state);
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_VERTEX;
    }
};

/*********************************************************************************************/
/**                                   Texture                                               **/
/*********************************************************************************************/

class Texture : public ICommandStageShader
{
public:
    Texture(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~Texture() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.tcgen = 8;
        state.tcgen = &cmd_TcGenTexture;
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_TEXTURE;
    }
};

/*********************************************************************************************/
/**                                   Lightmap                                              **/
/*********************************************************************************************/

class Lightmap : public ICommandStageShader
{
public:
    Lightmap(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~Lightmap() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.tcgen = 9;
        state.tcgen = &cmd_TcGenLightmap;
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_LIGHTMAP;
    }
};

/*********************************************************************************************/
/**                                   Environment                                           **/
/*********************************************************************************************/

class Environment : public ICommandStageShader
{
public:
    Environment(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~Environment() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.tcgen = 10;
        state.tcgen = &cmd_TcGenEnvironment;
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_ENVIRONMENT;
    }
};

/*********************************************************************************************/
/**                                   Bulge                                                 **/
/*********************************************************************************************/

class Bulge : public ICommandStageShader
{
public:
    Bulge(E_SHADER_COMMAND command)
       : ICommandStageShader(command),
         Width(0.f), Height(0.f), Speed(0.f)
    {    }

    virtual ~Bulge() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos)
    {
        Width  = quake3::getAsFloat( v.content, pos );
		Height = quake3::getAsFloat( v.content, pos );
		Speed  = quake3::getAsFloat( v.content, pos );
    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        state.function.bulgewidth  = Width;
        state.function.bulgeheight = Height;
        state.function.bulgespeed  = Speed;
        cmd_Bulge(state);
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_BULGE;
    }

    f32 Width;
	f32 Height;
	f32 Speed;
};

/*********************************************************************************************/
/**                                   AutoSprite                                            **/
/*********************************************************************************************/

class AutoSprite : public ICommandStageShader
{
public:
    AutoSprite(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~AutoSprite() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        cmd_AutoSprite(state);
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_AUTOSPRITE;
    }
};

/*********************************************************************************************/
/**                                   LightingDiffuse                                       **/
/*********************************************************************************************/

class LightingDiffuse : public ICommandStageShader
{
public:
    LightingDiffuse(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~LightingDiffuse() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
        cmd_RgbGenLightingDiffuse(state);
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_LIGHTINGDIFFUSE;
    }
};

/*********************************************************************************************/
/**                                   None                                                  **/
/*********************************************************************************************/

class None : public ICommandStageShader
{
public:
    None(E_SHADER_COMMAND command) : ICommandStageShader(command) {    }

    virtual ~None() {    }

    virtual void parse(const quake3::SVariable &v, u32 &pos) {    }

    virtual void operate(SStateCommadShaderStage &state)
    {
    }

    virtual E_SHADER_FUNCTION getFunction() const
    {
        return SFN_NONE;
    }
};

/*********************************************************************************************/
/**                                   ICommandStageShader                                   **/
/*********************************************************************************************/

ICommandStageShader* ICommandStageShader::newInstance(const quake3::SVariable &v)
{
    static const c8 * modifierList[] =
    {
        "tcmod","deformvertexes","rgbgen","tcgen","map"
    };

    static const c8 * funclist[] =
    {
        "scroll",          "scale",     "rotate", "stretch",    "turb",
        "wave",            "identity",  "vertex", "texture",    "lightmap",
        "environment",     "$lightmap", "bulge",  "autosprite", "autosprite2",
        "lightingdiffuse"
    };

    ICommandStageShader* result;
    result = NULL;

    u32 pos;
    s32 masterfunc0;

    pos = 0;
    masterfunc0 = quake3::isEqual( v.name, pos, modifierList, 5 );
    if (masterfunc0 != -2)
    {
        E_SHADER_COMMAND command = (E_SHADER_COMMAND)masterfunc0;
        pos = 0;
        s32 masterfunc1;
		masterfunc1 = quake3::isEqual( v.content, pos, funclist, 16 );

		if (masterfunc1 != -2)
		{
		    E_SHADER_FUNCTION commad_function = (E_SHADER_FUNCTION) masterfunc1;
		    switch (commad_function)
		    {
		        case SFN_SCROLL      : result = new Scroll();             break;
		        case SFN_SCALE       : result = new Scale();              break;
		        case SFN_ROTATE      : result = new Rotate();             break;
		        case SFN_STRETCH     : result = new Stretch();            break;
		        case SFN_TURB        : result = new Turb();               break;
		        case SFN_WAVE        : result = new Wave(command);        break;
		        case SFN_IDENTITY    : result = new Identity(command);    break;
		        case SFN_VERTEX      : result = new Vertex(command);      break;
		        case SFN_TEXTURE     : result = new Texture(command);     break;
		        case SFN_LIGHTMAP    : result = new Lightmap(command);    break;
		        case SFN_ENVIRONMENT : result = new Environment(command); break;
		        case SFN_SLIGHTMAP   : result = new Lightmap(command);    break;
		        case SFN_BULGE       : result = new Bulge(command);       break;
		        case SFN_AUTOSPRITE  : result = new AutoSprite(command);  break;
		        case SFN_AUTOSPRITE2 : result = new AutoSprite(command);  break;
		        case SFN_LIGHTINGDIFFUSE : result = new None(command);break; //result = new LightingDiffuse(command);  break;
		        default              : result = NULL;
		    }

		    _IRR_DEBUG_BREAK_IF(!result) // access violation

		    result->parse(v, pos);
		}
    }

    return (ICommandStageShader*) result;
}


} // end namespace quake3
} // end namespace scene
} // end namespace irr

