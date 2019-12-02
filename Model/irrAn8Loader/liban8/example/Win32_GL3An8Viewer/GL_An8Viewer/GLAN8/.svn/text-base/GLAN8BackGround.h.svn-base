#ifndef _GLAN8BackGround_H
#define _GLAN8BackGround_H

#include"GLGX/GLGX.h"

class GLAN8BackGround
{
public:

	std::string TextureFileNames[6];
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

	GLboolean _EnableLighting;
	GLboolean _EnableBlend;
	GLboolean _EnableDepthTest;
	GLboolean _EnableTexture2d;
	GLboolean _EnableCullFace;

	GLGXMATRIX MatrixProjection;
	GLGXMATRIX MatrixView;  // Identity
	GLGXMATRIX Matrix; // Identity


	void Begin( void );
	void End( void );

	bool Build (void );
	void Render( GLuint* i_ShaderProgramObject );

	GLAN8BackGround( void );
	~GLAN8BackGround( void );
};



#endif	

