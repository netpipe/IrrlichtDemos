#include "Extensions.h"

PFNGLACTIVETEXTUREPROC				glActiveTexture			= NULL;
PFNGLCLIENTACTIVETEXTUREPROC		glClientActiveTexture	= NULL;
PFNGLMULTITEXCOORD1DPROC			glMultiTexCoord1d		= NULL;
PFNGLMULTITEXCOORD1DVPROC			glMultiTexCoord1dv		= NULL;
PFNGLMULTITEXCOORD1FPROC			glMultiTexCoord1f		= NULL;
PFNGLMULTITEXCOORD1FVPROC			glMultiTexCoord1fv		= NULL;
PFNGLMULTITEXCOORD1IPROC			glMultiTexCoord1i		= NULL;
PFNGLMULTITEXCOORD1IVPROC			glMultiTexCoord1iv		= NULL;
PFNGLMULTITEXCOORD1SPROC			glMultiTexCoord1s		= NULL;
PFNGLMULTITEXCOORD1SVPROC			glMultiTexCoord1sv		= NULL;
PFNGLMULTITEXCOORD2DPROC			glMultiTexCoord2d		= NULL;
PFNGLMULTITEXCOORD2DVPROC			glMultiTexCoord2dv		= NULL;
PFNGLMULTITEXCOORD2FPROC			glMultiTexCoord2f		= NULL;
PFNGLMULTITEXCOORD2FVPROC			glMultiTexCoord2fv		= NULL;
PFNGLMULTITEXCOORD2IPROC			glMultiTexCoord2i		= NULL;
PFNGLMULTITEXCOORD2IVPROC			glMultiTexCoord2iv		= NULL;
PFNGLMULTITEXCOORD2SPROC			glMultiTexCoord2s		= NULL;
PFNGLMULTITEXCOORD2SVPROC			glMultiTexCoord2sv		= NULL;
PFNGLMULTITEXCOORD3DPROC			glMultiTexCoord3d		= NULL;
PFNGLMULTITEXCOORD3DVPROC			glMultiTexCoord3dv		= NULL;
PFNGLMULTITEXCOORD3FPROC			glMultiTexCoord3f		= NULL;
PFNGLMULTITEXCOORD3FVPROC			glMultiTexCoord3fv		= NULL;
PFNGLMULTITEXCOORD3IPROC			glMultiTexCoord3i		= NULL;
PFNGLMULTITEXCOORD3IVPROC			glMultiTexCoord3iv		= NULL;
PFNGLMULTITEXCOORD3SPROC			glMultiTexCoord3s		= NULL;
PFNGLMULTITEXCOORD3SVPROC			glMultiTexCoord3sv		= NULL;
PFNGLMULTITEXCOORD4DPROC			glMultiTexCoord4d		= NULL;
PFNGLMULTITEXCOORD4DVPROC			glMultiTexCoord4dv		= NULL;
PFNGLMULTITEXCOORD4FPROC			glMultiTexCoord4f		= NULL;
PFNGLMULTITEXCOORD4FVPROC			glMultiTexCoord4fv		= NULL;
PFNGLMULTITEXCOORD4IPROC			glMultiTexCoord4i		= NULL;
PFNGLMULTITEXCOORD4IVPROC			glMultiTexCoord4iv		= NULL;
PFNGLMULTITEXCOORD4SPROC			glMultiTexCoord4s		= NULL;
PFNGLMULTITEXCOORD4SVPROC			glMultiTexCoord4sv		= NULL;

void SetUp_GL_ARB_multitexture()
{
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)
				wglGetProcAddress("glActiveTextureARB");
	glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)
				wglGetProcAddress("glClientActiveTextureARB");

	glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC)
				wglGetProcAddress("glMultiTexCoord1dARB");
	glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)
				wglGetProcAddress("glMultiTexCoord1dvARB");
	glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC)
				wglGetProcAddress("glMultiTexCoord1fARB");
	glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)
				wglGetProcAddress("glMultiTexCoord1fvARB");
	glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC)
				wglGetProcAddress("glMultiTexCoord1iARB");
	glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)
				wglGetProcAddress("glMultiTexCoord1ivARB");
	glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC)
				wglGetProcAddress("glMultiTexCoord1sARB");
	glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)
				wglGetProcAddress("glMultiTexCoord1svARB");

	glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)
				wglGetProcAddress("glMultiTexCoord2dARB");
	glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)
				wglGetProcAddress("glMultiTexCoord2dvARB");
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)
				wglGetProcAddress("glMultiTexCoord2fARB");
	glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)
				wglGetProcAddress("glMultiTexCoord2fvARB");
	glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC)
				wglGetProcAddress("glMultiTexCoord2iARB");
	glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)
				wglGetProcAddress("glMultiTexCoord2ivARB");
	glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC)
				wglGetProcAddress("glMultiTexCoord2sARB");
	glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)
				wglGetProcAddress("glMultiTexCoord2svARB");

	glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC)
				wglGetProcAddress("glMultiTexCoord3dARB");
	glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)
				wglGetProcAddress("glMultiTexCoord3dvARB");
	glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC)
				wglGetProcAddress("glMultiTexCoord3fARB");
	glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)
				wglGetProcAddress("glMultiTexCoord3fvARB");
	glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC)
				wglGetProcAddress("glMultiTexCoord3iARB");
	glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)
				wglGetProcAddress("glMultiTexCoord3ivARB");
	glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC)
				wglGetProcAddress("glMultiTexCoord3sARB");
	glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)
				wglGetProcAddress("glMultiTexCoord3svARB");

	glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC)
				wglGetProcAddress("glMultiTexCoord4dARB");
	glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)
				wglGetProcAddress("glMultiTexCoord4dvARB");
	glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC)
				wglGetProcAddress("glMultiTexCoord4fARB");
	glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)
				wglGetProcAddress("glMultiTexCoord4fvARB");
	glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC)
				wglGetProcAddress("glMultiTexCoord4iARB");
	glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)
				wglGetProcAddress("glMultiTexCoord4ivARB");
	glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC)
				wglGetProcAddress("glMultiTexCoord4sARB");
	glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)
				wglGetProcAddress("glMultiTexCoord4svARB");
}