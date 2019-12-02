#include "GLAN8ObjectElement.h"
#include<stdio.h>

#include"../Engine/CameraManager.h"
extern CameraManager g_CameraManager;

GLAN8ObjectElement::GLAN8ObjectElement(void)
{
	ObjectID = 0;
	Object = NULL;
	GLGXMatrixIdentity(&Matrix);
}

GLAN8ObjectElement::~GLAN8ObjectElement(void)
{
	// Do not delete Object (managed by the scene)
}

void GLAN8ObjectElement::Render(void)
{
	glUniformMatrix4fv(glGetUniformLocation(*Object->ShaderProgramObject, "g_MatWorld"),1,false,Matrix);

	//float fvMatrix[ 16 ];
	//glGetFloatv( GL_MODELVIEW_MATRIX, fvMatrix );
      
	for( int i = 0 ; i < Object->NbrMesh ; ++i )
	{
		Object->Meshes[i].Render();
	}
}
