#ifndef _GLAN8Light_H
#define _GLAN8Light_H

#include"GLGX/GLGX.h"


#define GLAN8LIGHT_INFINITE 1
#define GLAN8LIGHT_LOCAL 2
#define GLAN8LIGHT_SPOT 3

class GLAN8Light
{
public:
	GLint ID;
	GLint Kind; 	

	GLGXVECTOR4 Loc;
	GLGXVECTOR4 Eye;
	GLGXMATRIX Matrix;   ///< Matrix

	GLfloat InAngle;
	GLfloat OutAngle;

	GLint Color[3]; ///< RGB Color

	GLuint* ShaderProgramObject;
	GLvoid SetShaderProgramObject(GLuint* i_shaderProgramObject){this->ShaderProgramObject = i_shaderProgramObject;};

public:
	void Set( GLvoid );
	GLAN8Light(GLvoid);
	~GLAN8Light(GLvoid);
};

#endif	
