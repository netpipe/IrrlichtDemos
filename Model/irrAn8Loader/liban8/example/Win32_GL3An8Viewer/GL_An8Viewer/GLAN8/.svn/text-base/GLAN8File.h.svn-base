#ifndef _GLAN8Level_H
#define _GLAN8Level_H

#include"GLAN8Scene.h"
#include"GLAN8Texture.h"

#include"../Engine/CameraManager.h"

class GLAN8File
{
public:

	GLuint* ShaderProgramObject;
	GLvoid SetShaderProgramObject(GLuint* i_shaderProgramObject)
	{
		this->ShaderProgramObject = i_shaderProgramObject;
		for( int i = 0 ; i < NbrObject ; ++i )
		{
			Objects[i].SetShaderProgramObject(ShaderProgramObject);
		}
		Scenes[0].SetShaderProgramObject(ShaderProgramObject);
	
	};

	unsigned int SelectedScene;
	unsigned int SelectedCamera;
	Camera* GetCamera( GLvoid );

	GLint NbrObject;
	GLAN8Object* Objects;

	//GLint NbrTexture;
	//GLAN8Texture* Textures;
	GLint NbrScene;
	GLAN8Scene* Scenes;

	GLvoid SetEnvironment( GLvoid );
	GLvoid SetLights( GLvoid );
	GLvoid RenderGroundGrid( GLvoid );
	GLvoid Render( GLvoid );

	bool Build( void );

	GLAN8File(GLvoid);
	~GLAN8File(GLvoid);
};

#endif	
