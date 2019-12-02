#ifndef _GLAN8ObjectElement_H
#define _GLAN8ObjectElement_H

#include"GLAN8Object.h"

#include"GLGX/GLGX.h"


class GLAN8ObjectElement
{
public:
	GLint ObjectID;
	GLAN8Object* Object;
	GLGXMATRIX Matrix;

	GLvoid Render(GLvoid);

	GLAN8ObjectElement(GLvoid);
	~GLAN8ObjectElement(GLvoid);
};

#endif	
