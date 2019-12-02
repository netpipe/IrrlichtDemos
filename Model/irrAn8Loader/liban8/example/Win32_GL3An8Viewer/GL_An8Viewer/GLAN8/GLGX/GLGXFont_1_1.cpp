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

#include <stdio.h>
#include <stdarg.h> // Linux

GLGXFONT_1_1::GLGXFONT_1_1()
{
	_fScaleX = 1.0f;
	_fScaleY = 1.0f;

	_iScreenSizeX = 640;
	_iScreenSizeY = 480;

	_iCharSize = 16;
	_iCharWidth  = 10;  
	_iCharHeight = 16; 

	_iColor[0] = 255;
	_iColor[1] = 255;
	_iColor[2] = 255;

	_TexID  = 0;
	_DLBase = 0;

	_EnableLighting  = false;
	_EnableBlend     = false;
	_EnableDepthTest = true;
	_EnableTexture2d = false;
	_EnableCullFace  = false;

}

GLGXFONT_1_1::~GLGXFONT_1_1()
{
  Release();
}

void GLGXFONT_1_1::SetNbrCharLine(int i_Nbr)
{
	_iCharSize = i_Nbr;
}

bool GLGXFONT_1_1::CreateEx( std::string i_FileName, int i_iScreenWidth, int i_iScreenHeight,int i_iCharWidth, int i_iCharHeight)
{
	_iScreenSizeX = i_iScreenWidth;
	_iScreenSizeY = i_iScreenHeight;
	_iCharWidth   = i_iCharWidth;
	_iCharHeight  = i_iCharHeight;
	return Create(i_FileName);
}

bool GLGXFONT_1_1::Create( std::string i_FileName )
{
	GLGXIMAGE image;
	if(image.LoadFromFile( (char*)i_FileName.c_str()))
	{ 

	  image.SetAverageColorAsAlpha( 64 ); 
	  image.FillAllWithRGB( 255, 255 , 255 );

      glGenTextures(1, &_TexID);				    
      glBindTexture(GL_TEXTURE_2D, _TexID);
	  glTexImage2D(GL_TEXTURE_2D, 0, image.BPP, image.Width,image.Height, 0, image.Format,GL_UNSIGNED_BYTE,image.Data);
	}
	else
	{
		return false;
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );//GL_LINEAR);		
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );//GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	

  int iLoop = 0;
  float fCharPosX = 0;
  float fCharPosY = 0;
  float fFloatSize = 0.0f;
  fFloatSize += _iCharSize;
  float fTemp = 1.0f / fFloatSize;

  _DLBase = glGenLists(256);							    //  256 Display Lists creation

	for (iLoop = 0; iLoop < 256; iLoop++)				
	{
		fCharPosX = (float)(iLoop % _iCharSize) / fFloatSize;	// Charactere position X
		fCharPosY = (float)(iLoop / _iCharSize) / fFloatSize;	// Charactere position Y

		glNewList(_DLBase + iLoop, GL_COMPILE);					// Creation 

			glBegin(GL_QUADS);									// one charactère
	  
				glTexCoord2f(fCharPosX, 1.0f - fCharPosY - fTemp);		    // Texture Coord (bottom left)
				glVertex2d(0,_iCharHeight);									// Vertex Coord (bottom gauche)
      
				glTexCoord2f(fCharPosX + fTemp, 1.0f - fCharPosY - fTemp);	// Texture Coord (bottom right)
				glVertex2i(_iCharWidth,_iCharHeight);						// Vertex Coord (Bbottomas right)

				glTexCoord2f(fCharPosX + fTemp, 1.0f - fCharPosY - 0.001f);	// Texture Coord (top right)
				glVertex2i(_iCharWidth,0);									// Vertex Coord (top right)
	  
				glTexCoord2f(fCharPosX ,1.0f - fCharPosY - 0.001f);	// Texture Coord (top right)
				glVertex2i(0, 0);									// Vertex Coord (top left)
	  
			glEnd();											
			glTranslated(_iCharWidth, 0, 0);					// offset 
		glEndList();											// End of Display List
	}	
	return true;
}

void GLGXFONT_1_1::Begin(void)
{
	_EnableLighting  = glIsEnabled(GL_LIGHTING);
	_EnableBlend     = glIsEnabled(GL_BLEND);
	_EnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
	_EnableTexture2d = glIsEnabled(GL_TEXTURE_2D);
	_EnableCullFace  = glIsEnabled(GL_CULL_FACE);

	if( _EnableLighting )
		glDisable(GL_LIGHTING); 
	if( !_EnableBlend )
		glEnable(GL_BLEND);	
	if( _EnableDepthTest )
		glDisable(GL_DEPTH_TEST);
	if( !_EnableTexture2d )
		glEnable(GL_TEXTURE_2D);
	if( _EnableCullFace )
		glDisable(GL_CULL_FACE);

	/////////////// 2D mode  /////////////////////////
	glMatrixMode(GL_PROJECTION);						
	glPushMatrix();									
	glLoadIdentity();								
	glOrtho( 0, _iScreenSizeX, _iScreenSizeY, 0, 0, 1 );	
	glMatrixMode(GL_MODELVIEW);								
  
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGetFloatv( GL_CURRENT_COLOR ,_CurrentColor);
	glColor3ub(_iColor[0],_iColor[1],_iColor[2]);
}

void GLGXFONT_1_1::End(void)
{
	if( _EnableLighting )
		glEnable(GL_LIGHTING);
	if( !_EnableBlend )
		glDisable(GL_BLEND);
	if( _EnableDepthTest )
		glEnable(GL_DEPTH_TEST);
	if( !_EnableTexture2d )
		glDisable(GL_TEXTURE_2D);
	if( _EnableCullFace )
		glEnable(GL_CULL_FACE);

	/////////// Previous mode  /////////////////
	glMatrixMode( GL_PROJECTION );							
	glPopMatrix();											
	glMatrixMode( GL_MODELVIEW );	

	glColor4fv(_CurrentColor);
}

void GLGXFONT_1_1::DrawText(GLint x, GLint y, int set, const char *fmt, ...)	
{
  char		text[1024];			// string
  va_list	ap;					// List of arguments

  if(fmt == NULL)				// no text
	return;			

  va_start(ap, fmt);			// Add variables to the the text
  vsprintf(text, fmt, ap);							
  va_end(ap);												

  if (set > 1)				// security
	set = 1;				// set = 1 (Italic)

	glBindTexture(GL_TEXTURE_2D, _TexID);

	glLoadIdentity();								
	glTranslated(x, y, 0);				// Position of text (0,0 - top left)
	glListBase(_DLBase - 32 + (128 * set));
	glScalef(_fScaleX,_fScaleY,1.0f);						
	glCallLists((GLsizei)strlen(text),GL_UNSIGNED_BYTE, text); // display text

}




void GLGXFONT_1_1::SetScale(float i_fScaleX,float i_fScaleY)
{
  _fScaleX = i_fScaleX;
  _fScaleY = i_fScaleY;
}

void GLGXFONT_1_1::SetRGBColor(int i_R,int i_G,int i_B)
{
	_iColor[0] = i_R;
	_iColor[1] = i_G;
	_iColor[2] = i_B;
}

void GLGXFONT_1_1::Release(void)										
{
	if( _DLBase != 0 )
	{
		glDeleteLists(_DLBase,256);			// delete Display Lists
	}

	if( _TexID != 0 ) // 0 is never used by glGenTextures
	{
		glDeleteTextures(1, &_TexID);
	}
}


