#include "GLAN8Mesh.h"
#include<stdio.h>


GLAN8Mesh::GLAN8Mesh(void)
{
	NbrVertex = 0;
	Vertices      = NULL;
	TextureCoords = NULL;
	Normals       = NULL;

	NbrIndex = 0;
	Indices       = NULL;

	MaterialID = 0;

}

GLAN8Mesh::~GLAN8Mesh(void)
{
	if( Vertices != NULL)
	{
		delete [] Vertices;
		Vertices = NULL;
	}
	if( TextureCoords != NULL)
	{
		delete [] TextureCoords;
		TextureCoords = NULL;
	}
	if( Normals != NULL)
	{
		delete [] Normals;
		Normals = NULL;
	}
	if( Indices != NULL)
	{
		delete [] Indices;
		Indices = NULL;
	}

	glDeleteBuffersARB(1, &VertexVBO);
	glDeleteBuffersARB(1, &NormalVBO);
	glDeleteBuffersARB(1, &TexCoordVBO);
	glDeleteBuffersARB(1, &IndexVBO);

}

void GLAN8Mesh::Render(void)
{

	glBindVertexArray(VAO);										//errorTest = glGetError();		

	Material->Set();

	/*GLuint positionIndex = glGetAttribLocation(*Material->ShaderProgramObject, "g_vboPosition");
	GLuint textureIndex	= glGetAttribLocation(*Material->ShaderProgramObject, "g_vboTexture");
	GLuint normalIndex	= glGetAttribLocation(*Material->ShaderProgramObject, "g_vboNormal");

	// Vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VertexVBO);
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionIndex);

	// Textures
	if( Material->DiffuseIsTexture == GL_TRUE )
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, TexCoordVBO);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(textureIndex);
	}

	// Normals
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, NormalVBO);
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalIndex);
         
	// Indices
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IndexVBO);*/

	// Draw
	glDrawElements(GL_TRIANGLES, NbrIndex, GL_UNSIGNED_INT, 0);

	/*glDisableVertexAttribArray(positionIndex);
	glDisableVertexAttribArray(normalIndex);
	if( Material->DiffuseIsTexture == GL_TRUE )
		glDisableVertexAttribArray(textureIndex);*/

	glBindVertexArray(0);			//errorTest = glGetError();
}

GLboolean GLAN8Mesh::Build(GLuint* ShaderProgramObject)
{
		glGenVertexArrays(1, &VAO);		
		glBindVertexArray(VAO);			

		
		_shaderLocPosition	= glGetAttribLocation(*ShaderProgramObject, "g_vboPosition");
		_shaderLocTexture	= glGetAttribLocation(*ShaderProgramObject, "g_vboTexture");
		_shaderLocNormal	= glGetAttribLocation(*ShaderProgramObject, "g_vboNormal");
		glBindFragDataLocation(*ShaderProgramObject,0,"o_color");
		

		// Vertices
		glGenBuffersARB( 1, &VertexVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, VertexVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*NbrVertex, Vertices, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );
		int nParam_ArrayObjectSize = 0;
		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;
		glVertexAttribPointer(_shaderLocPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);											//errorTest = glGetError();
		glEnableVertexAttribArray(_shaderLocPosition);					

		// TextureCoords
		glGenBuffersARB( 1, &TexCoordVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, TexCoordVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*NbrVertex, TextureCoords, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );
		nParam_ArrayObjectSize = 0;
		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;
		glVertexAttribPointer(_shaderLocTexture, 2, GL_FLOAT, GL_FALSE, 0, 0);											//errorTest = glGetError();
		glEnableVertexAttribArray(_shaderLocTexture);			

		// Normals
		glGenBuffersARB( 1, &NormalVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NormalVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*NbrVertex, Normals, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );
		nParam_ArrayObjectSize = 0;
		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;
		glVertexAttribPointer(_shaderLocNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);											//errorTest = glGetError();
		glEnableVertexAttribArray(_shaderLocNormal);			

		nParam_ArrayObjectSize = 0;
		glGenBuffersARB( 1, &IndexVBO );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, IndexVBO );
		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(int)*NbrIndex, Indices, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );
		glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;

		//glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		//glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

		glBindVertexArray(0);					
		


	return GL_TRUE;
}
