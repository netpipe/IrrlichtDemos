#ifndef _GLAN8Object_H
#define _GLAN8Object_H

#include"GLAN8Mesh.h"

class GLAN8Object
{
public:
	GLGXMATRIX* CurrentWorldMatrix;
	

	GLuint* ShaderProgramObject;
	GLvoid SetShaderProgramObject(GLuint* i_shaderProgramObject)
	{
		this->ShaderProgramObject = i_shaderProgramObject;
		for( int i = 0 ; i < NbrMaterial ; ++i )
		{
			Materials[i].SetShaderProgramObject(ShaderProgramObject);
		}
	
	};

	void Render(void);

	void SetWorldMatrix(GLGXMATRIX* matrix);

	GLint NbrMesh;
	GLAN8Mesh *Meshes;

	GLint NbrMaterial;
	GLAN8Material* Materials;

	GLboolean Build(GLvoid);

	GLAN8Object(GLvoid);
	~GLAN8Object(GLvoid);
};

#endif	
