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

// Thanks to nehe.gamedev.net

#include"GLGXFont.h"
#include"GLGXTexture.h"

#include"GLGXShader.h"


#include <stdio.h>
#include <stdarg.h> // Linux


char g_GLGXFontVertexShaderString [] =
{
	"#version 130\n"
	"uniform mat4 g_MatProj;\n"
	"uniform vec4 g_DiffuseColor;\n"
	"out vec4 g_TextureCoord;\n"
	"uniform sampler2D g_DiffuseTexture;\n"
	"in vec3 g_vboPosition; \n"
	"in vec2 g_vboTexture; \n"
	"uniform vec4 g_PosScale;\n"
	"uniform vec2 g_uv;\n"
	"uniform float g_CharSize;\n"
	"out vec4 go_color;\n"
	"\n"
	"void main(void)\n"
	"{\n"
	"	g_TextureCoord = vec4(\n"
	"		g_uv.x+g_vboTexture.x*g_CharSize,\n"
	"		1.0f-(g_uv.y+g_vboTexture.y*g_CharSize),\n"
	"		1,1);\n"
	"	go_color  = g_DiffuseColor;\n"
	"	\n"
	"	vec4 pos = vec4(g_vboPosition,1);\n"
	"	pos = pos * vec4(g_PosScale.z,g_PosScale.w,1.0f,1.0f);\n"
	"	pos = pos + vec4(g_PosScale.x,g_PosScale.y,0.0f,0.0f);\n"
	"	gl_Position    = g_MatProj * pos; \n"
	"}   \n"
};

char g_GLGXFontFragmentShaderString [] =
{
	"#version 130\n"
	"in vec4 g_TextureCoord;\n"
	"uniform sampler2D g_DiffuseTexture;\n"
	"in vec4 go_color;\n"
	"out vec4 o_color;\n"
	"\n"
	"void main (void)\n"
	"{\n"
	"	vec4 color = texture2D(g_DiffuseTexture,g_TextureCoord.st);\n"
	"	o_color = go_color * color;\n"
	"}\n" 
};

GLGXFONT::GLGXFONT()
{
	ShaderProgramObject = 0;

	_fScaleX = 1.0f;
	_fScaleY = 1.0f;

	_iScreenSizeX = 640;
	_iScreenSizeY = 480;

	_iCharSize = 16;
	_iCharWidth  = 10;  
	_iCharHeight = 16; 

	_iColor[0] = 1.0f;
	_iColor[1] = 1.0f;
	_iColor[2] = 1.0f;
	_iColor[3] = 1.0f;

	_TexID  = 0;

	_EnableBlend     = false;
	_EnableDepthTest = true;
	_EnableCullFace  = false;

	VAO		 = 0;
	VertexVBO = 0;
	TexCoordVBO = 0;
	IndexVBO = 0;
}

GLGXFONT::~GLGXFONT()
{
  Release();
}

void GLGXFONT::SetNbrCharLine(int i_Nbr)
{
	_iCharSize = i_Nbr;

}

bool GLGXFONT::CreateEx( std::string i_FileName, int i_iScreenWidth, int i_iScreenHeight,int i_iCharWidth, int i_iCharHeight)
{
	_iScreenSizeX = i_iScreenWidth;
	_iScreenSizeY = i_iScreenHeight;
	_iCharWidth   = i_iCharWidth;
	_iCharHeight  = i_iCharHeight;
	return Create(i_FileName);
}

bool GLGXFONT::Create( std::string i_FileName )
{
	//int errorTest = 0;
	GLGXIMAGE image;
	if(image.LoadFromFile( (char*)i_FileName.c_str()))
	{ 
	  image.SetAverageColorAsAlpha( 64 ); 
	  image.FillAllWithRGB( 255, 255 , 255 );	
      glGenTextures(1, &_TexID);	//errorTest = glGetError();			    
      glBindTexture(GL_TEXTURE_2D, _TexID);//errorTest = glGetError();
	  glTexImage2D(GL_TEXTURE_2D, 0, image.Format, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);//errorTest = glGetError();
	}
	else
	{
		return false;
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );//errorTest = glGetError();			    //GL_LINEAR);		
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );//errorTest = glGetError();			    //GL_LINEAR);		

	std::string error;
	if( GLGXCreateEffectFromStrings(&ShaderProgramObject,&error,g_GLGXFontVertexShaderString,g_GLGXFontFragmentShaderString) == false ) // Load shaders
	{
		return false;
	}
	//errorTest = glGetError();
	glUseProgram(ShaderProgramObject);//errorTest = glGetError();

	_shaderLocPosition	= glGetAttribLocation(ShaderProgramObject, "g_vboPosition");//errorTest = glGetError();
	_shaderLocTexture	= glGetAttribLocation(ShaderProgramObject, "g_vboTexture");//errorTest = glGetError();
	_shaderLocPosScale	= glGetUniformLocation(ShaderProgramObject, "g_PosScale");//errorTest = glGetError();
	_shaderLocMatProj		 = glGetUniformLocation(ShaderProgramObject, "g_MatProj");//errorTest = glGetError();
	_shaderLocDiffuseTexture = glGetUniformLocation(ShaderProgramObject, "g_DiffuseTexture");//errorTest = glGetError();
	_shaderLocUV = glGetUniformLocation(ShaderProgramObject, "g_uv");//errorTest = glGetError();
	_shaderLocColor = glGetUniformLocation(ShaderProgramObject, "g_DiffuseColor");//errorTest = glGetError();
	_shaderLocCharSize = glGetUniformLocation(ShaderProgramObject, "g_CharSize");//errorTest = glGetError();

	glBindFragDataLocation(ShaderProgramObject,0,"o_color");//errorTest = glGetError();


	float vertices[12]={
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	float textureCoords[8]={
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f
	};
	int indices[12]={
		0,1,2,
		0,2,3
	};

	// VAO
	glGenVertexArrays(1, &VAO);		//errorTest = glGetError();
	glBindVertexArray(VAO);			//errorTest = glGetError();
		
	// Vertices
	glGenBuffersARB( 1, &VertexVBO );																				//errorTest = glGetError();
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, VertexVBO );																//errorTest = glGetError();
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*4, vertices, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );	//errorTest = glGetError();
	int nParam_ArrayObjectSize = 0;
	glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );					//errorTest = glGetError();
	if( nParam_ArrayObjectSize <= 0 )
		return GL_FALSE;
	glVertexAttribPointer(_shaderLocPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);											//errorTest = glGetError();
	glEnableVertexAttribArray(_shaderLocPosition);																	//errorTest = glGetError();

	// TextureCoords
	glGenBuffersARB( 1, &TexCoordVBO );																					//errorTest = glGetError();
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, TexCoordVBO );																//errorTest = glGetError();
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*4, textureCoords, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );	//errorTest = glGetError();
	nParam_ArrayObjectSize = 0;
	glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );						//errorTest = glGetError();
	if( nParam_ArrayObjectSize <= 0 )
		return GL_FALSE;
	glVertexAttribPointer(_shaderLocTexture, 2, GL_FLOAT, GL_FALSE, 0, 0);											//errorTest = glGetError();
	glEnableVertexAttribArray(_shaderLocTexture);																	//errorTest = glGetError();

	// Index
	nParam_ArrayObjectSize = 0;
	glGenBuffersARB( 1, &IndexVBO );							//errorTest = glGetError();
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, IndexVBO );	//errorTest = glGetError();
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(int)*6, indices, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );	//errorTest = glGetError();
	glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );				//errorTest = glGetError();
	if( nParam_ArrayObjectSize <= 0 )
		return GL_FALSE;

	float fCharSize = 1.0f/float(_iCharSize);
	glUniform1f(_shaderLocCharSize,fCharSize);	//errorTest = glGetError();

	glBindVertexArray(0);						//errorTest = glGetError();
	glUseProgram(0);							//errorTest = glGetError();

	return true;
}

void GLGXFONT::Begin(void)
{
	int errorTest = 0;
	glUseProgram(ShaderProgramObject);				//errorTest = glGetError();

	_EnableBlend     = glIsEnabled(GL_BLEND);		//errorTest = glGetError();// deprecated ???
	_EnableDepthTest = glIsEnabled(GL_DEPTH_TEST);	//errorTest = glGetError();
	_EnableCullFace  = glIsEnabled(GL_CULL_FACE);	//errorTest = glGetError();

	if( !_EnableBlend )
		glEnable(GL_BLEND);										//errorTest = glGetError();	// deprecated ???
	if( _EnableDepthTest )
		glDisable(GL_DEPTH_TEST);								//errorTest = glGetError();
	if( _EnableCullFace )
		glDisable(GL_CULL_FACE);								//errorTest = glGetError();

	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			//errorTest = glGetError();

	GLGXMatrixOrthoOffCenter2D(&MatrixProj,0.0f, _iScreenSizeX, _iScreenSizeY, 0);
	glUniformMatrix4fv(_shaderLocMatProj,1,false,MatrixProj);	//errorTest = glGetError();
      
	glActiveTexture(GL_TEXTURE0);								//errorTest = glGetError();
	glBindTexture(GL_TEXTURE_2D,_TexID);						//errorTest = glGetError();
	glUniform1i(_shaderLocDiffuseTexture, 0);					//errorTest = glGetError();

	// Render vertices ////////////////////////
	glBindVertexArray(VAO);										//errorTest = glGetError();		
}

void GLGXFONT::End(void)
{
	//int errorTest = 0;

	glBindVertexArray(0);			//errorTest = glGetError();

	if( !_EnableBlend )
		glDisable(GL_BLEND);		//errorTest = glGetError(); // deprecated  ???
	if( _EnableDepthTest )
		glEnable(GL_DEPTH_TEST);	//errorTest = glGetError();
	if( _EnableCullFace )
		glEnable(GL_CULL_FACE);		//errorTest = glGetError();

	glUseProgram(0);				//errorTest = glGetError();
}

void GLGXFONT::DrawText(GLint x, GLint y, int set, const char *fmt, ...)	
{
	//int errorTest = 0;
	GLGXVECTOR2 uv;
	int asciCode;
	char		text[1024];			// string
	va_list	ap;					// List of arguments

	if(fmt == NULL)				// no text
		return;			

	va_start(ap, fmt);			// Add variables to the the text
	vsprintf(text, fmt, ap);							
	va_end(ap);												

	if (set > 1)				// security
		set = 1;				// set = 1 (Italic)


	int nbrCharacters = (GLsizei)strlen(text);

	GLGXVECTOR4 positionScale; // position (x,y) and scale (z,w)
	positionScale.x = x;
	positionScale.y = y;
	positionScale.z = _iCharWidth*_fScaleX;
	positionScale.w = _iCharHeight*_fScaleY;

	glBindFragDataLocation(ShaderProgramObject,0,"o_color");//errorTest = glGetError();
	
	glUniform4fv(_shaderLocColor, 1,_iColor); //errorTest = glGetError();

	for( int i = 0; i < nbrCharacters; ++i)
	{
		asciCode = text[i] - 32 + (128 * set);

		uv.x = (float)(asciCode % _iCharSize) / float(_iCharSize);	// Charactere position X
		uv.y = (float)(asciCode / _iCharSize) / float(_iCharSize);	// Charactere position Y
		glUniform2fv(_shaderLocUV, 1,uv);		//errorTest = glGetError();

		positionScale.x += _iCharWidth*_fScaleX;
		glUniform4fv(_shaderLocPosScale, 1,positionScale);	//errorTest = glGetError();

		// Draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // errorTest = glGetError();// 4 indices
	}
}




void GLGXFONT::SetScale(float i_fScaleX,float i_fScaleY)
{
  _fScaleX = i_fScaleX;
  _fScaleY = i_fScaleY;
}

void GLGXFONT::SetRGBColor(int i_R,int i_G,int i_B)
{
	_iColor[0] = float(i_R)/255.0f;
	_iColor[1] = float(i_G)/255.0f;
	_iColor[2] = float(i_B)/255.0f;
}

void GLGXFONT::Release(void)										
{

	glDeleteBuffersARB(1, &VertexVBO);
	glDeleteBuffersARB(1, &TexCoordVBO);
	glDeleteBuffersARB(1, &IndexVBO);

	if( _TexID != 0 ) // 0 is never used by glGenTextures
	{
		glDeleteTextures(1, &_TexID);
	}

	glDeleteObjectARB(ShaderProgramObject);
	ShaderProgramObject = 0;
}


