// Copyright NVIDIA Corporation 2002-2006
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** @file */

#include "nvsgcommon.h"

#include  <set>
#include  <string>

namespace nvgl
{
  
  //! The \c GLFunctions class holds the OpenGL extension function pointers.
  /** Just like a \c GLContext, you will never have to use this class explicitly. A \c GLContext holds a \c GLFunctions member
    * to hold the extension functions of that OpenGL context. For every OpenGL context there might exist a different set of
    * extension functions. A \c GLFunctions holds all function pointers of one context. If you're using the uppercase \c WGL
    * prefix-functions \c WGLCreateContext(), \c WGLMakeCurrent(), and \c WGLDeleteContext(), one \c GLFunctions object is
    * maintained per OpenGL context and switched accordingly. */
  class GLFunctions
  {
    public:
      //! Constructor
#if defined(_WIN32)      
      GLFunctions( void );
#elif defined(LINUX)
      GLFunctions( Display* dpy );
#endif

      //! Destructor
      ~GLFunctions( void );

      //! Test if a given extension is available on the current context.
      /** An OpenGL extension is not only exported, but also available, if all the functions required for that
        * extension are exported.
        * \note Not all exported extensions might be available.
        * \returns  \c true, if the \a extension is available, otherwise \c false  */
      bool  isAvailable( const std::string& extension   //!< extension to test
                       ) const;

      //! Test if a given extension is exported on the current context.
      /** An OpenGL extension is exported, if it is part of the return value of \c glGetString(GL_EXTENSIONS),
        * \c wglGetExtensionsStringEXT() (Windows), or \c glXQueryExtensionsString() (Linux).
        * \note Not all exported extensions might be available.
        * \returns  \c true, if \a extension is exported, otherwise c\ false  */
      bool  isExported( const std::string& extension    //!< extension to test
                      ) const;

      //! Returns the version of the OpenGL available on the target system.
      /** \returns The floating point version of the OpenGL with the representation 
        * major.minor is returned as an integer calculated as 10*major+minor.\n\n
        * The OpenGL version 1.4, for example, will be reported as 14. */
      int getVersion() const;

    public:
//  Don't need to document all these wrapper functions !!!
#if ! defined( DOXYGEN_IGNORE )
      //  version 1.2 functions
      PFNGLBLENDCOLORPROC                         glBlendColor;
      PFNGLBLENDEQUATIONPROC                      glBlendEquation;
      PFNGLCOLORTABLEPROC                         glColorTable;
      PFNGLCOLORTABLEPARAMETERFVPROC              glColorTableParameterfv;
      PFNGLCOLORTABLEPARAMETERIVPROC              glColorTableParameteriv;
      PFNGLCOPYCOLORTABLEPROC                     glCopyColorTable;
      PFNGLCOPYTEXSUBIMAGE3DPROC                  glCopyTexSubImage3D;
      PFNGLDRAWRANGEELEMENTSPROC                  glDrawRangeElements;
      PFNGLGETCOLORTABLEPROC                      glGetColorTable;
      PFNGLGETCOLORTABLEPARAMETERFVPROC           glGetColorTableParameterfv;
      PFNGLGETCOLORTABLEPARAMETERIVPROC           glGetColorTableParameteriv;
      PFNGLTEXIMAGE3DPROC                         glTexImage3D;
      PFNGLTEXSUBIMAGE3DPROC                      glTexSubImage3D;

      //  version 1.3 functions
      PFNGLACTIVETEXTUREPROC                      glActiveTexture;
      PFNGLCLIENTACTIVETEXTUREPROC                glClientActiveTexture;
      PFNGLCOMPRESSEDTEXIMAGE3DPROC               glCompressedTexImage3D;
      PFNGLCOMPRESSEDTEXIMAGE2DPROC               glCompressedTexImage2D;
      PFNGLCOMPRESSEDTEXIMAGE1DPROC               glCompressedTexImage1D;
      PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC            glCompressedTexSubImage3D;
      PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC            glCompressedTexSubImage2D;
      PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC            glCompressedTexSubImage1D;
      PFNGLGETCOMPRESSEDTEXIMAGEPROC              glGetCompressedTexImage;
      PFNGLLOADTRANSPOSEMATRIXFPROC               glLoadTransposeMatrixf;
      PFNGLLOADTRANSPOSEMATRIXDPROC               glLoadTransposeMatrixd;
      PFNGLMULTITEXCOORD1DPROC                    glMultiTexCoord1d;
      PFNGLMULTITEXCOORD1DVPROC                   glMultiTexCoord1dv;
      PFNGLMULTITEXCOORD1FPROC                    glMultiTexCoord1f;
      PFNGLMULTITEXCOORD1FVPROC                   glMultiTexCoord1fv;
      PFNGLMULTITEXCOORD1IPROC                    glMultiTexCoord1i;
      PFNGLMULTITEXCOORD1IVPROC                   glMultiTexCoord1iv;
      PFNGLMULTITEXCOORD1SPROC                    glMultiTexCoord1s;
      PFNGLMULTITEXCOORD1SVPROC                   glMultiTexCoord1sv;
      PFNGLMULTITEXCOORD2DPROC                    glMultiTexCoord2d;
      PFNGLMULTITEXCOORD2DVPROC                   glMultiTexCoord2dv;
      PFNGLMULTITEXCOORD2FPROC                    glMultiTexCoord2f;
      PFNGLMULTITEXCOORD2FVPROC                   glMultiTexCoord2fv;
      PFNGLMULTITEXCOORD2IPROC                    glMultiTexCoord2i;
      PFNGLMULTITEXCOORD2IVPROC                   glMultiTexCoord2iv;
      PFNGLMULTITEXCOORD2SPROC                    glMultiTexCoord2s;
      PFNGLMULTITEXCOORD2SVPROC                   glMultiTexCoord2sv;
      PFNGLMULTITEXCOORD3DPROC                    glMultiTexCoord3d;
      PFNGLMULTITEXCOORD3DVPROC                   glMultiTexCoord3dv;
      PFNGLMULTITEXCOORD3FPROC                    glMultiTexCoord3f;
      PFNGLMULTITEXCOORD3FVPROC                   glMultiTexCoord3fv;
      PFNGLMULTITEXCOORD3IPROC                    glMultiTexCoord3i;
      PFNGLMULTITEXCOORD3IVPROC                   glMultiTexCoord3iv;
      PFNGLMULTITEXCOORD3SPROC                    glMultiTexCoord3s;
      PFNGLMULTITEXCOORD3SVPROC                   glMultiTexCoord3sv;
      PFNGLMULTITEXCOORD4DPROC                    glMultiTexCoord4d;
      PFNGLMULTITEXCOORD4DVPROC                   glMultiTexCoord4dv;
      PFNGLMULTITEXCOORD4FPROC                    glMultiTexCoord4f;
      PFNGLMULTITEXCOORD4FVPROC                   glMultiTexCoord4fv;
      PFNGLMULTITEXCOORD4IPROC                    glMultiTexCoord4i;
      PFNGLMULTITEXCOORD4IVPROC                   glMultiTexCoord4iv;
      PFNGLMULTITEXCOORD4SPROC                    glMultiTexCoord4s;
      PFNGLMULTITEXCOORD4SVPROC                   glMultiTexCoord4sv;
      PFNGLMULTTRANSPOSEMATRIXFPROC               glMultTransposeMatrixf;
      PFNGLMULTTRANSPOSEMATRIXDPROC               glMultTransposeMatrixd;
      PFNGLSAMPLECOVERAGEPROC                     glSampleCoverage;

      //  version 1.4 functions
      PFNGLBLENDFUNCSEPARATEPROC                  glBlendFuncSeparate;
      PFNGLFOGCOORDDPROC                          glFogCoordd;
      PFNGLFOGCOORDDVPROC                         glFogCoorddv;
      PFNGLFOGCOORDFPROC                          glFogCoordf;
      PFNGLFOGCOORDFVPROC                         glFogCoordfv;
      PFNGLFOGCOORDPOINTERPROC                    glFogCoordPointer;
      PFNGLMULTIDRAWARRAYSPROC                    glMultiDrawArrays;
      PFNGLMULTIDRAWELEMENTSPROC                  glMultiDrawElements;
      PFNGLPOINTPARAMETERFPROC                    glPointParameterf;
      PFNGLPOINTPARAMETERFVPROC                   glPointParameterfv;
      PFNGLPOINTPARAMETERIPROC                    glPointParameteri;
      PFNGLPOINTPARAMETERIVPROC                   glPointParameteriv;
      PFNGLSECONDARYCOLOR3BPROC                   glSecondaryColor3b;
      PFNGLSECONDARYCOLOR3BVPROC                  glSecondaryColor3bv;
      PFNGLSECONDARYCOLOR3DPROC                   glSecondaryColor3d;
      PFNGLSECONDARYCOLOR3DVPROC                  glSecondaryColor3dv;
      PFNGLSECONDARYCOLOR3FPROC                   glSecondaryColor3f;
      PFNGLSECONDARYCOLOR3FVPROC                  glSecondaryColor3fv;
      PFNGLSECONDARYCOLOR3IPROC                   glSecondaryColor3i;
      PFNGLSECONDARYCOLOR3IVPROC                  glSecondaryColor3iv;
      PFNGLSECONDARYCOLOR3SPROC                   glSecondaryColor3s;
      PFNGLSECONDARYCOLOR3SVPROC                  glSecondaryColor3sv;
      PFNGLSECONDARYCOLOR3UBPROC                  glSecondaryColor3ub;
      PFNGLSECONDARYCOLOR3UBVPROC                 glSecondaryColor3ubv;
      PFNGLSECONDARYCOLOR3UIPROC                  glSecondaryColor3ui;
      PFNGLSECONDARYCOLOR3UIVPROC                 glSecondaryColor3uiv;
      PFNGLSECONDARYCOLOR3USPROC                  glSecondaryColor3us;
      PFNGLSECONDARYCOLOR3USVPROC                 glSecondaryColor3usv;
      PFNGLSECONDARYCOLORPOINTERPROC              glSecondaryColorPointer;
      PFNGLWINDOWPOS2DPROC                        glWindowPos2d;
      PFNGLWINDOWPOS2FPROC                        glWindowPos2f;
      PFNGLWINDOWPOS2IPROC                        glWindowPos2i;
      PFNGLWINDOWPOS2SPROC                        glWindowPos2s;
      PFNGLWINDOWPOS2DVPROC                       glWindowPos2dv;
      PFNGLWINDOWPOS2FVPROC                       glWindowPos2fv;
      PFNGLWINDOWPOS2IVPROC                       glWindowPos2iv;
      PFNGLWINDOWPOS2SVPROC                       glWindowPos2sv;
      PFNGLWINDOWPOS3DPROC                        glWindowPos3d;
      PFNGLWINDOWPOS3FPROC                        glWindowPos3f;
      PFNGLWINDOWPOS3IPROC                        glWindowPos3i;
      PFNGLWINDOWPOS3SPROC                        glWindowPos3s;
      PFNGLWINDOWPOS3DVPROC                       glWindowPos3dv;
      PFNGLWINDOWPOS3FVPROC                       glWindowPos3fv;
      PFNGLWINDOWPOS3IVPROC                       glWindowPos3iv;
      PFNGLWINDOWPOS3SVPROC                       glWindowPos3sv;

      //  GL_ARB_fragment_program functions see GL_ARB_vertex_program

      //  GL_ARB_imaging functions
      PFNGLCOLORSUBTABLEPROC                      glColorSubTable;
      PFNGLCOPYCOLORSUBTABLEPROC                  glCopyColorSubTable;
      PFNGLCONVOLUTIONFILTER1DPROC                glConvolutionFilter1D;
      PFNGLCONVOLUTIONFILTER2DPROC                glConvolutionFilter2D;
      PFNGLCONVOLUTIONPARAMETERFPROC              glConvolutionParameterf;
      PFNGLCONVOLUTIONPARAMETERFVPROC             glConvolutionParameterfv;
      PFNGLCONVOLUTIONPARAMETERIPROC              glConvolutionParameteri;
      PFNGLCONVOLUTIONPARAMETERIVPROC             glConvolutionParameteriv;
      PFNGLCOPYCONVOLUTIONFILTER1DPROC            glCopyConvolutionFilter1D;
      PFNGLCOPYCONVOLUTIONFILTER2DPROC            glCopyConvolutionFilter2D;
      PFNGLGETCONVOLUTIONFILTERPROC               glGetConvolutionFilter;
      PFNGLGETCONVOLUTIONPARAMETERFVPROC          glGetConvolutionParameterfv;
      PFNGLGETCONVOLUTIONPARAMETERIVPROC          glGetConvolutionParameteriv;
      PFNGLGETSEPARABLEFILTERPROC                 glGetSeparableFilter;
      PFNGLSEPARABLEFILTER2DPROC                  glSeparableFilter2D;
      PFNGLGETHISTOGRAMPROC                       glGetHistogram;
      PFNGLGETHISTOGRAMPARAMETERFVPROC            glGetHistogramParameterfv;
      PFNGLGETHISTOGRAMPARAMETERIVPROC            glGetHistogramParameteriv;
      PFNGLGETMINMAXPROC                          glGetMinmax;
      PFNGLGETMINMAXPARAMETERFVPROC               glGetMinmaxParameterfv;
      PFNGLGETMINMAXPARAMETERIVPROC               glGetMinmaxParameteriv;
      PFNGLHISTOGRAMPROC                          glHistogram;
      PFNGLMINMAXPROC                             glMinmax;
      PFNGLRESETHISTOGRAMPROC                     glResetHistogram;
      PFNGLRESETMINMAXPROC                        glResetMinmax;

      //  GL_ARB_occlusion_query
      PFNGLGENQUERIESARBPROC                      glGenQueriesARB;
      PFNGLDELETEQUERIESARBPROC                   glDeleteQueriesARB;
      PFNGLISQUERYARBPROC                         glIsQueryARB;
      PFNGLBEGINQUERYARBPROC                      glBeginQueryARB;
      PFNGLENDQUERYARBPROC                        glEndQueryARB;
      PFNGLGETQUERYOBJECTIVARBPROC                glGetQueryObjectivARB;
      PFNGLGETQUERYOBJECTUIVARBPROC               glGetQueryObjectuivARB;
      PFNGLGETQUERYIVARBPROC                      glGetQueryivARB;

      //  GL_ARB_point_parameters
      PFNGLPOINTPARAMETERFARBPROC                 glPointParameterfARB;
      PFNGLPOINTPARAMETERFVARBPROC                glPointParameterfvARB;

      //  GL_ARB_vertex_buffer_object functions
      PFNGLBINDBUFFERARBPROC                      glBindBufferARB;
      PFNGLBUFFERDATAARBPROC                      glBufferDataARB;
      PFNGLBUFFERSUBDATAARBPROC                   glBufferSubDataARB;
      PFNGLDELETEBUFFERSARBPROC                   glDeleteBuffersARB;
      PFNGLGENBUFFERSARBPROC                      glGenBuffersARB;
      PFNGLGETBUFFERPARAMETERIVARBPROC            glGetBufferParameterivARB;
      PFNGLGETBUFFERPOINTERVARBPROC               glGetBufferPointervARB;
      PFNGLGETBUFFERSUBDATAARBPROC                glGetBufferSubDataARB;
      PFNGLISBUFFERARBPROC                        glIsBufferARB;
      PFNGLMAPBUFFERARBPROC                       glMapBufferARB;
      PFNGLUNMAPBUFFERARBPROC                     glUnmapBufferARB;

      //  GL_ARB_vertex_program functions
      PFNGLBINDPROGRAMARBPROC                     glBindProgramARB;
      PFNGLENABLEVERTEXATTRIBARRAYARBPROC         glEnableVertexAttribArrayARB;
      PFNGLDELETEPROGRAMSARBPROC                  glDeleteProgramsARB;
      PFNGLDISABLEVERTEXATTRIBARRAYARBPROC        glDisableVertexAttribArrayARB;
      PFNGLGENPROGRAMSARBPROC                     glGenProgramsARB;
      PFNGLGETPROGRAMENVPARAMETERDVARBPROC        glGetProgramEnvParameterdvARB;
      PFNGLGETPROGRAMENVPARAMETERFVARBPROC        glGetProgramEnvParameterfvARB;
      PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC      glGetProgramLocalParameterdvARB;
      PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC      glGetProgramLocalParameterfvARB;
      PFNGLGETPROGRAMSTRINGARBPROC                glGetProgramStringARB;
      PFNGLGETPROGRAMIVARBPROC                    glGetProgramivARB;
      PFNGLGETVERTEXATTRIBDVARBPROC               glGetVertexAttribdvARB;
      PFNGLGETVERTEXATTRIBFVARBPROC               glGetVertexAttribfvARB;
      PFNGLGETVERTEXATTRIBIVARBPROC               glGetVertexAttribivARB;
      PFNGLGETVERTEXATTRIBPOINTERVARBPROC         glGetVertexAttribPointervARB;
      PFNGLISPROGRAMARBPROC                       glIsProgramARB;
      PFNGLPROGRAMENVPARAMETER4DARBPROC           glProgramEnvParameter4dARB;
      PFNGLPROGRAMENVPARAMETER4DVARBPROC          glProgramEnvParameter4dvARB;
      PFNGLPROGRAMENVPARAMETER4FARBPROC           glProgramEnvParameter4fARB;
      PFNGLPROGRAMENVPARAMETER4FVARBPROC          glProgramEnvParameter4fvARB;
      PFNGLPROGRAMLOCALPARAMETER4DARBPROC         glProgramLocalParameter4dARB;
      PFNGLPROGRAMLOCALPARAMETER4DVARBPROC        glProgramLocalParameter4dvARB;
      PFNGLPROGRAMLOCALPARAMETER4FARBPROC         glProgramLocalParameter4fARB;
      PFNGLPROGRAMLOCALPARAMETER4FVARBPROC        glProgramLocalParameter4fvARB;
      PFNGLPROGRAMSTRINGARBPROC                   glProgramStringARB;
      PFNGLVERTEXATTRIB1DARBPROC                  glVertexAttrib1dARB;
      PFNGLVERTEXATTRIB1DVARBPROC                 glVertexAttrib1dvARB;
      PFNGLVERTEXATTRIB1FARBPROC                  glVertexAttrib1fARB;
      PFNGLVERTEXATTRIB1FVARBPROC                 glVertexAttrib1fvARB;
      PFNGLVERTEXATTRIB1SARBPROC                  glVertexAttrib1sARB;
      PFNGLVERTEXATTRIB1SVARBPROC                 glVertexAttrib1svARB;
      PFNGLVERTEXATTRIB2DARBPROC                  glVertexAttrib2dARB;
      PFNGLVERTEXATTRIB2DVARBPROC                 glVertexAttrib2dvARB;
      PFNGLVERTEXATTRIB2FARBPROC                  glVertexAttrib2fARB;
      PFNGLVERTEXATTRIB2FVARBPROC                 glVertexAttrib2fvARB;
      PFNGLVERTEXATTRIB2SARBPROC                  glVertexAttrib2sARB;
      PFNGLVERTEXATTRIB2SVARBPROC                 glVertexAttrib2svARB;
      PFNGLVERTEXATTRIB3DARBPROC                  glVertexAttrib3dARB;
      PFNGLVERTEXATTRIB3DVARBPROC                 glVertexAttrib3dvARB;
      PFNGLVERTEXATTRIB3FARBPROC                  glVertexAttrib3fARB;
      PFNGLVERTEXATTRIB3FVARBPROC                 glVertexAttrib3fvARB;
      PFNGLVERTEXATTRIB3SARBPROC                  glVertexAttrib3sARB;
      PFNGLVERTEXATTRIB3SVARBPROC                 glVertexAttrib3svARB;
      PFNGLVERTEXATTRIB4NBVARBPROC                glVertexAttrib4NbvARB;
      PFNGLVERTEXATTRIB4NIVARBPROC                glVertexAttrib4NivARB;
      PFNGLVERTEXATTRIB4NSVARBPROC                glVertexAttrib4NsvARB;
      PFNGLVERTEXATTRIB4NUBARBPROC                glVertexAttrib4NubARB;
      PFNGLVERTEXATTRIB4NUBVARBPROC               glVertexAttrib4NubvARB;
      PFNGLVERTEXATTRIB4NUIVARBPROC               glVertexAttrib4NuivARB;
      PFNGLVERTEXATTRIB4NUSVARBPROC               glVertexAttrib4NusvARB;
      PFNGLVERTEXATTRIB4BVARBPROC                 glVertexAttrib4bvARB;
      PFNGLVERTEXATTRIB4DARBPROC                  glVertexAttrib4dARB;
      PFNGLVERTEXATTRIB4DVARBPROC                 glVertexAttrib4dvARB;
      PFNGLVERTEXATTRIB4FARBPROC                  glVertexAttrib4fARB;
      PFNGLVERTEXATTRIB4FVARBPROC                 glVertexAttrib4fvARB;
      PFNGLVERTEXATTRIB4IVARBPROC                 glVertexAttrib4ivARB;
      PFNGLVERTEXATTRIB4SARBPROC                  glVertexAttrib4sARB;
      PFNGLVERTEXATTRIB4SVARBPROC                 glVertexAttrib4svARB;
      PFNGLVERTEXATTRIB4UBVARBPROC                glVertexAttrib4ubvARB;
      PFNGLVERTEXATTRIB4UIVARBPROC                glVertexAttrib4uivARB;
      PFNGLVERTEXATTRIB4USVARBPROC                glVertexAttrib4usvARB;
      PFNGLVERTEXATTRIBPOINTERARBPROC             glVertexAttribPointerARB;

      //  GL_ARB_window_pos functions
      PFNGLWINDOWPOS2DARBPROC                     glWindowPos2dARB;
      PFNGLWINDOWPOS2FARBPROC                     glWindowPos2fARB;
      PFNGLWINDOWPOS2IARBPROC                     glWindowPos2iARB;
      PFNGLWINDOWPOS2SARBPROC                     glWindowPos2sARB;
      PFNGLWINDOWPOS2DVARBPROC                    glWindowPos2dvARB;
      PFNGLWINDOWPOS2FVARBPROC                    glWindowPos2fvARB;
      PFNGLWINDOWPOS2IVARBPROC                    glWindowPos2ivARB;
      PFNGLWINDOWPOS2SVARBPROC                    glWindowPos2svARB;
      PFNGLWINDOWPOS3DARBPROC                     glWindowPos3dARB;
      PFNGLWINDOWPOS3FARBPROC                     glWindowPos3fARB;
      PFNGLWINDOWPOS3IARBPROC                     glWindowPos3iARB;
      PFNGLWINDOWPOS3SARBPROC                     glWindowPos3sARB;
      PFNGLWINDOWPOS3DVARBPROC                    glWindowPos3dvARB;
      PFNGLWINDOWPOS3FVARBPROC                    glWindowPos3fvARB;
      PFNGLWINDOWPOS3IVARBPROC                    glWindowPos3ivARB;
      PFNGLWINDOWPOS3SVARBPROC                    glWindowPos3svARB;

      //  GL_Autodesk_valid_back_buffer_hint functions
      PFNGLWINDOWBACKBUFFERHINTAUTODESKPROC       glWindowBackBufferHintAutodesk;
      PFNGLVALIDBACKBUFFERHINTAUTODESKPROC        glValidBackBufferHintAutodesk;

      //  GL_EXT_blend_func_separate functions
      PFNGLBLENDFUNCSEPARATEEXTPROC               glBlendFuncSeparateEXT;

      //  GL_EXT_compiled_vertex_array functions
      PFNGLLOCKARRAYSEXTPROC                      glLockArraysEXT;
      PFNGLUNLOCKARRAYSEXTPROC                    glUnlockArraysEXT;

      //  GL_EXT_depth_bounds_test functions
      PFNGLDEPTHBOUNDSEXTPROC                     glDepthBoundsEXT;

      //  GL_EXT_point_parameters functions
      PFNGLPOINTPARAMETERFEXTPROC                 glPointParameterfEXT;
      PFNGLPOINTPARAMETERFVEXTPROC                glPointParameterfvEXT;

      //  GL_EXT_stencil_two_side functions
      PFNGLACTIVESTENCILFACEEXTPROC               glActiveStencilFaceEXT;

      //  GL_NV_fence functions
      PFNGLDELETEFENCESNVPROC                     glDeleteFencesNV;
      PFNGLFINISHFENCENVPROC                      glFinishFenceNV;
      PFNGLGENFENCESNVPROC                        glGenFencesNV;
      PFNGLGETFENCEIVNVPROC                       glGetFenceivNV;
      PFNGLISFENCENVPROC                          glIsFenceNV;
      PFNGLSETFENCENVPROC                         glSetFenceNV;
      PFNGLTESTFENCENVPROC                        glTestFenceNV;

      //  GL_NV_fragment_program functions
      PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC       glGetProgramNamedParameterdvNV;
      PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC       glGetProgramNamedParameterfvNV;
      PFNGLPROGRAMNAMEDPARAMETER4DNVPROC          glProgramNamedParameter4dNV;
      PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC         glProgramNamedParameter4dvNV;
      PFNGLPROGRAMNAMEDPARAMETER4FNVPROC          glProgramNamedParameter4fNV;
      PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC         glProgramNamedParameter4fvNV;

      //  GL_NV_half_float functions
      PFNGLCOLOR3HNVPROC                          glColor3hNV;
      PFNGLCOLOR3HVNVPROC                         glColor3hvNV;
      PFNGLCOLOR4HNVPROC                          glColor4hNV;
      PFNGLCOLOR4HVNVPROC                         glColor4hvNV;
      PFNGLFOGCOORDHNVPROC                        glFogCoordhNV;
      PFNGLFOGCOORDHVNVPROC                       glFogCoordhvNV;
      PFNGLMULTITEXCOORD1HNVPROC                  glMultiTexCoord1hNV;
      PFNGLMULTITEXCOORD1HVNVPROC                 glMultiTexCoord1hvNV;
      PFNGLMULTITEXCOORD2HNVPROC                  glMultiTexCoord2hNV;
      PFNGLMULTITEXCOORD2HVNVPROC                 glMultiTexCoord2hvNV;
      PFNGLMULTITEXCOORD3HNVPROC                  glMultiTexCoord3hNV;
      PFNGLMULTITEXCOORD3HVNVPROC                 glMultiTexCoord3hvNV;
      PFNGLMULTITEXCOORD4HNVPROC                  glMultiTexCoord4hNV;
      PFNGLMULTITEXCOORD4HVNVPROC                 glMultiTexCoord4hvNV;
      PFNGLNORMAL3HNVPROC                         glNormal3hNV;
      PFNGLNORMAL3HVNVPROC                        glNormal3hvNV;
      PFNGLSECONDARYCOLOR3HNVPROC                 glSecondaryColor3hNV;
      PFNGLSECONDARYCOLOR3HVNVPROC                glSecondaryColor3hvNV;
      PFNGLTEXCOORD1HNVPROC                       glTexCoord1hNV;
      PFNGLTEXCOORD1HVNVPROC                      glTexCoord1hvNV;
      PFNGLTEXCOORD2HNVPROC                       glTexCoord2hNV;
      PFNGLTEXCOORD2HVNVPROC                      glTexCoord2hvNV;
      PFNGLTEXCOORD3HNVPROC                       glTexCoord3hNV;
      PFNGLTEXCOORD3HVNVPROC                      glTexCoord3hvNV;
      PFNGLTEXCOORD4HNVPROC                       glTexCoord4hNV;
      PFNGLTEXCOORD4HVNVPROC                      glTexCoord4hvNV;
      PFNGLVERTEX2HNVPROC                         glVertex2hNV;
      PFNGLVERTEX2HVNVPROC                        glVertex2hvNV;
      PFNGLVERTEX3HNVPROC                         glVertex3hNV;
      PFNGLVERTEX3HVNVPROC                        glVertex3hvNV;
      PFNGLVERTEX4HNVPROC                         glVertex4hNV;
      PFNGLVERTEX4HVNVPROC                        glVertex4hvNV;
      PFNGLVERTEXATTRIB1HNVPROC                   glVertexAttrib1hNV;
      PFNGLVERTEXATTRIB1HVNVPROC                  glVertexAttrib1hvNV;
      PFNGLVERTEXATTRIB2HNVPROC                   glVertexAttrib2hNV;
      PFNGLVERTEXATTRIB2HVNVPROC                  glVertexAttrib2hvNV;
      PFNGLVERTEXATTRIB3HNVPROC                   glVertexAttrib3hNV;
      PFNGLVERTEXATTRIB3HVNVPROC                  glVertexAttrib3hvNV;
      PFNGLVERTEXATTRIB4HNVPROC                   glVertexAttrib4hNV;
      PFNGLVERTEXATTRIB4HVNVPROC                  glVertexAttrib4hvNV;
      PFNGLVERTEXATTRIBS1HVNVPROC                 glVertexAttribs1hvNV;
      PFNGLVERTEXATTRIBS2HVNVPROC                 glVertexAttribs2hvNV;
      PFNGLVERTEXATTRIBS3HVNVPROC                 glVertexAttribs3hvNV;
      PFNGLVERTEXATTRIBS4HVNVPROC                 glVertexAttribs4hvNV;

      //  GL_NV_occlusion_query functions
      PFNGLBEGINOCCLUSIONQUERYNVPROC              glBeginOcclusionQueryNV;
      PFNGLDELETEOCCLUSIONQUERIESNVPROC           glDeleteOcclusionQueriesNV;
      PFNGLENDOCCLUSIONQUERYNVPROC                glEndOcclusionQueryNV;
      PFNGLGENOCCLUSIONQUERIESNVPROC              glGenOcclusionQueriesNV;
      PFNGLGETOCCLUSIONQUERYIVNVPROC              glGetOcclusionQueryivNV;
      PFNGLGETOCCLUSIONQUERYUIVNVPROC             glGetOcclusionQueryuivNV;
      PFNGLISOCCLUSIONQUERYNVPROC                 glIsOcclusionQueryNV;

      //  GL_NV_pixel_data_range functions
      PFNGLFLUSHPIXELDATARANGENVPROC              glFlushPixelDataRangeNV;
      PFNGLPIXELDATARANGENVPROC                   glPixelDataRangeNV;

      //  GL_NV_primitive_restart functions
      PFNGLPRIMITIVERESTARTNVPROC                 glPrimitiveRestartNV;
      PFNGLPRIMITIVERESTARTINDEXNVPROC            glPrimitiveRestartIndexNV;

      //  GL_NV_register_combiners functions
      PFNGLCOMBINERPARAMETERFVNVPROC              glCombinerParameterfvNV;
      PFNGLCOMBINERPARAMETERFNVPROC               glCombinerParameterfNV;
      PFNGLCOMBINERPARAMETERIVNVPROC              glCombinerParameterivNV;
      PFNGLCOMBINERPARAMETERINVPROC               glCombinerParameteriNV;
      PFNGLCOMBINERINPUTNVPROC                    glCombinerInputNV;
      PFNGLCOMBINEROUTPUTNVPROC                   glCombinerOutputNV;
      PFNGLFINALCOMBINERINPUTNVPROC               glFinalCombinerInputNV;
      PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC      glGetCombinerInputParameterfvNV;
      PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC      glGetCombinerInputParameterivNV;
      PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC     glGetCombinerOutputParameterfvNV;
      PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC     glGetCombinerOutputParameterivNV;
      PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV;
      PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV;

      //  GL_NV_register_combiners2 functions
      PFNGLCOMBINERSTAGEPARAMETERFVNVPROC         glCombinerStageParameterfvNV;
      PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC      glGetCombinerStageParameterfvNV;

      //  GL_NV_set_window_stereomode functions
      PFNGLSETWINDOWSTEREOMODENVPROC              glSetWindowStereoModeNV;

      //  GL_NV_vertex_array_range functions
      PFNGLFLUSHVERTEXARRAYRANGENVPROC            glFlushVertexArrayRangeNV;
      PFNGLVERTEXARRAYRANGENVPROC                 glVertexArrayRangeNV;

      //  GL_NV_vertex_program functions
      PFNGLAREPROGRAMSRESIDENTNVPROC              glAreProgramsResidentNV;
      PFNGLBINDPROGRAMNVPROC                      glBindProgramNV;
      PFNGLDELETEPROGRAMSNVPROC                   glDeleteProgramsNV;
      PFNGLEXECUTEPROGRAMNVPROC                   glExecuteProgramNV;
      PFNGLGENPROGRAMSNVPROC                      glGenProgramsNV;
      PFNGLGETPROGRAMPARAMETERDVNVPROC            glGetProgramParameterdvNV;
      PFNGLGETPROGRAMPARAMETERFVNVPROC            glGetProgramParameterfvNV;
      PFNGLGETPROGRAMIVNVPROC                     glGetProgramivNV;
      PFNGLGETPROGRAMSTRINGNVPROC                 glGetProgramStringNV;
      PFNGLGETTRACKMATRIXIVNVPROC                 glGetTrackMatrixivNV;
      PFNGLGETVERTEXATTRIBDVNVPROC                glGetVertexAttribdvNV;
      PFNGLGETVERTEXATTRIBFVNVPROC                glGetVertexAttribfvNV;
      PFNGLGETVERTEXATTRIBIVNVPROC                glGetVertexAttribivNV;
      PFNGLGETVERTEXATTRIBPOINTERVNVPROC          glGetVertexAttribPointervNV;
      PFNGLISPROGRAMNVPROC                        glIsProgramNV;
      PFNGLLOADPROGRAMNVPROC                      glLoadProgramNV;
      PFNGLPROGRAMPARAMETER4DNVPROC               glProgramParameter4dNV;
      PFNGLPROGRAMPARAMETER4DVNVPROC              glProgramParameter4dvNV;
      PFNGLPROGRAMPARAMETER4FNVPROC               glProgramParameter4fNV;
      PFNGLPROGRAMPARAMETER4FVNVPROC              glProgramParameter4fvNV;
      PFNGLPROGRAMPARAMETERS4DVNVPROC             glProgramParameters4dvNV;
      PFNGLPROGRAMPARAMETERS4FVNVPROC             glProgramParameters4fvNV;
      PFNGLREQUESTRESIDENTPROGRAMSNVPROC          glRequestResidentProgramsNV;
      PFNGLTRACKMATRIXNVPROC                      glTrackMatrixNV;
      PFNGLVERTEXATTRIBPOINTERNVPROC              glVertexAttribPointerNV;
      PFNGLVERTEXATTRIB1DNVPROC                   glVertexAttrib1dNV;
      PFNGLVERTEXATTRIB1DVNVPROC                  glVertexAttrib1dvNV;
      PFNGLVERTEXATTRIB1FNVPROC                   glVertexAttrib1fNV;
      PFNGLVERTEXATTRIB1FVNVPROC                  glVertexAttrib1fvNV;
      PFNGLVERTEXATTRIB1SNVPROC                   glVertexAttrib1sNV;
      PFNGLVERTEXATTRIB1SVNVPROC                  glVertexAttrib1svNV;
      PFNGLVERTEXATTRIB2DNVPROC                   glVertexAttrib2dNV;
      PFNGLVERTEXATTRIB2DVNVPROC                  glVertexAttrib2dvNV;
      PFNGLVERTEXATTRIB2FNVPROC                   glVertexAttrib2fNV;
      PFNGLVERTEXATTRIB2FVNVPROC                  glVertexAttrib2fvNV;
      PFNGLVERTEXATTRIB2SNVPROC                   glVertexAttrib2sNV;
      PFNGLVERTEXATTRIB2SVNVPROC                  glVertexAttrib2svNV;
      PFNGLVERTEXATTRIB3DNVPROC                   glVertexAttrib3dNV;
      PFNGLVERTEXATTRIB3DVNVPROC                  glVertexAttrib3dvNV;
      PFNGLVERTEXATTRIB3FNVPROC                   glVertexAttrib3fNV;
      PFNGLVERTEXATTRIB3FVNVPROC                  glVertexAttrib3fvNV;
      PFNGLVERTEXATTRIB3SNVPROC                   glVertexAttrib3sNV;
      PFNGLVERTEXATTRIB3SVNVPROC                  glVertexAttrib3svNV;
      PFNGLVERTEXATTRIB4DNVPROC                   glVertexAttrib4dNV;
      PFNGLVERTEXATTRIB4DVNVPROC                  glVertexAttrib4dvNV;
      PFNGLVERTEXATTRIB4FNVPROC                   glVertexAttrib4fNV;
      PFNGLVERTEXATTRIB4FVNVPROC                  glVertexAttrib4fvNV;
      PFNGLVERTEXATTRIB4SNVPROC                   glVertexAttrib4sNV;
      PFNGLVERTEXATTRIB4SVNVPROC                  glVertexAttrib4svNV;
      PFNGLVERTEXATTRIB4UBNVPROC                  glVertexAttrib4ubNV;
      PFNGLVERTEXATTRIB4UBVNVPROC                 glVertexAttrib4ubvNV;
      PFNGLVERTEXATTRIBS1DVNVPROC                 glVertexAttribs1dvNV;
      PFNGLVERTEXATTRIBS1FVNVPROC                 glVertexAttribs1fvNV;
      PFNGLVERTEXATTRIBS1SVNVPROC                 glVertexAttribs1svNV;
      PFNGLVERTEXATTRIBS2DVNVPROC                 glVertexAttribs2dvNV;
      PFNGLVERTEXATTRIBS2FVNVPROC                 glVertexAttribs2fvNV;
      PFNGLVERTEXATTRIBS2SVNVPROC                 glVertexAttribs2svNV;
      PFNGLVERTEXATTRIBS3DVNVPROC                 glVertexAttribs3dvNV;
      PFNGLVERTEXATTRIBS3FVNVPROC                 glVertexAttribs3fvNV;
      PFNGLVERTEXATTRIBS3SVNVPROC                 glVertexAttribs3svNV;
      PFNGLVERTEXATTRIBS4DVNVPROC                 glVertexAttribs4dvNV;
      PFNGLVERTEXATTRIBS4FVNVPROC                 glVertexAttribs4fvNV;
      PFNGLVERTEXATTRIBS4SVNVPROC                 glVertexAttribs4svNV;
      PFNGLVERTEXATTRIBS4UBVNVPROC                glVertexAttribs4ubvNV;

      //  GL_WIN_swap_hint functions
      PFNGLADDSWAPHINTRECTWINPROC                 glAddSwapHintRectWIN;

      // GL_EXT_framebuffer_object
      PFNGLISRENDERBUFFEREXTPROC                  glIsRenderbufferEXT;
      PFNGLBINDRENDERBUFFEREXTPROC                glBindRenderbufferEXT;
      PFNGLDELETERENDERBUFFERSEXTPROC             glDeleteRenderbuffersEXT;
      PFNGLGENRENDERBUFFERSEXTPROC                glGenRenderbuffersEXT;
      PFNGLRENDERBUFFERSTORAGEEXTPROC             glRenderbufferStorageEXT;
      PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC      glGetRenderbufferParameterivEXT;
      PFNGLISFRAMEBUFFEREXTPROC                   glIsFramebufferEXT;
      PFNGLBINDFRAMEBUFFEREXTPROC                 glBindFramebufferEXT;
      PFNGLDELETEFRAMEBUFFERSEXTPROC              glDeleteFramebuffersEXT;
      PFNGLGENFRAMEBUFFERSEXTPROC                 glGenFramebuffersEXT;
      PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC          glCheckFramebufferStatusEXT;
      PFNGLFRAMEBUFFERTEXTURE1DEXTPROC            glFramebufferTexture1DEXT;
      PFNGLFRAMEBUFFERTEXTURE2DEXTPROC            glFramebufferTexture2DEXT;
      PFNGLFRAMEBUFFERTEXTURE3DEXTPROC            glFramebufferTexture3DEXT;
      PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC         glFramebufferRenderbufferEXT;
      PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC  glGetFramebufferAttachmentParameterivEXT;
      PFNGLGENERATEMIPMAPEXTPROC                       glGenerateMipmapEXT; 

#if defined(_WIN32)      
      //  WGL_ARB_buffer_region functions
      PFNWGLCREATEBUFFERREGIONARBPROC             wglCreateBufferRegionARB;
      PFNWGLDELETEBUFFERREGIONARBPROC             wglDeleteBufferRegionARB;
      PFNWGLRESTOREBUFFERREGIONARBPROC            wglRestoreBufferRegionARB;
      PFNWGLSAVEBUFFERREGIONARBPROC               wglSaveBufferRegionARB;

      //  WGL_ARB_extensions_string functions
      PFNWGLGETEXTENSIONSSTRINGARBPROC            wglGetExtensionsStringARB;

      //  WGL_ARB_make_current_read functions
      PFNWGLMAKECONTEXTCURRENTARBPROC             wglMakeContextCurrentARB;
      PFNWGLGETCURRENTREADDCARBPROC               wglGetCurrentReadDCARB;

      //  WGL_ARB_pbuffer functions
      PFNWGLCREATEPBUFFERARBPROC                  wglCreatePbufferARB;
      PFNWGLDESTROYPBUFFERARBPROC                 wglDestroyPbufferARB;
      PFNWGLGETPBUFFERDCARBPROC                   wglGetPbufferDCARB;
      PFNWGLQUERYPBUFFERARBPROC                   wglQueryPbufferARB;
      PFNWGLRELEASEPBUFFERDCARBPROC               wglReleasePbufferDCARB;

      //  WGL_ARB_pixel_format functions
      PFNWGLCHOOSEPIXELFORMATARBPROC              wglChoosePixelFormatARB;
      PFNWGLGETPIXELFORMATATTRIBIVARBPROC         wglGetPixelFormatAttribivARB;
      PFNWGLGETPIXELFORMATATTRIBFVARBPROC         wglGetPixelFormatAttribfvARB;

      //  WGL_ARB_render_texture functions
      PFNWGLBINDTEXIMAGEARBPROC                   wglBindTexImageARB;
      PFNWGLRELEASETEXIMAGEARBPROC                wglReleaseTexImageARB;
      PFNWGLSETPBUFFERATTRIBARBPROC               wglSetPbufferAttribARB;

      //  WGL_EXT_extensions_string functions
      PFNWGLGETEXTENSIONSSTRINGEXTPROC            wglGetExtensionsStringEXT;

      //  WGL_EXT_genlock functions
      PFNWGLENABLEGENLOCKEXTPROC			            wglEnableGenlockEXT;
      PFNWGLDISABLEGENLOCKEXTPROC			            wglDisableGenlockEXT;
      PFNWGLGENLOCKSOURCEEXTPROC			            wglGenlockSourceEXT;
      PFNWGLGENLOCKSAMPLERATEEXTPROC		          wglGenlockSampleRateEXT;
      PFNWGLGENLOCKSOURCEDELAYEXTPROC		          wglGenlockSourceDelayEXT;
      PFNWGLGENLOCKSOURCEEDGEEXTPROC		          wglGenlockSourceEdgeEXT;
      PFNWGLGETGENLOCKSOURCEEXTPROC		            wglGetGenlockSourceEXT;
      PFNWGLGETGENLOCKSAMPLERATEEXTPROC	          wglGetGenlockSampleRateEXT;
      PFNWGLGETGENLOCKSOURCEDELAYEXTPROC	        wglGetGenlockSourceDelayEXT;
      PFNWGLGETGENLOCKSOURCEEDGEEXTPROC	          wglGetGenlockSourceEdgeEXT;
      PFNWGLISENABLEDGENLOCKEXTPROC		            wglIsEnabledGenlockEXT;
      PFNWGLQUERYGENLOCKMAXSOURCEDELAYEXTPROC     wglQueryGenlockMaxSourceDelayEXT;

      //  WGL_EXT_swap_control functions
      PFNWGLSWAPINTERVALEXTPROC                   wglSwapIntervalEXT;
      PFNWGLGETSWAPINTERVALEXTPROC                wglGetSwapIntervalEXT;

      //  WGL_EXT_swap_frame_lock functions
      PFNWGLENABLEFRAMELOCKEXTPROC                wglEnableFrameLockEXT;
      PFNWGLDISABLEFRAMELOCKEXTPROC               wglDisableFrameLockEXT;
      PFNWGLISENABLEDFRAMELOCKEXTPROC             wglIsEnabledFrameLockEXT;
      PFNWGLQUERYFRAMELOCKMASTEREXTPROC           wglQueryFrameLockMasterEXT;

      //  WGL_I3D_genlock functions
      PFNWGLENABLEGENLOCKI3DPROC                  wglEnableGenlockI3D;
      PFNWGLDISABLEGENLOCKI3DPROC                 wglDisableGenlockI3D;
      PFNWGLISENABLEDGENLOCKI3DPROC               wglIsEnabledGenlockI3D;
      PFNWGLGENLOCKSOURCEI3DPROC                  wglGenlockSourceI3D;
      PFNWGLGETGENLOCKSOURCEI3DPROC               wglGetGenlockSourceI3D;
      PFNWGLGENLOCKSOURCEEDGEI3DPROC              wglGenlockSourceEdgeI3D;
      PFNWGLGETGENLOCKSOURCEEDGEI3DPROC           wglGetGenlockSourceEdgeI3D;
      PFNWGLGENLOCKSAMPLERATEI3DPROC              wglGenlockSampleRateI3D;
      PFNWGLGETGENLOCKSAMPLERATEI3DPROC           wglGetGenlockSampleRateI3D;
      PFNWGLGENLOCKSOURCEDELAYI3DPROC             wglGenlockSourceDelayI3D;
      PFNWGLGETGENLOCKSOURCEDELAYI3DPROC          wglGetGenlockSourceDelayI3D;
      PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC     wglQueryGenlockMaxSourceDelayI3D;

      //  WGL_KTX_buffer_region functions
      PFNWGLNEWBUFFERREGIONPROC                   wglNewBufferRegion;
      PFNWGLDELETEBUFFERREGIONPROC                wglDeleteBufferRegion;
      PFNWGLREADBUFFERREGIONPROC                  wglReadBufferRegion;
      PFNWGLDRAWBUFFERREGIONPROC                  wglDrawBufferRegion;
      PFNWGLBUFFERREGIONENABLEDPROC               wglBufferRegionEnabled;

      //  WGL_NV_allocate_memory functions
      PFNWGLALLOCATEMEMORYNVPROC                  wglAllocateMemoryNV;
      PFNWGLFREEMEMORYNVPROC                      wglFreeMemoryNV;

      //  WGL_NV_swap_group functions
      PFNWGLBINDSWAPBARRIERNVPROC                 wglBindSwapBarrierNV;
      PFNWGLJOINSWAPGROUPNVPROC                   wglJoinSwapGroupNV;
      PFNWGLQUERYSWAPGROUPNVPROC                  wglQuerySwapGroupNV;
      PFNWGLQUERYMAXSWAPGROUPSNVPROC              wglQueryMaxSwapGroupsNV;
      PFNWGLQUERYFRAMECOUNTNVPROC                 wglQueryFrameCountNV;
      PFNWGLRESETFRAMECOUNTNVPROC                 wglResetFrameCountNV;
    
      // WGL_NV_gpu_affinity
      PFNWGLENUMGPUSNVPROC                        wglEnumGpusNV;
      PFNWGLENUMGPUDEVICESNVPROC                  wglEnumGpuDevicesNV;
      PFNWGLCREATEAFFINITYDCNVPROC                wglCreateAffinityDCNV;
      PFNWGLENUMGPUSFROMAFFINITYDCNVPROC          wglEnumGpusFromAffinityDCNV;
      PFNWGLDELETEDCNVPROC                        wglDeleteDCNV;

#elif defined(LINUX)
      PFNGLXQUERYEXTENSIONSSTRINGPROC             glXQueryExtensionsString;     

      // GLX_SGIX_pbuffer
      PFNGLXCREATEGLXPBUFFERSGIXPROC              glXCreateGLXPbufferSGIX;
      PFNGLXDESTROYGLXPBUFFERSGIXPROC             glXDestroyGLXPbufferSGIX;
      PFNGLXQUERYGLXPBUFFERSGIXPROC               glXQueryGLXPbufferSGIX;      
#endif
#endif  //  DOXYGEN_IGNORE

    private:
      int         m_glVersion;
      std::set<std::string> m_available;
      std::set<std::string> m_exported;
  };

  inline  bool  GLFunctions::isAvailable( const std::string& extension ) const
  {
    return( m_available.find( extension ) != m_available.end() );
  }

  inline  bool   GLFunctions::isExported( const std::string& extension ) const
  {
    return( m_exported.find( extension ) != m_exported.end() );
  }
  
  inline int GLFunctions::getVersion() const
  {
    return m_glVersion;
  }
}
