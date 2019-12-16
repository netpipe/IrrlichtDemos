// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#include "utOpenGL.h"
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;


namespace glExt
{
	bool EXT_framebuffer_object = false;
	bool EXT_framebuffer_multisample = false;
	bool EXT_texture_filter_anisotropic = false;
	bool EXT_texture_compression_s3tc = false;
	bool ARB_texture_float = false;
	bool ARB_texture_non_power_of_two = false;

	int	majorVersion = 1, minorVersion = 1;
}


//OpenGL 1.2
PFNGLBLENDCOLORPROC glBlendColor = 0x0;
PFNGLBLENDEQUATIONPROC glBlendEquation = 0x0;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = 0x0;
PFNGLCOLORTABLEPROC glColorTable = 0x0;
PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv = 0x0;
PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv = 0x0;
PFNGLCOPYCOLORTABLEPROC glCopyColorTable = 0x0;
PFNGLGETCOLORTABLEPROC glGetColorTable = 0x0;
PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv = 0x0;
PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv = 0x0;
PFNGLCOLORSUBTABLEPROC glColorSubTable = 0x0;
PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable = 0x0;
PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D = 0x0;
PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D = 0x0;
PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf = 0x0;
PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv = 0x0;
PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri = 0x0;
PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv = 0x0;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D = 0x0;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D = 0x0;
PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter = 0x0;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv = 0x0;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv = 0x0;
PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter = 0x0;
PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D = 0x0;
PFNGLGETHISTOGRAMPROC glGetHistogram = 0x0;
PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv = 0x0;
PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv = 0x0;
PFNGLGETMINMAXPROC glGetMinmax = 0x0;
PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv = 0x0;
PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv = 0x0;
PFNGLHISTOGRAMPROC glHistogram = 0x0;
PFNGLMINMAXPROC glMinmax = 0x0;
PFNGLRESETHISTOGRAMPROC glResetHistogram = 0x0;
PFNGLRESETMINMAXPROC glResetMinmax = 0x0;
PFNGLTEXIMAGE3DPROC glTexImage3D = 0x0;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = 0x0;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = 0x0;


// OpenGL 1.3
PFNGLACTIVETEXTUREPROC glActiveTexture = 0x0;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture = 0x0;
PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d = 0x0;
PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv = 0x0;
PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f = 0x0;
PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv = 0x0;
PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i = 0x0;
PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv = 0x0;
PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s = 0x0;
PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv = 0x0;
PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d = 0x0;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv = 0x0;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f = 0x0;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv = 0x0;
PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i = 0x0;
PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv = 0x0;
PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s = 0x0;
PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv = 0x0;
PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d = 0x0;
PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv = 0x0;
PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f = 0x0;
PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv = 0x0;
PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i = 0x0;
PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv = 0x0;
PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s = 0x0;
PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv = 0x0;
PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d = 0x0;
PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv = 0x0;
PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f = 0x0;
PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv = 0x0;
PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i = 0x0;
PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv = 0x0;
PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s = 0x0;
PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv = 0x0;
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf = 0x0;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd = 0x0;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf = 0x0;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd = 0x0;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = 0x0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = 0x0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = 0x0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = 0x0;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = 0x0;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = 0x0;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = 0x0;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = 0x0;


// OpenGL 1.4
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = 0x0;
PFNGLFOGCOORDFPROC glFogCoordf = 0x0;
PFNGLFOGCOORDFVPROC glFogCoordfv = 0x0;
PFNGLFOGCOORDDPROC glFogCoordd = 0x0;
PFNGLFOGCOORDDVPROC glFogCoorddv = 0x0;
PFNGLFOGCOORDPOINTERPROC glFogCoordPointer = 0x0;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = 0x0;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = 0x0;
PFNGLPOINTPARAMETERFPROC glPointParameterf = 0x0;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = 0x0;
PFNGLPOINTPARAMETERIPROC glPointParameteri = 0x0;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = 0x0;
PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b = 0x0;
PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv = 0x0;
PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d = 0x0;
PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv = 0x0;
PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f = 0x0;
PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv = 0x0;
PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i = 0x0;
PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv = 0x0;
PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s = 0x0;
PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv = 0x0;
PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub = 0x0;
PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv = 0x0;
PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui = 0x0;
PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv = 0x0;
PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us = 0x0;
PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv = 0x0;
PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer = 0x0;
PFNGLWINDOWPOS2DPROC glWindowPos2d = 0x0;
PFNGLWINDOWPOS2DVPROC glWindowPos2dv = 0x0;
PFNGLWINDOWPOS2FPROC glWindowPos2f = 0x0;
PFNGLWINDOWPOS2FVPROC glWindowPos2fv = 0x0;
PFNGLWINDOWPOS2IPROC glWindowPos2i = 0x0;
PFNGLWINDOWPOS2IVPROC glWindowPos2iv = 0x0;
PFNGLWINDOWPOS2SPROC glWindowPos2s = 0x0;
PFNGLWINDOWPOS2SVPROC glWindowPos2sv = 0x0;
PFNGLWINDOWPOS3DPROC glWindowPos3d = 0x0;
PFNGLWINDOWPOS3DVPROC glWindowPos3dv = 0x0;
PFNGLWINDOWPOS3FPROC glWindowPos3f = 0x0;
PFNGLWINDOWPOS3FVPROC glWindowPos3fv = 0x0;
PFNGLWINDOWPOS3IPROC glWindowPos3i = 0x0;
PFNGLWINDOWPOS3IVPROC glWindowPos3iv = 0x0;
PFNGLWINDOWPOS3SPROC glWindowPos3s = 0x0;
PFNGLWINDOWPOS3SVPROC glWindowPos3sv = 0x0;


// OpenGL 1.5
PFNGLGENQUERIESPROC glGenQueries = 0x0;
PFNGLDELETEQUERIESPROC glDeleteQueries = 0x0;
PFNGLISQUERYPROC glIsQuery = 0x0;
PFNGLBEGINQUERYPROC glBeginQuery = 0x0;
PFNGLENDQUERYPROC glEndQuery = 0x0;
PFNGLGETQUERYIVPROC glGetQueryiv = 0x0;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = 0x0;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = 0x0;
PFNGLBINDBUFFERPROC glBindBuffer = 0x0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0x0;
PFNGLGENBUFFERSPROC glGenBuffers = 0x0;
PFNGLISBUFFERPROC glIsBuffer = 0x0;
PFNGLBUFFERDATAPROC glBufferData = 0x0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0x0;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = 0x0;
PFNGLMAPBUFFERPROC glMapBuffer = 0x0;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = 0x0;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = 0x0;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv = 0x0;


// OpenGL 2.0
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = 0x0;
PFNGLDRAWBUFFERSPROC glDrawBuffers = 0x0;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate = 0x0;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate = 0x0;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate = 0x0;
PFNGLATTACHSHADERPROC glAttachShader = 0x0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0x0;
PFNGLCOMPILESHADERPROC glCompileShader = 0x0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0x0;
PFNGLCREATESHADERPROC glCreateShader = 0x0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0x0;
PFNGLDELETESHADERPROC glDeleteShader = 0x0;
PFNGLDETACHSHADERPROC glDetachShader = 0x0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0x0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0x0;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0x0;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0x0;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = 0x0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0x0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0x0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0x0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0x0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0x0;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = 0x0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0x0;
PFNGLGETUNIFORMFVPROC glGetUniformfv = 0x0;
PFNGLGETUNIFORMIVPROC glGetUniformiv = 0x0;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = 0x0;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = 0x0;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = 0x0;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = 0x0;
PFNGLISPROGRAMPROC glIsProgram = 0x0;
PFNGLISSHADERPROC glIsShader = 0x0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0x0;
PFNGLSHADERSOURCEPROC glShaderSource = 0x0;
PFNGLUSEPROGRAMPROC glUseProgram = 0x0;
PFNGLUNIFORM1FPROC glUniform1f = 0x0;
PFNGLUNIFORM2FPROC glUniform2f = 0x0;
PFNGLUNIFORM3FPROC glUniform3f = 0x0;
PFNGLUNIFORM4FPROC glUniform4f = 0x0;
PFNGLUNIFORM1IPROC glUniform1i = 0x0;
PFNGLUNIFORM2IPROC glUniform2i = 0x0;
PFNGLUNIFORM3IPROC glUniform3i = 0x0;
PFNGLUNIFORM4IPROC glUniform4i = 0x0;
PFNGLUNIFORM1FVPROC glUniform1fv = 0x0;
PFNGLUNIFORM2FVPROC glUniform2fv = 0x0;
PFNGLUNIFORM3FVPROC glUniform3fv = 0x0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0x0;
PFNGLUNIFORM1IVPROC glUniform1iv = 0x0;
PFNGLUNIFORM2IVPROC glUniform2iv = 0x0;
PFNGLUNIFORM3IVPROC glUniform3iv = 0x0;
PFNGLUNIFORM4IVPROC glUniform4iv = 0x0;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = 0x0;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = 0x0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0x0;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = 0x0;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d = 0x0;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv = 0x0;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = 0x0;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = 0x0;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s = 0x0;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv = 0x0;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d = 0x0;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv = 0x0;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f = 0x0;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = 0x0;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s = 0x0;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv = 0x0;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d = 0x0;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv = 0x0;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f = 0x0;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = 0x0;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s = 0x0;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv = 0x0;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv = 0x0;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv = 0x0;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv = 0x0;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub = 0x0;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv = 0x0;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv = 0x0;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv = 0x0;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv = 0x0;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d = 0x0;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv = 0x0;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = 0x0;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = 0x0;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv = 0x0;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s = 0x0;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv = 0x0;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv = 0x0;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv = 0x0;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv = 0x0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0x0;


// OpenGL 2.1
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv = 0x0;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv = 0x0;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv = 0x0;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv = 0x0;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv = 0x0;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv = 0x0;


// GL_EXT_framebuffer_object
PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = 0x0;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = 0x0;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = 0x0;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = 0x0;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = 0x0;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = 0x0;
PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = 0x0;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = 0x0;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = 0x0;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = 0x0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = 0x0;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = 0x0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = 0x0;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = 0x0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = 0x0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = 0x0;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = 0x0;


// GL_EXT_framebuffer_blit
PFNGLBLITFRAMEBUFFEREXTPROC glBlitFramebufferEXT = 0x0;


// GL_EXT_framebuffer_multisample
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT = 0x0;


bool isExtensionSupported( string extString )
{
	size_t pos = 0;
	string extensions = (char *)glGetString( GL_EXTENSIONS );
	
	while( (pos = extensions.find( extString, pos )) != string::npos )
	{
		char c = extensions.c_str()[pos + extString.length()];
		if( c == ' ' || c == '\0' ) return true;
		pos += extString.length();
	}

	return false;
}


void getOpenGLVersion()
{
	string version = (char *)glGetString( GL_VERSION );
	
	size_t pos1 = version.find( "." );
	size_t pos2 = version.find( ".", pos1 + 1 );
	if( pos2 == string::npos ) pos2 = version.find( " ", pos1 + 1 );
	if( pos2 == string::npos ) pos2 = version.length();
	
	glExt::majorVersion = atoi( version.substr( 0, pos1 ).c_str() );
	glExt::minorVersion = atoi( version.substr( pos1 + 1, pos2 ).c_str() );
}


void *platGetProcAddress( const char *funcName )
{
#if defined( PLATFORM_WIN )
	return (void *)wglGetProcAddress( funcName );
#elif defined( PLATFORM_MAC )
	CFStringRef functionName = CFStringCreateWithCString( kCFAllocatorDefault, funcName, kCFStringEncodingASCII );
	CFURLRef bundleURL = CFURLCreateWithFileSystemPath(
		kCFAllocatorDefault, CFSTR( "/System/Library/Frameworks/OpenGL.framework" ), kCFURLPOSIXPathStyle, true );
	CFBundleRef bundle = CFBundleCreate( kCFAllocatorDefault, bundleURL );
   
	void *function = CFBundleGetFunctionPointerForName( bundle, functionName );
   
	CFRelease( bundle );
	CFRelease( bundleURL );
	CFRelease( functionName );
   
	return function; 
#else
	return (void *)glXGetProcAddressARB( (const GLubyte *)funcName );
#endif
}


bool initOpenGLExtensions()
{
	bool r = true;
	
	getOpenGLVersion();
	
	// OpenGL 1.2
	r &= (glBlendColor = (PFNGLBLENDCOLORPROC) platGetProcAddress( "glBlendColor" )) != 0x0;
	r &= (glBlendEquation = (PFNGLBLENDEQUATIONPROC) platGetProcAddress( "glBlendEquation" )) != 0x0;
	r &= (glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) platGetProcAddress( "glDrawRangeElements" )) != 0x0;
	r &= (glTexImage3D = (PFNGLTEXIMAGE3DPROC) platGetProcAddress( "glTexImage3D" )) != 0x0;
	r &= (glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) platGetProcAddress( "glTexSubImage3D" )) != 0x0;
	r &= (glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) platGetProcAddress( "glCopyTexSubImage3D" )) != 0x0;
	if( isExtensionSupported( "ARB_imaging" ) )
	{
		r &= (glColorTable = (PFNGLCOLORTABLEPROC) platGetProcAddress( "glColorTable" )) != 0x0;
		r &= (glColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFVPROC) platGetProcAddress( "glColorTableParameterfv" )) != 0x0;
		r &= (glColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIVPROC) platGetProcAddress( "glColorTableParameteriv" )) != 0x0;
		r &= (glCopyColorTable = (PFNGLCOPYCOLORTABLEPROC) platGetProcAddress( "glCopyColorTable" )) != 0x0;
		r &= (glGetColorTable = (PFNGLGETCOLORTABLEPROC) platGetProcAddress( "glGetColorTable" )) != 0x0;
		r &= (glGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFVPROC) platGetProcAddress( "glGetColorTableParameterfv" )) != 0x0;
		r &= (glGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIVPROC) platGetProcAddress( "glGetColorTableParameteriv" )) != 0x0;
		r &= (glColorSubTable = (PFNGLCOLORSUBTABLEPROC) platGetProcAddress( "glColorSubTable" )) != 0x0;
		r &= (glCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLEPROC) platGetProcAddress( "glCopyColorSubTable" )) != 0x0;
		r &= (glConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1DPROC) platGetProcAddress( "glConvolutionFilter1D" )) != 0x0;
		r &= (glConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2DPROC) platGetProcAddress( "glConvolutionFilter2D" )) != 0x0;
		r &= (glConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERFPROC) platGetProcAddress( "glConvolutionParameterf" )) != 0x0;
		r &= (glConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFVPROC) platGetProcAddress( "glConvolutionParameterfv" )) != 0x0;
		r &= (glConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERIPROC) platGetProcAddress( "glConvolutionParameteri" )) != 0x0;
		r &= (glConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIVPROC) platGetProcAddress( "glConvolutionParameteriv" )) != 0x0;
		r &= (glCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1DPROC) platGetProcAddress( "glCopyConvolutionFilter1D" )) != 0x0;
		r &= (glCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2DPROC) platGetProcAddress( "glCopyConvolutionFilter2D" )) != 0x0;
		r &= (glGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTERPROC) platGetProcAddress( "glGetConvolutionFilter" )) != 0x0;
		r &= (glGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFVPROC) platGetProcAddress( "glGetConvolutionParameterfv" )) != 0x0;
		r &= (glGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIVPROC) platGetProcAddress( "glGetConvolutionParameteriv" )) != 0x0;
		r &= (glGetSeparableFilter = (PFNGLGETSEPARABLEFILTERPROC) platGetProcAddress( "glGetSeparableFilter" )) != 0x0;
		r &= (glSeparableFilter2D = (PFNGLSEPARABLEFILTER2DPROC) platGetProcAddress( "glSeparableFilter2D" )) != 0x0;
		r &= (glGetHistogram = (PFNGLGETHISTOGRAMPROC) platGetProcAddress( "glGetHistogram" )) != 0x0;
		r &= (glGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFVPROC) platGetProcAddress( "glGetHistogramParameterfv" )) != 0x0;
		r &= (glGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIVPROC) platGetProcAddress( "glGetHistogramParameteriv" )) != 0x0;
		r &= (glGetMinmax = (PFNGLGETMINMAXPROC) platGetProcAddress( "glGetMinmax" )) != 0x0;
		r &= (glGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFVPROC) platGetProcAddress( "glGetMinmaxParameterfv" )) != 0x0;
		r &= (glGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIVPROC) platGetProcAddress( "glGetMinmaxParameteriv" )) != 0x0;
		r &= (glHistogram = (PFNGLHISTOGRAMPROC) platGetProcAddress( "glHistogram" )) != 0x0;
		r &= (glMinmax = (PFNGLMINMAXPROC) platGetProcAddress( "glMinmax" )) != 0x0;
		r &= (glResetHistogram = (PFNGLRESETHISTOGRAMPROC) platGetProcAddress( "glResetHistogram" )) != 0x0;
		r &= (glResetMinmax = (PFNGLRESETMINMAXPROC) platGetProcAddress( "glResetMinmax" )) != 0x0;
	}

	
	// OpenGL 1.3
	r &= (glActiveTexture = (PFNGLACTIVETEXTUREPROC) platGetProcAddress( "glActiveTexture" )) != 0x0;
	r &= (glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) platGetProcAddress( "glClientActiveTexture" )) != 0x0;
	r &= (glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC) platGetProcAddress( "glMultiTexCoord1d" )) != 0x0;
	r &= (glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC) platGetProcAddress( "glMultiTexCoord1dv" )) != 0x0;
	r &= (glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC) platGetProcAddress( "glMultiTexCoord1f" )) != 0x0;
	r &= (glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC) platGetProcAddress( "glMultiTexCoord1fv" )) != 0x0;
	r &= (glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC) platGetProcAddress( "glMultiTexCoord1i" )) != 0x0;
	r &= (glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC) platGetProcAddress( "glMultiTexCoord1iv" )) != 0x0;
	r &= (glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC) platGetProcAddress( "glMultiTexCoord1s" )) != 0x0;
	r &= (glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC) platGetProcAddress( "glMultiTexCoord1sv" )) != 0x0;
	r &= (glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC) platGetProcAddress( "glMultiTexCoord2d" )) != 0x0;
	r &= (glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC) platGetProcAddress( "glMultiTexCoord2dv" )) != 0x0;
	r &= (glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC) platGetProcAddress( "glMultiTexCoord2f" )) != 0x0;
	r &= (glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC) platGetProcAddress( "glMultiTexCoord2fv" )) != 0x0;
	r &= (glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC) platGetProcAddress( "glMultiTexCoord2i" )) != 0x0;
	r &= (glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC) platGetProcAddress( "glMultiTexCoord2iv" )) != 0x0;
	r &= (glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC) platGetProcAddress( "glMultiTexCoord2s" )) != 0x0;
	r &= (glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC) platGetProcAddress( "glMultiTexCoord2sv" )) != 0x0;
	r &= (glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC) platGetProcAddress( "glMultiTexCoord3d" )) != 0x0;
	r &= (glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC) platGetProcAddress( "glMultiTexCoord3dv" )) != 0x0;
	r &= (glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC) platGetProcAddress( "glMultiTexCoord3f" )) != 0x0;
	r &= (glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC) platGetProcAddress( "glMultiTexCoord3fv" )) != 0x0;
	r &= (glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC) platGetProcAddress( "glMultiTexCoord3i" )) != 0x0;
	r &= (glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC) platGetProcAddress( "glMultiTexCoord3iv" )) != 0x0;
	r &= (glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC) platGetProcAddress( "glMultiTexCoord3s" )) != 0x0;
	r &= (glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC) platGetProcAddress( "glMultiTexCoord3sv" )) != 0x0;
	r &= (glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC) platGetProcAddress( "glMultiTexCoord4d" )) != 0x0;
	r &= (glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC) platGetProcAddress( "glMultiTexCoord4dv" )) != 0x0;
	r &= (glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC) platGetProcAddress( "glMultiTexCoord4f" )) != 0x0;
	r &= (glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC) platGetProcAddress( "glMultiTexCoord4fv" )) != 0x0;
	r &= (glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC) platGetProcAddress( "glMultiTexCoord4i" )) != 0x0;
	r &= (glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC) platGetProcAddress( "glMultiTexCoord4iv" )) != 0x0;
	r &= (glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC) platGetProcAddress( "glMultiTexCoord4s" )) != 0x0;
	r &= (glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC) platGetProcAddress( "glMultiTexCoord4sv" )) != 0x0;
	r &= (glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC) platGetProcAddress( "glLoadTransposeMatrixf" )) != 0x0;
	r &= (glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC) platGetProcAddress( "glLoadTransposeMatrixd" )) != 0x0;
	r &= (glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC) platGetProcAddress( "glMultTransposeMatrixf" )) != 0x0;
	r &= (glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC) platGetProcAddress( "glMultTransposeMatrixd" )) != 0x0;
	r &= (glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) platGetProcAddress( "glSampleCoverage" )) != 0x0;
	r &= (glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) platGetProcAddress( "glCompressedTexImage3D" )) != 0x0;
	r &= (glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) platGetProcAddress( "glCompressedTexImage2D" )) != 0x0;
	r &= (glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) platGetProcAddress( "glCompressedTexImage1D" )) != 0x0;
	r &= (glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) platGetProcAddress( "glCompressedTexSubImage3D" )) != 0x0;
	r &= (glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) platGetProcAddress( "glCompressedTexSubImage2D" )) != 0x0;
	r &= (glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) platGetProcAddress( "glCompressedTexSubImage1D" )) != 0x0;
	r &= (glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) platGetProcAddress( "glGetCompressedTexImage" )) != 0x0;
	

	// OpenGL 1.4
	r &= (glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) platGetProcAddress( "glBlendFuncSeparate" )) != 0x0;
	r &= (glFogCoordf = (PFNGLFOGCOORDFPROC) platGetProcAddress( "glFogCoordf" )) != 0x0;
	r &= (glFogCoordfv = (PFNGLFOGCOORDFVPROC) platGetProcAddress( "glFogCoordfv" )) != 0x0;
	r &= (glFogCoordd = (PFNGLFOGCOORDDPROC) platGetProcAddress( "glFogCoordd" )) != 0x0;
	r &= (glFogCoorddv = (PFNGLFOGCOORDDVPROC) platGetProcAddress( "glFogCoorddv" )) != 0x0;
	r &= (glFogCoordPointer = (PFNGLFOGCOORDPOINTERPROC) platGetProcAddress( "glFogCoordPointer" )) != 0x0;
	r &= (glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) platGetProcAddress( "glMultiDrawArrays" )) != 0x0;
	r &= (glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) platGetProcAddress( "glMultiDrawElements" )) != 0x0;
	r &= (glPointParameterf = (PFNGLPOINTPARAMETERFPROC) platGetProcAddress( "glPointParameterf" )) != 0x0;
	r &= (glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) platGetProcAddress( "glPointParameterfv" )) != 0x0;
	r &= (glPointParameteri = (PFNGLPOINTPARAMETERIPROC) platGetProcAddress( "glPointParameteri" )) != 0x0;
	r &= (glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) platGetProcAddress( "glPointParameteriv" )) != 0x0;
	r &= (glSecondaryColor3b = (PFNGLSECONDARYCOLOR3BPROC) platGetProcAddress( "glSecondaryColor3b" )) != 0x0;
	r &= (glSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BVPROC) platGetProcAddress( "glSecondaryColor3bv" )) != 0x0;
	r &= (glSecondaryColor3d = (PFNGLSECONDARYCOLOR3DPROC) platGetProcAddress( "glSecondaryColor3d" )) != 0x0;
	r &= (glSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DVPROC) platGetProcAddress( "glSecondaryColor3dv" )) != 0x0;
	r &= (glSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC) platGetProcAddress( "glSecondaryColor3f" )) != 0x0;
	r &= (glSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FVPROC) platGetProcAddress( "glSecondaryColor3fv" )) != 0x0;
	r &= (glSecondaryColor3i = (PFNGLSECONDARYCOLOR3IPROC) platGetProcAddress( "glSecondaryColor3i" )) != 0x0;
	r &= (glSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IVPROC) platGetProcAddress( "glSecondaryColor3iv" )) != 0x0;
	r &= (glSecondaryColor3s = (PFNGLSECONDARYCOLOR3SPROC) platGetProcAddress( "glSecondaryColor3s" )) != 0x0;
	r &= (glSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SVPROC) platGetProcAddress( "glSecondaryColor3sv" )) != 0x0;
	r &= (glSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UBPROC) platGetProcAddress( "glSecondaryColor3ub" )) != 0x0;
	r &= (glSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBVPROC) platGetProcAddress( "glSecondaryColor3ubv" )) != 0x0;
	r &= (glSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UIPROC) platGetProcAddress( "glSecondaryColor3ui" )) != 0x0;
	r &= (glSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIVPROC) platGetProcAddress( "glSecondaryColor3uiv" )) != 0x0;
	r &= (glSecondaryColor3us = (PFNGLSECONDARYCOLOR3USPROC) platGetProcAddress( "glSecondaryColor3us" )) != 0x0;
	r &= (glSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USVPROC) platGetProcAddress( "glSecondaryColor3usv" )) != 0x0;
	r &= (glSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTERPROC) platGetProcAddress( "glSecondaryColorPointer" )) != 0x0;
	r &= (glWindowPos2d = (PFNGLWINDOWPOS2DPROC) platGetProcAddress( "glWindowPos2d" )) != 0x0;
	r &= (glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC) platGetProcAddress( "glWindowPos2dv" )) != 0x0;
	r &= (glWindowPos2f = (PFNGLWINDOWPOS2FPROC) platGetProcAddress( "glWindowPos2f" )) != 0x0;
	r &= (glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC) platGetProcAddress( "glWindowPos2fv" )) != 0x0;
	r &= (glWindowPos2i = (PFNGLWINDOWPOS2IPROC) platGetProcAddress( "glWindowPos2i" )) != 0x0;
	r &= (glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC) platGetProcAddress( "glWindowPos2iv" )) != 0x0;
	r &= (glWindowPos2s = (PFNGLWINDOWPOS2SPROC) platGetProcAddress( "glWindowPos2s" )) != 0x0;
	r &= (glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC) platGetProcAddress( "glWindowPos2sv" )) != 0x0;
	r &= (glWindowPos3d = (PFNGLWINDOWPOS3DPROC) platGetProcAddress( "glWindowPos3d" )) != 0x0;
	r &= (glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC) platGetProcAddress( "glWindowPos3dv" )) != 0x0;
	r &= (glWindowPos3f = (PFNGLWINDOWPOS3FPROC) platGetProcAddress( "glWindowPos3f" )) != 0x0;
	r &= (glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC) platGetProcAddress( "glWindowPos3fv" )) != 0x0;
	r &= (glWindowPos3i = (PFNGLWINDOWPOS3IPROC) platGetProcAddress( "glWindowPos3i" )) != 0x0;
	r &= (glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC) platGetProcAddress( "glWindowPos3iv" )) != 0x0;
	r &= (glWindowPos3s = (PFNGLWINDOWPOS3SPROC) platGetProcAddress( "glWindowPos3s" )) != 0x0;
	r &= (glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC) platGetProcAddress( "glWindowPos3sv" )) != 0x0;
	
	// OpenGL 1.5
	r &= (glGenQueries = (PFNGLGENQUERIESPROC) platGetProcAddress( "glGenQueries" )) != 0x0;
	r &= (glDeleteQueries = (PFNGLDELETEQUERIESPROC) platGetProcAddress( "glDeleteQueries" )) != 0x0;
	r &= (glIsQuery = (PFNGLISQUERYPROC) platGetProcAddress( "glIsQuery" )) != 0x0;
	r &= (glBeginQuery = (PFNGLBEGINQUERYPROC) platGetProcAddress( "glBeginQuery" )) != 0x0;
	r &= (glEndQuery = (PFNGLENDQUERYPROC) platGetProcAddress( "glEndQuery" )) != 0x0;
	r &= (glGetQueryiv = (PFNGLGETQUERYIVPROC) platGetProcAddress( "glGetQueryiv" )) != 0x0;
	r &= (glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) platGetProcAddress( "glGetQueryObjectiv" )) != 0x0;
	r &= (glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) platGetProcAddress( "glGetQueryObjectuiv" )) != 0x0;
	r &= (glBindBuffer = (PFNGLBINDBUFFERPROC) platGetProcAddress( "glBindBuffer" )) != 0x0;
	r &= (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) platGetProcAddress( "glDeleteBuffers" )) != 0x0;
	r &= (glGenBuffers = (PFNGLGENBUFFERSPROC) platGetProcAddress( "glGenBuffers" )) != 0x0;
	r &= (glIsBuffer = (PFNGLISBUFFERPROC) platGetProcAddress( "glIsBuffer" )) != 0x0;
	r &= (glBufferData = (PFNGLBUFFERDATAPROC) platGetProcAddress( "glBufferData" )) != 0x0;
	r &= (glBufferSubData = (PFNGLBUFFERSUBDATAPROC) platGetProcAddress( "glBufferSubData" )) != 0x0;
	r &= (glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) platGetProcAddress( "glGetBufferSubData" )) != 0x0;
	r &= (glMapBuffer = (PFNGLMAPBUFFERPROC) platGetProcAddress( "glMapBuffer" )) != 0x0;
	r &= (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) platGetProcAddress( "glUnmapBuffer" )) != 0x0;
	r &= (glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) platGetProcAddress( "glGetBufferParameteriv" )) != 0x0;
	r &= (glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) platGetProcAddress( "glGetBufferPointerv" )) != 0x0;
	
	// OpenGL 2.0
	r &= (glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) platGetProcAddress( "glBlendEquationSeparate" )) != 0x0;
	r &= (glDrawBuffers = (PFNGLDRAWBUFFERSPROC) platGetProcAddress( "glDrawBuffers" )) != 0x0;
	r &= (glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) platGetProcAddress( "glStencilOpSeparate" )) != 0x0;
	r &= (glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) platGetProcAddress( "glStencilFuncSeparate" )) != 0x0;
	r &= (glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) platGetProcAddress( "glStencilMaskSeparate" )) != 0x0;
	r &= (glAttachShader = (PFNGLATTACHSHADERPROC) platGetProcAddress( "glAttachShader" )) != 0x0;
	r &= (glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) platGetProcAddress( "glBindAttribLocation" )) != 0x0;
	r &= (glCompileShader = (PFNGLCOMPILESHADERPROC) platGetProcAddress( "glCompileShader" )) != 0x0;
	r &= (glCreateProgram = (PFNGLCREATEPROGRAMPROC) platGetProcAddress( "glCreateProgram" )) != 0x0;
	r &= (glCreateShader = (PFNGLCREATESHADERPROC) platGetProcAddress( "glCreateShader" )) != 0x0;
	r &= (glDeleteProgram = (PFNGLDELETEPROGRAMPROC) platGetProcAddress( "glDeleteProgram" )) != 0x0;
	r &= (glDeleteShader = (PFNGLDELETESHADERPROC) platGetProcAddress( "glDeleteShader" )) != 0x0;
	r &= (glDetachShader = (PFNGLDETACHSHADERPROC) platGetProcAddress( "glDetachShader" )) != 0x0;
	r &= (glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) platGetProcAddress( "glDisableVertexAttribArray" )) != 0x0;
	r &= (glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) platGetProcAddress( "glEnableVertexAttribArray" )) != 0x0;
	r &= (glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) platGetProcAddress( "glGetActiveAttrib" )) != 0x0;
	r &= (glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) platGetProcAddress( "glGetActiveUniform" )) != 0x0;
	r &= (glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) platGetProcAddress( "glGetAttachedShaders" )) != 0x0;
	r &= (glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) platGetProcAddress( "glGetAttribLocation" )) != 0x0;
	r &= (glGetProgramiv = (PFNGLGETPROGRAMIVPROC) platGetProcAddress( "glGetProgramiv" )) != 0x0;
	r &= (glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) platGetProcAddress( "glGetProgramInfoLog" )) != 0x0;
	r &= (glGetShaderiv = (PFNGLGETSHADERIVPROC) platGetProcAddress( "glGetShaderiv" )) != 0x0;
	r &= (glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) platGetProcAddress( "glGetShaderInfoLog" )) != 0x0;
	r &= (glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) platGetProcAddress( "glGetShaderSource" )) != 0x0;
	r &= (glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) platGetProcAddress( "glGetUniformLocation" )) != 0x0;
	r &= (glGetUniformfv = (PFNGLGETUNIFORMFVPROC) platGetProcAddress( "glGetUniformfv" )) != 0x0;
	r &= (glGetUniformiv = (PFNGLGETUNIFORMIVPROC) platGetProcAddress( "glGetUniformiv" )) != 0x0;
	r &= (glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) platGetProcAddress( "glGetVertexAttribdv" )) != 0x0;
	r &= (glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) platGetProcAddress( "glGetVertexAttribfv" )) != 0x0;
	r &= (glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) platGetProcAddress( "glGetVertexAttribiv" )) != 0x0;
	r &= (glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) platGetProcAddress( "glGetVertexAttribPointerv" )) != 0x0;
	r &= (glIsProgram = (PFNGLISPROGRAMPROC) platGetProcAddress( "glIsProgram" )) != 0x0;
	r &= (glIsShader = (PFNGLISSHADERPROC) platGetProcAddress( "glIsShader" )) != 0x0;
	r &= (glLinkProgram = (PFNGLLINKPROGRAMPROC) platGetProcAddress( "glLinkProgram" )) != 0x0;
	r &= (glShaderSource = (PFNGLSHADERSOURCEPROC) platGetProcAddress( "glShaderSource" )) != 0x0;
	r &= (glUseProgram = (PFNGLUSEPROGRAMPROC) platGetProcAddress( "glUseProgram" )) != 0x0;
	r &= (glUniform1f = (PFNGLUNIFORM1FPROC) platGetProcAddress( "glUniform1f" )) != 0x0;
	r &= (glUniform2f = (PFNGLUNIFORM2FPROC) platGetProcAddress( "glUniform2f" )) != 0x0;
	r &= (glUniform3f = (PFNGLUNIFORM3FPROC) platGetProcAddress( "glUniform3f" )) != 0x0;
	r &= (glUniform4f = (PFNGLUNIFORM4FPROC) platGetProcAddress( "glUniform4f" )) != 0x0;
	r &= (glUniform1i = (PFNGLUNIFORM1IPROC) platGetProcAddress( "glUniform1i" )) != 0x0;
	r &= (glUniform2i = (PFNGLUNIFORM2IPROC) platGetProcAddress( "glUniform2i" )) != 0x0;
	r &= (glUniform3i = (PFNGLUNIFORM3IPROC) platGetProcAddress( "glUniform3i" )) != 0x0;
	r &= (glUniform4i = (PFNGLUNIFORM4IPROC) platGetProcAddress( "glUniform4i" )) != 0x0;
	r &= (glUniform1fv = (PFNGLUNIFORM1FVPROC) platGetProcAddress( "glUniform1fv" )) != 0x0;
	r &= (glUniform2fv = (PFNGLUNIFORM2FVPROC) platGetProcAddress( "glUniform2fv" )) != 0x0;
	r &= (glUniform3fv = (PFNGLUNIFORM3FVPROC) platGetProcAddress( "glUniform3fv" )) != 0x0;
	r &= (glUniform4fv = (PFNGLUNIFORM4FVPROC) platGetProcAddress( "glUniform4fv" )) != 0x0;
	r &= (glUniform1iv = (PFNGLUNIFORM1IVPROC) platGetProcAddress( "glUniform1iv" )) != 0x0;
	r &= (glUniform2iv = (PFNGLUNIFORM2IVPROC) platGetProcAddress( "glUniform2iv" )) != 0x0;
	r &= (glUniform3iv = (PFNGLUNIFORM3IVPROC) platGetProcAddress( "glUniform3iv" )) != 0x0;
	r &= (glUniform4iv = (PFNGLUNIFORM4IVPROC) platGetProcAddress( "glUniform4iv" )) != 0x0;
	r &= (glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) platGetProcAddress( "glUniformMatrix2fv" )) != 0x0;
	r &= (glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) platGetProcAddress( "glUniformMatrix3fv" )) != 0x0;
	r &= (glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) platGetProcAddress( "glUniformMatrix4fv" )) != 0x0;
	r &= (glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) platGetProcAddress( "glValidateProgram" )) != 0x0;
	r &= (glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) platGetProcAddress( "glVertexAttrib1d" )) != 0x0;
	r &= (glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) platGetProcAddress( "glVertexAttrib1dv" )) != 0x0;
	r &= (glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) platGetProcAddress( "glVertexAttrib1f" )) != 0x0;
	r &= (glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) platGetProcAddress( "glVertexAttrib1fv" )) != 0x0;
	r &= (glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) platGetProcAddress( "glVertexAttrib1s" )) != 0x0;
	r &= (glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) platGetProcAddress( "glVertexAttrib1sv" )) != 0x0;
	r &= (glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) platGetProcAddress( "glVertexAttrib2d" )) != 0x0;
	r &= (glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) platGetProcAddress( "glVertexAttrib2dv" )) != 0x0;
	r &= (glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) platGetProcAddress( "glVertexAttrib2f" )) != 0x0;
	r &= (glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) platGetProcAddress( "glVertexAttrib2fv" )) != 0x0;
	r &= (glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) platGetProcAddress( "glVertexAttrib2s" )) != 0x0;
	r &= (glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) platGetProcAddress( "glVertexAttrib2sv" )) != 0x0;
	r &= (glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) platGetProcAddress( "glVertexAttrib3d" )) != 0x0;
	r &= (glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) platGetProcAddress( "glVertexAttrib3dv" )) != 0x0;
	r &= (glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) platGetProcAddress( "glVertexAttrib3f" )) != 0x0;
	r &= (glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) platGetProcAddress( "glVertexAttrib3fv" )) != 0x0;
	r &= (glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) platGetProcAddress( "glVertexAttrib3s" )) != 0x0;
	r &= (glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) platGetProcAddress( "glVertexAttrib3sv" )) != 0x0;
	r &= (glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) platGetProcAddress( "glVertexAttrib4Nbv" )) != 0x0;
	r &= (glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) platGetProcAddress( "glVertexAttrib4Niv" )) != 0x0;
	r &= (glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) platGetProcAddress( "glVertexAttrib4Nsv" )) != 0x0;
	r &= (glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) platGetProcAddress( "glVertexAttrib4Nub" )) != 0x0;
	r &= (glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) platGetProcAddress( "glVertexAttrib4Nubv" )) != 0x0;
	r &= (glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) platGetProcAddress( "glVertexAttrib4Nuiv" )) != 0x0;
	r &= (glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) platGetProcAddress( "glVertexAttrib4Nusv" )) != 0x0;
	r &= (glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) platGetProcAddress( "glVertexAttrib4bv" )) != 0x0;
	r &= (glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) platGetProcAddress( "glVertexAttrib4d" )) != 0x0;
	r &= (glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) platGetProcAddress( "glVertexAttrib4dv" )) != 0x0;
	r &= (glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) platGetProcAddress( "glVertexAttrib4f" )) != 0x0;
	r &= (glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) platGetProcAddress( "glVertexAttrib4fv" )) != 0x0;
	r &= (glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) platGetProcAddress( "glVertexAttrib4iv" )) != 0x0;
	r &= (glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) platGetProcAddress( "glVertexAttrib4s" )) != 0x0;
	r &= (glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) platGetProcAddress( "glVertexAttrib4sv" )) != 0x0;
	r &= (glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) platGetProcAddress( "glVertexAttrib4ubv" )) != 0x0;
	r &= (glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) platGetProcAddress( "glVertexAttrib4uiv" )) != 0x0;
	r &= (glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) platGetProcAddress( "glVertexAttrib4usv" )) != 0x0;
	r &= (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) platGetProcAddress( "glVertexAttribPointer" )) != 0x0;

	/*// OpenGL 2.1
	r &= (glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) platGetProcAddress( "glUniformMatrix2x3fv" )) != 0x0;
	r &= (glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) platGetProcAddress( "glUniformMatrix3x2fv" )) != 0x0;
	r &= (glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) platGetProcAddress( "glUniformMatrix2x4fv" )) != 0x0;
	r &= (glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) platGetProcAddress( "glUniformMatrix4x2fv" )) != 0x0;
	r &= (glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) platGetProcAddress( "glUniformMatrix3x4fv" )) != 0x0;
	r &= (glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) platGetProcAddress( "glUniformMatrix4x3fv" )) != 0x0;*/
	
	// Extensions
	glExt::EXT_framebuffer_object = isExtensionSupported( "GL_EXT_framebuffer_object" );
	if( glExt::EXT_framebuffer_object )
	{
		r &= (glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC) platGetProcAddress( "glIsRenderbufferEXT" )) != 0x0;
		r &= (glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) platGetProcAddress( "glBindRenderbufferEXT" )) != 0x0;
		r &= (glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC) platGetProcAddress( "glDeleteRenderbuffersEXT" )) != 0x0;
		r &= (glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) platGetProcAddress( "glGenRenderbuffersEXT" )) != 0x0;
		r &= (glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) platGetProcAddress( "glRenderbufferStorageEXT" )) != 0x0;
		r &= (glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) platGetProcAddress( "glGetRenderbufferParameterivEXT" )) != 0x0;
		r &= (glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC) platGetProcAddress( "glIsFramebufferEXT" )) != 0x0;
		r &= (glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) platGetProcAddress( "glBindFramebufferEXT" )) != 0x0;
		r &= (glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC) platGetProcAddress( "glDeleteFramebuffersEXT" )) != 0x0;
		r &= (glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) platGetProcAddress( "glGenFramebuffersEXT" )) != 0x0;
		r &= (glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) platGetProcAddress( "glCheckFramebufferStatusEXT" )) != 0x0;
		r &= (glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) platGetProcAddress( "glFramebufferTexture1DEXT" )) != 0x0;
		r &= (glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) platGetProcAddress( "glFramebufferTexture2DEXT" )) != 0x0;
		r &= (glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) platGetProcAddress( "glFramebufferTexture3DEXT" )) != 0x0;
		r &= (glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) platGetProcAddress( "glFramebufferRenderbufferEXT" )) != 0x0;
		r &= (glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) platGetProcAddress( "glGetFramebufferAttachmentParameterivEXT" )) != 0x0;
		r &= (glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC) platGetProcAddress( "glGenerateMipmapEXT" )) != 0x0;
	}
	
	glExt::EXT_texture_filter_anisotropic = isExtensionSupported( "GL_EXT_texture_filter_anisotropic" );

	glExt::EXT_texture_compression_s3tc = isExtensionSupported( "GL_EXT_texture_compression_s3tc" );

	glExt::ARB_texture_float = isExtensionSupported( "GL_ARB_texture_float" ) ||
							   isExtensionSupported( "GL_ATI_texture_float" );

	glExt::ARB_texture_non_power_of_two = isExtensionSupported( "GL_ARB_texture_non_power_of_two" );

	glExt::EXT_framebuffer_multisample = isExtensionSupported( "GL_EXT_framebuffer_multisample" ) &&
										 isExtensionSupported( "GL_EXT_framebuffer_blit" );
	if( glExt::EXT_framebuffer_multisample )
	{
		// From GL_EXT_framebuffer_blit
		r &= (glBlitFramebufferEXT = (PFNGLBLITFRAMEBUFFEREXTPROC) platGetProcAddress( "glBlitFramebufferEXT" )) != 0x0;
		// From GL_EXT_framebuffer_multisample
		r &= (glRenderbufferStorageMultisampleEXT = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) platGetProcAddress( "glRenderbufferStorageMultisampleEXT" )) != 0x0;
	}

	return r;
}
