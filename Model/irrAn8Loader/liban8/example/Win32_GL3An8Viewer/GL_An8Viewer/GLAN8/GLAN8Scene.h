#ifndef _GLAN8Scene_H
#define _GLAN8Scene_H

#include"GLAN8Object.h"
#include"GLAN8ObjectElement.h"
#include"GLAN8Light.h"
#include"GLAN8GroundGrid.h"
#include"GLAN8BackGround.h"

class GLAN8Scene
{
public:

	GLuint* ShaderProgramObject;

	GLint NbrObjectElement;
	GLAN8ObjectElement* ObjectElements;

	GLint NbrCamera;
	//GLAN8Camera* Cameras;
	int* Cameras;

	GLint NbrLight;
	GLAN8Light* Lights;

	// Environement
	bool HasFog;
	GLGXVECTOR4 FogColor;
	float FogPercent;
	float FogStart;
	float FogEnd;

	int	 BackGroundType;
	float BackGround[3];

	bool HasGroundGrid;
	GLAN8GroundGrid* GroundGrid;



	GLAN8BackGround* BackGroundImage;
	GLvoid RenderBackGround( GLvoid );

	GLvoid SetEnvironment( GLvoid );
	GLvoid SetLights( GLvoid );

	GLvoid RenderGroundGrid( GLvoid );
	GLvoid Render( GLvoid );

	GLvoid SetShaderProgramObject(GLuint* i_shaderProgramObject)
	{
		this->ShaderProgramObject = i_shaderProgramObject;
		for( int i = 0 ; i < NbrLight ; ++i )
		{
			Lights[i].SetShaderProgramObject(ShaderProgramObject);
		}	
	};

	bool Build();

	GLAN8Scene(GLvoid);
	~GLAN8Scene(GLvoid);
};

#endif	
