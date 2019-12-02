#ifndef _GLAN8Mesh_H
#define _GLAN8Mesh_H


#include"GLAN8Material.h"

class GLAN8Mesh
{
public:

	GLint	NbrFace;
	GLint NbrVertex;
	GLfloat* Vertices;
	GLfloat* TextureCoords;
	GLfloat* Normals;

	GLint NbrIndex;
	GLint*   Indices;

	GLuint VAO;
	GLuint _shaderLocPosition;
	GLuint _shaderLocTexture;
	GLuint _shaderLocNormal;

	GLuint VertexVBO;
	GLuint TexCoordVBO;
	GLuint NormalVBO;
	GLuint IndexVBO;

	GLint MaterialID;
	GLAN8Material* Material;

	GLGXVECTOR3 BoundingSphereCenter;
	float       BoundingSphereRadius;

	GLvoid Render(GLvoid);

	GLboolean Build(GLuint* ShaderProgramObject);

	GLAN8Mesh(GLvoid);
	~GLAN8Mesh(GLvoid);
};

#endif	

