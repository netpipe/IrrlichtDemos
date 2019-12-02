#ifndef _GLAN8GroundGrid_H
#define _GLAN8GroundGrid_H

#include"GLGX/GLGX.h"

class GLAN8GroundGrid
{
public:

	std::string TextureFileName;
	GLuint Texture;

	GLint NbrVertex;
	GLfloat* Vertices;
	GLfloat* TextureCoords;
	GLfloat* Normals;

	GLint NbrIndex;
	GLint*   Indices;

	GLuint VertexVBO;
	GLuint TexCoordVBO;
	GLuint NormalVBO;
	GLuint IndexVBO;

	GLGXMATRIX Matrix; // Identity




	bool Build (void );
	void Render( GLuint* i_ShaderProgramObject );

	GLAN8GroundGrid( void );
	~GLAN8GroundGrid( void );
};



#endif	

