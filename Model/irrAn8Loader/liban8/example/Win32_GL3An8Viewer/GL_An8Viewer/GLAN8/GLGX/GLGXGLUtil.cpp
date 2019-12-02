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

#include"GLGXGLUtil.h"



GLboolean GLGXIsPointOccluded( GLint* pViewportMatrix,GLdouble* pProjectionMatrix, GLdouble* pModelViewMatrix, GLfloat x, GLfloat y, GLfloat z)
{	
  // How to get matrices ?
  //glGetIntegerv(GL_VIEWPORT, pViewportMatrix); 
  //glGetDoublev (GL_MODELVIEW_MATRIX, pModelViewMatrix); 
  //glGetDoublev (GL_PROJECTION_MATRIX, pProjectionMatrix); 
  
  GLdouble winx, winy, winz;				
  GLdouble flareZ;					
  GLfloat bufferZ;	
  
  gluProject(x, y, z, pModelViewMatrix, pProjectionMatrix, pViewportMatrix, &winx, &winy, &winz);
  flareZ = winz;

  glReadPixels((GLint)winx, (GLint)winy,1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &bufferZ);

  if (bufferZ < flareZ)
    return GL_TRUE;
  else
    return GL_FALSE;

}

