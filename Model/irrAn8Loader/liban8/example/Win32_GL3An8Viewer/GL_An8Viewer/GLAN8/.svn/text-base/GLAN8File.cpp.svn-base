#include "GLAN8File.h"

#include"../Engine/TextureManager.h"

extern TextureManager g_TextureManager;
extern CameraManager g_CameraManager;

GLAN8File::GLAN8File(void)
{
	NbrObject  = 0;
	NbrScene   = 0;
	//NbrTexture = 0;
	Objects = NULL;
	SelectedScene = 0;
	SelectedCamera = 0;
	ShaderProgramObject = NULL;


	Objects  = NULL;
	//Textures = NULL;
	Scenes   = NULL;
}

GLAN8File::~GLAN8File(void)
{
	if( Objects != NULL )
	{
		delete [] Objects;
		Objects = NULL;
	}
	if( Scenes != NULL )
	{
		delete [] Scenes;
		Scenes = NULL;
	}
	/*if( Textures != NULL )
	{
		delete [] Textures;
		Textures = NULL;
	}
	*/
}

Camera* GLAN8File::GetCamera( GLvoid )
{
	if( Scenes == NULL ) 
		return NULL;

	return g_CameraManager.Get(0);

	//return &Scenes[SelectedScene].Cameras[SelectedCamera];
}

GLvoid GLAN8File::SetEnvironment( GLvoid )
{
	if( Scenes == NULL ) 
		return;

	Scenes[SelectedScene].SetEnvironment();
}
GLvoid GLAN8File::SetLights( GLvoid )
{
	if( Scenes == NULL ) 
		return;

	Scenes[SelectedScene].SetLights();
}

GLvoid GLAN8File::RenderGroundGrid( GLvoid )
{
	if( Scenes == NULL ) 
		return;

	Scenes[SelectedScene].RenderGroundGrid();
}


GLvoid GLAN8File::Render( GLvoid )
{
	if( Scenes == NULL ) 
		return;

	glUseProgram(*ShaderProgramObject); 
	Scenes[SelectedScene].Render();
}


bool GLAN8File::Build( void )
{

	for( int i = 0 ; i < this->NbrObject; ++i )
	{
		for( int j = 0 ; j < this->Objects[i].NbrMaterial; ++j )
		{
			this->Objects[i].Materials[j].DiffuseTexture = g_TextureManager.GetGLID(&this->Objects[i].Materials[j].DiffuseTextureName);
		}

	}

	for( int i = 0 ; i < NbrObject; ++i )
	{
		if( Objects[i].Build() == false )
		{
			return false;
		}
	}

	for( int i = 0 ; i < NbrScene; ++i )
	{
		if( Scenes[i].Build() == false )
		{
			return false;
		}
	}
	return true;
}


