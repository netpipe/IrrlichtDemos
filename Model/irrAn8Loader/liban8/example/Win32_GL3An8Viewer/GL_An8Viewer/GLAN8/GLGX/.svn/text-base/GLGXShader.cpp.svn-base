/*
* GLGX 
* An OpenGL Utility Library
*
* Copyright (c) 2002-2010, Grégory Smialek
*
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted 
* provided that the following conditions are met:
* 
*   * Redistributions of source code must retain the above copyright notice, 
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation 
*     and/or other materials provided with the distribution.
*   * Neither the name of the Grégory Smialek nor the names of its contributors
*     may be used to endorse or promote products derived from this software 
*     without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GLGXShader.h"
#include "GLGXUtil.h"



bool GLGXCreateEffectFromFiles(unsigned int* pProgramObject, std::string* pError, char* pVertexShaderFileName,char* pFragmentShaderFileName,char* pGeometryShaderFileName)
{
	if( pError == NULL)
	{
		pError = new std::string();
	}
	if( pVertexShaderFileName == NULL)
	{
		*pError = "Vertex shader's file name = NULL";
		return false;
	}
	if( pFragmentShaderFileName == NULL)
	{
		*pError = "Fragment shader's file name = NULL";
		return false;
	}

	// Vertex Shader
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	if( GLGXCompileShaderFromFile(&vertexShaderObject,pError,pVertexShaderFileName) == false )
	{
		std::string strFileName = pVertexShaderFileName;
		strFileName = strFileName + "\n";
		*pError = strFileName + *pError;
		return false;
	}

	// Fragment Shader
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	if( GLGXCompileShaderFromFile(&fragmentShaderObject,pError,pFragmentShaderFileName) == false )
	{
		std::string strFileName = pFragmentShaderFileName;
		strFileName = strFileName + "\n";
		*pError = strFileName + *pError;
		return false;
	}

	// Geometry shader
	GLuint geometryShaderObject = 0;
	if( pGeometryShaderFileName != NULL)
	{
		geometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		if( GLGXCompileShaderFromFile(&geometryShaderObject,pError,pGeometryShaderFileName) == false )
		{
			std::string strFileName = pGeometryShaderFileName;
			strFileName = strFileName + "\n";
			*pError = strFileName + *pError;
			return false;
		}
	}

	*pProgramObject = glCreateProgram();

	glAttachShader(*pProgramObject, vertexShaderObject);
	glAttachShader(*pProgramObject, fragmentShaderObject);
	if( pGeometryShaderFileName != NULL)
		glAttachShader(*pProgramObject, geometryShaderObject);
	glLinkProgram(*pProgramObject);        
	 
	GLint linked;
	glGetProgramiv(*pProgramObject, GL_LINK_STATUS, &linked);
	if (!linked)
	{  
		if( geometryShaderObject != 0 )
			*pError = "Can't link shaders: "+std::string(pVertexShaderFileName) + " and "+std::string(pFragmentShaderFileName) + " and "+std::string(pGeometryShaderFileName);
		else
			*pError = "Can't link shaders: "+std::string(pVertexShaderFileName) + " and "+std::string(pFragmentShaderFileName);
		
		return false;
	}     
	return true;
}

bool GLGXCreateVertexShaderFromFile(unsigned int* pProgramObject, std::string* pError ,char* pVertexShaderFileName)
{
	if( pError == NULL)
	{
		pError = new std::string();
	}
	if( pVertexShaderFileName == NULL)
	{
		*pError = "Vertex shader's file name = NULL";
		return false;
	}

	// Vertex Shader
	GLuint vertexShaderObject;
	vertexShaderObject   = glCreateShader(GL_VERTEX_SHADER);

	char* VertexShaderSource = NULL;
	VertexShaderSource = GLGXTextLoadFromFile(pVertexShaderFileName);
	if( VertexShaderSource == NULL)
	{
		std::string strFileName(pVertexShaderFileName);
		*pError = "Vertex shader's file not found: "+strFileName;
		return false;
	}

	int vlength = (int)strlen(VertexShaderSource);

	glShaderSourceARB(vertexShaderObject, 1, (const char**)&VertexShaderSource, &vlength);
	glCompileShaderARB(vertexShaderObject);

	delete [] VertexShaderSource;

	GLint compiled;

	glGetObjectParameterivARB(vertexShaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint blen = 0;	GLsizei slen = 0;

		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH , &blen);       

		if (blen > 1)
		{
			 GLchar* compiler_log = (GLchar*)malloc(blen);
			 glGetInfoLogARB(vertexShaderObject, blen, &slen, compiler_log);
			 *pError = std::string(compiler_log);
			 free (compiler_log);
			 return false;
		}
	}

	*pProgramObject = glCreateProgram();

	glAttachShader(*pProgramObject, vertexShaderObject);
	glLinkProgram(*pProgramObject);        
	 
	GLint linked;
	glGetProgramiv(*pProgramObject, GL_LINK_STATUS, &linked);
	if (!linked)
	{  
		*pError = "Can't link vertex shader: "+std::string(pVertexShaderFileName);
		return false;
	}     
	return true;
}

bool GLGXCompileShaderFromFile(unsigned int* shaderObject,std::string* pError, char* pShaderFileName)
{
	if( pError == NULL)
	{
		pError = new std::string();
	}
	if( pShaderFileName == NULL)
	{
		*pError = "Shader's file name = NULL";
		return false;
	}

	char* shaderSource = NULL;
	shaderSource = GLGXTextLoadFromFile(pShaderFileName);
	if( shaderSource == NULL)
	{
		std::string strFileName(pShaderFileName);
		*pError = "Vertex shader's file not found: "+strFileName;
		return false;
	}

	int vlength = (int)strlen(shaderSource);

	glShaderSourceARB(*shaderObject, 1, (const char**)&shaderSource, &vlength);
	glCompileShaderARB(*shaderObject);

	delete [] shaderSource;

	GLint compiled;

	glGetObjectParameterivARB(*shaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint blen = 0;	GLsizei slen = 0;

		glGetShaderiv(*shaderObject, GL_INFO_LOG_LENGTH , &blen);       

		if (blen > 1)
		{
			 GLchar* compiler_log = (GLchar*)malloc(blen);
			 glGetInfoLogARB(*shaderObject, blen, &slen, compiler_log);
			 *pError = std::string(compiler_log);
			 free (compiler_log);
			 return false;
		}
	}

	return true;
}

bool GLGXCompileShaderFromString(unsigned int* shaderObject,std::string* pError, char* pShaderString)
{
	if( pError == NULL)
	{
		pError = new std::string();
	}
	if( pShaderString == NULL)
	{
		*pError = "Shader's String = NULL";
		return false;
	}

	int vlength = (int)strlen(pShaderString);

	glShaderSourceARB(*shaderObject, 1, (const char**)&pShaderString, &vlength);
	glCompileShaderARB(*shaderObject);

	GLint compiled;

	glGetObjectParameterivARB(*shaderObject, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint blen = 0;	GLsizei slen = 0;

		glGetShaderiv(*shaderObject, GL_INFO_LOG_LENGTH , &blen);       

		if (blen > 1)
		{
			 GLchar* compiler_log = (GLchar*)malloc(blen);
			 glGetInfoLogARB(*shaderObject, blen, &slen, compiler_log);
			 *pError = std::string(compiler_log);
			 free (compiler_log);
			 return false;
		}
	}

	return true;
}

bool GLGXCreateEffectFromStrings(unsigned int* pProgramObject, std::string* pError, char* pVertexShaderString,char* pFragmentShaderString,char* pGeometryShaderString)
{
	if( pError == NULL)
	{
		pError = new std::string();
	}
	if( pVertexShaderString == NULL)
	{
		*pError = "Vertex shader's String = NULL";
		return false;
	}
	if( pFragmentShaderString == NULL)
	{
		*pError = "Fragment shader's String = NULL";
		return false;
	}

	// Vertex Shader
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	if( GLGXCompileShaderFromString(&vertexShaderObject,pError,pVertexShaderString) == false )
	{
		std::string strFileName = pVertexShaderString;
		strFileName = strFileName + "\n";
		*pError = strFileName + *pError;
		return false;
	}

	// Fragment Shader
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	if( GLGXCompileShaderFromString(&fragmentShaderObject,pError,pFragmentShaderString) == false )
	{
		std::string strFileName = pFragmentShaderString;
		strFileName = strFileName + "\n";
		*pError = strFileName + *pError;
		return false;
	}

	// Geometry shader
	GLuint geometryShaderObject = 0;
	if( pGeometryShaderString != NULL)
	{
		geometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		if( GLGXCompileShaderFromString(&geometryShaderObject,pError,pGeometryShaderString) == false )
		{
			std::string strFileName = pGeometryShaderString;
			strFileName = strFileName + "\n";
			*pError = strFileName + *pError;
			return false;
		}
	}

	*pProgramObject = glCreateProgram();

	glAttachShader(*pProgramObject, vertexShaderObject);
	glAttachShader(*pProgramObject, fragmentShaderObject);
	if( pGeometryShaderString != NULL)
		glAttachShader(*pProgramObject, geometryShaderObject);
	glLinkProgram(*pProgramObject);        
	 
	GLint linked;
	glGetProgramiv(*pProgramObject, GL_LINK_STATUS, &linked);
	if (!linked)
	{  
		if( geometryShaderObject != 0 )
			*pError = "Can't link shaders: "+std::string(pVertexShaderString) + " and "+std::string(pFragmentShaderString) + " and "+std::string(pGeometryShaderString);
		else
			*pError = "Can't link shaders: "+std::string(pVertexShaderString) + " and "+std::string(pFragmentShaderString);
		
		return false;
	}     
	return true;
}


