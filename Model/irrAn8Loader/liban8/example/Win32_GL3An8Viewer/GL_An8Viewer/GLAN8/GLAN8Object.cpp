#include "GLAN8Object.h"
#include<stdio.h>

GLAN8Object::GLAN8Object(void)
{
	CurrentWorldMatrix = NULL;

	NbrMesh = 0;
	Meshes = NULL;

	NbrMaterial = 0;
	Materials = NULL;
	ShaderProgramObject = NULL;
}

GLAN8Object::~GLAN8Object(void)
{
	if( Meshes != NULL )
	{
		delete [] Meshes;
		Meshes = NULL;
	}

	if( Materials != NULL )
	{
		delete [] Materials;
		Materials = NULL;
	}
}

GLboolean GLAN8Object::Build(void)
{
	for( int i = 0 ; i < NbrMesh ; ++i )
	{
		if (Meshes[i].Build(ShaderProgramObject) == GL_FALSE)
			return GL_FALSE;
	}
	return GL_TRUE;
}

void GLAN8Object::SetWorldMatrix(GLGXMATRIX* matrix)
{
	CurrentWorldMatrix = matrix;
}

void GLAN8Object::Render(void)
{
	glUseProgram(*ShaderProgramObject);

	if( CurrentWorldMatrix != NULL)
		glUniformMatrix4fv(glGetUniformLocation(*ShaderProgramObject, "g_MatWorld"),1,false,*CurrentWorldMatrix);

	for( int i = 0 ; i < NbrMesh ; ++i )
	{
		Meshes[i].Render();
	}
	CurrentWorldMatrix = NULL;
}
