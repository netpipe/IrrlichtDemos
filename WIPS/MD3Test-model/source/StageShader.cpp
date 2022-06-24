#include <os.h>
#include <irrString.h>
#include <IMaterialRenderer.h>

#include "StageShader.h"
#include "shader.h"

namespace irr
{
namespace scene
{
namespace quake3
{
/*********************************************************************************************/
/**                                   load ??? MapTextures                                  **/
/*********************************************************************************************/

void loadLightMapTextures(quake3::tTexArray &Lightmap, const quake3::SVarGroup *group, io::IFileSystem *fileSystem, video::IVideoDriver *driver)
{
    const core::stringc &lightmapname = group->get("q3map_lightimage");
    if (lightmapname.size() > 0)
    {
        u32 pos = 0;
        quake3::getTextures(Lightmap, lightmapname, pos, fileSystem, driver);
    }
}

void loadMapTextures(quake3::tTexArray &Texture, quake3::tTexArray &Lightmap, const quake3::SVarGroup *group, io::IFileSystem *fileSystem, video::IVideoDriver *driver)
{
	const core::stringc &mapname = group->get("map");
	if (mapname.size() > 0)
	{
		if (mapname == "$lightmap")
		    loadLightMapTextures(Lightmap, group, fileSystem, driver);
		else
		{
			u32 pos = 0;
			quake3::getTextures(Texture, mapname, pos, fileSystem, driver);
		}
	}
}

void loadAnimMapTextures(quake3::tTexArray &Texture, f32 &TextureFrequency, const quake3::SVarGroup *group, io::IFileSystem *fileSystem, video::IVideoDriver *driver)
{
    const core::stringc &animmap = group->get( "animmap" );
    if (animmap.size() > 0)
    {
        u32 pos = 0;
        TextureFrequency = core::max_( 0.0001f, quake3::getAsFloat( animmap, pos ) );
        quake3::getTextures(Texture, animmap, pos, fileSystem, driver);
    }
}

void loadClampMapTextures(quake3::tTexArray &Texture, video::E_TEXTURE_CLAMP &TextureAddressMode, const quake3::SVarGroup *group, io::IFileSystem *fileSystem, video::IVideoDriver *driver)
{
    const core::stringc &clampmap = group->get( "clampmap" );
    if (clampmap.size() > 0)
    {
        TextureAddressMode = video::ETC_CLAMP;
        u32 pos = 0;
        quake3::getTextures(Texture, clampmap, pos, fileSystem, driver);
    }
}

/*********************************************************************************************/
/**                                   StageShader                                           **/
/*********************************************************************************************/

StageShader::StageShader(Shader &shader)
    : IStageShader(shader),
      Lightmap(NULL),
      TextureIndex(0),
      TextureFrequency(0.f)
{
    TextureAddressMode = Owner.getMaterial().TextureLayer[0].TextureWrap;
    ZBuffer            = Owner.getMaterial().ZBuffer;
    ZWriteEnable       = Owner.getMaterial().ZWriteEnable;
    MaterialType       = Owner.getMaterial().MaterialType;
    MaterialTypeParam  = Owner.getMaterial().MaterialTypeParam;
}

StageShader::~StageShader()
{
    free();
}

void StageShader::free()
{
    u32 i;
    for (i = 0; i < Commands.size(); ++i)
           delete Commands[i];
    Commands.clear();

    Texture.clear();

    Lightmap = NULL;
    TextureIndex = 0;
    TextureFrequency = 0.f;

    video::SMaterial m;
    TextureAddressMode = m.TextureLayer[0].TextureWrap;
    ZBuffer            = m.ZBuffer;
    ZWriteEnable       = m.ZWriteEnable;
    MaterialType       = m.MaterialType;
    MaterialTypeParam  = m.MaterialTypeParam;
}

void StageShader::loadTextures(const quake3::SVarGroup *group, io::IFileSystem * fileSystem)
{
	if (group && fileSystem && Owner.getState().SceneManager)
	{
       video::IVideoDriver *driver = Owner.getState().SceneManager->getVideoDriver();

       quake3::tTexArray lightmap;

       loadMapTextures(Texture, lightmap, group, fileSystem, driver);
       if (lightmap.size() > 0)
           Lightmap = lightmap[0];

       if (Texture.size() == 0)
           loadAnimMapTextures(Texture, TextureFrequency, group, fileSystem, driver);

       if (Texture.size() == 0)
           loadClampMapTextures(Texture, TextureAddressMode, group, fileSystem, driver);
	}
}


void StageShader::loadCommand(const quake3::SVarGroup *group)
{
 // Walk group for all modifiers.
    u32 g;
	for ( g = 0; g != group->Variable.size(); ++g )
	{
	    const quake3::SVariable &v = group->Variable[g];
	    ICommandStageShader* cmd;
	    cmd = ICommandStageShader::newInstance(v);
	    if (cmd)
	        Commands.push_back(cmd);
	}
}

void StageShader::loadMaterial(const quake3::SVarGroup *group)
{
    getDepthMaterial(group);
    getBlendMaterial(group);
}

inline void StageShader::getDepthMaterial(const quake3::SVarGroup *group)
{

 // Depth function.
    static const c8 * funclist[] = { "lequal","equal" };

    u32 pos = 0;
    s32 r = 0;

    const core::stringc &string = group->get("depthfunc");

    if ( string.size() > 0 && (r = isEqual(string, pos, funclist, 2)) == -2)
        r = 0;

    ZBuffer = (char) (r + 1);

 // Depth write.
    ZWriteEnable = ( group->getIndex( "depthwrite" ) >= 0 );
}

inline void StageShader::getBlendMaterial(const quake3::SVarGroup *group)
{
    static const u32 blendFuncListSize = 2;
    static const c8* blendFuncList[] =
    {
        "blendfunc", "alphafunc"
    };

    u32 i;
    for ( i = 0; i < blendFuncListSize; ++i )
    {
        const core::stringc &string = group->get( blendFuncList[i] );
        if ( string.size() == 0 )
			continue;

		// maps to E_BLEND_FACTOR
		static const c8 * funclist[] =
		{
			"gl_zero",
			"gl_one",
			"gl_dst_color",
			"gl_one_minus_dst_color",
			"gl_src_color",
			"gl_one_minus_src_color",
			"gl_src_alpha",
			"gl_one_minus_src_alpha",
			"gl_dst_alpha",
			"gl_one_minus_dst_alpha",
			"gl_src_alpha_sat",

			"add",
			"filter",
			"blend",

			"ge128",
			"gt0"
		};


		u32 pos = 0;
		s32 srcFact = isEqual ( string, pos, funclist, 16 );

		if ( srcFact < 0 )
			continue;

		u32 resolved = 0;
		s32 dstFact = isEqual ( string, pos, funclist, 16 );

		switch ( srcFact )
		{
			case video::EBF_ONE:
				switch ( dstFact )
				{
					// gl_one gl_zero
					case video::EBF_ZERO:
						MaterialType = video::EMT_SOLID;
						IsTransparent = false;
						resolved = 1;
						break;

					// gl_one gl_one
					case video::EBF_ONE:
						MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
						IsTransparent = true;
						resolved = 1;
						break;
				} break;

			case video::EBF_SRC_ALPHA:
				switch ( dstFact )
				{
					// gl_src_alpha gl_one_minus_src_alpha
					case video::EBF_ONE_MINUS_SRC_ALPHA:
						MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
						MaterialTypeParam = 1.f / 255.f;
						IsTransparent = true;
						resolved = 1;
						break;
				} break;

			case 11:
				// add
				MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
				IsTransparent = true;
				resolved = 1;
				break;
			case 12:
				// filter = gl_dst_color gl_zero
				MaterialType = video::EMT_ONETEXTURE_BLEND;
				MaterialTypeParam = video::pack_texureBlendFunc ( video::EBF_DST_COLOR, video::EBF_ZERO, defaultModulate );
				IsTransparent = false;
				resolved = 1;
				break;
			case 13:
				// blend
				MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
				MaterialTypeParam = 1.f / 255.f;
				IsTransparent = true;
				resolved = 1;
				break;
			case 14:
				// alphafunc ge128
				MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				MaterialTypeParam = 0.5f;
				IsTransparent = true;
				resolved = 1;
				break;
			case 15:
				// alphafunc gt0
				MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				MaterialTypeParam = 1.f / 255.f;
				IsTransparent = true;
				resolved = 1;
				break;
		}

		// use the generic blender
		if ( 0 == resolved )
		{
			MaterialType = video::EMT_ONETEXTURE_BLEND;
			MaterialTypeParam = video::pack_texureBlendFunc (
					(video::E_BLEND_FACTOR) srcFact,
					(video::E_BLEND_FACTOR) dstFact,
					defaultModulate);

			if (srcFact == video::EBF_SRC_COLOR && dstFact == video::EBF_ZERO)
			{
				IsTransparent = 0;
			}
			else
			{
				IsTransparent = true;
			}

		}
    } // for ( i=...
}

void StageShader::parse(const quake3::SVarGroup *group, io::IFileSystem * fileSystem)
{
    free();
    loadTextures(group, fileSystem);
    loadCommand(group);
    loadMaterial(group);
}

bool StageShader::animate(f32 timeMs)
{
    SStateCommadShaderStage &state = Owner.getState();

 // Init stage operation.
    state.dt = timeMs;
    state.m2 = core::matrix4();
    state.texture = core::matrix4();
    state.function = quake3::SModifierFunction();
    state.tcgen = NULL;

 // select current texture
	if ( TextureFrequency != 0.f )
	{
		s32 v = core::floor32( state.dt * TextureFrequency );
		TextureIndex = v % Texture.size();
	}

 // Walk group for all commands.
    u32 i;
    for ( i = 0; i < Commands.size(); ++i)
        Commands[i]->execute(state);

    if (state.tcgen)
        state.tcgen(state);


 // End if not drawable.
    if (!isDrawable())
        return false;

 // Updating final material.
    state.material.setTexture( 0, Texture[TextureIndex] );
    state.material.TextureLayer[0].TextureWrap = TextureAddressMode;
    state.material.setTextureMatrix( 0, state.texture );
    state.material.ZBuffer = ZBuffer;
    //state.material.ZWriteEnable = ZWriteEnable;
    state.material.MaterialType = MaterialType;
    state.material.MaterialTypeParam = MaterialTypeParam;

    return isDrawable();
}


inline bool NullStageShader::isTransparent() const
{
    if (Owner.getState().SceneManager)
    {
        video::IMaterialRenderer* rnd =
               Owner.getState().SceneManager->getVideoDriver()->getMaterialRenderer(
                        Owner.getMaterial().MaterialType
                        );

        return (rnd && rnd->isTransparent());
    }
    else
        return false;
}


inline bool NullStageShader::animate(f32 timeMs)
{
    return isDrawable();
}

} // end namespace quake3
} // end namespace scene
} // end namespace irr
