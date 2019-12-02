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

#ifndef _GLGXFont_H
#define _GLGXFont_H

#include"GLGXMath.h"
#include "GLGXSetup.h"
#include <string>
			
// OpenGL 2.0 and higher
class GLGXFONT
{
private:

	GLuint ShaderProgramObject;

	int _iCharSize;  // nbr of characters per line/columns 
	int _iCharWidth; // char width on the screen
	int _iCharHeight; // char height on the screen

	float _iColor[4];

	float _fScaleX;
	float _fScaleY;

	int _iScreenSizeX;
	int _iScreenSizeY;

	GLuint _shaderLocPosition;
	GLuint _shaderLocTexture;
	GLuint _shaderLocAsciCode;
	GLuint _shaderLocPosScale;
	GLuint _shaderLocMatProj;
	GLuint _shaderLocDiffuseTexture;
	GLuint _shaderLocUV;
	GLuint _shaderLocColor;
	GLuint _shaderLocCharSize;



	GLuint _TexID;

	GLGXMATRIX MatrixProj;

	GLuint VAO;
	GLuint VertexVBO; 
	GLuint TexCoordVBO; 
	GLuint IndexVBO;

	GLboolean _EnableBlend;
	GLboolean _EnableDepthTest;
	GLboolean _EnableCullFace;
	GLfloat   _CurrentColor[4];

	void BuildFont(void);

public:

	void SetNbrCharLine(int i_Nbr);
	void SetScale(float i_fScaleX,float i_fScaleY);
	void SetRGBColor(int i_R,int i_G,int i_B);

	void Begin( void );
	void DrawText(GLint x, GLint y, int set, const char *fmt, ...);
	void End( void );


	bool Create( std::string i_FileName );
	bool CreateEx( std::string i_FileName, int i_iScreenWidth, int i_iScreenHeight,int i_iCharWidth, int i_iCharHeight);
	void Release(void);

	GLGXFONT(void);
    ~GLGXFONT(void);

};

// OpenGL 1.1 and compatible
class GLGXFONT_1_1
{
private:

	GLuint _DLBase;

	int _iCharSize;  // nbr of characters per line/columns 
	int _iCharWidth; // char width on the screen
	int _iCharHeight; // char height on the screen

	int _iColor[3];

	float _fScaleX;
	float _fScaleY;

	int _iScreenSizeX;
	int _iScreenSizeY;

	GLuint _TexID;

	GLboolean _EnableLighting;
	GLboolean _EnableBlend;
	GLboolean _EnableDepthTest;
	GLboolean _EnableTexture2d;
	GLboolean _EnableCullFace;
	GLfloat   _CurrentColor[4];

	void BuildFont(void);

public:

	void SetNbrCharLine(int i_Nbr);
	void SetScale(float i_fScaleX,float i_fScaleY);
	void SetRGBColor(int i_R,int i_G,int i_B);

	void Begin( void );
	void DrawText(GLint x, GLint y, int set, const char *fmt, ...);
	void End( void );


	bool Create( std::string i_FileName );
	bool CreateEx( std::string i_FileName, int i_iScreenWidth, int i_iScreenHeight,int i_iCharWidth, int i_iCharHeight);
	void Release(void);

	GLGXFONT_1_1(void);
    ~GLGXFONT_1_1(void);

};

#endif	// _GLGXFont_H
