// Copyright NVIDIA Corporation 2002-2005
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

#include "nvsgcommon.h"

#include <GL/gl.h>  // standard OpenGL header satisfying OpenGL 1.1 API

// Calling convention define required below.
// Note: GLAPI is already defined in gl.h from Mesa 3-D graphics library (Linux build),
// where it is used to specify the linkage and not the calling convention.
#ifdef _WIN32
# define GLAPI __stdcall
#else
# define GLAPI 
#endif

// Note: Be sure to define GL_GLEXT_LEGACY when building on Linux with Mesa 3-D headers!
#include "GL/glext.h"   // OpenGL extension
#if defined(_WIN32)
# include "GL/wglext.h" // Microsoft specific OpenGL extensions
#elif defined(LINUX)
# include "GL/glx.h"
# include "nvgl/glxtypes.h"
# include <pthread.h>
#endif               
 
// STL header
#include <map>
#include <set>

// GL error tracing macro
#if !defined(NDEBUG) && !defined(NVSG_HYBRID)
# define ASSERT_GL_NO_ERROR               NVSG_ASSERT( ( glError = glGetError() ) == GL_NO_ERROR )
# define ASSERT_GL_NO_ERROR_IN_BEGIN_END  NVSG_ASSERT( glContext()->m_inBeginEndLoop || ( glError = glGetError() ) == GL_NO_ERROR )
#else
# define ASSERT_GL_NO_ERROR
# define ASSERT_GL_NO_ERROR_IN_BEGIN_END
#endif

//! Namespace for OpenGL
/** This namespace wraps around the native OpenGL implementation.
  * It organizes caching, attribute handling, etc....
  * \note Do not use native OpenGL calls when extending NVSG or NVSG-based viewers.
  * Instead, use the implementation of the OpenGL commands contained in this namespace 
  * so you do not break caching, etc.
  *
  * \note Do not include other OpenGL headers. 
  * nvgl.h should be the only OpenGL header you need. */
namespace nvgl
{
  class GLContext;
  
#if defined(_WIN32)
  extern  NVSG_API DWORD glContextTlsIdx;
#elif defined(LINUX)
  extern  NVSG_API pthread_key_t glContextKey; 
#else
#error Unsupported Operating System!
#endif

#if !defined(NDEBUG)
  extern  NVSG_API GLenum  glError; //!<  holds the current OpenGL error code to ease debugging
#endif

  typedef std::set<GLuint> NameSet;
  typedef NameSet::const_iterator ConstNameSetIter;
}


