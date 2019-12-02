#include "Engine.h"


#include"GLAN8/GLAN8FileLoader.h"

#include"Engine/TextureManager.h"
#include"Engine/CameraManager.h"

TextureManager g_TextureManager;
CameraManager g_CameraManager;

//--------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------
Engine::Engine( void (*fctDisplayMessage)(char*, char*) )
{
	this->fctDisplayMessage = fctDisplayMessage; // function used for messages
	Levels = NULL;
	PlayerModel = NULL;
	ShaderProgramObject = 0;

	IsToonRendering = false;
			
	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);				
	glClearDepth(10000.0f);								
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glFrontFace(GL_CW);
	glDisable(GL_CULL_FACE);

	if( Font.Create("Data/Font.bmp") == false )
		this->fctDisplayMessage("ERROR","Can't load Font.bmp");

	std::string error;
	if( GLGXCreateEffectFromFiles(&ShaderProgramObject,&error,"Data/Shaders/diffuseTexture.vert","Data/Shaders/diffuseTexture.frag") == false ) // Load shaders
	{
		this->fctDisplayMessage("GLSL Shader ERROR",(char*)error.c_str());
		ShaderProgramObject = 0;
	}
}

//--------------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------------
Engine::~Engine(void)
{
	if (Levels != NULL )
	{
		delete [] Levels;
		Levels = NULL;
	}
	if (PlayerModel != NULL )
	{
		delete PlayerModel;
		PlayerModel = NULL;
	}
	GLAN8FileLoader::Release();
	
	glDeleteObjectARB(ShaderProgramObject);
	ShaderProgramObject = 0;
}

void Engine::SetToonRendering(bool state)
{
	IsToonRendering = state;

	if( state == true )
	{
		if( ShaderProgramObject != 0 ) // If GLSL is supported, use it
		{
			this->Levels[0].SetShaderProgramObject(&ShaderProgramObject); // Give the shader pointer to the scene graph
			this->PlayerModel->SetShaderProgramObject(&ShaderProgramObject);
		}
	}
	else
	{
		this->Levels[0].SetShaderProgramObject(&GLAN8FileLoader::DefaultShaderProgramObject);
		this->PlayerModel->SetShaderProgramObject(&GLAN8FileLoader::DefaultShaderProgramObject);
	}
}

//--------------------------------------------------------------------------------------
// Init: Load the .an8 file
//--------------------------------------------------------------------------------------
GLboolean Engine::LoadFile(char* fileName)
{
	g_TextureManager.Release();
	g_CameraManager.Release();

	if (Levels != NULL )
	{
		delete [] Levels;
		Levels = NULL;
	}
	if (PlayerModel != NULL )
	{
		delete PlayerModel;
		PlayerModel = NULL;
	}

	AN8XFileNameFromPath(&CurrentFileName,&std::string(fileName));

	this->Levels = new GLAN8File[1]; // Correspond to an .an8 file

	std::string error;
	if( GLAN8FileLoader::LoadSceneFromFile(&error,fileName,&this->Levels[0]) == false ) // Load the file in the scene graph
	{
		this->fctDisplayMessage("ERROR",(char*)error.c_str());
		delete [] this->Levels;this->Levels = NULL;
		return GL_FALSE;
	}

	// We add an object to the scene. We load it from another .an8 file.
	this->PlayerModel = new GLAN8Object();
	if( GLAN8FileLoader::LoadObjectFromFile(&error,"sphere.an8",this->PlayerModel) == false ) // Load the file in the scene graph
	{
		this->fctDisplayMessage("ERROR",(char*)error.c_str());
		delete [] this->PlayerModel;this->PlayerModel = NULL;
		return GL_FALSE;
	}


	// Fog, background color and lights will not change
	//this->Levels[0].SetEnvironment(); 

	// Compute Projection Matrix
	this->Levels[0].GetCamera()->SetProjection((GLdouble)WindowWidth,(GLdouble)WindowHeight);

	return GL_TRUE;
}




//--------------------------------------------------------------------------------------
// Render
//--------------------------------------------------------------------------------------
void Engine::OnFrameRender( double i_fTime, float i_fElapsedTime )
{
	GLGXMATRIX MatrixIdentity;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	if(this->Levels == NULL )
	{
		Font.Begin();
			Font.DrawText(20,5,0,"Click on the top-left icon to load a new file");
		Font.End();
		return;
	}


	if( IsToonRendering == true)
	{
		//glLineWidth(1.01f);

		// Render Outline
		glFrontFace(GL_CCW); // Cull Front faces
		glEnable(GL_CULL_FACE); // Cull Front faces
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		// Render the main scene
		this->Levels[0].Render();

		// Set the object at (0,0,0) with an identity matrix
		this->PlayerModel->SetWorldMatrix(&MatrixIdentity);
		this->PlayerModel->Render();


		// Render the scene (front face filled)
		glFrontFace(GL_CW);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	// Render the main scene
	this->Levels[0].Render();

	// Set the object at (0,0,0) with an identity matrix
	this->PlayerModel->SetWorldMatrix(&MatrixIdentity);
	this->PlayerModel->Render();







	Font.Begin();
		Font.DrawText(20,5,0,"Click on the top-left icon to load a new file");
		Font.DrawText(20,20,1,CurrentFileName.c_str());
	Font.End();
	
}

//--------------------------------------------------------------------------------------
// Resize
//--------------------------------------------------------------------------------------
void Engine::OnFrameResize( GLint width , GLint height )
{
	if (height==0) height=1;

	WindowWidth = width;
	WindowHeight = height;

	glViewport(0,0,width,height);						

	if(this->Levels != NULL )
		this->Levels[0].GetCamera()->SetProjection((GLdouble)width,(GLdouble)height);
}

