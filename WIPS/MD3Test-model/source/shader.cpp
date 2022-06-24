#include <os.h>
#include "shader.h"

namespace irr
{
namespace scene
{
namespace quake3
{

/*********************************************************************************************/
/**                                   Shader                                                **/
/*********************************************************************************************/
Shader::Shader( const quake3::SShader *shader, io::IFileSystem * fileSystem, scene::ISceneManager * sceneManager )
    : IsTransparent(false), BackfaceCulling(true), IsNull(false)
{
    _IRR_DEBUG_BREAK_IF(!sceneManager || !fileSystem) // access violation

    State.SceneManager = sceneManager;

    parse(shader, fileSystem);
}

Shader::Shader( scene::ISceneManager * sceneManager, const video::SMaterial & material )
    : IsTransparent(false), BackfaceCulling(true), IsNull(true)
{
    _IRR_DEBUG_BREAK_IF(!sceneManager) // access violation

    State.SceneManager = sceneManager;
    Material = material;

    IStageShader* stage = new NullStageShader(*this);
    _IRR_DEBUG_BREAK_IF(!stage) // access violation
    Stages.push_back( stage );

    IsTransparent = stage->isTransparent();
}

Shader::~Shader()
{
    clear();
}

void Shader::clear()
{
 // delete all stages
    IStageShader* stage;

    core::list<IStageShader*>::Iterator it = Stages.begin();

    for (; it != Stages.end(); ++it)
    {
        stage = *it;
        delete stage;
    }

    Stages.clear();
}


void Shader::parse(const quake3::SShader *shader, io::IFileSystem * fileSystem)
{
    _IRR_DEBUG_BREAK_IF(!fileSystem)

    if (shader && fileSystem && State.SceneManager)
    {
        IsNull = false;

     // Stages.
        u32 i;
        for ( i = 1; i < shader->VarGroup->VariableGroup.size(); ++i )
        {
            StageShader *stage = new StageShader(*this);
            stage->parse( &shader->VarGroup->VariableGroup[i], fileSystem );
            Stages.push_back(stage);
        }

     // Generic stage.
        const quake3::SVarGroup *group;
        group = shader->getGroup(1);

     // Is transparent?
        if (group && group->isDefined( "surfaceparm", "trans" ))
            IsTransparent = true;
        else
        {
            IStageShader *stage;

            core::list<IStageShader*>::Iterator it = Stages.begin();

            for (; it != Stages.end(); ++it)
            {
		    	stage = *it;
		    	if ( stage->isDrawable() )
		    	{
		    	    IsTransparent = stage->isTransparent();
		    	    break;
		    	}
		    }
        } // else for if (group && group->isDefined( "surfaceparm", "trans" )) ...

     // Is there culling?
        u32 pos = 0;
        static const c8 * funclist[] = {"none", "disable"};
        const core::stringc &string = group->get("cull");
        BackfaceCulling = (string.size() == 0) || ( isEqual( string, pos, funclist, 2) == -2 );

    } // if (shader && fileSystem && State.SceneManager) ...
}

core::list<IStageShader*>& Shader::getStages( video::S3DVertex2TCoords *vsrc, video::S3DVertex2TCoords *vdst, u32 vsize )
{
    _IRR_DEBUG_BREAK_IF(!vsrc || !vdst || !vsize) // access violation

    State.vsrc  = vsrc;
    State.vdst  = vdst;
    State.vsize = vsize;

    State.material = Material;
//    State.material.Lighting = false;
    State.material.NormalizeNormals = false;
    State.material.setTexture(1, 0);
    State.material.BackfaceCulling = State.material.BackfaceCulling || BackfaceCulling;

    return Stages;
}

/*********************************************************************************************/
/**                                   ShadersParser                                         **/
/*********************************************************************************************/

void ShadersParser::parse()
{
	quake3::SVarGroupList *groupList;

	s32 active;
	s32 last;

	quake3::SVariable entity;

	groupList = new quake3::SVarGroupList ();

	groupList->VariableGroup.push_back ( quake3::SVarGroup () );
	active = last = 0;

	bool parsing = true;
	do
	{

		switch ( getScript()->nextToken() )
		{
			case ETK_TOKEN_START_LIST:
			{
				groupList->VariableGroup.push_back ( quake3::SVarGroup () );
				last = active;
				active = groupList->VariableGroup.size() - 1;
				entity.clear ();
			}  break;

			// a unregisterd variable is finished
			case ETK_TOKEN_EOL:
			{
				if ( entity.isValid() )
				{
					groupList->VariableGroup[active].Variable.push_back ( entity );
					entity.clear ();
				}
			} break;

			case ETK_TOKEN_TOKEN:
			{
             // store content based on line-delemiter
				if ( 0 == entity.isValid() )
				{
					entity.name = getScript()->token().getValue();
					entity.content = "";

				}
				else
				{
					if ( entity.content.size() )
					{
						entity.content += " ";
					}
					entity.content += getScript()->token().getValue();
				}
			} break;

			case ETK_TOKEN_END_LIST:
			{
				// close tag for first
				if ( active == 1 )
				{
                 // Save the shader.
				    if (groupList->VariableGroup.size() > 0 && groupList->VariableGroup[0].Variable.size() > 0)
				    {
				        quake3::SShader shader;
				        shader.name = groupList->VariableGroup[0].Variable[0].name.c_str();
				        shader.id = Shader.size();
				        shader.VarGroup = groupList;
				        Shader.push_back(shader);
				    }

                 // new group
					groupList = new quake3::SVarGroupList ();
					groupList->VariableGroup.push_back ( quake3::SVarGroup () );
					last = 0;
				}

				active = last;
				entity.clear();

			} break;

			case ETK_TOKEN_ENTITY:
			// It is a shader, not a entity: Is that a error?
			  {
			      core::stringc error;
			      error = "Parse Shader failed at line ";
			      error += getScript()->line();
			      error += ": Contains entities.";
			      os::Printer::log(error.c_str());
			      Shader.clear();
			      return;
			  }
			  break;

			case ETK_TOKEN_UNRESOLVED:
			{
                core::stringc error;
                error = "Parse Shader failed at line ";
                error += getScript()->line();
                error += ": Contains token unresolved.";
                os::Printer::log(error.c_str());
                error = "TOKEN: {";
                error += getScript()->token().getValue();
                error += "}";
                os::Printer::log(error.c_str());
                Shader.clear();
                return;
			} break;
			case ETK_TOKEN_EOF:
			{
			    parsing = false;
			} break;
		}

	} while ( parsing );
}

s32 ShadersParser::findShaderByTexture(const core::stringc textureName)
{
    s32 r;
    r = -1;

    if (textureName != "")
    {
        core::stringc name;
        name = cutFilenameExtension(name, textureName);
        name.make_lower();
        quake3::SShader search;
        search.name = name;
        r = Shader.linear_search(search);
    }

    return r;
}

} // end namespace quake3
} // end namespace scene
} // end namespace irr


