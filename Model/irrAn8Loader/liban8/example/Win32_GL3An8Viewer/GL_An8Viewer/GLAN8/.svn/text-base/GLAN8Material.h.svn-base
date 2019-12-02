#ifndef _GLAN8Material_H
#define _GLAN8Material_H


#include"GLGX/GLGX.h"
#include<string>
using namespace std;

class GLAN8Material
{
public:

	GLuint* ShaderProgramObject;
	GLvoid SetShaderProgramObject(GLuint* i_shaderProgramObject){this->ShaderProgramObject = i_shaderProgramObject;};


	string Name;

	GLGXCOLOR   DiffuseRGB;
	GLfloat DiffuseFactor;
	GLboolean   DiffuseIsTexture;
	GLint	  DiffuseBlendMode;
	GLint   DiffuseAlphaMode;
	GLint   DiffuseTextureID;
	GLuint DiffuseTexture;
	string DiffuseTextureName;

	GLGXCOLOR   SpecularRGB;
	GLfloat SpecularFactor;
	GLboolean   SpecularIsTexture;
	GLint	  SpecularBlendMode;
	GLint   SpecularAlphaMode;
	GLint   SpecularTextureID;
	GLuint SpecularTexture;
	
	GLboolean UseAlpha;
	GLint   Alpha;

	GLboolean HasEnvironment;
	GLint   EnvironmentPercent;
	GLint	  EnvironmentBlendMode;
	GLint   EnvironmentAlphaMode;
	GLint   EnvironmentTextureID;
	GLuint EnvironmentTexture;
	
	GLboolean HasBumpMap; 
	GLint   BumpMapPercent;
	GLint	  BumpMapBlendMode;
	GLint   BumpMapAlphaMode;
	GLint   BumpTextureID;
	GLuint BumpTexture;

	GLvoid Set( GLvoid );
	
	GLAN8Material(GLvoid);
	~GLAN8Material(GLvoid);
};

#endif	
