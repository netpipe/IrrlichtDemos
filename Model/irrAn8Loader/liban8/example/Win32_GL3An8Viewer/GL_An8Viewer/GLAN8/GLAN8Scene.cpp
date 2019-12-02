#include "GLAN8Scene.h"
#include<stdio.h>

#include"../Engine/CameraManager.h"
extern CameraManager g_CameraManager;

GLAN8Scene::GLAN8Scene(void)
{
	NbrObjectElement = 0;
	ObjectElements = NULL;

	NbrCamera = 0;
	Cameras = NULL;

	NbrLight = 0;
	Lights = NULL;
	ShaderProgramObject = NULL;

	// Environement
	HasFog = false;
	HasGroundGrid = false;

	GroundGrid = NULL;

	BackGroundType = 0; // color
	BackGroundImage = NULL;
}

GLAN8Scene::~GLAN8Scene(void)
{
	if( ObjectElements != NULL )
	{
		delete [] ObjectElements;
		ObjectElements = NULL;
	}

	if( GroundGrid != NULL)
	{
		delete GroundGrid;
		GroundGrid = NULL;
	}

	
	if( BackGroundImage != NULL)
	{
		delete BackGroundImage;
		BackGroundImage = NULL;
	}

	if( Cameras != NULL )
	{
		delete [] Cameras;
		Cameras = NULL;
	}

	if( Lights != NULL )
	{
		delete [] Lights;
		Lights = NULL;
	}
}

bool GLAN8Scene::Build(void)
{
	if( HasGroundGrid == true )
	{
		GroundGrid = new GLAN8GroundGrid();
		GroundGrid->Build();
	}

	if( BackGroundImage != NULL ) // image
	{
		BackGroundImage->Build();
	}

	/*for( int i = 0; i < this->NbrCamera ; ++i )
	{
		Cameras[i].Build();
	}*/
	return true;
}

void GLAN8Scene::SetEnvironment( void )
{
	// Fog
  if( HasFog == true)
  {
  /*  glEnable(GL_FOG);

    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR, FogColor);
        
    glFogf(GL_FOG_DENSITY, FogPercent);
        
    glHint(GL_FOG_HINT,GL_DONT_CARE);
        
    glFogf(GL_FOG_START, FogStart);
    glFogf(GL_FOG_END,   FogEnd);
        
    glClearColor(FogColor[0],FogColor[1],FogColor[2],FogColor[3]);*/
  }
  else
  {
    glClearColor(BackGround[0],BackGround[1],BackGround[2], 1.0f);
  }
	// BackGround
}

GLvoid GLAN8Scene::SetLights( GLvoid )
{
	for( int i = 0 ; i < NbrLight ; ++i )
	{
		Lights[i].Set();
	}
}

GLvoid GLAN8Scene::RenderGroundGrid( GLvoid )
{
	if( GroundGrid != NULL )
	{
		GroundGrid->Render(ShaderProgramObject);
	}
}

GLvoid GLAN8Scene::RenderBackGround( GLvoid )
{
	if( BackGroundImage != NULL )
	{
		BackGroundImage->Render(ShaderProgramObject);
	}
}

void GLAN8Scene::Render(void)
{
	RenderBackGround();
	
	RenderGroundGrid();


	glUniformMatrix4fv(glGetUniformLocation(*ShaderProgramObject, "g_MatView"),1,false,g_CameraManager.Get(0)->MatrixView);
	glUniformMatrix4fv(glGetUniformLocation(*ShaderProgramObject, "g_MatProj"),1,false,g_CameraManager.Get(0)->MatrixProjection);
      

	SetLights();

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		ObjectElements[i].Render();
	}
	
}
