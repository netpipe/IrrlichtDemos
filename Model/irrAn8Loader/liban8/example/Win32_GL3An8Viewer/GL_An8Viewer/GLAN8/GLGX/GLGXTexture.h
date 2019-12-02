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

#ifndef _GLGXTEXTURE_H
#define _GLGXTEXTURE_H

#include "GLGXSetup.h"



class GLGXIMAGE
{
public:
    unsigned char *Data;	

    int Width;
	int Height;
	int BPP;
	int Format; 

	int	NumMipMaps;

	bool LoadFromFile(char* filename);
	void Release( void );

	void SetColorKey(int keyR, int keyG, int keyB, int newR, int newG,int newB);
	void SetColorKey(int keyR, int keyG, int keyB, int newR, int newG,int newB, int newA );
	void SetAverageColorAsAlpha( int threshold );
	void FillAllWithRGB( int newR, int newG, int newB );

	GLGXIMAGE();
	~GLGXIMAGE();
};


bool GLGXCreateTextureFromFile(char* filename,GLuint* pTextureID);

bool GLGXCreateEmptyTexture(GLuint* pTextureID, GLint width, GLint height, int red, int green, int blue, int alpha  );

bool GLGXCreateCubeTextureFromFile(
	char *filenameRight,
	char *filenameLeft,
	char *filenameUp,
	char *filenameDown,
	char *filenameFront,
	char *filenameBack,
	GLuint* pTextureID
);

bool GLGXCreateNormalisationCubeMapTexture(GLuint* pTextureID,int size);

#endif	// _GLGXTEXTURE_H
