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
#include "nvgl/nvgl.h"
#include "nvgl/GLStatistics.h"
#include "nvmath/Vec4f.h"
#include "nvutil/RCObject.h"
#include "nvsg/TextureImage.h"
#include <map>
#include <stack>
#include <string>
#include <algorithm>
#include <utility>


// common device context helper data types for Windows and Linux
#if defined(_WIN32)
typedef HDC   DEVICE_CONTEXT; //!< Helper type that maps to HDC on Windows and Drawable on Linux.
typedef HGLRC RENDER_CONTEXT; //!< Helper type that maps to HGLRC on Windows and GLXContext on Linux.
#elif defined(LINUX)
typedef Drawable   DEVICE_CONTEXT;
typedef GLXContext RENDER_CONTEXT;
#endif

namespace nvsg
{
  class DALHost;
  class TextureImage;
}

namespace nvgl
{
class GLFunctions;
class GLObjects;
struct GLTextureCache;
struct GLDisplayListCache;
struct GLArrayBufferCache;
struct GLElementArrayBufferCache;
struct GLPixelBufferCache;

#if !defined(DOXYGEN_IGNORE)
typedef void (*PFNUPLOADTEXTUREPROC)(GLint, GLint, GLint, nvsg::TextureImage*, GLTextureCache*);
#endif // DOXYGEN_IGNORE

// common device context helper function for Windows and Linux
#if defined(_WIN32)
//! Get the current device context.
/** This is a helper macro defined for both Windows and Linux. It maps to either WGLGetCurrentDC or GLXGetCurrentDrawable. */
# define GET_CURRENT_DC()    WGLGetCurrentDC()
//! Get the current render context.
/** This is a helper macro defined for both Windows and Linux. It maps to either WGLGetCurrentContext or GLXGetCurrentContext. */
# define GET_CURRENT_RC()    WGLGetCurrentContext()
//! Make the given device context and render context current.
/** This is a helper macro defined for both Windows and Linux. It maps to either WGLMakeCurrent or GLXMakeCurrent. */
# define MAKE_CURRENT(DC,RC) WGLMakeCurrent(DC,RC)
#elif defined(LINUX)
# define GET_CURRENT_DC()    GLXGetCurrentDrawable()
# define GET_CURRENT_RC()    GLXGetCurrentContext()
# define MAKE_CURRENT(DC,RC) GLXMakeCurrent(glContext()->getDisplay(),DC,RC)
#else
# error Unsupported Operating System!  
#endif

#if !defined(DOXYGEN_IGNORE)
# define GL_CACHING
# if defined( GL_CACHING )
#  define GL_SUPPORT_RESET
# endif
#endif // DOXYGEN_IGNORE

//! GLTexImageFmt specifies the format and data type of a texture image.
struct GLTexImageFmt
{
  GLint intFmt;       //!< The OpenGL internal format of a texture.
  GLint usrFmt;       //!< The OpenGL user format of a texture
  GLint type;         //!< The OpenGL type of a texture
  GLint uploadHint;   //!< An upload hint for the texture: 0=glTexImage, 1=PBO
};

#define NVSG_CACHE_FRONT  0   //!< ID for the front caches
#define NVSG_CACHE_BACK   1   //!< ID for the back caches

// provide zero-based mapping of buffer bindings
enum
{
  // vertex buffer targets
  NVSG_ARRAY_BUFFER = 0,
  NVSG_FIRST_VERTEX_BUFFER = NVSG_ARRAY_BUFFER,
  NVSG_ELEMENT_ARRAY_BUFFER,
  NVSG_LAST_VERTEX_BUFFER = NVSG_ELEMENT_ARRAY_BUFFER,
  // pixel buffer targets
  NVSG_PIXEL_PACK_BUFFER,
  NVSG_FIRST_PIXEL_BUFFER = NVSG_PIXEL_PACK_BUFFER,  
  NVSG_PIXEL_UNPACK_BUFFER,
  NVSG_LAST_PIXEL_BUFFER = NVSG_PIXEL_UNPACK_BUFFER,
  
  NVSG_NUM_VERTEX_BUFFERS = NVSG_LAST_VERTEX_BUFFER - NVSG_FIRST_VERTEX_BUFFER + 1,
  NVSG_NUM_PIXEL_BUFFERS  = NVSG_LAST_PIXEL_BUFFER - NVSG_FIRST_PIXEL_BUFFER + 1,
  NVSG_NUM_BUFFERS = NVSG_NUM_VERTEX_BUFFERS + NVSG_NUM_PIXEL_BUFFERS
};

#define BUFFER_OFFSET(i) ((char*)NULL + (i))  //!< Helper macro with buffer-usage

//! Wrapper for OpenGL functions to provide caching, extension handling, and, in debug mode, error handling per context.
/** A \c GLContext is defined per pair of <tt><HDC,HGLRC></tt> for Windows or <tt><Display*,GLXContext></tt> for Linux.
  * It holds informations to cache the most important OpenGL states. \n
  * You will never have to use this class explicitly. For creation, deletion, and consistent interaction, instead of using
  * the functions with the lowercase prefix \c wgl on Windows, use the corresponding functions with the uppercase prefix
  * \c WGL. That is, instead of using \c wglCreateContext(), use \c WGLCreateContext(). On Linux, instead of using the
  * prefix \c glX, use the prefix \c GLX. \n
  * To use it consistently, all OpenGL calls should be replaced by the corresponding functions with uppercase \c GL, defined
  * in GLContext.h. I.e. instead of using \c glAccum(), you should use \c GLAccum(). For some of the OpenGL state handling
  * functions, there are slightly faster specialized functions with uppercase \c GL. For example, instead of using
  * <tt>glDisable(GL_BLEND)</tt>, you could either use \link GLDisable() <tt>GLDisable(GL_BLEND)</tt>\endlink  or the
  * specialized function \c GLDisableBlend(). Note, that there might be states that are not covered with specialized functions. \n
  * In the debug build, all the functions with uppercase prefix also test for the OpenGL error code being \c GL_NO_ERROR and
  * assert on failure. */
class GLContext
{
  friend NVSG_API void GLAccum( GLenum op, GLfloat value );
  friend NVSG_API void GLActiveTexture( GLenum texture );
  friend NVSG_API void GLAlphaFunc( GLenum func, GLclampf ref );
  friend NVSG_API GLboolean GLAreTexturesResident( GLsizei n, const GLuint *textures, GLboolean *residences );
  friend NVSG_API void GLArrayElement( GLint i );
  friend NVSG_API void GLBegin( GLenum mode );
  friend NVSG_API void GLBeginOcclusionQuery( GLuint id );
  friend NVSG_API void GLBeginQuery( GLenum target, GLuint id );
  friend NVSG_API void GLBindBuffer(GLenum target, GLuint id);
  friend NVSG_API void GLBindProgram( GLenum target, GLuint id );
  friend NVSG_API void GLBitmap( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap );
  friend NVSG_API void GLBindTexture( GLenum target, GLuint texture );
  friend NVSG_API void GLBlendFunc( GLenum sfactor, GLenum dfactor );
  friend NVSG_API void GLBlendFuncSeparate( GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha );
  friend NVSG_API void GLBufferData( GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage );
  friend NVSG_API void GLBufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data );
  friend NVSG_API void GLCallList( GLuint list );
  friend NVSG_API void GLCallLists( GLsizei n, GLenum type, const GLvoid *lists );
  friend NVSG_API void GLClear( GLbitfield mask );
  friend NVSG_API void GLClearAccum( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
  friend NVSG_API void GLClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
  friend NVSG_API void GLClearDepth( GLclampd depth );
  friend NVSG_API void GLClearIndex( GLfloat c );
  friend NVSG_API void GLClearStencil( GLint s );
  friend NVSG_API void GLClientActiveTexture( GLenum texture );
  friend NVSG_API void GLClipPlane( GLenum plane, const GLdouble *equation );
  friend NVSG_API void GLColor3b( GLbyte red, GLbyte green, GLbyte blue );
  friend NVSG_API void GLColor3bv( const GLbyte *v );
  friend NVSG_API void GLColor3d( GLdouble red, GLdouble green, GLdouble blue );
  friend NVSG_API void GLColor3dv( const GLdouble *v );
  friend NVSG_API void GLColor3f( GLfloat red, GLfloat green, GLfloat blue );
  friend NVSG_API void GLColor3fv( const GLfloat * v );
  friend NVSG_API void GLColor3i( GLint red, GLint green, GLint blue );
  friend NVSG_API void GLColor3iv( const GLint * v );
  friend NVSG_API void GLColor3s( GLshort red, GLshort green, GLshort blue );
  friend NVSG_API void GLColor3sv( const GLshort * v );
  friend NVSG_API void GLColor3ub( GLubyte red, GLubyte green, GLubyte blue );
  friend NVSG_API void GLColor3ubv( const GLubyte * v );
  friend NVSG_API void GLColor3ui( GLuint red, GLuint green, GLuint blue );
  friend NVSG_API void GLColor3uiv( const GLuint * v );
  friend NVSG_API void GLColor3us( GLushort red, GLushort green, GLushort blue );
  friend NVSG_API void GLColor3usv( const GLushort * v );
  friend NVSG_API void GLColor4b( GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha );
  friend NVSG_API void GLColor4bv( const GLbyte * v );
  friend NVSG_API void GLColor4d( GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha );
  friend NVSG_API void GLColor4dv( const GLdouble * v );
  friend NVSG_API void GLColor4f( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
  friend NVSG_API void GLColor4fv( const GLfloat * v );
  friend NVSG_API void GLColor4i( GLint red, GLint green, GLint blue, GLint alpha );
  friend NVSG_API void GLColor4iv( const GLint * v );
  friend NVSG_API void GLColor4s( GLshort red, GLshort green, GLshort blue, GLshort alpha );
  friend NVSG_API void GLColor4sv( const GLshort * v );
  friend NVSG_API void GLColor4ub( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha );
  friend NVSG_API void GLColor4ubv( const GLubyte * v );
  friend NVSG_API void GLColor4ui( GLuint red, GLuint green, GLuint blue, GLuint alpha );
  friend NVSG_API void GLColor4uiv( const GLuint * v );
  friend NVSG_API void GLColor4us( GLushort red, GLushort green, GLushort blue, GLushort alpha );
  friend NVSG_API void GLColor4usv( const GLushort * v );
  friend NVSG_API void GLColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
  friend NVSG_API void GLColorMaterial( GLenum face, GLenum mode );
  friend NVSG_API void GLColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLCompressedTexImage1D( GLenum target, int level, GLenum internalformat, GLsizei width, int border, GLsizei imageSize, const void *data );
  friend NVSG_API void GLCompressedTexImage2D( GLenum target, int level, GLenum internalformat, GLsizei width, GLsizei height, int border, GLsizei imageSize, const void *data );
  friend NVSG_API void GLCompressedTexImage3D( GLenum target, int level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, int border, GLsizei imageSize, const void *data );
  friend NVSG_API void GLCopyPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum type );
  friend NVSG_API void GLCopyTexImage1D( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border );
  friend NVSG_API void GLCopyTexImage2D( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border );
  friend NVSG_API void GLCopyTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width );
  friend NVSG_API void GLCopyTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height );
  friend NVSG_API void GLCullFace( GLenum mode );
  friend NVSG_API void GLCullFaceLock( GLenum mode );
  friend NVSG_API void GLCullFaceUnlock( GLenum mode );
  friend NVSG_API void GLDeleteBuffers( GLsizei n, const GLuint *buffers );
  friend NVSG_API void GLDeleteLists( GLuint list, GLsizei range );
  friend NVSG_API void GLDeleteOcclusionQueries( GLsizei n, const GLuint *ids );
  friend NVSG_API void GLDeletePrograms( GLsizei n, const GLuint *ids );
  friend NVSG_API void GLDeleteQueries( GLsizei n, const GLuint *ids );
  friend NVSG_API void GLDeleteTextures( GLsizei n, const GLuint *textures );
  friend NVSG_API void GLDepthFunc( GLenum func );
  friend NVSG_API void GLDepthMask( GLboolean flag );
  friend NVSG_API void GLDepthRange( GLclampd znear, GLclampd zfar );
  friend NVSG_API void GLDisable( GLenum cap );
  friend NVSG_API void GLDisableAlphaTest( void );
  friend NVSG_API void GLDisableAutoNormal( void );
  friend NVSG_API void GLDisableBlend( void );
  friend NVSG_API void GLDisableClientState( GLenum array );
  friend NVSG_API void GLDisableClipPlane( GLenum plane );
  friend NVSG_API void GLDisableColorArray( void );
  friend NVSG_API void GLDisableColorLogicOp( void );
  friend NVSG_API void GLDisableColorMaterial( void );
  friend NVSG_API void GLDisableCullFace( void );
  friend NVSG_API void GLDisableCullFaceLock( void );
  friend NVSG_API void GLDisableCullFaceUnlock( void );
  friend NVSG_API void GLDisableDepthTest( void );
  friend NVSG_API void GLDisableFogCoordinateArray( void );
  friend NVSG_API void GLDisableNormalArray( void );
  friend NVSG_API void GLDisableDither( void );
  friend NVSG_API void GLDisableFog( void );
  friend NVSG_API void GLDisableLight( GLenum light );
  friend NVSG_API void GLDisableLighting( void );
  friend NVSG_API void GLDisableLightingLock( void );
  friend NVSG_API void GLDisableLightingUnlock( void );
  friend NVSG_API void GLDisableLineSmooth( void );
  friend NVSG_API void GLDisableLineStipple( void );
  friend NVSG_API void GLDisableLogicOp( void );
  friend NVSG_API void GLDisableNormalize( void );
  friend NVSG_API void GLDisablePointSmooth( void );
  friend NVSG_API void GLDisablePolygonOffsetFill( void );
  friend NVSG_API void GLDisablePolygonOffsetLine( void );
  friend NVSG_API void GLDisablePolygonOffsetPoint( void );
  friend NVSG_API void GLDisablePolygonSmooth( void );
  friend NVSG_API void GLDisablePolygonStipple( void );
  friend NVSG_API void GLDisableScissorTest( void );
  friend NVSG_API void GLDisableSecondaryColorArray( void );
  friend NVSG_API void GLDisableStencilTest( void );
  friend NVSG_API void GLDisableTexture( GLenum target );
  friend NVSG_API void GLDisableTextureCoordArray( void );
  friend NVSG_API void GLDisableTextureGen( GLenum target );
  friend NVSG_API void GLDisableVertexArray( void );
  friend NVSG_API void GLDrawArrays( GLenum mode, GLint first, GLsizei count );
  friend NVSG_API void GLDrawBuffer( GLenum mode );
  friend NVSG_API void GLDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices );
  friend NVSG_API void GLDrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );
  friend NVSG_API void GLDrawRangeElements( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
  friend NVSG_API void GLEdgeFlagPointer( GLsizei stride, const GLboolean *pointer );
  friend NVSG_API void GLEdgeFlag( GLboolean flag );
  friend NVSG_API void GLEdgeFlagv( const GLboolean *flag );
  friend NVSG_API void GLEnable( GLenum cap );
  friend NVSG_API void GLEnableAlphaTest( void );
  friend NVSG_API void GLEnableAutoNormal( void );
  friend NVSG_API void GLEnableBlend( void );
  friend NVSG_API void GLEnableClientState( GLenum array );
  friend NVSG_API void GLEnableClipPlane( GLenum plane );
  friend NVSG_API void GLEnableColorArray( void );
  friend NVSG_API void GLEnableColorLogicOp( void );
  friend NVSG_API void GLEnableColorMaterial( void );
  friend NVSG_API void GLEnableCullFace( void );
  friend NVSG_API void GLEnableCullFaceLock( void );
  friend NVSG_API void GLEnableCullFaceUnlock( void );
  friend NVSG_API void GLEnableDepthTest( void );
  friend NVSG_API void GLEnableFogCoordinateArray( void );
  friend NVSG_API void GLEnableDither( void );
  friend NVSG_API void GLEnableFog( void );
  friend NVSG_API void GLEnableLight( GLenum light );
  friend NVSG_API void GLEnableLighting( void );
  friend NVSG_API void GLEnableLineSmooth( void );
  friend NVSG_API void GLEnableLineStipple( void );
  friend NVSG_API void GLEnableLogicOp( void );
  friend NVSG_API void GLEnableNormalArray( void );
  friend NVSG_API void GLEnableNormalize( void );
  friend NVSG_API void GLEnablePointSmooth( void );
  friend NVSG_API void GLEnablePolygonOffsetFill( void );
  friend NVSG_API void GLEnablePolygonOffsetLine( void );
  friend NVSG_API void GLEnablePolygonOffsetPoint( void );
  friend NVSG_API void GLEnablePolygonSmooth( void );
  friend NVSG_API void GLEnablePolygonStipple( void );
  friend NVSG_API void GLEnableScissorTest( void );
  friend NVSG_API void GLEnableStencilTest( void );
  friend NVSG_API void GLEnableSecondaryColorArray( void );
  friend NVSG_API void GLEnableTexture( GLenum target );
  friend NVSG_API void GLEnableTextureCoordArray( void );
  friend NVSG_API void GLEnableTextureGen( GLenum target );
  friend NVSG_API void GLEnableVertexArray( void );
  friend NVSG_API void GLEnd( void );
  friend NVSG_API void GLEndList( void );
  friend NVSG_API void GLEndOcclusionQuery( void );
  friend NVSG_API void GLEndQuery( GLenum target );
  friend NVSG_API void GLEvalCoord1d( GLdouble u );
  friend NVSG_API void GLEvalCoord1f( GLfloat u );
  friend NVSG_API void GLEvalCoord2d( GLdouble u, GLdouble v );
  friend NVSG_API void GLEvalCoord2f( GLfloat u, GLfloat v );
  friend NVSG_API void GLEvalMesh1( GLenum mode, GLint i1, GLint i2 );
  friend NVSG_API void GLEvalMesh2( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 );
  friend NVSG_API void GLEvalPoint1( GLint i );
  friend NVSG_API void GLEvalPoint2( GLint i, GLint j );
  friend NVSG_API void GLFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer );
  friend NVSG_API void GLFinish( void );
  friend NVSG_API void GLFlush( void );
  friend NVSG_API void GLFogColor( const GLfloat * color );
  friend NVSG_API void GLFogCoordPointer( GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLFogDensity( GLfloat density );
  friend NVSG_API void GLFogEnd( GLfloat end );
  friend NVSG_API void GLFogf( GLenum pname, GLfloat param );
  friend NVSG_API void GLFogfv( GLenum pname, const GLfloat *params );
  friend NVSG_API void GLFogi( GLenum pname, GLint param );
  friend NVSG_API void GLFogiv( GLenum pname, const GLint *params );
  friend NVSG_API void GLFogMode( GLenum param );
  friend NVSG_API void GLFogStart( GLfloat start );
  friend NVSG_API void GLFrontFace( GLenum mode );
  friend NVSG_API void GLFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
  friend NVSG_API void GLGenBuffers( GLsizei n, GLuint *buffers );
  friend NVSG_API GLuint GLGenLists( GLsizei range );
  friend NVSG_API void GLGenOcclusionQueries( GLsizei n, GLuint *ids );
  friend NVSG_API void GLGenPrograms( GLsizei n, GLuint *ids );
  friend NVSG_API void GLGenQueries( GLsizei n, GLuint *ids );
  friend NVSG_API void GLGenTextures( GLsizei n, GLuint *textures );
  friend NVSG_API void GLGetBooleanv( GLenum pname, GLboolean *params );
  friend NVSG_API void GLGetDoublev( GLenum pname, GLdouble *params );
  friend NVSG_API void GLGetFloatv( GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetIntegerv( GLenum pname, GLint *params );
  friend NVSG_API void GLGetClipPlane( GLenum plane, GLdouble *equation );
  friend NVSG_API void GLGetLightfv( GLenum light, GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetLightiv( GLenum light, GLenum pname, GLint *params );
  friend NVSG_API void GLGetMapdv( GLenum target, GLenum query, GLdouble *v );
  friend NVSG_API void GLGetMapfv( GLenum target, GLenum query, GLfloat *v );
  friend NVSG_API void GLGetMapiv( GLenum target, GLenum query, GLint *v );
  friend NVSG_API void GLGetMaterialfv( GLenum face, GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetMaterialiv( GLenum face, GLenum pname, GLint *params );
  friend NVSG_API GLint GLGetMaxTexCoords( void );
  friend NVSG_API GLint GLGetMaxTexUnits( void );
  friend NVSG_API GLint GLGetMaxTexImageUnits( void );  
  friend NVSG_API void GLGetOcclusionQueryuiv( GLuint id, GLenum pname, GLuint *params );
  friend NVSG_API void GLGetPixelMapfv( GLenum map, GLfloat *values );
  friend NVSG_API void GLGetPixelMapuiv( GLenum map, GLuint *values );
  friend NVSG_API void GLGetPixelMapusv( GLenum map, GLushort *values );
  friend NVSG_API void GLGetPointerv( GLenum pname, GLvoid* *params );
  friend NVSG_API void GLGetPolygonStipple( GLubyte *mask );
  friend NVSG_API void GLGetQueryiv( GLenum target, GLenum pname, GLint *params );
  friend NVSG_API void GLGetQueryObjectiv( GLuint id, GLenum pname, GLint *params );
  friend NVSG_API void GLGetQueryObjectuiv( GLuint id, GLenum pname, GLuint *params );
  friend NVSG_API const GLubyte * GLGetString( GLenum name );
  friend NVSG_API void GLGetTexEnvfv( GLenum target, GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetTexEnviv( GLenum target, GLenum pname, GLint *params );
  friend NVSG_API void GLGetTexGendv( GLenum coord, GLenum pname, GLdouble *params );
  friend NVSG_API void GLGetTexGenfv( GLenum coord, GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetTexGeniv( GLenum coord, GLenum pname, GLint *params );
  friend NVSG_API void GLGetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels );
  friend NVSG_API void GLGetTexLevelParameterfv( GLenum target, GLint level, GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetTexLevelParameteriv( GLenum target, GLint level, GLenum pname, GLint *params );
  friend NVSG_API void GLGetTexParameterfv( GLenum target, GLenum pname, GLfloat *params );
  friend NVSG_API void GLGetTexParameteriv( GLenum target, GLenum pname, GLint *params );
  friend NVSG_API GLint GLGetVersion();
  friend NVSG_API void GLHint( GLenum target, GLenum mode );
  friend NVSG_API void GLIndexd( GLdouble c );
  friend NVSG_API void GLIndexf( GLfloat c );
  friend NVSG_API void GLIndexi( GLint c );
  friend NVSG_API void GLIndexMask( GLuint mask );
  friend NVSG_API void GLIndexPointer( GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLIndexs( GLshort c );
  friend NVSG_API void GLIndexub( GLubyte c );
  friend NVSG_API void GLInitNames( void );
  friend NVSG_API void GLInterleavedArrays( GLenum format, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API GLboolean GLIsBuffer( GLuint name );
  friend NVSG_API GLboolean GLIsEnabled( GLenum cap );
  friend NVSG_API GLboolean GLIsEnabledAlphaTest();
  friend NVSG_API GLboolean GLIsEnabledAutoNormal();
  friend NVSG_API GLboolean GLIsEnabledBlend();
  friend NVSG_API GLboolean GLIsEnabledClipPlane( GLenum plane );
  friend NVSG_API GLboolean GLIsEnabledColorLogicOp();
  friend NVSG_API GLboolean GLIsEnabledColorMaterial();
  friend NVSG_API GLboolean GLIsEnabledCullFace();
  friend NVSG_API GLboolean GLIsEnabledDepthTest();
  friend NVSG_API GLboolean GLIsEnabledDither();
  friend NVSG_API GLboolean GLIsEnabledFog();
  friend NVSG_API GLboolean GLIsEnabledLight( GLenum cap );
  friend NVSG_API GLboolean GLIsEnabledLighting();
  friend NVSG_API GLboolean GLIsEnabledLineSmooth();
  friend NVSG_API GLboolean GLIsEnabledLineStipple();
  friend NVSG_API GLboolean GLIsEnabledLogicOp();
  friend NVSG_API GLboolean GLIsEnabledNormalize();
  friend NVSG_API GLboolean GLIsEnabledPointSmooth();
  friend NVSG_API GLboolean GLIsEnabledPolygonOffsetFill();
  friend NVSG_API GLboolean GLIsEnabledPolygonOffsetLine();
  friend NVSG_API GLboolean GLIsEnabledPolygonOffsetPoint();
  friend NVSG_API GLboolean GLIsEnabledPolygonSmooth();
  friend NVSG_API GLboolean GLIsEnabledPolygonStipple();
  friend NVSG_API GLboolean GLIsEnabledScissorTest();
  friend NVSG_API GLboolean GLIsEnabledStencilTest();
  friend NVSG_API GLboolean GLIsEnabledTexture( GLenum cap );
  friend NVSG_API GLboolean GLIsEnabledTextureGen( GLenum cap );
  friend NVSG_API GLboolean GLIsExtensionAvailable( const std::string& extension );
  friend NVSG_API GLboolean GLIsExtensionExported( const std::string& extension );
  friend NVSG_API GLboolean GLIsList( GLuint list );
  friend NVSG_API GLboolean GLIsOcclusionQuery( GLuint id );
  friend NVSG_API GLboolean GLIsProgram( GLuint id );
  friend NVSG_API GLboolean GLIsQuery( GLuint id );
  friend NVSG_API GLboolean GLIsTexture( GLuint texture );
  friend NVSG_API void GLLightAmbient( GLenum light, const GLfloat *params );
  friend NVSG_API void GLLightConstantAttenuation( GLenum light, GLfloat param );
  friend NVSG_API void GLLightDiffuse( GLenum light, const GLfloat *params );
  friend NVSG_API void GLLightf( GLenum light, GLenum pname, GLfloat param );
  friend NVSG_API void GLLightfv( GLenum light, GLenum pname, const GLfloat *params );
  friend NVSG_API void GLLighti( GLenum light, GLenum pname, GLint param );
  friend NVSG_API void GLLightiv( GLenum light, GLenum pname, GLint *params );
  friend NVSG_API void GLLightPosition( GLenum light, const GLfloat * params );
  friend NVSG_API void GLLightLinearAttenuation( GLenum light, GLfloat param );
  friend NVSG_API void GLLightQuadraticAttenuation( GLenum light, GLfloat param );
  friend NVSG_API void GLLightSpecular( GLenum light, const GLfloat *params );
  friend NVSG_API void GLLightSpotCutoff( GLenum light, GLfloat param );
  friend NVSG_API void GLLightSpotDirection( GLenum light, const GLfloat *params );
  friend NVSG_API void GLLightSpotExponent( GLenum light, GLfloat param );
  friend NVSG_API void GLLightModelf( GLenum pname, GLfloat param );
  friend NVSG_API void GLLightModelfv( GLenum pname, const GLfloat *params );
  friend NVSG_API void GLLightModeli( GLenum pname, GLint param );
  friend NVSG_API void GLLightModeliv( GLenum pname, const GLint *params );
  friend NVSG_API void GLLightModelAmbient( const GLfloat * params );
  friend NVSG_API void GLLightModelLocalViewer( GLint param );
  friend NVSG_API void GLLightModelTwoSide( GLint param );
  friend NVSG_API void GLLightModelTwoSideLock( GLint param );
  friend NVSG_API void GLLightModelTwoSideUnlock( GLint param );
  friend NVSG_API void GLLineStipple( GLint factor, GLushort pattern );
  friend NVSG_API void GLLineWidth( GLfloat width );
  friend NVSG_API void GLListBase( GLuint base );
  friend NVSG_API void GLLoadIdentity( void );
  friend NVSG_API void GLLoadMatrixd( const GLdouble *m );
  friend NVSG_API void GLLoadMatrixf( const GLfloat *m );
  friend NVSG_API void GLLoadTransposedMatrixd( const GLdouble *m );
  friend NVSG_API void GLLoadTransposeMatrixf( const GLfloat *m );
  friend NVSG_API void GLLoadName( GLuint name );
  friend NVSG_API void GLLoadProgram( GLenum target, GLuint id, GLsizei len, const GLubyte *program );
  friend NVSG_API void GLLogicOp( GLenum opcode );
  friend NVSG_API void GLMap1d( GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points );
  friend NVSG_API void GLMap1f( GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points );
  friend NVSG_API void GLMap2d( GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points );
  friend NVSG_API void GLMap2f( GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points );
  friend NVSG_API GLvoid* GLMapBuffer( GLenum target, GLenum access );
  friend NVSG_API void GLMapGrid1d( GLint un, GLdouble u1, GLdouble u2 );
  friend NVSG_API void GLMapGrid1f( GLint un, GLfloat u1, GLfloat u2 );
  friend NVSG_API void GLMapGrid2d( GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2 );
  friend NVSG_API void GLMapGrid2f( GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2 );
  friend NVSG_API void GLMaterialAmbient( GLenum face, const GLfloat *params );
  friend NVSG_API void GLMaterialAmbientBack( const GLfloat *params );
  friend NVSG_API void GLMaterialAmbientFront( const GLfloat *params );
  friend NVSG_API void GLMaterialAmbientFrontAndBack( const GLfloat *params );
  friend NVSG_API void GLMaterialDiffuse( GLenum face, const GLfloat *params );
  friend NVSG_API void GLMaterialDiffuseBack( const GLfloat *params );
  friend NVSG_API void GLMaterialDiffuseFront( const GLfloat *params );
  friend NVSG_API void GLMaterialDiffuseFrontAndBack( const GLfloat *params );
  friend NVSG_API void GLMaterialEmission( GLenum face, const GLfloat *params );
  friend NVSG_API void GLMaterialEmissionBack( const GLfloat *params );
  friend NVSG_API void GLMaterialEmissionFront( const GLfloat *params );
  friend NVSG_API void GLMaterialEmissionFrontAndBack( const GLfloat *params );
  friend NVSG_API void GLMaterialf( GLenum face, GLenum pname, GLfloat param );
  friend NVSG_API void GLMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
  friend NVSG_API void GLMateriali( GLenum face, GLenum pname, GLint param );
  friend NVSG_API void GLMaterialiv( GLenum face, GLenum pname, const GLint *params );
  friend NVSG_API void GLMaterialShininess( GLenum face, GLfloat param );
  friend NVSG_API void GLMaterialShininessBack( GLfloat param );
  friend NVSG_API void GLMaterialShininessFront( GLfloat param );
  friend NVSG_API void GLMaterialShininessFrontAndBack( GLfloat param );
  friend NVSG_API void GLMaterialSpecular( GLenum face, const GLfloat *params );
  friend NVSG_API void GLMaterialSpecularBack( const GLfloat *params );
  friend NVSG_API void GLMaterialSpecularFront( const GLfloat *params );
  friend NVSG_API void GLMaterialSpecularFrontAndBack( const GLfloat *params );
  friend NVSG_API void GLMatrixMode( GLenum mode );
  friend NVSG_API void GLMultiDrawElements( GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount );
  friend NVSG_API void GLMultiTexCoord2f( GLenum texture, GLfloat tx, GLfloat ty );
  friend NVSG_API void GLMultMatrixd( const GLdouble *m );
  friend NVSG_API void GLMultMatrixf( const GLfloat *m );
  friend NVSG_API void GLMultTransposeMatrixd( const GLdouble *m );
  friend NVSG_API void GLMultTransposeMatrixf( const GLfloat *m );
  friend NVSG_API void GLNewList( GLuint list, GLenum mode );
  friend NVSG_API void GLNormal3b( GLbyte nx, GLbyte ny, GLbyte nz );
  friend NVSG_API void GLNormal3bv( const GLbyte *v );
  friend NVSG_API void GLNormal3d( GLdouble nx, GLdouble ny, GLdouble nz );
  friend NVSG_API void GLNormal3dv( const GLdouble *v );
  friend NVSG_API void GLNormal3f( GLfloat nx, GLfloat ny, GLfloat nz );
  friend NVSG_API void GLNormal3fv( const GLfloat *v );
  friend NVSG_API void GLNormal3i( GLint nx, GLint ny, GLint nz );
  friend NVSG_API void GLNormal3iv( const GLint *v );
  friend NVSG_API void GLNormal3s( GLshort nx, GLshort ny, GLshort nz );
  friend NVSG_API void GLNormal3sv( const GLshort *v );
  friend NVSG_API void GLNormalPointer( GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
  friend NVSG_API void GLPassThrough( GLfloat token );
  friend NVSG_API void GLPixelMapfv( GLenum map, GLsizei mapsize, const GLfloat *values );
  friend NVSG_API void GLPixelMapuiv( GLenum map, GLsizei mapsize, const GLuint *values );
  friend NVSG_API void GLPixelMapusv( GLenum map, GLsizei mapsize, const GLushort *values );
  friend NVSG_API void GLPixelStoref( GLenum pname, GLfloat param );
  friend NVSG_API void GLPixelStorei( GLenum pname, GLint param );
  friend NVSG_API void GLPixelTransferf( GLenum pname, GLfloat param );
  friend NVSG_API void GLPixelTransferi( GLenum pname, GLint param );
  friend NVSG_API void GLPixelZoom( GLfloat xfactor, GLfloat yfactor );
  friend NVSG_API void GLPointSize( GLfloat size );
  friend NVSG_API void GLPolygonMode( GLenum face, GLenum mode );
  friend NVSG_API void GLPolygonModeBack( GLenum mode );
  friend NVSG_API void GLPolygonModeFront( GLenum mode );
  friend NVSG_API void GLPolygonModeFrontAndBack( GLenum mode );
  friend NVSG_API void GLPolygonModeLock( GLenum face, GLenum mode );
  friend NVSG_API void GLPolygonModeUnlock( GLenum face, GLenum mode );
  friend NVSG_API void GLPolygonOffset( GLfloat factor, GLfloat units );
  friend NVSG_API void GLPolygonStipple( const GLubyte *mask );
  friend NVSG_API void GLPopAttrib( void );
  friend NVSG_API void GLPopClientAttrib( void );
  friend NVSG_API void GLPopMatrix( void );
  friend NVSG_API void GLPopName( void );
  friend NVSG_API void GLPrimitiveRestartIndex( GLuint index );
  friend NVSG_API void GLPrioritizeTextures( GLsizei n, const GLuint *textures, const GLclampf *priorities );
  friend NVSG_API void GLProgramNamedParameter4f( GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  friend NVSG_API void GLPushAttrib( GLbitfield mask );
  friend NVSG_API void GLPushClientAttrib( GLbitfield mask );
  friend NVSG_API void GLPushMatrix( void );
  friend NVSG_API void GLPushName( GLuint name );
  friend NVSG_API void GLRasterPos2d( GLdouble x, GLdouble y );
  friend NVSG_API void GLRasterPos2f( GLfloat x, GLfloat y );
  friend NVSG_API void GLRasterPos2i( GLint x, GLint y );
  friend NVSG_API void GLRasterPos2s( GLshort x, GLshort y );
  friend NVSG_API void GLRasterPos3d( GLdouble x, GLdouble y, GLdouble z );
  friend NVSG_API void GLRasterPos3f( GLfloat x, GLfloat y, GLfloat z );
  friend NVSG_API void GLRasterPos3i( GLint x, GLint y, GLint z );
  friend NVSG_API void GLRasterPos3s( GLshort x, GLshort y, GLshort z );
  friend NVSG_API void GLRasterPos4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
  friend NVSG_API void GLRasterPos4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  friend NVSG_API void GLRasterPos4i( GLint x, GLint y, GLint z, GLint w );
  friend NVSG_API void GLRasterPos4s( GLshort x, GLshort y, GLshort z, GLshort w );
  friend NVSG_API void GLReadBuffer( GLenum mode );
  friend NVSG_API void GLReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels );
  friend NVSG_API void GLRectd( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2 );
  friend NVSG_API void GLRectf( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
  friend NVSG_API void GLRecti( GLint x1, GLint y1, GLint x2, GLint y2 );
  friend NVSG_API void GLRects( GLshort x1, GLshort y1, GLshort x2, GLshort y2 );
  friend NVSG_API void GLRenderMode( GLenum mode );
  friend NVSG_API void GLRotated( GLdouble angle, GLdouble x, GLdouble y, GLdouble z );
  friend NVSG_API void GLRotatef( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
  friend NVSG_API void GLScaled( GLdouble x, GLdouble y, GLdouble z );
  friend NVSG_API void GLScalef( GLfloat x, GLfloat y, GLfloat z );
  friend NVSG_API void GLScissor( GLint x, GLint y, GLsizei width, GLsizei height );
  friend NVSG_API void GLSecondaryColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLSelectBuffer( GLsizei size, GLuint *buffer );
  friend NVSG_API void GLSetWindowStereoMode( GLboolean displayMode );
  friend NVSG_API void GLShadeModel( GLenum mode );
  friend NVSG_API void GLStencilFunc( GLenum func, GLint ref, GLuint mask );
  friend NVSG_API void GLStencilMask( GLuint mask );
  friend NVSG_API void GLStencilOp( GLenum fail, GLenum zfail, GLenum zpass );
  friend NVSG_API void GLTexCoord1d( GLdouble s );
  friend NVSG_API void GLTexCoord1f( GLfloat s );
  friend NVSG_API void GLTexCoord1i( GLint s );
  friend NVSG_API void GLTexCoord1s( GLshort s );
  friend NVSG_API void GLTexCoord2d( GLdouble s, GLdouble t );
  friend NVSG_API void GLTexCoord2f( GLfloat s, GLfloat t );
  friend NVSG_API void GLTexCoord2fv( const GLfloat *v );
  friend NVSG_API void GLTexCoord2i( GLint s, GLint t );
  friend NVSG_API void GLTexCoord2s( GLshort s, GLshort t );
  friend NVSG_API void GLTexCoord3d( GLdouble s, GLdouble t, GLdouble r );
  friend NVSG_API void GLTexCoord3f( GLfloat s, GLfloat t, GLfloat r );
  friend NVSG_API void GLTexCoord3i( GLint s, GLint t, GLint r );
  friend NVSG_API void GLTexCoord3s( GLshort s, GLshort t, GLshort r );
  friend NVSG_API void GLTexCoord4d( GLdouble s, GLdouble t, GLdouble r, GLdouble q );
  friend NVSG_API void GLTexCoord4f( GLfloat s, GLfloat t, GLfloat r, GLfloat q );
  friend NVSG_API void GLTexCoord4i( GLint s, GLint t, GLint r, GLint q );
  friend NVSG_API void GLTexCoord4s( GLshort s, GLshort t, GLshort r, GLshort q );
  friend NVSG_API void GLTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLTexEnvf( GLenum target, GLenum pname, GLfloat param );
  friend NVSG_API void GLTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
  friend NVSG_API void GLTexEnvi( GLenum target, GLenum pname, GLint param );
  friend NVSG_API void GLTexEnviv( GLenum target, GLenum pname, const GLint *params );
  friend NVSG_API void GLTexGend( GLenum coord, GLenum pname, GLdouble param );
  friend NVSG_API void GLTexGenf( GLenum coord, GLenum pname, GLfloat param );
  friend NVSG_API void GLTexGenfv( GLenum coord, GLenum pname, const GLfloat *param );
  friend NVSG_API void GLTexGeni( GLenum coord, GLenum pname, GLint param );
  friend NVSG_API void GLTexImage1D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
  friend NVSG_API void GLTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
  friend NVSG_API void GLTexImage3D( GLenum target, int level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, int border, GLenum format, GLenum type, const void* pixels);
  friend NVSG_API void GLTexParameterf( GLenum target, GLenum pname, GLfloat param );
  friend NVSG_API void GLTexParameteri( GLenum target, GLenum pname, GLint param );
  friend NVSG_API void GLTexParameterfv( GLenum target, GLenum pname, const GLfloat *params );
  friend NVSG_API void GLTexParameteriv( GLenum target, GLenum pname, const GLint *params );
  friend NVSG_API void GLTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels );
  friend NVSG_API void GLTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );
  friend NVSG_API void GLTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels );
  friend NVSG_API void GLTextureBorderColor( GLenum target, const GLfloat *params );
  friend NVSG_API void GLTextureEnvColor( const GLfloat * params );
  friend NVSG_API void GLTextureEnvMode( GLint param );
  friend NVSG_API void GLTextureMagFilter( GLenum target, GLint param );
  friend NVSG_API void GLTextureMinFilter( GLenum target, GLint param );
  friend NVSG_API void GLTextureWrapR( GLenum target, GLint param );
  friend NVSG_API void GLTextureWrapS( GLenum target, GLint param );
  friend NVSG_API void GLTextureWrapT( GLenum target, GLint param );
  friend NVSG_API void GLTranslated( GLdouble x, GLdouble y, GLdouble z );
  friend NVSG_API void GLTranslatef( GLfloat x, GLfloat y, GLfloat z );
  friend NVSG_API GLboolean GLUnmapBuffer( GLenum target );
  friend NVSG_API void GLVertex2d( GLdouble x, GLdouble y );
  friend NVSG_API void GLVertex2f( GLfloat x, GLfloat y );
  friend NVSG_API void GLVertex2i( GLint x, GLint y );
  friend NVSG_API void GLVertex2s( GLshort x, GLshort y );
  friend NVSG_API void GLVertex3d( GLdouble x, GLdouble y, GLdouble z );
  friend NVSG_API void GLVertex3f( GLfloat x, GLfloat y, GLfloat z );
  friend NVSG_API void GLVertex3fv( const GLfloat *v );
  friend NVSG_API void GLVertex3i( GLint x, GLint y, GLint z );
  friend NVSG_API void GLVertex3s( GLshort x, GLshort y, GLshort z );
  friend NVSG_API void GLVertex4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
  friend NVSG_API void GLVertex4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  friend NVSG_API void GLVertex4i( GLint x, GLint y, GLint z, GLint w );
  friend NVSG_API void GLVertex4s( GLshort x, GLshort y, GLshort z, GLshort w );
  friend NVSG_API void GLVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  friend NVSG_API void GLViewport( GLint x, GLint y, GLsizei width, GLsizei height );
  // EXT_framebuffer_object API
  friend NVSG_API GLboolean GLIsRenderbuffer(GLuint renderbuffer);
  friend NVSG_API void GLBindRenderbuffer(GLenum target, GLuint renderbuffer);
  friend NVSG_API void GLDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
  friend NVSG_API void GLGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
  friend NVSG_API void GLRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
  friend NVSG_API void GLGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params);
  friend NVSG_API GLboolean GLIsFramebuffer(GLuint framebuffer);
  friend NVSG_API void GLBindFramebuffer(GLenum target, GLuint framebuffer);
  friend NVSG_API void GLDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
  friend NVSG_API void GLGenFramebuffers(GLsizei n, GLuint *framebuffers);
  friend NVSG_API GLenum GLCheckFramebufferStatus(GLenum target);
  friend NVSG_API void GLFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
  friend NVSG_API void GLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
  friend NVSG_API void GLFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
  friend NVSG_API void GLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
  friend NVSG_API void GLGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params);
  friend NVSG_API void GLGenerateMipmap(GLenum target);
#if defined(_WIN32)
  friend NVSG_API BOOL WGLBindTexImage( HPBUFFERARB hPbuffer, int iBuffer );
  friend NVSG_API BOOL WGLChoosePixelFormat( HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats );
  friend NVSG_API HGLRC WGLCreateContext( HDC hdc );
  friend NVSG_API HPBUFFERARB WGLCreatePbuffer( HDC hdc, int pixelFormat, int width, int height, const int *attribList );
  friend NVSG_API BOOL WGLDeleteContext( HGLRC hglrc );
  friend NVSG_API BOOL WGLDestroyPbuffer( HPBUFFERARB hPbuffer );
  friend NVSG_API HGLRC WGLGetCurrentContext(void);
  friend NVSG_API HDC WGLGetCurrentDC(void);
  friend NVSG_API BOOL WGLMakeCurrent( HDC hdc, HGLRC hglrc );
  friend NVSG_API HDC WGLGetPbufferDC( HPBUFFERARB hPbuffer );
  friend NVSG_API BOOL WGLQueryPbuffer( HPBUFFERARB hPbuffer, int attribute, int *value );
  friend NVSG_API int WGLReleasePbufferDC( HPBUFFERARB hPbuffer, HDC hdc );
  friend NVSG_API BOOL WGLReleaseTexImage( HPBUFFERARB hPbuffer, int iBuffer );
  friend NVSG_API BOOL WGLShareLists( HGLRC hglrc1, HGLRC hglrc2 );
  friend NVSG_API BOOL WGLJoinSwapGroupNV(HDC hDC, GLuint group);
  friend NVSG_API BOOL WGLBindSwapBarrierNV(GLuint group, GLuint barrier);
  friend NVSG_API BOOL WGLQuerySwapGroupNV(HDC hDC, GLuint * group, GLuint * barrier);
  friend NVSG_API BOOL WGLQueryMaxSwapGroupsNV(HDC hDC, GLuint * maxGroups, GLuint * maxBarriers);
  friend NVSG_API BOOL WGLQueryFrameCountNV(HDC hDC, GLuint * count);
  friend NVSG_API BOOL WGLResetFrameCountNV(HDC hDC);
  friend NVSG_API BOOL WGLEnumGpus(UINT iIndex, HGPUNV *hGpu);
  friend NVSG_API BOOL WGLEnumGpuDevices(HGPUNV hGpu, UINT iIndex, PGPU_DEVICE pGpuDevice);
  friend NVSG_API HDC  WGLCreateAffinityDC(const HGPUNV *pGpuList);
  friend NVSG_API BOOL WGLEnumGpusFromAffinityDC(HDC hAffinityDC, UINT iIndex, HGPUNV *hGpu);
  friend NVSG_API BOOL WGLDeleteDC(HDC hAffinityDC);
  // routines to handle bypassing of WGLCreateContext and WGLDeleteContext  
  friend NVSG_API void WGLAttachContext(HDC, HGLRC);
  friend NVSG_API void WGLDetachContext(HGLRC);
#endif
#if defined(LINUX)
  friend NVSG_API GLXContext GLXCreateContext(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
  friend NVSG_API void GLXDestroyContext(Display *dpy, GLXContext ctx);
  friend NVSG_API GLXContext GLXGetCurrentContext();
  friend NVSG_API GLXDrawable GLXGetCurrentDrawable();
  friend NVSG_API Bool GLXMakeCurrent(Display *dpy, GLXDrawable drawable, GLXContext ctx);
  // routines to handle bypassing of GLXCreateContext and GLXDestroyContext  
  friend NVSG_API void GLXAttachContext(Display*, GLXContext);
  friend NVSG_API void GLXDetachContext(GLXContext);
#endif
  friend NVSG_API void GLResetAlphaFunc();
  friend NVSG_API void GLResetAlphaTest();
  friend NVSG_API void GLResetAutoNormal();
  friend NVSG_API void GLResetBlend();
  friend NVSG_API void GLResetBlendFunc();
  friend NVSG_API void GLResetBlendFuncSeparate();
  friend NVSG_API void GLResetClearColor();
  friend NVSG_API void GLResetClearDepth();
  friend NVSG_API void GLResetClearStencil();
  friend NVSG_API void GLResetClipPlane( GLenum plane );
  friend NVSG_API void GLResetClipPlaneEquation( GLenum plane );
  friend NVSG_API void GLResetColorLogicOp();
  friend NVSG_API void GLResetColorMask();
  friend NVSG_API void GLResetColorMaterial();
  friend NVSG_API void GLResetColorMaterialMode();
  friend NVSG_API void GLResetCullFace();
  friend NVSG_API void GLResetCullFaceMode();
  friend NVSG_API void GLResetDepthFunc();
  friend NVSG_API void GLResetDepthMask();
  friend NVSG_API void GLResetDepthRange();
  friend NVSG_API void GLResetDepthTest();
  friend NVSG_API void GLResetDither();
  friend NVSG_API void GLResetFog();
  friend NVSG_API void GLResetFogColor();
  friend NVSG_API void GLResetFogDensity();
  friend NVSG_API void GLResetFogEnd();
  friend NVSG_API void GLResetFogMode();
  friend NVSG_API void GLResetFogStart();
  friend NVSG_API void GLResetFrontFace();
  friend NVSG_API void GLResetLight( GLenum light );
  friend NVSG_API void GLResetLighting();
  friend NVSG_API void GLResetLightAmbient( GLenum light );
  friend NVSG_API void GLResetLightConstantAttenuation( GLenum light );
  friend NVSG_API void GLResetLightDiffuse( GLenum light );
  friend NVSG_API void GLResetLightLinearAttenuation( GLenum light );
  friend NVSG_API void GLResetLightModelAmbient();
  friend NVSG_API void GLResetLightModelLocalViewer();
  friend NVSG_API void GLResetLightModelTwoSide();
  friend NVSG_API void GLResetLightPosition( GLenum light );
  friend NVSG_API void GLResetLightQuadraticAttenuation( GLenum light );
  friend NVSG_API void GLResetLightSpecular( GLenum light );
  friend NVSG_API void GLResetLightSpotCutoff( GLenum light );
  friend NVSG_API void GLResetLightSpotDirection( GLenum light );
  friend NVSG_API void GLResetLightSpotExponent( GLenum light );
  friend NVSG_API void GLResetLineSmooth();
  friend NVSG_API void GLResetLineStipple();
  friend NVSG_API void GLResetLineStippleDefinition();
  friend NVSG_API void GLResetLineWidth();
  friend NVSG_API void GLResetLogicOp();
  friend NVSG_API void GLResetMaterialAmbientBack();
  friend NVSG_API void GLResetMaterialAmbientFront();
  friend NVSG_API void GLResetMaterialAmbientFrontAndBack();
  friend NVSG_API void GLResetMaterialDiffuseBack();
  friend NVSG_API void GLResetMaterialDiffuseFront();
  friend NVSG_API void GLResetMaterialDiffuseFrontAndBack();
  friend NVSG_API void GLResetMaterialEmissionBack();
  friend NVSG_API void GLResetMaterialEmissionFront();
  friend NVSG_API void GLResetMaterialEmissionFrontAndBack();
  friend NVSG_API void GLResetMaterialShininessBack();
  friend NVSG_API void GLResetMaterialShininessFront();
  friend NVSG_API void GLResetMaterialShininessFrontAndBack();
  friend NVSG_API void GLResetMaterialSpecularBack();
  friend NVSG_API void GLResetMaterialSpecularFront();
  friend NVSG_API void GLResetMaterialSpecularFrontAndBack();
  friend NVSG_API void GLResetNormalize();
  friend NVSG_API void GLResetPointSize();
  friend NVSG_API void GLResetPointSmooth();
  friend NVSG_API void GLResetPolygonModeBack();
  friend NVSG_API void GLResetPolygonModeFront();
  friend NVSG_API void GLResetPolygonModeFrontAndBack();
  friend NVSG_API void GLResetPolygonOffset();
  friend NVSG_API void GLResetPolygonOffsetFill();
  friend NVSG_API void GLResetPolygonOffsetLine();
  friend NVSG_API void GLResetPolygonOffsetPoint();
  friend NVSG_API void GLResetPolygonSmooth();
  friend NVSG_API void GLResetPolygonStipple();
  friend NVSG_API void GLResetScissor();
  friend NVSG_API void GLResetScissorTest();
  friend NVSG_API void GLResetShadeModel();
  friend NVSG_API void GLResetStencilFunc();
  friend NVSG_API void GLResetStencilMask();
  friend NVSG_API void GLResetStencilOp();
  friend NVSG_API void GLResetStencilTest();
  friend NVSG_API void GLResetTexture( GLenum target );

#if !defined(DOXYGEN_IGNORE)

  //  uncached versions of otherwise cached functions
  friend NVSG_API void _GLDisable( GLenum cap );
  friend NVSG_API void _GLDisableClientState( GLenum array );
  friend NVSG_API void _GLEnable( GLenum cap );
  friend NVSG_API void _GLEnableClientState( GLenum array );
  friend NVSG_API void _GLFogf( GLenum pname, GLfloat param );
  friend NVSG_API void _GLFogfv( GLenum pname, const GLfloat * params );
  friend NVSG_API void _GLFogi( GLenum pname, GLint param );
  friend NVSG_API void _GLFogiv( GLenum pname, const GLint * params );
  friend NVSG_API GLboolean _GLIsEnabled( GLenum cap );
  friend NVSG_API void _GLLightf( GLenum light, GLenum pname, GLfloat param );
  friend NVSG_API void _GLLightfv( GLenum light, GLenum pname, const GLfloat *params );
  friend NVSG_API void _GLLighti( GLenum light, GLenum pname, GLint param );
  friend NVSG_API void _GLLightiv( GLenum light, GLenum pname, GLint *params );
  friend NVSG_API void _GLLightModelf( GLenum pname, GLfloat param );
  friend NVSG_API void _GLLightModelfv( GLenum pname, const GLfloat *params );
  friend NVSG_API void _GLLightModeli( GLenum pname, GLint param );
  friend NVSG_API void _GLLightModeliv( GLenum pname, const GLint *params );
  friend NVSG_API void _GLMaterialf( GLenum face, GLenum pname, GLfloat param );
  friend NVSG_API void _GLMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
  friend NVSG_API void _GLMateriali( GLenum face, GLenum pname, GLint param );
  friend NVSG_API void _GLMaterialiv( GLenum face, GLenum pname, const GLint *params );
  friend NVSG_API void _GLPolygonMode( GLenum face, GLenum mode );
  friend NVSG_API void _GLTexEnvf( GLenum target, GLenum pname, GLfloat param );
  friend NVSG_API void _GLTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
  friend NVSG_API void _GLTexEnvi( GLenum target, GLenum pname, GLint param );
  friend NVSG_API void _GLTexEnviv( GLenum target, GLenum pname, const GLint *params );
  friend NVSG_API void _GLTexParameterf( GLenum target, GLenum pname, GLfloat param );
  friend NVSG_API void _GLTexParameteri( GLenum target, GLenum pname, GLint param );
  friend NVSG_API void _GLTexParameterfv( GLenum target, GLenum pname, const GLfloat *params );
  friend NVSG_API void _GLTexParameteriv( GLenum target, GLenum pname, const GLint *params );

#if defined( GL_CACHING )
  friend NVSG_API void _GLUpdateActiveTexture( void );
  friend NVSG_API void _GLUpdateClientActiveTexture( void );
#endif

#if !defined(NDEBUG)
  friend NVSG_API void ASSERT_GL_CURRENT_CONTEXT_CONFORMITY();
  friend NVSG_API void ASSERT_GL_ARRAY_BUFFER_BINDING();
  friend NVSG_API void ASSERT_GL_ELEMENT_ARRAY_BUFFER_BINDING();
  friend NVSG_API void ASSERT_GL_PIXEL_PACK_BUFFER_BINDING();
  friend NVSG_API void ASSERT_GL_PIXEL_UNPACK_BUFFER_BINDING();
#endif

#endif

  friend NVSG_API void GLGenDisplayListCache(const nvsg::DALHost * host, GLDisplayListCache *& cache);
  friend NVSG_API void GLDeleteDisplayListCaches();
  friend NVSG_API void GLGenArrayBufferCache(const nvsg::DALHost * host, GLArrayBufferCache *& cache);
  friend NVSG_API void GLGenElementArrayBufferCache(const nvsg::DALHost * host, GLElementArrayBufferCache *& cache);
  friend NVSG_API void GLGenPixelBufferCache(const nvsg::DALHost * host, GLPixelBufferCache *& cache);
  friend NVSG_API void GLDeleteBufferCaches();
  friend NVSG_API void GLGenTextureCache(const nvsg::DALHost * host, GLTextureCache *& cache);
  friend NVSG_API void GLGenTextureCache(const nvsg::DALHost * host, GLTextureCache *& cache, GLuint texture, GLenum target);
  friend NVSG_API void GLDeleteTextureCaches();

  /*! \brief Returns a GLTexImageFmt according to the passed in PixelFormat and PixelDataType descriptors. 
   *
   *  \param fmt Input PixelFormat descriptor of an nvsg::Image
   *  \param type Input PixelDataType descriptor of an nvsg::Image
   *
   *  \remarks 
   *  The function translates the passed in NVSG specific PixelFormat and PixelDataType descriptors
   *  into an OpenGL specific texture image format descriptor. Thereby, the function always tries to 
   *  return the 'best match' of a texture image format which is supported by the current hardware 
   *  and driver configuration.
   *
   *  \returns 
   *  The function returns a pointer to a GLTexImageFmt structure representing the best match of an
   *  OpenGL texture image format according to the input PixelFormat and PixelDataType descriptors.
   *  The function returns NULL if no corresponding format could be found.
   */
  friend NVSG_API const GLTexImageFmt* GLGetTexImageFmt(nvsg::Image::PixelFormat fmt, nvsg::Image::PixelDataType type);

  friend NVSG_API void GLUploadTexImage1D(GLint intFmt, GLint usrFmt, GLint type, nvsg::TextureImage*, GLTextureCache*);
  friend NVSG_API void GLUploadTexImage2D(GLint intFmt, GLint usrFmt, GLint type, nvsg::TextureImage*, GLTextureCache*);
  friend NVSG_API void GLUploadTexImage3D(GLint intFmt, GLint usrFmt, GLint type, nvsg::TextureImage*, GLTextureCache*);
  friend NVSG_API void GLUploadTexImageCube(GLint intFmt, GLint usrFmt, GLint type, nvsg::TextureImage*, GLTextureCache*);

public:  // public interface
#if defined(LINUX)
  /*! \brief Get the Display of this GLContext (Linux only).
   *  \return The pointer to the Display of this GLContext. */
  inline Display* getDisplay() { return m_dpy; }  // public access to the Display
#endif 

private: // private interface

#if defined(_WIN32)
  GLContext(HDC hdc, HGLRC hglrc);  
#elif defined(LINUX)
  GLContext(Display *dpy, GLXContext ctx);  
#endif
  ~GLContext();

#if defined(_WIN32)
  void init(); // init function pointers and caches
#elif defined(LINUX)
  void init(GLXDrawable drw);
#endif
  void exit(); // cleanup when context dies

  void deleteGLObjects(); // deletes all GLObjects
  void deleteSharedGLObjects(); // deletes only shared GLObjects

  // Share sharable GL objects with the GLContext referenced through \a source.
  void share(const GLContext& source);

  // helper

  void addBuffers(GLsizei n, const GLuint * names);
  void removeBuffers(GLsizei n, const GLuint * names);
  ConstNameSetIter firstBuffer() const;
  ConstNameSetIter lastBuffer() const;
  bool isBuffer(GLuint name) const;
  void releaseBuffers();

  void addLists(GLsizei n, GLuint first);
  void removeLists(GLsizei n, GLuint first);
  ConstNameSetIter firstList() const;
  ConstNameSetIter lastList() const;
  bool isList(GLuint name) const;
  void releaseLists();

  void addPrograms(GLsizei n, const GLuint * names);
  void removePrograms(GLsizei n, const GLuint * names);
  ConstNameSetIter firstProgram() const;
  ConstNameSetIter lastProgram() const;
  bool isProgram(GLuint name) const;
  void releasePrograms();

  void addTextures(GLsizei n, const GLuint * names);
  void removeTextures(GLsizei n, const GLuint * names);
  ConstNameSetIter firstTexture() const;
  ConstNameSetIter lastTexture() const;
  bool isTexture(GLuint name) const;
  void releaseTextures();

  void addOcclusionQueries(GLsizei n, const GLuint * names);
  void removeOcclusionQueries(GLsizei n, const GLuint * names);
  ConstNameSetIter firstOcclusionQuery() const;
  ConstNameSetIter lastOcclusionQuery() const;
  bool isOcclusionQuery(GLuint name) const;
  void releaseOcclusionQueries();

  void addQueries(GLsizei n, const GLuint * names);
  void removeQueries(GLsizei n, const GLuint * names);
  ConstNameSetIter firstQuery() const;
  ConstNameSetIter lastQuery() const;
  bool isQuery(GLuint name) const;
  void releaseQueries();

  void genTextureCache(const nvsg::DALHost * host, GLTextureCache *& cache);
  void genTextureCache(const nvsg::DALHost * host, GLTextureCache *& cache, GLuint texture, GLenum target);
  void deleteTextureCaches();
  void genDisplayListCache(const nvsg::DALHost * host, GLDisplayListCache *& cache);
  void deleteDisplayListCaches();
  void genElementArrayBufferCache(const nvsg::DALHost * host, GLElementArrayBufferCache *& cache);
  void genArrayBufferCache(const nvsg::DALHost * host, GLArrayBufferCache *& cache);
  void genPixelBufferCache(const nvsg::DALHost * host, GLPixelBufferCache *& cache);
  void deleteBufferCaches();


private:  //  private members
#if defined( GL_CACHING )
  class Vec4d
  {
    public:
      Vec4d()
      {
        m_vec[0] = 0.0;
        m_vec[1] = 0.0;
        m_vec[2] = 0.0;
        m_vec[3] = 0.0;
      }
      Vec4d(double x, double y, double z, double w )
      {
        m_vec[0] = x;
        m_vec[1] = y;
        m_vec[2] = z;
        m_vec[3] = w;
      }
      double& operator[](size_t i) { 
        NVSG_ASSERT(i>=0 && i<4); return m_vec[i]; 
      }
      const double& operator[](size_t i) const { 
        NVSG_ASSERT(i>=0 && i<4); return m_vec[i]; 
      }

    private:
      double m_vec[4];
  };

  // helper struct to enable a GLint[2] to be stored in a STL container
  template <int I>
  struct BufferBinding
  {
    GLint& operator[] (int i) { NVSG_ASSERT(0<=i && i<I); return m_bindings[i]; }
    const GLint& operator[] (int i) const { NVSG_ASSERT(0<=i && i<I); return m_bindings[i]; }
    GLint m_bindings[I];
  };
  // and more expressive typedefs
  typedef BufferBinding<NVSG_NUM_VERTEX_BUFFERS> VertexBufferBinding;
  typedef BufferBinding<NVSG_NUM_PIXEL_BUFFERS> PixelBufferBinding;

  class BufferBindingAttributes
  {
  public:

    BufferBindingAttributes() { 
      // init bindings to non-specified
      memset(m_bindings, 0, NVSG_NUM_BUFFERS * sizeof(GLint)); 
    }

    // map GL buffer target to zero-based
    GLint& operator[] (GLenum target) {
      NVSG_ASSERT(  target==GL_ARRAY_BUFFER_ARB 
                 || target==GL_ELEMENT_ARRAY_BUFFER_ARB 
                 || target==GL_PIXEL_PACK_BUFFER_ARB 
                 || target==GL_PIXEL_UNPACK_BUFFER_ARB );
      switch ( target )
      {
      case GL_ARRAY_BUFFER_ARB:         return m_bindings[NVSG_ARRAY_BUFFER];
      case GL_ELEMENT_ARRAY_BUFFER_ARB: return m_bindings[NVSG_ELEMENT_ARRAY_BUFFER];
      case GL_PIXEL_PACK_BUFFER_ARB:    return m_bindings[NVSG_PIXEL_PACK_BUFFER];
      case GL_PIXEL_UNPACK_BUFFER_ARB:  return m_bindings[NVSG_PIXEL_UNPACK_BUFFER];
      default : NVSG_ASSERT(!"unknown buffer target"); // undefined behavior if this fires!
      }
      return m_bindings[NVSG_ARRAY_BUFFER]; // return array buffer binding in case of error?!
                                       // don't know better
    }
     
    void setVertexBufferBinding(const VertexBufferBinding& binding) {
      m_bindings[NVSG_ARRAY_BUFFER] = binding[0];
      m_bindings[NVSG_ELEMENT_ARRAY_BUFFER] = binding[1];
    }

    const VertexBufferBinding& getVertexBufferBinding() const {
      return *((VertexBufferBinding *)&m_bindings[NVSG_FIRST_VERTEX_BUFFER]);
    } 

    void setPixelBufferBinding(const PixelBufferBinding& binding) {
      m_bindings[NVSG_PIXEL_PACK_BUFFER] = binding[0];
      m_bindings[NVSG_PIXEL_UNPACK_BUFFER] = binding[1];
    }

    const PixelBufferBinding& getPixelBufferBinding() const {
      return *((PixelBufferBinding *)&m_bindings[NVSG_FIRST_PIXEL_BUFFER]);
    } 

  private:
    GLint m_bindings[NVSG_NUM_BUFFERS];
  };

  class ColorBufferAttributes
  {
    public:
      ColorBufferAttributes( void )
        : m_alphaFunc(GL_ALWAYS)
        , m_alphaRef(0.0f)
        , m_blendSrcRGB(GL_ONE)
        , m_blendSrcAlpha(GL_ONE)
        , m_blendDstRGB(GL_ZERO)
        , m_blendDstAlpha(GL_ZERO)
        , m_clearColor(0.0f,0.0f,0.0f,0.0f)
      {
        for ( size_t i=0 ; i<4 ; i++ )
        {
          m_colorWriteMask[i] = GL_TRUE;
        }
      }

    public:
      GLenum        m_alphaFunc;
      GLclampf      m_alphaRef;
      GLboolean     m_colorWriteMask[4];
      GLenum        m_blendSrcRGB;
      GLenum        m_blendSrcAlpha;
      GLenum        m_blendDstRGB;
      GLenum        m_blendDstAlpha;
      nvmath::Vec4f m_clearColor;
  };

  class ColorBufferEnableAttributes
  {
    public:
      ColorBufferEnableAttributes( void )
        : m_alphaTest(GL_FALSE)
        , m_blend(GL_FALSE)
        , m_colorLogicOp(GL_FALSE)
        , m_dither(GL_TRUE)
        , m_logicOp(GL_FALSE)
      {}

    public:
      GLboolean m_alphaTest;
      GLboolean m_blend;
      GLboolean m_colorLogicOp;
      GLboolean m_dither;
      GLboolean m_logicOp;
  };

  class DepthBufferAttributes
  {
    public:
      DepthBufferAttributes( void )
        : m_depthClearValue(1.0f)
        , m_depthFunc(GL_LESS)
        , m_depthWriting(GL_TRUE)
      {}

    public:
      GLclampd  m_depthClearValue;
      GLenum    m_depthFunc;
      GLboolean m_depthWriting;
  };

  class DepthBufferEnableAttributes
  {
    public:
      DepthBufferEnableAttributes( void )
        : m_depthTesting(GL_FALSE)
      {}

    public:
      GLboolean m_depthTesting;
  };

  class EvalAttributes
  {
    public:
      EvalAttributes( void )
        : m_autoNormal(GL_FALSE)
      {}

    public:
      GLboolean m_autoNormal;
  };

  class FogAttributes
  {
    public:
      FogAttributes()
        : m_color(0.0f,0.0f,0.0f,0.0f)
        , m_density(1.0f)
        , m_end(1.0f)
        , m_mode(GL_EXP)
        , m_start(0.0f)
      {}

    public:
      nvmath::Vec4f m_color;
      GLfloat       m_density;
      GLfloat       m_end;
      GLenum        m_mode;
      GLfloat       m_start;
  };

  class FogEnableAttributes
  {
    public:
      FogEnableAttributes( void )
        : m_fog(GL_FALSE)
      {}

    public:
      GLboolean m_fog;
  };

  class LightParameters
  {
    public:
      LightParameters( void )
        : m_ambient(0.0f,0.0f,0.0f,1.0f)
        , m_attenuation(1.0f,0.0f,0.0f)
        , m_diffuse(0.0f,0.0f,0.0f,1.0f)
        , m_position(0.0f,0.0f,1.0f,0.0f)
        , m_specular(0.0f,0.0f,0.0f,1.0f)
        , m_spotCutoff(180.0f)
        , m_spotDirection(0.0f,0.0f,-1.0f,0.0f)
        , m_spotExponent(0.0f)
      {}

    public:
      nvmath::Vec4f m_ambient;
      nvmath::Vec3f m_attenuation;
      nvmath::Vec4f m_diffuse;
      nvmath::Vec4f m_position;
      nvmath::Vec4f m_specular;
      float         m_spotCutoff;
      nvmath::Vec4f m_spotDirection;
      float         m_spotExponent;
  };

  class LightingAttributes
  {
    public:
      LightingAttributes( void )
        : m_colorMaterial(GL_FALSE)
        , m_colorMaterialFace(GL_FRONT_AND_BACK)
        , m_colorMaterialMode(GL_AMBIENT_AND_DIFFUSE)
        , m_lightModelLocalViewer(GL_FALSE)
        , m_lightModelTwoSide(GL_FALSE)
        , m_lightModelAmbient(0.2f,0.2f,0.2f,1.0f)
        , m_shadeModel(GL_SMOOTH)
      {
        m_materialAmbient[NVSG_CACHE_FRONT]   = m_materialAmbient[NVSG_CACHE_BACK]   = nvmath::Vec4f( 0.2f, 0.2f, 0.2f, 1.0f );
        m_materialDiffuse[NVSG_CACHE_FRONT]   = m_materialDiffuse[NVSG_CACHE_BACK]   = nvmath::Vec4f( 0.8f, 0.8f, 0.8f, 1.0f );
        m_materialSpecular[NVSG_CACHE_FRONT]  = m_materialSpecular[NVSG_CACHE_BACK]  = nvmath::Vec4f( 0.0f, 0.0f, 0.0f, 1.0f );
        m_materialEmission[NVSG_CACHE_FRONT]  = m_materialEmission[NVSG_CACHE_BACK]  = nvmath::Vec4f( 0.0f, 0.0f, 0.0f, 1.0f );
        m_materialShininess[NVSG_CACHE_FRONT] = m_materialShininess[NVSG_CACHE_BACK] = 0.0f;
      }

    public:
      GLboolean                     m_colorMaterial;
      GLenum                        m_colorMaterialFace;
      GLenum                        m_colorMaterialMode;
      GLboolean                     m_lightModelLocalViewer;
      GLboolean                     m_lightModelTwoSide;
      nvmath::Vec4f                 m_lightModelAmbient;
      std::vector<LightParameters>  m_lightParameters;
      nvmath::Vec4f                 m_materialAmbient[2];
      nvmath::Vec4f                 m_materialDiffuse[2];
      nvmath::Vec4f                 m_materialSpecular[2];
      nvmath::Vec4f                 m_materialEmission[2];
      float                         m_materialShininess[2];
      GLenum                        m_shadeModel;
  };

  class LightingEnableAttributes
  {
    public:
      LightingEnableAttributes( void )
        : m_lighting(GL_FALSE)
      {}

    public:
      std::vector<GLboolean>  m_lights;
      GLboolean               m_lighting;
  };

  class LineAttributes
  {
    public:
      LineAttributes( void )
        : m_width(1.0f)
        , m_stipplePattern(0xFFFF)
        , m_stippleRepeat(1)
      {}

    public:
      float     m_width;
      GLushort  m_stipplePattern;
      GLint     m_stippleRepeat;
  };

  class LineEnableAttributes
  {
    public:
      LineEnableAttributes( void )
        : m_smooth(GL_FALSE)
        , m_stipple(GL_FALSE)
      {}

    public:
      GLboolean m_smooth;
      GLboolean m_stipple;
  };

  class PointAttributes
  {
    public:
      PointAttributes( void )
        : m_size(1.0f)
      {}

    public:
      float m_size;
  };

  class PointEnableAttributes
  {
    public:
      PointEnableAttributes( void )
        : m_smooth(GL_FALSE)
      {}

    public:
      GLboolean m_smooth;
  };

  class PolygonAttributes
  {
    public:
      PolygonAttributes( void )
        : m_cullFaceMode(GL_BACK)
        , m_frontFaceMode(GL_CCW)
        , m_offsetFactor(0.0f)
        , m_offsetBias(0.0f)
      {
        m_rasterMode[NVSG_CACHE_FRONT] = m_rasterMode[NVSG_CACHE_BACK] = GL_FILL;
      }


    public:
      GLenum  m_cullFaceMode;
      GLenum  m_frontFaceMode;
      GLenum  m_rasterMode[2];
      GLfloat m_offsetFactor;
      GLfloat m_offsetBias;
  };

  class PolygonEnableAttributes
  {
    public:
      PolygonEnableAttributes( void )
        : m_cullFace(GL_FALSE)
        , m_offsetFill(GL_FALSE)
        , m_offsetLine(GL_FALSE)
        , m_offsetPoint(GL_FALSE)
        , m_smooth(GL_FALSE)
        , m_stipple(GL_FALSE)
      {}

    public:
      GLboolean m_cullFace;
      GLboolean m_offsetFill;
      GLboolean m_offsetLine;
      GLboolean m_offsetPoint;
      GLboolean m_smooth;
      GLboolean m_stipple;
  };

  class ScissorAttributes
  {
    public:
      GLint   m_x;
      GLint   m_y;
      GLsizei m_width;
      GLsizei m_height;
  };

  class ScissorEnableAttributes
  {
    public:
      ScissorEnableAttributes()
        : m_scissorTest(GL_FALSE)
      {}

    public:
      GLboolean m_scissorTest;
  };

  class StencilBufferAttributes
  {
    public:
      StencilBufferAttributes( void )
        : m_stencilClearValue(0)
        , m_stencilFail(GL_KEEP)
        , m_stencilFunc(GL_ALWAYS)
        , m_stencilPassDepthFail(GL_KEEP)
        , m_stencilPassDepthPass(GL_KEEP)
        , m_stencilRef(0)
        , m_stencilWriteMask(0xFFFFFFFF)
        , m_stencilValueMask(0xFFFFFFFF)
      {}

    public:
      GLint   m_stencilClearValue;
      GLenum  m_stencilFail;
      GLenum  m_stencilFunc;
      GLenum  m_stencilPassDepthFail;
      GLenum  m_stencilPassDepthPass;
      GLint   m_stencilRef;
      GLuint  m_stencilWriteMask;
      GLuint  m_stencilValueMask;
  };

  class StencilBufferEnableAttributes
  {
    public:
      StencilBufferEnableAttributes()
        :m_stencilTest(GL_FALSE)
      {}

    public:
      GLboolean m_stencilTest;
  };

  class TextureAttributesPerUnit
  {
    public:
      TextureAttributesPerUnit( void )
        : m_envColor(0.0f,0.0f,0.0f,0.0f)
        , m_envMode(GL_MODULATE)
      {}

    public:
      nvmath::Vec4f                       m_envColor;
      GLenum                              m_envMode;
  };

  class TextureAttributes
  {
    public:
      TextureAttributes( void )
        : m_active(0)
        , m_toActivate(0)
      {}

    public:
      std::vector<TextureAttributesPerUnit> m_unit;
      GLenum                                m_active;
      GLenum                                m_toActivate;
  };

  class TextureEnableParameters
  {
    public:
      TextureEnableParameters( void )
      {
        for ( size_t i=0 ; i<4 ; i++ )
        {
          m_gen[i] = GL_FALSE;
        }
      }

    public:
      std::map<GLenum,GLboolean>  m_texture;  // texture enable per target
      GLboolean                   m_gen[4];
  };

  class TextureEnableAttributes
  {
    public:
      std::vector<TextureEnableParameters> m_unit;
  };

  class TransformAttributes
  {
    public:
      TransformAttributes( void )
        : m_matrixMode(GL_MODELVIEW)
      {}

    public:
      std::vector<Vec4d>  m_clipPlaneEquations;
      GLenum              m_matrixMode;
  };

  class TransformEnableAttributes
  {
    public:
      TransformEnableAttributes( void )
        : m_normalize(GL_FALSE)
      {}

    public:
      std::vector<GLboolean>  m_clipPlanes;
      GLboolean               m_normalize;
  };

  class VertexArrayParameters
  {
    public:
      VertexArrayParameters( void )
        : m_size(0)
        , m_type(GL_INVALID_VALUE)
        , m_stride(0)
        , m_pointer(NULL) 
      {}
      VertexArrayParameters( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer )
        : m_size(size)
        , m_type(type)
        , m_stride(stride)
        , m_pointer(pointer)
      {}

    public:
      void invalidate( void );
      bool operator!=( const VertexArrayParameters &rhs );

    public:
      GLint           m_size;
      GLenum          m_type;
      GLsizei         m_stride;
      const GLvoid *  m_pointer;
  };

  class VertexArrayAttributes
  {
    public:
      VertexArrayAttributes( void )
        : m_vertexArray(GL_FALSE)
        , m_normalArray(GL_FALSE)
        , m_colorArray(GL_FALSE)
        , m_secondaryColorArray(GL_FALSE)
        , m_fogCoordArray(GL_FALSE)
        , m_primitiveRestartIndex(0)
        , m_clientTextureActive(0)
        , m_clientTextureToActivate(0)
      {}

    public:
      GLboolean                           m_vertexArray;
      GLboolean                           m_normalArray;
      GLboolean                           m_colorArray;
      GLboolean                           m_secondaryColorArray;
      GLboolean                           m_fogCoordArray;
      std::vector<GLboolean>              m_textureCoordArray;
      VertexArrayParameters               m_vertexArrayParams;
      VertexArrayParameters               m_normalArrayParams;
      VertexArrayParameters               m_colorArrayParams;
      VertexArrayParameters               m_secondaryColorArrayParams;
      VertexArrayParameters               m_fogCoordArrayParams;
      std::vector<VertexArrayParameters>  m_textureCoordArrayParams;
      GLuint                              m_primitiveRestartIndex;
      GLenum                              m_clientTextureActive;
      GLenum                              m_clientTextureToActivate;
  };

  class ViewportAttributes
  {
    public:
      ViewportAttributes( void )
        : m_depthRangeFar(1.0)
        , m_depthRangeNear(0.0)
      {}

    public:
      GLclampd  m_depthRangeFar;
      GLclampd  m_depthRangeNear;
  };

  BufferBindingAttributes       m_bufferBindingAttributes; // vertex and pixel buffer binding states
  ColorBufferAttributes         m_colorBufferAttributes;
  ColorBufferEnableAttributes   m_colorBufferEnableAttributes;
  DepthBufferAttributes         m_depthBufferAttributes;
  DepthBufferEnableAttributes   m_depthBufferEnableAttributes;
  EvalAttributes                m_evalAttributes;
  FogAttributes                 m_fogAttributes;
  FogEnableAttributes           m_fogEnableAttributes;
  LightingAttributes            m_lightingAttributes;
  LightingEnableAttributes      m_lightingEnableAttributes;
  LineAttributes                m_lineAttributes;
  LineEnableAttributes          m_lineEnableAttributes;
  PointAttributes               m_pointAttributes;
  PointEnableAttributes         m_pointEnableAttributes;
  PolygonAttributes             m_polygonAttributes;
  PolygonEnableAttributes       m_polygonEnableAttributes;
  ScissorAttributes             m_scissorAttributes;
  ScissorEnableAttributes       m_scissorEnableAttributes;
  StencilBufferAttributes       m_stencilBufferAttributes;
  StencilBufferEnableAttributes m_stencilBufferEnableAttributes;
  TextureAttributes             m_textureAttributes;
  TextureEnableAttributes       m_textureEnableAttributes;
  TransformAttributes           m_transformAttributes;
  TransformEnableAttributes     m_transformEnableAttributes;
  VertexArrayAttributes         m_vertexArrayAttributes;
  ViewportAttributes            m_viewportAttributes;

  std::stack<GLbitfield>                    m_pushMasks;
  std::stack<GLbitfield>                    m_clientPushMasks;
  std::stack<ColorBufferAttributes>         m_colorBufferAttributesStack;
  std::stack<ColorBufferEnableAttributes>   m_colorBufferEnableAttributesStack;
  std::stack<DepthBufferAttributes>         m_depthBufferAttributesStack;
  std::stack<DepthBufferEnableAttributes>   m_depthBufferEnableAttributesStack;
  std::stack<EvalAttributes>                m_evalAttributesStack;
  std::stack<FogAttributes>                 m_fogAttributesStack;
  std::stack<FogEnableAttributes>           m_fogEnableAttributesStack;
  std::stack<LightingAttributes>            m_lightingAttributesStack;
  std::stack<LightingEnableAttributes>      m_lightingEnableAttributesStack;
  std::stack<LineAttributes>                m_lineAttributesStack;
  std::stack<LineEnableAttributes>          m_lineEnableAttributesStack;
  std::stack<PointAttributes>               m_pointAttributesStack;
  std::stack<PointEnableAttributes>         m_pointEnableAttributesStack;
  std::stack<PolygonAttributes>             m_polygonAttributesStack;
  std::stack<PolygonEnableAttributes>       m_polygonEnableAttributesStack;
  std::stack<ScissorAttributes>             m_scissorAttributesStack;
  std::stack<ScissorEnableAttributes>       m_scissorEnableAttributesStack;
  std::stack<StencilBufferAttributes>       m_stencilBufferAttributesStack;
  std::stack<StencilBufferEnableAttributes> m_stencilBufferEnableAttributesStack;
  std::stack<TextureAttributes>             m_textureAttributesStack;
  std::stack<TextureEnableAttributes>       m_textureEnableAttributesStack;
  std::stack<TransformAttributes>           m_transformAttributesStack;
  std::stack<TransformEnableAttributes>     m_transformEnableAttributesStack;
  std::stack<VertexArrayAttributes>         m_vertexArrayAttributesStack;
  std::stack<ViewportAttributes>            m_viewportAttributesStack;
  std::stack<VertexBufferBinding>           m_vertexBufferBindingAttributesStack;
  std::stack<PixelBufferBinding>            m_pixelBufferBindingAttributesStack;

# if defined(GL_SUPPORT_RESET)
  class PreviousAttributes
  {
    public:
      PreviousAttributes()
        : m_alphaFunc(GL_ALWAYS)
        , m_alphaRef(0.0f)
        , m_alphaTest(GL_FALSE)
        , m_autoNormal(GL_FALSE)
        , m_blend(GL_FALSE)
        , m_blendSrcRGB(GL_ONE)
        , m_blendSrcAlpha(GL_ONE)
        , m_blendDstRGB(GL_ZERO)
        , m_blendDstAlpha(GL_ZERO)
        , m_clearColor(0.0f,0.0f,0.0f,0.0f)
        , m_colorLogicOp(GL_FALSE)
        , m_colorMaterial(GL_FALSE)
        , m_colorMaterialFace(GL_FRONT_AND_BACK)
        , m_colorMaterialMode(GL_AMBIENT_AND_DIFFUSE)
        , m_cullFace(GL_FALSE)
        , m_cullFaceMode(GL_BACK)
        , m_depthClearValue(1.0f)
        , m_depthFunc(GL_LESS)
        , m_depthRangeFar(1.0)
        , m_depthRangeNear(0.0)
        , m_depthTesting(GL_FALSE)
        , m_depthWriting(GL_TRUE)
        , m_dither(GL_TRUE)
        , m_fog(GL_FALSE)
        , m_fogColor(0.0f,0.0f,0.0f,0.0f)
        , m_fogDensity(1.0f)
        , m_fogEnd(1.0f)
        , m_fogMode(GL_EXP)
        , m_fogStart(0.0f)
        , m_frontFaceMode(GL_CCW)
        , m_lighting(GL_FALSE)
        , m_lightModelAmbient(0.2f,0.2f,0.2f,1.0f)
        , m_lightModelLocalViewer(GL_FALSE)
        , m_lightModelTwoSide(GL_FALSE)
        , m_lineSmooth(GL_FALSE)
        , m_lineStipple(GL_FALSE)
        , m_lineStipplePattern(0xFFFF)
        , m_lineStippleRepeat(1)
        , m_lineWidth(1.0f)
        , m_logicOp(GL_FALSE)
        , m_normalize(GL_FALSE)
        , m_pointSize(1.0f)
        , m_pointSmooth(GL_FALSE)
        , m_polygonOffsetFactor(0.0f)
        , m_polygonOffsetBias(0.0f)
        , m_polygonSmooth(GL_FALSE)
        , m_polygonStipple(GL_FALSE)
        , m_offsetFill(GL_FALSE)
        , m_offsetLine(GL_FALSE)
        , m_offsetPoint(GL_FALSE)
        , m_scissorTest(GL_FALSE)
        , m_shadeModel(GL_SMOOTH)
        , m_stencilClearValue(0)
        , m_stencilFail(GL_KEEP)
        , m_stencilFunc(GL_ALWAYS)
        , m_stencilPassDepthFail(GL_KEEP)
        , m_stencilPassDepthPass(GL_KEEP)
        , m_stencilRef(0)
        , m_stencilTest(GL_FALSE)
        , m_stencilValueMask(0xFFFFFFFF)
        , m_stencilWriteMask(0xFFFFFFFF)
      {
        for ( size_t i=0 ; i<4 ; i++ )
        {
          m_colorWriteMask[i] = GL_TRUE;
        }
        m_materialAmbient[NVSG_CACHE_FRONT]   = m_materialAmbient[NVSG_CACHE_BACK]   = nvmath::Vec4f( 0.2f, 0.2f, 0.2f, 1.0f );
        m_materialDiffuse[NVSG_CACHE_FRONT]   = m_materialDiffuse[NVSG_CACHE_BACK]   = nvmath::Vec4f( 0.8f, 0.8f, 0.8f, 1.0f );
        m_materialSpecular[NVSG_CACHE_FRONT]  = m_materialSpecular[NVSG_CACHE_BACK]  = nvmath::Vec4f( 0.0f, 0.0f, 0.0f, 1.0f );
        m_materialEmission[NVSG_CACHE_FRONT]  = m_materialEmission[NVSG_CACHE_BACK]  = nvmath::Vec4f( 0.0f, 0.0f, 0.0f, 1.0f );
        m_materialShininess[NVSG_CACHE_FRONT] = m_materialShininess[NVSG_CACHE_BACK] = 0.0f;
        m_rasterMode[NVSG_CACHE_FRONT] = m_rasterMode[NVSG_CACHE_BACK] = GL_FILL;
      }

    public:
      GLenum                                m_alphaFunc;
      GLclampf                              m_alphaRef;
      GLboolean                             m_alphaTest;
      GLboolean                             m_autoNormal;
      GLboolean                             m_blend;
      GLenum                                m_blendSrcRGB;
      GLenum                                m_blendSrcAlpha;
      GLenum                                m_blendDstRGB;
      GLenum                                m_blendDstAlpha;
      nvmath::Vec4f                         m_clearColor;
      std::vector<GLboolean>                m_clipPlanes;
      std::vector<Vec4d>                    m_clipPlaneEquations;
      GLboolean                             m_colorLogicOp;
      GLboolean                             m_colorMaterial;
      GLenum                                m_colorMaterialFace;
      GLenum                                m_colorMaterialMode;
      GLboolean                             m_colorWriteMask[4];
      GLboolean                             m_cullFace;
      GLenum                                m_cullFaceMode;
      GLclampd                              m_depthClearValue;
      GLenum                                m_depthFunc;
      GLclampd                              m_depthRangeFar;
      GLclampd                              m_depthRangeNear;
      GLboolean                             m_depthTesting;
      GLboolean                             m_depthWriting;
      GLboolean                             m_dither;
      GLboolean                             m_fog;
      nvmath::Vec4f                         m_fogColor;
      GLfloat                               m_fogDensity;
      GLfloat                               m_fogEnd;
      GLenum                                m_fogMode;
      GLfloat                               m_fogStart;
      GLenum                                m_frontFaceMode;
      GLboolean                             m_lighting;
      std::vector<GLboolean>                m_lights;
      nvmath::Vec4f                         m_lightModelAmbient;
      std::vector<LightParameters>          m_lightParameters;
      GLboolean                             m_lightModelLocalViewer;
      GLboolean                             m_lightModelTwoSide;
      GLboolean                             m_lineSmooth;
      GLboolean                             m_lineStipple;
      GLushort                              m_lineStipplePattern;
      GLint                                 m_lineStippleRepeat;
      GLfloat                               m_lineWidth;
      GLboolean                             m_logicOp;
      nvmath::Vec4f                         m_materialAmbient[2];
      nvmath::Vec4f                         m_materialDiffuse[2];
      nvmath::Vec4f                         m_materialSpecular[2];
      nvmath::Vec4f                         m_materialEmission[2];
      float                                 m_materialShininess[2];
      GLboolean                             m_normalize;
      GLfloat                               m_pointSize;
      GLboolean                             m_pointSmooth;
      GLfloat                               m_polygonOffsetFactor;
      GLfloat                               m_polygonOffsetBias;
      GLboolean                             m_polygonSmooth;
      GLboolean                             m_polygonStipple;
      GLboolean                             m_offsetFill;
      GLboolean                             m_offsetLine;
      GLboolean                             m_offsetPoint;
      GLenum                                m_rasterMode[2];
      GLsizei                               m_scissorHeight;
      GLboolean                             m_scissorTest;
      GLsizei                               m_scissorWidth;
      GLint                                 m_scissorX;
      GLint                                 m_scissorY;
      GLenum                                m_shadeModel;
      GLint                                 m_stencilClearValue;
      GLenum                                m_stencilFail;
      GLenum                                m_stencilFunc;
      GLenum                                m_stencilPassDepthFail;
      GLenum                                m_stencilPassDepthPass;
      GLint                                 m_stencilRef;
      GLboolean                             m_stencilTest;
      GLuint                                m_stencilValueMask;
      GLuint                                m_stencilWriteMask;
      std::vector<TextureEnableParameters>  m_textureEnables;
  };

  PreviousAttributes  m_previousAttributes;
# endif // GL_SUPPORT_RESET

#endif // GL_CACHING
  GLObjects     * m_glObjects;    // managing shareable and non-shareable objects

  class GLLocks
  {
    public:
      GLLocks( void );

    public:
      bool  m_cullFace;
      bool  m_cullFaceMode;
      bool  m_lighting;
      bool  m_lightModelTwoSide;
      bool  m_polygonMode;
  };

  GLLocks         m_locks;

#if defined(_WIN32)
  HDC             m_hdc;        // Handle to the corresponding device context
  HGLRC           m_hglrc;      // Handle of the corresponding GL render context
#elif defined(LINUX)  
  static Display * m_dpy;       // Handle to corresponding X display
  GLXContext       m_ctx;       // Handle to corresponding X render context
  GLXDrawable      m_drawable;  // Handle to corresponding X drawable
#endif

  GLFunctions   * m_glFunctions;  // Pointer to OpenGL functions wrapper object
  GLint           m_maxTexCoords; // Maximal number of supported client texture slots on this HW
  GLint           m_maxTexUnits;  // Maximal number of supported texture slots on this HW
  GLint           m_maxTexImageUnits;  // Maximal number of supported texture image units on this HW

  // Table of texture image formats mapping given PixelFormat and PixelDataType descriptors to a
  // 'best match' GL format. This table will be initialized at GLContext construction.
  GLTexImageFmt m_glTexImageFmts[nvsg::Image::IMG_NUM_FORMATS][nvsg::Image::IMG_NUM_TYPES]; 

#if !defined(NDEBUG)
  bool            m_inBeginEndLoop;  // \c true, when inside a glBegin/glEnd loop, otherwise \c false
#endif
#if defined( GL_STATISTICS )
  GLStatistics  * m_glStats;
#endif
};

} // namespace nvgl

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// inline implementations
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// need defines now with implementing
#include "nvgl/GLFunctions.h"
#include "nvgl/GLObjects.h"

namespace nvgl 
{
inline void GLContext::addBuffers(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  m_glObjects->m_shareables->m_buffers.insert(&names[0], &names[n]);
}

inline void GLContext::removeBuffers(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  for( GLsizei i=0; i<n; ++i )
  {
    m_glObjects->m_shareables->m_buffers.erase(names[i]);
  }
}

inline void GLContext::addLists(GLsizei n, GLuint first)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && first>0);
  for( GLuint i=first; i<first+n; ++i )
  {
    m_glObjects->m_shareables->m_lists.insert(i);
  }
}

inline void GLContext::removeLists(GLsizei n, GLuint first)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && first>0);
  for( GLuint i=first; i<first+n; ++i )
  {
    m_glObjects->m_shareables->m_lists.erase(i);
  }
}

inline void GLContext::addPrograms(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  m_glObjects->m_shareables->m_programs.insert(&names[0], &names[n]);
}

inline void GLContext::removePrograms(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  for( GLsizei i=0; i<n; ++i )
  {
    m_glObjects->m_shareables->m_programs.erase(names[i]);
  }
}

inline void GLContext::addTextures(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  m_glObjects->m_shareables->m_textures.insert(&names[0], &names[n]);
}

inline void GLContext::removeTextures(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  for( GLsizei i=0; i<n; ++i )
  {
    m_glObjects->m_shareables->m_textures.erase(names[i]);
  }
}

inline void GLContext::addOcclusionQueries(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  m_glObjects->m_occlusionQueries.insert(&names[0], &names[n]);
}

inline void GLContext::removeOcclusionQueries(GLsizei n, const GLuint * names)

{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  for( GLsizei i=0; i<n; ++i )
  {
    m_glObjects->m_occlusionQueries.erase(names[i]);
  }
}

inline void GLContext::addQueries(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  m_glObjects->m_queries.insert(&names[0], &names[n]);
}

inline void GLContext::removeQueries(GLsizei n, const GLuint * names)
{
  NVSG_TRACE();
  NVSG_ASSERT(n>0 && names);
  for( GLsizei i=0; i<n; ++i )
  {
    m_glObjects->m_queries.erase(names[i]);
  }
}

/*! \brief Get the GLContext of the current thread.
 *  \return The GLContext of the current thread.
 *  \remarks When using WGLMakeCurrent (or GLXMakeCurrent on Linux), to make a HGLRC current, the
 *  corresponding GLContext is assigned. As there can be only one HGLRC current per thread, the same
 *  holds for the GLContext. Using this function returns the GLContext of the current thread.
 *  \sa WGLMakeCurrent, GLXMakeCurrent, GLContext */
inline GLContext * glContext()
{
#if defined(_WIN32)
  NVSG_ASSERT(glContextTlsIdx!=TLS_OUT_OF_INDEXES);
  return (GLContext*)TlsGetValue(glContextTlsIdx);
#elif defined(LINUX) 
  return (GLContext*)pthread_getspecific(glContextKey);
#else
#error Unsupported Operating System!
#endif
}

//////////////////////////////////////////////////////////////////////////
#if !defined(DOXYGEN_IGNORE)
# if !defined(NDEBUG)
inline void ASSERT_GL_CURRENT_CONTEXT_CONFORMITY()
{
#  if defined(_WIN32)
  HGLRC hglrc = wglGetCurrentContext();
  NVSG_ASSERT((glContext()!=NULL)==(hglrc!=NULL));
  NVSG_ASSERT(!glContext()||glContext()->m_hglrc==hglrc);
#  elif defined(LINUX)
  GLXContext ctx = glXGetCurrentContext();
  NVSG_ASSERT((glContext()!=NULL)==(ctx!=NULL));
  NVSG_ASSERT(!glContext()||glContext()->m_ctx==ctx);
#  else // unsupported OS
#  endif
}
# else
# define ASSERT_GL_CURRENT_CONTEXT_CONFORMITY() static_cast<void*>(0)
# endif

# if !defined(NDEBUG) && defined(GL_CACHING)

inline void ASSERT_GL_ARRAY_BUFFER_BINDING()
{
  GLContext * ctx = glContext();
  GLint name;
  GLGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &name);
  NVSG_ASSERT(ctx->m_bufferBindingAttributes[GL_ARRAY_BUFFER_ARB]==name);
}

inline void ASSERT_GL_ELEMENT_ARRAY_BUFFER_BINDING()
{
  GLContext * ctx = glContext();
  GLint name;
  GLGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB, &name);
  NVSG_ASSERT(ctx->m_bufferBindingAttributes[GL_ELEMENT_ARRAY_BUFFER_ARB]==name);
}

inline void ASSERT_GL_PIXEL_PACK_BUFFER_BINDING()
{
  GLContext * ctx = glContext();
  GLint name;
  GLGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING_ARB, &name);
  NVSG_ASSERT(ctx->m_bufferBindingAttributes[GL_PIXEL_PACK_BUFFER_ARB]==name);
}

inline void ASSERT_GL_PIXEL_UNPACK_BUFFER_BINDING()
{
  GLContext * ctx = glContext();
  GLint name;
  GLGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING_ARB, &name);
  NVSG_ASSERT(ctx->m_bufferBindingAttributes[GL_PIXEL_UNPACK_BUFFER_ARB]==name);
}

# else

# define ASSERT_GL_ARRAY_BUFFER_BINDING()         static_cast<void*>(0)
# define ASSERT_GL_ELEMENT_ARRAY_BUFFER_BINDING() static_cast<void*>(0)
# define ASSERT_GL_PIXEL_PACK_BUFFER_BINDING()    static_cast<void*>(0)
# define ASSERT_GL_PIXEL_UNPACK_BUFFER_BINDING()  static_cast<void*>(0)

# endif
#endif //DOXYGEN_IGNORE

//! Fast float compare helper
inline bool equalf(GLfloat a, GLfloat b)
{
  return (*((int*)&a) == *((int*)&b));
}
//! Fast float compare helper
inline bool equal3f(GLfloat a1, GLfloat a2, GLfloat a3, const GLfloat *b)
{
  return (  *((int*)&a1) == *((int*)&b[0])
         && *((int*)&a2) == *((int*)&b[1])
         && *((int*)&a3) == *((int*)&b[2]));
}  
//! Fast float compare helper
inline bool equal3fv(const GLfloat *a, const GLfloat *b)
{
  return (  *((int*)&a[0]) == *((int*)&b[0])
         && *((int*)&a[1]) == *((int*)&b[1])
         && *((int*)&a[2]) == *((int*)&b[2]));
}  
//! Fast float compare helper
inline bool equal4f(GLfloat a1, GLfloat a2, GLfloat a3, GLfloat a4, const GLfloat *b)
{
  return (  *((int*)&a1) == *((int*)&b[0])
         && *((int*)&a2) == *((int*)&b[1])
         && *((int*)&a3) == *((int*)&b[2])
         && *((int*)&a4) == *((int*)&b[3]));
}  
//! Fast float compare helper
inline bool equal4fv(const GLfloat *a, const GLfloat *b)
{
  return (  *((int*)&a[0]) == *((int*)&b[0])
         && *((int*)&a[1]) == *((int*)&b[1])
         && *((int*)&a[2]) == *((int*)&b[2])
         && *((int*)&a[3]) == *((int*)&b[3]));
}  

//////////////////////////////////////////////////////////////////////////

//! Replacement of glAccum
/** This function asserts on the result before returning.  */
inline void  GLAccum( GLenum op, GLfloat value )
{
  NVSG_GL_STATS_FCT_COUNT();
  glAccum( op, value );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glActiveTexture
/** This function stores the texture to activate for subsequent texture operations.  */
inline  void  GLActiveTexture( GLenum texture )
{
#if defined( GL_CACHING )
  glContext()->m_textureAttributes.m_toActivate = texture - GL_TEXTURE0;
#else
  NVSG_GL_STATS_ACTIVE_TEXTURE( texture );
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glActiveTexture );
  glContext()->m_glFunctions->glActiveTexture(texture);
  ASSERT_GL_NO_ERROR;
#endif
}

//! Replacement of glAlphaFunc
/** This function caches the alpha func and ref and asserts on the result before returning. */
inline  void  GLAlphaFunc( GLenum func, GLclampf ref )
{
#if defined(GL_CACHING)
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_alphaFunc = ctx->m_colorBufferAttributes.m_alphaFunc;
  ctx->m_previousAttributes.m_alphaRef  = ctx->m_colorBufferAttributes.m_alphaRef;
# endif
  if (    ( ctx->m_colorBufferAttributes.m_alphaFunc != func )
      ||  ( ctx->m_colorBufferAttributes.m_alphaRef  != ref ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glAlphaFunc( func, ref );
    ASSERT_GL_NO_ERROR;
#if defined(GL_CACHING)
    ctx->m_colorBufferAttributes.m_alphaFunc = func;
    ctx->m_colorBufferAttributes.m_alphaRef  = ref;
#endif
  }
}

/*! \brief Helper function to reset alpha function and alpha reference value.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this functions sets the alpha
 *  function and alpha reference value to the previous function and value. Otherwise, the alpha
 *  function is set to GL_ALWAYS, and the alpha reference value is set to zero.
 *  \sa GLAlphaFunc */
inline void GLResetAlphaFunc()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLAlphaFunc( ctx->m_previousAttributes.m_alphaFunc, ctx->m_previousAttributes.m_alphaRef );
#else
  GLAlphaFunc( GL_ALWAYS, 0.0f );
#endif
}

//! Replacement of glAreTexturesResident
/** This function asserts on the result before returning.  */
inline  GLboolean GLAreTexturesResident( GLsizei n, const GLuint *textures, GLboolean *residences )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLboolean ok = glAreTexturesResident( n, textures, residences );
  ASSERT_GL_NO_ERROR;
  return( ok );
}

//! Replacement of glArrayElement
/** This function asserts on the result before returning.  */
inline  void  GLArrayElement( GLint i )
{
  NVSG_GL_STATS_FCT_COUNT();
  glArrayElement( i );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glBegin
/** This function asserts on the result before returning.  */
inline  void  GLBegin( GLenum mode )
{
  NVSG_GL_STATS_FCT_COUNT();
  glBegin( mode );
#if !defined(NDEBUG)
  glContext()->m_inBeginEndLoop = true;
#endif
}

//! Replacement of glBeginOcclusionQueryNV
/** This function hides the extension character of glBeginOcclusionQueryNV and asserts on the 
  * result before returning.  */
inline  void  GLBeginOcclusionQuery( GLuint id )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glBeginOcclusionQueryNV );
  glContext()->m_glFunctions->glBeginOcclusionQueryNV( id );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glBeginQueryARB
/** This function hides the extension character of glBeginQueryARB and asserts on the 
  * result before returning. */
inline  void  GLBeginQuery( GLenum target, GLuint id )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glBeginQueryARB );
  glContext()->m_glFunctions->glBeginQueryARB( target, id );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glBindBufferARB
/** This function hides the extension character of glBindBufferARB, caches, and 
  * asserts on the result before returning.  */
inline  void  GLBindBuffer( GLenum target, GLuint id )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
  NVSG_ASSERT(  ( target == GL_ARRAY_BUFFER_ARB ) 
             || ( target == GL_ELEMENT_ARRAY_BUFFER_ARB ) 
             || ( target == GL_PIXEL_PACK_BUFFER_ARB )
             || ( target == GL_PIXEL_UNPACK_BUFFER_ARB ) );

  if ( ctx->m_bufferBindingAttributes[target] != id )
#endif
  {
    NVSG_GL_STATS_BIND_BUFFER( target, id );
    NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glBindBufferARB );
    ctx->m_glFunctions->glBindBufferARB( target, id );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_bufferBindingAttributes[target] = id;
    if ( GL_ARRAY_BUFFER_ARB==target )
    {
      ctx->m_vertexArrayAttributes.m_vertexArrayParams.invalidate();
      ctx->m_vertexArrayAttributes.m_normalArrayParams.invalidate();
      ctx->m_vertexArrayAttributes.m_colorArrayParams.invalidate();
      ctx->m_vertexArrayAttributes.m_secondaryColorArrayParams.invalidate();
      ctx->m_vertexArrayAttributes.m_fogCoordArrayParams.invalidate();
      for ( size_t i=0 ; i<ctx->m_vertexArrayAttributes.m_textureCoordArrayParams.size() ; ++i )
      {      
        ctx->m_vertexArrayAttributes.m_textureCoordArrayParams[i].invalidate();
      }
    }
#endif
  }
}

//! Replacement of glBufferDataARB
/** This function hides the extension character of glBufferDataARB and asserts 
  * on the result before returning.  */
inline  void  GLBufferData( GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage )
{
  NVSG_GL_STATS_BUFFER_DATA( target, size, data, usage );
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glBufferDataARB );
  glContext()->m_glFunctions->glBufferDataARB( target, size, data, usage );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glBufferSubDataARB
/** This function hides the extension character of glBufferSubDataARB and asserts 
  * on the result before returning.  */
inline  void  GLBufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glBufferSubDataARB );
  glContext()->m_glFunctions->glBufferSubDataARB( target, offset, size, data );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDeleteBuffersARB
/** This function hides the extension character of glDeleteBuffersARB, updates some caches, and asserts 
  * on the result before returning.  */
inline  void  GLDeleteBuffers(GLsizei n, const GLuint *buffers)
{
  NVSG_GL_STATS_DELETE_BUFFERS( n, buffers );
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glDeleteBuffersARB );
#if defined(GL_CACHING)
  for ( GLsizei i=0; i<n; ++i )
  {
#define CHECK_UNBIND_BUFFER(t) \
    if (  ctx->m_bufferBindingAttributes[t] == buffers[i] ) { \
      GLBindBuffer(t, 0); \
    }
    CHECK_UNBIND_BUFFER(GL_ARRAY_BUFFER_ARB)
    CHECK_UNBIND_BUFFER(GL_ELEMENT_ARRAY_BUFFER_ARB)
    CHECK_UNBIND_BUFFER(GL_PIXEL_PACK_BUFFER_ARB)
    CHECK_UNBIND_BUFFER(GL_PIXEL_UNPACK_BUFFER_ARB)
#undef CHECK_UNBIND_BUFFER
  }
#endif
  ctx->removeBuffers(n, buffers);
  ctx->m_glFunctions->glDeleteBuffersARB( n, buffers );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGenBuffersARB
/** This function hides the extension character of glGenBuffersARB, asserts on the result, and updates some caches. */
inline  void  GLGenBuffers(GLsizei n, GLuint *buffers)
{
  NVSG_GL_STATS_GEN_BUFFERS( n, buffers );
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glGenBuffersARB );
  ctx->m_glFunctions->glGenBuffersARB( n, buffers );
  ctx->addBuffers(n, buffers);
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMapBufferARB
/** This function hides the extension character of glMapBufferARB and asserts 
  * on the result before returning.  */
inline  GLvoid*  GLMapBuffer( GLenum target, GLenum access )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glMapBufferARB );
  GLvoid * p = glContext()->m_glFunctions->glMapBufferARB( target, access );
  ASSERT_GL_NO_ERROR;
  return p;
}

//! Replacement of glUnmapBufferARB
/** This function hides the extension character of glUnmapBufferARB and asserts 
  * on the result before returning.  */
inline  GLboolean  GLUnmapBuffer( GLenum target )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glUnmapBufferARB );
  GLboolean b = glContext()->m_glFunctions->glUnmapBufferARB( target );
  ASSERT_GL_NO_ERROR;
  return b;
}

//! Replacement of glIsBufferARB
/** This function tests if for the current render context, name denotes a valid vertex buffer object. */ 
inline  GLboolean  GLIsBuffer( GLuint name )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext()->m_glFunctions && glContext()->m_glFunctions->glIsBufferARB);
  NVSG_ASSERT(!!glContext()->isBuffer(name)==!!glContext()->m_glFunctions->glIsBufferARB(name));
  return glContext()->isBuffer(name);
}

//! Replacement of glBindProgramNV
/** This function hides the extension character of glBindProgramNV and asserts 
  * on the result before returning.  */
inline  void  GLBindProgram( GLenum target, GLuint id )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glBindProgramNV );
  glContext()->m_glFunctions->glBindProgramNV( target, id );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glBitmap
/** This function asserts on the result before returning.  */
inline  void  GLBitmap( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap )
{
  NVSG_GL_STATS_FCT_COUNT();
  glBitmap( width, height, xorig, yorig, xmove, ymove, bitmap );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glBlendFunc
/** This function caches the blend func settings and asserts on the result before returning.  */
inline  void  GLBlendFunc( GLenum sfactor, GLenum dfactor )
{
#if defined(GL_CACHING)
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_blendSrcRGB   = ctx->m_colorBufferAttributes.m_blendSrcRGB;
  ctx->m_previousAttributes.m_blendSrcAlpha = ctx->m_colorBufferAttributes.m_blendSrcAlpha;
  ctx->m_previousAttributes.m_blendDstRGB   = ctx->m_colorBufferAttributes.m_blendDstRGB;
  ctx->m_previousAttributes.m_blendDstAlpha = ctx->m_colorBufferAttributes.m_blendDstAlpha;
# endif
  if (    ( ctx->m_colorBufferAttributes.m_blendSrcRGB   != sfactor )
      ||  ( ctx->m_colorBufferAttributes.m_blendSrcAlpha != sfactor )
      ||  ( ctx->m_colorBufferAttributes.m_blendDstRGB   != dfactor )
      ||  ( ctx->m_colorBufferAttributes.m_blendDstAlpha != dfactor ) )
#endif
  {
    NVSG_GL_STATS_BLEND_FUNC( sfactor, dfactor );
    glBlendFunc( sfactor, dfactor );
    ASSERT_GL_NO_ERROR;
#if defined(GL_CACHING)
    ctx->m_colorBufferAttributes.m_blendSrcRGB   = sfactor;
    ctx->m_colorBufferAttributes.m_blendSrcAlpha = sfactor;
    ctx->m_colorBufferAttributes.m_blendDstRGB   = dfactor;
    ctx->m_colorBufferAttributes.m_blendDstAlpha = dfactor;
#endif
  }
}

/*! \brief Helper function to reset the blend function.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this functions sets the blend
 *  function to the previous function. Otherwise, the blend function is set to GL_ONE for the source
 *  factor and GL_ZERO for the destination factor.
 *  \sa GLBlendFunc, GLBlendFuncSeparate */
inline void GLResetBlendFunc()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  if (    ( ctx->m_previousAttributes.m_blendSrcAlpha != ctx->m_previousAttributes.m_blendSrcRGB )
      ||  ( ctx->m_previousAttributes.m_blendDstAlpha != ctx->m_previousAttributes.m_blendDstRGB ) )
  {
    GLBlendFuncSeparate( ctx->m_previousAttributes.m_blendSrcRGB
                       , ctx->m_previousAttributes.m_blendDstRGB
                       , ctx->m_previousAttributes.m_blendSrcAlpha
                       , ctx->m_previousAttributes.m_blendDstAlpha );
  }
  else
  {
    GLBlendFunc( ctx->m_previousAttributes.m_blendSrcRGB, ctx->m_previousAttributes.m_blendDstRGB );
  }
#else
  GLBlendFunc( GL_ONE, GL_ZERO );
#endif
}

//! Replacement of glBlendFunc
/** This function caches the blend func settings and asserts on the result before returning.  */
inline  void  GLBlendFuncSeparate( GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha )
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_blend_func_separate"));
#if defined(GL_CACHING)
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_blendSrcRGB   = ctx->m_colorBufferAttributes.m_blendSrcRGB;
  ctx->m_previousAttributes.m_blendSrcAlpha = ctx->m_colorBufferAttributes.m_blendSrcAlpha;
  ctx->m_previousAttributes.m_blendDstRGB   = ctx->m_colorBufferAttributes.m_blendDstRGB;
  ctx->m_previousAttributes.m_blendDstAlpha = ctx->m_colorBufferAttributes.m_blendDstAlpha;
# endif
  if (    ( ctx->m_colorBufferAttributes.m_blendSrcRGB   != sfactorRGB )
      ||  ( ctx->m_colorBufferAttributes.m_blendSrcAlpha != sfactorAlpha )
      ||  ( ctx->m_colorBufferAttributes.m_blendDstRGB   != dfactorRGB )
      ||  ( ctx->m_colorBufferAttributes.m_blendDstAlpha != dfactorAlpha ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glBlendFuncSeparate );
    glContext()->m_glFunctions->glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    ASSERT_GL_NO_ERROR;
#if defined(GL_CACHING)
    ctx->m_colorBufferAttributes.m_blendSrcRGB   = sfactorRGB;
    ctx->m_colorBufferAttributes.m_blendSrcAlpha = sfactorAlpha;
    ctx->m_colorBufferAttributes.m_blendDstRGB   = dfactorRGB;
    ctx->m_colorBufferAttributes.m_blendDstAlpha = dfactorAlpha;
#endif
  }
}

/*! \brief Helper function to reset the blend function, using the GL_EXT_blend_func_separate extention.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this functions sets the four
 *  factors of the extended blend function the previous values. Otherwise, the blend function
 *  factors are set to GL_ONE, GL_ZERO, GL_ONE, and GL_ZERO, in this order.
 *  \sa GLBlendFuncSeparate */
inline void GLResetBlendFuncSeparate()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLBlendFuncSeparate( ctx->m_previousAttributes.m_blendSrcRGB
                     , ctx->m_previousAttributes.m_blendDstRGB
                     , ctx->m_previousAttributes.m_blendSrcAlpha
                     , ctx->m_previousAttributes.m_blendDstAlpha );
#else
  GLBlendFuncSeparate( GL_ONE, GL_ZERO, GL_ONE, GL_ZERO );
#endif
}

//! Replacement of glBindTexture
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLBindTexture( GLenum target, GLuint texture )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_BIND_TEXTURE( target, texture );
  glBindTexture( target, texture );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCallList
/** This function asserts on the result before returning.  */
inline  void  GLCallList( GLuint list )
{
  NVSG_GL_STATS_CALL_LIST( list );
  glCallList( list );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCallLists
/** This function asserts on the result before returning.  */
inline  void  GLCallLists( GLsizei n, GLenum type, const GLvoid *lists )
{
  NVSG_GL_STATS_FCT_COUNT();
  glCallLists( n, type, lists );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glClear
/** This function asserts on the result before returning.  */
inline  void  GLClear( GLbitfield mask )
{
  NVSG_GL_STATS_CLEAR( mask );
  glClear( mask );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glClearAccum
/** This function asserts on the result before returning.  */
inline  void  GLClearAccum( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
  NVSG_GL_STATS_FCT_COUNT();
  glClearAccum( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glClearColor
/** This function caches the clear color settings and asserts on the result before returning.  */
inline  void  GLClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_clearColor = ctx->m_colorBufferAttributes.m_clearColor;
# endif
  if ( ! equal4f( red, green, blue, alpha, &ctx->m_colorBufferAttributes.m_clearColor[0] ) )
#endif
  {
    NVSG_GL_STATS_CLEAR_COLOR( red, green, blue, alpha );
    glClearColor(red, green, blue, alpha);
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_colorBufferAttributes.m_clearColor[0] = red;
    ctx->m_colorBufferAttributes.m_clearColor[1] = green;
    ctx->m_colorBufferAttributes.m_clearColor[2] = blue;
    ctx->m_colorBufferAttributes.m_clearColor[3] = alpha;
#endif
  }
}

/*! \brief Helper function to reset the clear color.
 *  \remarks if GL_SUPPORT_RESET is defined (per default, it is), this function sets the four values
 *  of the clear color to the previous value. Otherwise, they are all set to 0.0f.
 *  \sa GLClearColor */
inline void GLResetClearColor()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLClearColor( ctx->m_previousAttributes.m_clearColor[0]
              , ctx->m_previousAttributes.m_clearColor[1]
              , ctx->m_previousAttributes.m_clearColor[2]
              , ctx->m_previousAttributes.m_clearColor[3] );
#else
  GLClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
#endif
}

//! Replacement of glClearDepth
/** This function caches the depth clear setting and asserts on the result before returning. */
inline  void  GLClearDepth( GLclampd depth )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_depthClearValue = ctx->m_depthBufferAttributes.m_depthClearValue;
# endif
  if ( ctx->m_depthBufferAttributes.m_depthClearValue != depth )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glClearDepth( depth );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_depthBufferAttributes.m_depthClearValue = depth;
#endif
  }
}

/*! \brief Helper function to reset the clear depth.
 *  \remarks if ( GL_SUPPORT_RESET is defined (per default, it is), this function sets the clear
 *  depth to the previous value. Otherwise, it is set to 1.0f.
 *  \sa GLClearDepth */
inline void GLResetClearDepth()
{
#if defined(GL_SUPPORT_RESET)
  GLClearDepth( glContext()->m_previousAttributes.m_depthClearValue );
#else
  GLClearDepth( 1.0 );
#endif
}

//! Replacement of glClearIndex
/** This function asserts on the result before returning.  */
inline  void  GLClearIndex( GLfloat c )
{
  NVSG_GL_STATS_FCT_COUNT();
  glClearIndex( c );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glClearStencil
/** This function caches the stencil clear setting and asserts on the result before returning.  */
inline  void  GLClearStencil( GLint s )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_stencilClearValue = ctx->m_stencilBufferAttributes.m_stencilClearValue;
# endif
  if ( ctx->m_stencilBufferAttributes.m_stencilClearValue != s )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glClearStencil( s );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_stencilBufferAttributes.m_stencilClearValue = s;
#endif
  }
}

/*! \brief Helper function to reset the stencil clear value.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the stencil
 *  clear value to the previous value. Otherwise, it is set to zero.
 *  \sa GLClearStencil */
inline void GLResetClearStencil()
{
#if defined(GL_SUPPORT_RESET)
  GLClearStencil( glContext()->m_previousAttributes.m_stencilClearValue );
#else
  GLClearStencil( 0 );
#endif
}

//! Replacement of glClientActiveTexture
/** This functions caches the next active texture unit and asserts on the result before returning.*/
inline  void  GLClientActiveTexture( GLenum texture )
{
#if defined( GL_CACHING )
  glContext()->m_vertexArrayAttributes.m_clientTextureToActivate = texture - GL_TEXTURE0;
#else
  NVSG_GL_STATS_CLIENT_ACTIVE_TEXTURE( texture );
  glContext()->m_glFunctions->glClientActiveTexture( texture );
  ASSERT_GL_NO_ERROR;
#endif
}

//! Replacement of glClipPlane
/** This function asserts on the result before returning.*/
inline  void  GLClipPlane( GLenum plane, const GLdouble *equation )
{
  //  NOTE: clip planes can't be cached !
  NVSG_GL_STATS_FCT_COUNT();
#if defined( GL_SUPPORT_RESET )
  GLContext * ctx = glContext();
  size_t idx = plane - GL_CLIP_PLANE0;
  NVSG_ASSERT(    ( idx < ctx->m_previousAttributes.m_clipPlaneEquations.size() )
              &&  ( idx < ctx->m_transformAttributes.m_clipPlaneEquations.size() ) );
  ctx->m_previousAttributes.m_clipPlaneEquations[idx] = ctx->m_transformAttributes.m_clipPlaneEquations[idx];
  memcpy( &ctx->m_transformAttributes.m_clipPlaneEquations[idx][0], equation, 4*sizeof(GLdouble) );
#endif
  glClipPlane(plane, equation);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Helper function to reset a clip plane equation.
 *  \param plane The id of the clip plane for which the plane equation is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the clip plane
 *  equation of \a plane to the previous equations. Otherwise, it is set to the default plane
 *  (0,0,0,0).
 *  \sa GLClipPlane */
inline void GLResetClipPlaneEquation( GLenum plane )
{
#if defined( GL_SUPPORT_RESET )
  GLClipPlane( plane, &glContext()->m_previousAttributes.m_clipPlaneEquations[plane-GL_CLIP_PLANE0][0] );
#else
  static Vec4d(0.0, 0.0, 0.0, 0.0); 
  GLClipPlane( plane, zeroPlane );
#endif
}

//! Replacement of glColor3b
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3b( GLbyte red, GLbyte green, GLbyte blue )
{
  NVSG_GL_STATS_COLOR3B( red, green, blue );
  glColor3b( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3bv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3bv( const GLbyte *v )
{
  NVSG_GL_STATS_COLOR3BV( v );
  glColor3bv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3d
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3d( GLdouble red, GLdouble green, GLdouble blue )
{
  NVSG_GL_STATS_COLOR3D( red, green, blue );
  glColor3d( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3dv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3dv( const GLdouble *v )
{
  NVSG_GL_STATS_COLOR3DV( v );
  glColor3dv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3f
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3f( GLfloat red, GLfloat green, GLfloat blue )
{
  NVSG_GL_STATS_COLOR3F( red, green, blue );
  glColor3f( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3fv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3fv( const GLfloat * v )
{
  NVSG_GL_STATS_COLOR3FV( v );
  glColor3fv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3i
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3i( GLint red, GLint green, GLint blue )
{
  NVSG_GL_STATS_COLOR3I( red, green, blue );
  glColor3i( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3iv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3iv( const GLint * v )
{
  NVSG_GL_STATS_COLOR3IV( v );
  glColor3iv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3s
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3s( GLshort red, GLshort green, GLshort blue )
{
  NVSG_GL_STATS_COLOR3S( red, green, blue );
  glColor3s( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3sv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3sv( const GLshort * v )
{
  NVSG_GL_STATS_COLOR3SV( v );
  glColor3sv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3ub
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3ub( GLubyte red, GLubyte green, GLubyte blue )
{
  NVSG_GL_STATS_COLOR3UB( red, green, blue );
  glColor3ub( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3ubv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3ubv( const GLubyte * v )
{
  NVSG_GL_STATS_COLOR3UBV( v );
  glColor3ubv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3ui
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3ui( GLuint red, GLuint green, GLuint blue )
{
  NVSG_GL_STATS_COLOR3UI( red, green, blue );
  glColor3ui( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3uiv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3uiv( const GLuint * v )
{
  NVSG_GL_STATS_COLOR3UIV( v );
  glColor3uiv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3us
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3us( GLushort red, GLushort green, GLushort blue )
{
  NVSG_GL_STATS_COLOR3US( red, green, blue );
  glColor3us( red, green, blue );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor3usv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor3usv( const GLushort * v )
{
  NVSG_GL_STATS_COLOR3USV( v );
  glColor3usv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4b
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4b( GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha )
{
  NVSG_GL_STATS_COLOR4B( red, green, blue, alpha );
  glColor4b( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4bv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4bv( const GLbyte * v )
{
  NVSG_GL_STATS_COLOR4BV( v );
  glColor4bv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4d
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4d( GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha )
{
  NVSG_GL_STATS_COLOR4D( red, green, blue, alpha );
  glColor4d( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4dv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4dv( const GLdouble * v )
{
  NVSG_GL_STATS_COLOR4DV( v );
  glColor4dv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4f
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4f( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
  NVSG_GL_STATS_COLOR4F( red, green, blue, alpha );
  glColor4f( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4fv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4fv( const GLfloat * v )
{
  NVSG_GL_STATS_COLOR4FV( v );
  glColor4fv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4i
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4i( GLint red, GLint green, GLint blue, GLint alpha )
{
  NVSG_GL_STATS_COLOR4I( red, green, blue, alpha );
  glColor4i( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4iv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4iv( const GLint * v )
{
  NVSG_GL_STATS_COLOR4IV( v );
  glColor4iv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4s
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4s( GLshort red, GLshort green, GLshort blue, GLshort alpha )
{
  NVSG_GL_STATS_COLOR4S( red, green, blue, alpha );
  glColor4s( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4sv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4sv( const GLshort * v )
{
  NVSG_GL_STATS_COLOR4SV( v );
  glColor4sv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4ub
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4ub( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha )
{
  NVSG_GL_STATS_COLOR4UB( red, green, blue, alpha );
  glColor4ub( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4ubv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4ubv( const GLubyte * v )
{
  NVSG_GL_STATS_COLOR4UBV( v );
  glColor4ubv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4ui
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4ui( GLuint red, GLuint green, GLuint blue, GLuint alpha )
{
  NVSG_GL_STATS_COLOR4UI( red, green, blue, alpha );
  glColor4ui( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4uiv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4uiv( const GLuint * v )
{
  NVSG_GL_STATS_COLOR4UIV( v );
  glColor4uiv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4us
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4us( GLushort red, GLushort green, GLushort blue, GLushort alpha )
{
  NVSG_GL_STATS_COLOR4US( red, green, blue, alpha );
  glColor4us( red, green, blue, alpha );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColor4usv
/** This function caches the color values and asserts on the result before returning.  */
inline  void  GLColor4usv( const GLushort * v )
{
  NVSG_GL_STATS_COLOR4USV( v );
  glColor4usv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glColorMask
/** This function asserts on the result before returning.  */
inline  void  GLColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  memcpy( &ctx->m_previousAttributes.m_colorWriteMask[0]
        , &ctx->m_colorBufferAttributes.m_colorWriteMask[0], 4*sizeof(GLboolean) );
# endif
  if (    ( ctx->m_colorBufferAttributes.m_colorWriteMask[0] != red )
      ||  ( ctx->m_colorBufferAttributes.m_colorWriteMask[1] != green )
      ||  ( ctx->m_colorBufferAttributes.m_colorWriteMask[2] != blue )
      ||  ( ctx->m_colorBufferAttributes.m_colorWriteMask[3] != alpha ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glColorMask( red, green, blue, alpha );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_colorBufferAttributes.m_colorWriteMask[0] = red;
    ctx->m_colorBufferAttributes.m_colorWriteMask[1] = green;
    ctx->m_colorBufferAttributes.m_colorWriteMask[2] = blue;
    ctx->m_colorBufferAttributes.m_colorWriteMask[3] = alpha;
#endif
  }
}

/*! \brief Helper function to reset the color mask.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the color mask
 *  to the previous mask. Otherwise, all four mask components are set to GL_TRUE.
 *  \sa GLColorMask */
inline void GLResetColorMask()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLColorMask( ctx->m_previousAttributes.m_colorWriteMask[0]
             , ctx->m_previousAttributes.m_colorWriteMask[1]
             , ctx->m_previousAttributes.m_colorWriteMask[2]
             , ctx->m_previousAttributes.m_colorWriteMask[3] );
#else
  GLColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
#endif
}

//! Replacement of glColorMaterial
/** This function asserts on the result before returning.  */
inline  void  GLColorMaterial( GLenum face, GLenum mode )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_colorMaterialFace = ctx->m_lightingAttributes.m_colorMaterialFace;
  ctx->m_previousAttributes.m_colorMaterialMode = ctx->m_lightingAttributes.m_colorMaterialMode;
# endif
  if (    ( ctx->m_lightingAttributes.m_colorMaterialFace != face )
      ||  ( ctx->m_lightingAttributes.m_colorMaterialMode != mode ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glColorMaterial( face, mode );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_lightingAttributes.m_colorMaterialFace = face;
    ctx->m_lightingAttributes.m_colorMaterialMode = mode;
#endif
  }
}

/*! \brief Helper function to reset the color material mode.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the color
 *  material mode to the previous mode. Otherwise, it is set to GL_AMBIENT_AND_DIFFUSE for
 *  GL_FRONT_AND_BACK faces.
 *  \sa GLColorMaterial */
inline void GLResetColorMaterialMode()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLColorMaterial( ctx->m_previousAttributes.m_colorMaterialFace
                 , ctx->m_previousAttributes.m_colorMaterialMode );
#else
  GLColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
#endif
}

//! Replacement of glColorPointer
/** This function caches the color pointer settings and asserts on the result before returning.  */
inline  void  GLColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  GLContext::VertexArrayParameters parameters( size, type, stride, pointer );
  if ( ctx->m_vertexArrayAttributes.m_colorArrayParams != parameters )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glColorPointer( size, type, stride, pointer );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_vertexArrayAttributes.m_colorArrayParams = parameters;
#endif
  }
}

//! Replacement of glCompressedTexImage1D
/** This function hides the extension character of glCompressedTexImage1D, updates the active texture,
  * and asserts on the result before returning.  */
inline  void  GLCompressedTexImage1D( GLenum target, int level, GLenum internalformat, GLsizei width, int border, GLsizei imageSize, const void *data )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glCompressedTexImage1D );
  glContext()->m_glFunctions->glCompressedTexImage1D( target, level, internalformat, width, border, imageSize, data );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCompressedTexImage2D
/** This function hides the extension character of glCompressedTexImage2D, updates the active texture,
  * and asserts on the result before returning.  */
inline  void  GLCompressedTexImage2D( GLenum target, int level, GLenum internalformat, GLsizei width, GLsizei height, int border, GLsizei imageSize, const void *data )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glCompressedTexImage2D );
  glContext()->m_glFunctions->glCompressedTexImage2D( target, level, internalformat, width, height, border, imageSize, data );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCompressedTexImage3D
/** This function hides the extension character of glCompressedTexImage3D, updates the active texture,
  * and asserts on the result before returning.  */
inline  void  GLCompressedTexImage3D( GLenum target, int level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, int border, GLsizei imageSize, const void *data )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glCompressedTexImage3D );
  glContext()->m_glFunctions->glCompressedTexImage3D( target, level, internalformat, width, height, depth, border, imageSize, data );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCopyPixels
/** This function asserts on the result before returning.  */
inline  void  GLCopyPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum type )
{
  NVSG_GL_STATS_FCT_COUNT();
  glCopyPixels( x, y, width, height, type );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCopyTexImage1D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLCopyTexImage1D( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glCopyTexImage1D( target, level, internalFormat, x, y, width, border );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCopyTexImage2D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLCopyTexImage2D( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glCopyTexImage2D( target, level, internalFormat, x, y, width, height, border );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCopyTexSubImage1D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLCopyTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glCopyTexSubImage1D( target,level, xoffset, x, y, width );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCopyTexSubImage2D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLCopyTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glCopyTexSubImage2D( target,level, xoffset, yoffset, x, y, width, height );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glCullFace
/** This function caches the cull face setting and asserts on the result before returning.  */
inline  void  GLCullFace( GLenum mode )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_cullFaceMode = ctx->m_polygonAttributes.m_cullFaceMode;
# endif
  if ( ctx->m_polygonAttributes.m_cullFaceMode != mode )
#endif
  {
    if ( ! ctx->m_locks.m_cullFaceMode )
    {
      NVSG_GL_STATS_FCT_COUNT();
      glCullFace( mode );
      ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
      ctx->m_polygonAttributes.m_cullFaceMode = mode;
#endif
    }
  }
}

/*! \brief Helper function to reset the cull face mode.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the cull face
 *  mode to the previous mode. Otherwise, it is set to GL_BACK.
 *  \sa GLCullFace */
inline void GLResetCullFaceMode()
{
#if defined(GL_SUPPORT_RESET)
  GLCullFace( glContext()->m_previousAttributes.m_cullFaceMode );
#else
  GLCullFace( GL_BACK );
#endif
}

/*! \brief Helper function to set the cull face mode and lock it.
 *  \param mode The cull face mode to set and lock.
 *  \remarks This function sets the cull face mode and locks this state. Any subsequent calls to
 *  GLCullFace will not change that value, until the next call to GLCullFaceUnlock.
 *  \sa GLCullFace, GLCullFaceUnlock */
inline void GLCullFaceLock( GLenum mode )
{
  NVSG_ASSERT( ! glContext()->m_locks.m_cullFaceMode );
  GLCullFace( mode );
  glContext()->m_locks.m_cullFaceMode = true;
}

/*! \brief Helper function to unlock the cull face and set it.
 *  \param mode The cull face mode to set, after unlocking.
 *  \remarks This function unlocks the cull face mode and sets it to \a mode. After a call to
 *  GLCullFaceLock, this function is needed to unlock the cull face mode again.
 *  \sa GLCullFace, GLCullFaceLock */
inline void GLCullFaceUnlock( GLenum mode )
{
  glContext()->m_locks.m_cullFaceMode = false;
  GLCullFace( mode );
}

//! Replacement of glDeleteLists
/** This function updates some caches and asserts on the result before returning.  */
inline  void  GLDeleteLists( GLuint list, GLsizei range )
{
  NVSG_GL_STATS_DELETE_LISTS( list, range );
  glContext()->removeLists(range, list);
  glDeleteLists( list, range );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDeleteOcclusionQueriesNV
/** This function hides the extension character of glDeleteOcclusionQueriesNV, updates some caches,
  * and asserts on the result before returning.  */
inline  void  GLDeleteOcclusionQueries( GLsizei n, const GLuint *ids )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glDeleteOcclusionQueriesNV );
  ctx->removeOcclusionQueries(n, ids);
  ctx->m_glFunctions->glDeleteOcclusionQueriesNV( n, ids );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDeleteProgramsNV
/** This function hides the extension character of glDeleteProgramsNV, updates some caches,
  * and asserts on the result before returning.  */
inline  void  GLDeletePrograms( GLsizei n, const GLuint *ids )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glDeleteProgramsNV );
  ctx->removePrograms(n, ids);
  ctx->m_glFunctions->glDeleteProgramsNV( n, ids );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDeleteQueriesARB
/** This function hides the extension character of glDeleteQueriesARB, updates some caches,
  * and asserts on the result before returning. */
inline  void  GLDeleteQueries( GLsizei n, const GLuint *ids )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glDeleteQueriesARB );
  ctx->removeQueries( n, ids );
  ctx->m_glFunctions->glDeleteQueriesARB( n, ids );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDeleteTextures
/** This function updates some caches and asserts on the result before returning.  */
inline  void  GLDeleteTextures( GLsizei n, const GLuint *textures )
{
  NVSG_GL_STATS_DELETE_TEXTURES( n, textures );
  GLContext * ctx = glContext();
  ctx->removeTextures(n, textures);
  glDeleteTextures( n, textures );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDepthFunc
/** This function asserts on the result before returning.  */
inline  void  GLDepthFunc( GLenum func )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_depthFunc = ctx->m_depthBufferAttributes.m_depthFunc;
# endif
  if ( ctx->m_depthBufferAttributes.m_depthFunc != func )
#endif
  {
    NVSG_GL_STATS_DEPTH_FUNC( func );
    glDepthFunc( func );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_depthBufferAttributes.m_depthFunc = func;
#endif
  }
}

/*! \brief Helper function to reset the depth func.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the depth func
 *  to the previous value. Otherwise, it is set to GL_LESS.
 *  \sa GLDepthFunc */
inline void GLResetDepthFunc()
{
#if defined(GL_SUPPORT_RESET)
  GLDepthFunc( glContext()->m_previousAttributes.m_depthFunc );
#else
  GLDepthFunc( GL_LESS );
#endif
}

//! Replacement of glDepthMask
/** This function asserts on the result before returning.  */
inline  void  GLDepthMask( GLboolean flag )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_depthWriting = ctx->m_depthBufferAttributes.m_depthWriting;
# endif
  if ( ctx->m_depthBufferAttributes.m_depthWriting != flag )
#endif
  {
    NVSG_GL_STATS_DEPTH_MASK( flag );
    glDepthMask( flag );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_depthBufferAttributes.m_depthWriting = flag;
#endif
  }
}

/*! \brief Helper function to reset the depth mask.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the depth mask
 *  to the previous value. Otherwise, it is set to GL_TRUE.
 *  \sa GLDepthMask */
inline void GLResetDepthMask()
{
#if defined(GL_SUPPORT_RESET)
  GLDepthMask( glContext()->m_previousAttributes.m_depthWriting );
#else
  GLDepthMask( GL_TRUE );
#endif
}

//! Replacement of glDepthRange
/** This function asserts on the result before returning.  */
inline  void  GLDepthRange( GLclampd znear, GLclampd zfar )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_depthRangeNear = ctx->m_viewportAttributes.m_depthRangeNear;
  ctx->m_previousAttributes.m_depthRangeFar  = ctx->m_viewportAttributes.m_depthRangeFar;
# endif
  if (    ( ctx->m_viewportAttributes.m_depthRangeNear != znear )
      ||  ( ctx->m_viewportAttributes.m_depthRangeFar  != zfar ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glDepthRange( znear, zfar );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_viewportAttributes.m_depthRangeNear = znear;
    ctx->m_viewportAttributes.m_depthRangeFar  = zfar;
#endif
  }
}

/*! \brief Helper function to reset the depth range.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the depth range
 *  to the previous values. Otherwise, it sets \a znear to 0.0 and \a zfar to 1.0.
 *  \sa GLDepthRange */
inline void GLResetDepthRange()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLDepthRange( ctx->m_previousAttributes.m_depthRangeNear
              , ctx->m_previousAttributes.m_depthRangeFar );
#else
  GLDepthRange( 0.0, 1.0 );
#endif
}

#if !defined(DOXYGEN_IGNORE)  // no need to document internal helper functions/macros
#if defined( GL_CACHING )
inline void _GLDisableCached( GLenum cap, GLboolean * cache )
{
  if ( *cache )
  {
    NVSG_ASSERT( glIsEnabled( cap ) );
    _GLDisable( cap );
    *cache = GL_FALSE;
  }
}
inline void _GLEnableCached( GLenum cap, GLboolean * cache )
{
  if ( !*cache )
  {
    NVSG_ASSERT( ! glIsEnabled( cap ) );
    _GLEnable( cap );
    *cache = GL_TRUE;
  }
}
inline GLboolean _GLIsEnabledCached( GLenum cap, GLboolean cache )
{
  NVSG_ASSERT( cache == glIsEnabled( cap ) );
  return( cache );
}
# if defined(GL_SUPPORT_RESET)
inline void _GLEnableCached( GLenum cap, GLboolean * cache, GLboolean * prev )
{
  *prev = *cache;
  _GLEnableCached( cap, cache );
}
inline void _GLDisableCached( GLenum cap, GLboolean * cache, GLboolean * prev )
{
  *prev = *cache;
  _GLDisableCached( cap, cache );
}
#  define GL_DISABLE( cap, cache, prev )  _GLDisableCached( cap, cache, prev )
#  define GL_ENABLE( cap, cache, prev )   _GLEnableCached( cap, cache, prev )
#  define GL_IS_ENABLED( cap, cache )      _GLIsEnabledCached( cap, cache )
# else
#  define GL_DISABLE( cap, cache, prev )   _GLDisable( cap )
#  define GL_ENABLE( cap, cache, prev )    _GLEnable( cap )
#  define GL_IS_ENABLED( cap, cache )      _GLIsEnabled( cap )
# endif
#else
# define GL_DISABLE( cap, cache, prev )  _GLDisable( cap )
# define GL_ENABLE( cap, cache, prev )   _GLEnable( cap )
#endif

#if defined(GL_SUPPORT_RESET)
# define GL_RESET( cap, cache, prev ) ( prev ? _GLEnableCached( cap, cache ) : _GLDisableCached( cap, cache ) )
#else
# define GL_RESET( cap, cache, prev ) _GLDisable( cap );
#endif
#endif // DOXYGEN_IGNORE


//! Cached version of GLDisable( GL_ALPHA_TEST )
/** This function caches the GL_ALPHA_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_ALPHA_TEST) directly. */
inline  void  GLDisableAlphaTest( void )
{
  GL_DISABLE( GL_ALPHA_TEST, &glContext()->m_colorBufferEnableAttributes.m_alphaTest
            , &glContext()->m_previousAttributes.m_alphaTest );
}

//! Cached version of GLDisable( GL_AUTO_NORMAL )
/** This function caches the GL_AUTO_NORMAL state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_AUTO_NORMAL) directly. */
inline  void  GLDisableAutoNormal( void )
{
  GL_DISABLE( GL_AUTO_NORMAL, &glContext()->m_evalAttributes.m_autoNormal
            , &glContext()->m_previousAttributes.m_autoNormal );
}

//! Cached version of GLDisable( GL_BLEND )
/** This function caches the GL_BLEND state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_BLEND) directly. */
inline  void  GLDisableBlend( void )
{
  GL_DISABLE( GL_BLEND, &glContext()->m_colorBufferEnableAttributes.m_blend
            , &glContext()->m_previousAttributes.m_blend );
}

//! Cached version of GLDisable( GL_CLIP_PLANEi )
/** This function caches the GL_CLIP_PLANEi state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_CLIP_PLANEi) directly. */
inline  void  GLDisableClipPlane( GLenum plane )
{
#if defined( GL_CACHING )
  NVSG_ASSERT( plane-GL_CLIP_PLANE0 < glContext()->m_transformEnableAttributes.m_clipPlanes.size() );
#endif
  GL_DISABLE( plane, &glContext()->m_transformEnableAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0]
            , &glContext()->m_previousAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0] );
}

//! Cached version of GLDisable( GL_COLOR_LOGIC_OP )
/** This function caches the GL_COLOR_LOGIC_OP state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_COLOR_LOGIC_OP) directly. */
inline  void  GLDisableColorLogicOp( void )
{
  GL_DISABLE( GL_COLOR_LOGIC_OP, &glContext()->m_colorBufferEnableAttributes.m_colorLogicOp
            , &glContext()->m_previousAttributes.m_colorLogicOp );
}

//! Cached version of GLDisable( GL_COLOR_MATERIAL )
/** This function caches the GL_COLOR_MATERIAL state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_COLOR_MATERIAL) directly. */
inline  void  GLDisableColorMaterial( void )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_colorMaterial = ctx->m_lightingAttributes.m_colorMaterial;
# endif
  if ( ctx->m_lightingAttributes.m_colorMaterial )
  {
    NVSG_ASSERT( glIsEnabled( GL_COLOR_MATERIAL ) );
#endif
    _GLDisable( GL_COLOR_MATERIAL );
#if defined( GL_CACHING )
    ctx->m_lightingAttributes.m_colorMaterial = GL_FALSE;
    glGetMaterialfv( GL_BACK, GL_AMBIENT, &ctx->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_BACK][0] );
    glGetMaterialfv( GL_FRONT, GL_AMBIENT, &ctx->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0] );
    glGetMaterialfv( GL_BACK, GL_DIFFUSE, &ctx->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0] );
    glGetMaterialfv( GL_FRONT, GL_DIFFUSE, &ctx->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0] );
  }
#endif
}

//! Cached version of GLDisable( GL_CULL_FACE )
/** This function is caches the GL_CULL_FACE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_CULL_FACE) directly. */
inline  void  GLDisableCullFace( void )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_cullFace = ctx->m_polygonEnableAttributes.m_cullFace;
# endif
  if ( ctx->m_polygonEnableAttributes.m_cullFace )
#endif
  {
    if ( ! ctx->m_locks.m_cullFace )
    {
#if defined( GL_CACHING )
      NVSG_ASSERT( glIsEnabled( GL_CULL_FACE ) );
#endif
      _GLDisable( GL_CULL_FACE );
#if defined( GL_CACHING )
      ctx->m_polygonEnableAttributes.m_cullFace = GL_FALSE;
#endif
    }
  }
}

/*! \brief Helper function to disable the cull face and lock it.
 *  \remarks This function disables face culling and locks it. Any subsequent calls to
 *  GLEnableCullFace, GLDisableCullFace, GLEnable(GL_CULL_FACE), or GLDisable(GL_CULL_FACE) will not
 *  change that setting, until the next call to GLDisableCullFaceUnlock or GLEnableCullFaceUnlock.
 *  \sa GLDisableCullFace, GLDisableCullFaceUnlock, GLEnableCullFaceLock */
inline void GLDisableCullFaceLock( void )
{
  NVSG_ASSERT( ! glContext()->m_locks.m_cullFace );
  GLDisableCullFace();
  glContext()->m_locks.m_cullFace = true;
}

/*! \brief Helper function to unlock the cull face and set it.
 *  \remarks This function unlocks face culling and disables it.
 *  \sa GLDisableCullFace, GLDisableCullFaceLock, GLEnableCullFaceUnlock */
inline void GLDisableCullFaceUnlock( void )
{
  NVSG_ASSERT( glContext()->m_locks.m_cullFace );
  glContext()->m_locks.m_cullFace = false;
  GLDisableCullFace();
}

//! Cached version of GLDisable( GL_DEPTH_TEST )
/** This function caches the GL_DEPTH_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_DEPTH_TEST) directly. */
inline  void  GLDisableDepthTest( void )
{
  GL_DISABLE( GL_DEPTH_TEST, &glContext()->m_depthBufferEnableAttributes.m_depthTesting
            , &glContext()->m_previousAttributes.m_depthTesting );
}

//! Cached version of GLDisable( GL_DITHER )
/** This function caches the GL_DITHER state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_DITHER) directly. */
inline  void  GLDisableDither( void )
{
  GL_DISABLE( GL_DITHER, &glContext()->m_colorBufferEnableAttributes.m_dither
            , &glContext()->m_previousAttributes.m_dither );
}

//! Cached version of GLDisable( GL_FOG )
/** This function caches the GL_DITHER state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_FOG) directly. */
inline  void  GLDisableFog( void )
{
  GL_DISABLE( GL_FOG, &glContext()->m_fogEnableAttributes.m_fog
            , &glContext()->m_previousAttributes.m_fog );
}

//! Cached version of GLDisable( GL_LIGHTi )
/** This function caches the GL_LIGHTi state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LIGHTi) directly. */
inline  void  GLDisableLight( GLenum light )
{
#if defined( GL_CACHING )
  NVSG_ASSERT( light-GL_LIGHT0 < glContext()->m_lightingEnableAttributes.m_lights.size() );
#endif
  GL_DISABLE( light, &glContext()->m_lightingEnableAttributes.m_lights[light-GL_LIGHT0]
            , &glContext()->m_previousAttributes.m_lights[light-GL_LIGHT0] );
}

//! Cached version of GLDisable( GL_LIGHTING )
/** This function is caches the GL_LIGHTING state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LIGHTING) directly. */
inline  void  GLDisableLighting( void )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lighting = ctx->m_lightingEnableAttributes.m_lighting;
# endif
  if ( ctx->m_lightingEnableAttributes.m_lighting )
#endif
  {
    if ( ! ctx->m_locks.m_lighting )
    {
#if defined( GL_CACHING )
      NVSG_ASSERT( glIsEnabled( GL_LIGHTING ) );
#endif
      _GLDisable( GL_LIGHTING );
#if defined( GL_CACHING )
      ctx->m_lightingEnableAttributes.m_lighting = GL_FALSE;
#endif
    }
  }
}

/*! \brief Helper function to disable lighting and lock it.
 *  \remarks This function disables lighting and locks it. Any subsequent calls to GLEnableLighting,
 *  GLDisableLighting, GLEnable(GL_LIGHTING), or GLDisable(GL_LIGHTING) will not change that
 *  setting, until the next call to GLDisableLightingUnlock or GLEnableLightingUnlock.
 *  \sa GLDisableLighting, GLDisableLightingUnlock, GLEnableLightingLock */
inline void GLDisableLightingLock( void )
{
  NVSG_ASSERT( ! glContext()->m_locks.m_lighting );
  GLDisableLighting();
  glContext()->m_locks.m_lighting = true;
}

/*! \brief Helper function to unlock the lighting and disable it.
 *  \remarks This function unlocks lighting and disables it.
 *  \sa GLDisableLighting, GLDisableLightingLock, GLEnableLightingUnlock */
inline void GLDisableLightingUnlock( void )
{
  NVSG_ASSERT( glContext()->m_locks.m_lighting );
  glContext()->m_locks.m_lighting = false;
  GLDisableLighting();
}

//! Cached version of GLDisable( GL_LINE_SMOOTH )
/** This function is caches the GL_LINE_SMOOTH state.
* It is faster to call this method instead of calling GL[Disable|Enable](GL_LINE_SMOOTH) directly. */
inline  void  GLDisableLineSmooth( void )
{
  GL_DISABLE( GL_LINE_SMOOTH, &glContext()->m_lineEnableAttributes.m_smooth
            , &glContext()->m_previousAttributes.m_lineSmooth );
}

//! Cached version of GLDisable( GL_LINE_STIPPLE )
/** This function is caches the GL_LINE_STIPPLE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LINE_STIPPLE) directly. */
inline  void  GLDisableLineStipple( void )
{
  GL_DISABLE( GL_LINE_STIPPLE, &glContext()->m_lineEnableAttributes.m_stipple
            , &glContext()->m_previousAttributes.m_lineStipple );
}

//! Cached version of GLDisable( GL_LOGIC_OP )
/** This function caches the GL_LOGIC_OP state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LOGIC_OP) directly. */
inline  void  GLDisableLogicOp( void )
{
  GL_DISABLE( GL_LOGIC_OP, &glContext()->m_colorBufferEnableAttributes.m_logicOp
            , &glContext()->m_previousAttributes.m_logicOp );
}

//! Cached version of GLDisable( GL_NORMALIZE )
/** This function is caches the GL_NORMALIZE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_NORMALIZE) directly. */
inline  void  GLDisableNormalize( void )
{
  GL_DISABLE( GL_NORMALIZE, &glContext()->m_transformEnableAttributes.m_normalize
            , &glContext()->m_previousAttributes.m_normalize );
}

//! Cached version of GLDisable( GL_POINT_SMOOTH )
/** This function is caches the GL_POINT_SMOOTH state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POINT_SMOOTH) directly. */
inline  void  GLDisablePointSmooth( void )
{
  GL_DISABLE( GL_POINT_SMOOTH, &glContext()->m_pointEnableAttributes.m_smooth
            , &glContext()->m_previousAttributes.m_pointSmooth );
}

//! Cached version of GLDisable( GL_POLYGON_OFFSET_FILL )
/** This function is caches the GL_POLYGON_OFFSET_FILL state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_OFFSET_FILL) directly. */
inline  void  GLDisablePolygonOffsetFill( void )
{
  GL_DISABLE( GL_POLYGON_OFFSET_FILL, &glContext()->m_polygonEnableAttributes.m_offsetFill
            , &glContext()->m_previousAttributes.m_offsetFill );
}

//! Cached version of GLDisable( GL_POLYGON_OFFSET_LINE )
/** This function is caches the GL_POLYGON_OFFSET_LINE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_OFFSET_LINE) directly. */
inline  void  GLDisablePolygonOffsetLine( void )
{
  GL_DISABLE( GL_POLYGON_OFFSET_LINE, &glContext()->m_polygonEnableAttributes.m_offsetLine
            , &glContext()->m_previousAttributes.m_offsetLine );
}

//! Cached version of GLDisable( GL_POLYGON_OFFSET_POINT )
/** This function is caches the GL_POLYGON_OFFSET_POINT state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_OFFSET_POINT) directly. */
inline  void  GLDisablePolygonOffsetPoint( void )
{
  GL_DISABLE( GL_POLYGON_OFFSET_POINT, &glContext()->m_polygonEnableAttributes.m_offsetPoint
            , &glContext()->m_previousAttributes.m_offsetPoint );
}

//! Cached version of GLDisable( GL_POLYGON_SMOOTH )
/** This function caches the GL_POLYGON_SMOOTH state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_SMOOTH) directly. */
inline  void  GLDisablePolygonSmooth( void )
{
  GL_DISABLE( GL_POLYGON_SMOOTH, &glContext()->m_polygonEnableAttributes.m_smooth
            , &glContext()->m_previousAttributes.m_polygonSmooth );
}

//! Cached version of GLDisable( GL_POLYGON_STIPPLE )
/** This function is caches the GL_POLYGON_STIPPLE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_STIPPLE) directly. */
inline  void  GLDisablePolygonStipple( void )
{
  GL_DISABLE( GL_POLYGON_STIPPLE, &glContext()->m_polygonEnableAttributes.m_stipple
            , &glContext()->m_previousAttributes.m_polygonStipple );
}

//! Cached version of GLDisable( GL_SCISSOR_TEST )
/** This function caches the GL_SCISSOR_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_SCISSOR_TEST) directly. */
inline  void  GLDisableScissorTest( void )
{
  GL_DISABLE( GL_SCISSOR_TEST, &glContext()->m_scissorEnableAttributes.m_scissorTest
            , &glContext()->m_previousAttributes.m_scissorTest );
}

//! Cached version of GLDisable( GL_STENCIL_TEST )
/** This function caches the GL_STENCIL_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_STENCIL_TEST) directly. */
inline  void  GLDisableStencilTest( void )
{
  GL_DISABLE( GL_STENCIL_TEST, &glContext()->m_stencilBufferEnableAttributes.m_stencilTest
            , &glContext()->m_previousAttributes.m_stencilTest );
}

//! Cached version of GLDisable( [GL_TEXTURE_1D|GL_TEXTURE_2D|GL_TEXTURE_3D|GL_TEXTURE_CUBE_MAP|GL_TEXTURE_RECTANGLE_NV] )
/** This function caches the texture target type per texture unit. It is faster to call this method instead of calling
  * GL[Disable|Enable]( [GL_TEXTURE_1D|GL_TEXTURE_2D|GL_TEXTURE_3D|GL_TEXTURE_CUBE_MAP|GL_TEXTURE_RECTANGLE_NV] ) directly. */
inline void GLDisableTexture( GLenum target )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  NVSG_ASSERT(   ( target == GL_TEXTURE_1D ) || ( target == GL_TEXTURE_2D ) || ( target == GL_TEXTURE_3D )
             ||  ( target == GL_TEXTURE_CUBE_MAP ) || ( target == GL_TEXTURE_RECTANGLE_NV ) );
  if ( ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_toActivate].m_texture[target] )
  {
    _GLUpdateActiveTexture();
    NVSG_ASSERT( glIsEnabled( target ) );
#endif
    _GLDisable( target );
#if defined( GL_CACHING )
    ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_active].m_texture[target] = GL_FALSE;
  }
#endif
}

//! Cached version of GLDisable( [GL_TEXTURE_GEN_S|GL_TEXTURE_GEN_T|GL_TEXTURE_GEN_R|GL_TEXTURE_GEN_Q] )
/** This function caches the texture generation type per texture unit. It is faster to call this method instead of calling
  * GL[Disable|Enable]( [GL_TEXTURE_GEN_S|GL_TEXTURE_GEN_T|GL_TEXTURE_GEN_R|GL_TEXTURE_GEN_Q] ) directly. */
inline void GLDisableTextureGen( GLenum target )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  NVSG_ASSERT(    ( target == GL_TEXTURE_GEN_S )
              ||  ( target == GL_TEXTURE_GEN_T )
              ||  ( target == GL_TEXTURE_GEN_R )
              ||  ( target == GL_TEXTURE_GEN_Q ) );
  if ( ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_toActivate].m_gen[target-GL_TEXTURE_GEN_S] )
  {
    _GLUpdateActiveTexture();
    NVSG_ASSERT( glIsEnabled( target ) );
#endif
    _GLDisable( target );
#if defined( GL_CACHING )
    ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_active].m_gen[target-GL_TEXTURE_GEN_S] = GL_FALSE;
  }
#endif
}

//! Replacement of glDisable
/** This function caches on some cap values and asserts on the result before returning. */
inline  void  GLDisable( GLenum cap )
{
#if defined( GL_CACHING )
  switch( cap )
  {
    case GL_ALPHA_TEST :
      GLDisableAlphaTest();
      break;
    case GL_AUTO_NORMAL :
      GLDisableAutoNormal();
      break;
    case GL_BLEND :
      GLDisableBlend();
      break;
    case GL_CLIP_PLANE0 :
    case GL_CLIP_PLANE1 :
    case GL_CLIP_PLANE2 :
    case GL_CLIP_PLANE3 :
    case GL_CLIP_PLANE4 :
    case GL_CLIP_PLANE5 :
      GLDisableClipPlane( cap );
      break;
    case GL_COLOR_LOGIC_OP :
      GLDisableColorLogicOp();
      break;
    case GL_COLOR_MATERIAL :
      GLDisableColorMaterial();
      break;
    case GL_CULL_FACE :
      GLDisableCullFace();
      break;
    case GL_DEPTH_TEST :
      GLDisableDepthTest();
      break;
    case GL_DITHER :
      GLDisableDither();
      break;
    case GL_FOG :
      GLDisableFog();
      break;
    case GL_LIGHT0 :
    case GL_LIGHT1 :
    case GL_LIGHT2 :
    case GL_LIGHT3 :
    case GL_LIGHT4 :
    case GL_LIGHT5 :
    case GL_LIGHT6 :
    case GL_LIGHT7 :
      GLDisableLight( cap );
      break;
    case GL_LIGHTING :
      GLDisableLighting();
      break;
    case GL_LINE_SMOOTH :
      GLDisableLineSmooth();
      break;
    case GL_LINE_STIPPLE :
      GLDisableLineStipple();
      break;
    case GL_LOGIC_OP :
      GLDisableLogicOp();
      break;
    case GL_NORMALIZE :
      GLDisableNormalize();
      break;
    case GL_POINT_SMOOTH :
      GLDisablePointSmooth();
      break;
    case GL_POLYGON_OFFSET_FILL :
      GLDisablePolygonOffsetFill();
      break;
    case GL_POLYGON_OFFSET_LINE :
      GLDisablePolygonOffsetLine();
      break;
    case GL_POLYGON_OFFSET_POINT :
      GLDisablePolygonOffsetPoint();
      break;
    case GL_POLYGON_SMOOTH :
      GLDisablePolygonSmooth();
      break;
    case GL_POLYGON_STIPPLE :
      GLDisablePolygonStipple();
      break;
    case GL_SCISSOR_TEST :
      GLDisableScissorTest();
      break;
    case GL_STENCIL_TEST :
      GLDisableStencilTest();
      break;
    case GL_TEXTURE_1D :
    case GL_TEXTURE_2D :
    case GL_TEXTURE_3D :
    case GL_TEXTURE_CUBE_MAP :
    case GL_TEXTURE_RECTANGLE_NV :
      GLDisableTexture( cap );
      break;
    case GL_TEXTURE_GEN_S :
    case GL_TEXTURE_GEN_T :
    case GL_TEXTURE_GEN_R :
    case GL_TEXTURE_GEN_Q :
      GLDisableTextureGen( cap );
      break;
    default :
      _GLDisable( cap );
      break;
  }
#else
  switch( cap )
  {
    case GL_CULL_FACE :
      GLDisableCullFace();
      break;
    default :
      _GLEnable( cap );
      break;
  }
#endif
}

#if !defined(DOXYGEN_IGNORE)
#if defined( GL_CACHING )
inline void _GLDisableClientStateCached( GLenum cap, GLboolean * cache )
{
  if ( *cache )
  {
    NVSG_ASSERT( glIsEnabled( cap ) );
    _GLDisableClientState( cap );
    *cache = GL_FALSE;
  }
}
#define GL_DISABLE_CLIENT_STATE( cap, cache )   _GLDisableClientStateCached( cap, cache )
#else
#define GL_DISABLE_CLIENT_STATE( cap, cache )   _GLDisableClientState( cap );
#endif
#endif // DOXYGEN_IGNORE

//! Cached version of GLDisableClientState( GL_COLOR_ARRAY )
/** This function caches the GL_COLOR_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_COLOR_ARRAY ) directly. */
inline  void  GLDisableColorArray( void )
{
  GL_DISABLE_CLIENT_STATE( GL_COLOR_ARRAY, &glContext()->m_vertexArrayAttributes.m_colorArray );
}

//! Cached version of GLDisableClientState( GL_FOG_COORDINATE_ARRAY )
/** This function caches the GL_FOG_COORDINATE_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_FOG_COORDINATE_ARRAY ) directly. */
inline  void  GLDisableFogCoordinateArray( void )
{
  GL_DISABLE_CLIENT_STATE( GL_FOG_COORDINATE_ARRAY, &glContext()->m_vertexArrayAttributes.m_fogCoordArray );
}

//! Cached version of GLDisableClientState( GL_NORMAL_ARRAY )
/** This function caches the GL_NORMAL_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_NORMAL_ARRAY ) directly. */
inline  void  GLDisableNormalArray( void )
{
  GL_DISABLE_CLIENT_STATE( GL_NORMAL_ARRAY, &glContext()->m_vertexArrayAttributes.m_normalArray );
}

//! Cached version of GLDisableClientState( GL_SECONDARY_COLOR_ARRAY )
/** This function caches the GL_SECONDARY_COLOR_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_SECONDARY_COLOR_ARRAY ) directly. */
inline  void  GLDisableSecondaryColorArray( void )
{
  GL_DISABLE_CLIENT_STATE( GL_SECONDARY_COLOR_ARRAY, &glContext()->m_vertexArrayAttributes.m_secondaryColorArray );
}

//! Cached version of GLDisableClientState( GL_TEXTURE_COORD_ARRAY )
/** This function caches the GL_TEXTURE_COORD_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_TEXTURE_COORD_ARRAY ) directly. */
inline  void  GLDisableTextureCoordArray( void )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  if ( ctx->m_vertexArrayAttributes.m_textureCoordArray[ctx->m_vertexArrayAttributes.m_clientTextureToActivate] )
  {
    _GLUpdateClientActiveTexture();
    NVSG_ASSERT( glIsEnabled( GL_TEXTURE_COORD_ARRAY ) );
#endif
    _GLDisableClientState( GL_TEXTURE_COORD_ARRAY );
#if defined( GL_CACHING )
    ctx->m_vertexArrayAttributes.m_textureCoordArray[ctx->m_vertexArrayAttributes.m_clientTextureActive] = GL_FALSE;
  }
#endif
}

//! Cached version of GLDisableClientState( GL_VERTEX_ARRAY )
/** This function caches the GL_VERTEX_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_VERTEX_ARRAY ) directly. */
inline  void  GLDisableVertexArray( void )
{
  GL_DISABLE_CLIENT_STATE( GL_VERTEX_ARRAY, &glContext()->m_vertexArrayAttributes.m_vertexArray );
}

//! Replacement of glDisableClientState
/** This function caches on some cap values and asserts on the result before returning. */
inline  void  GLDisableClientState( GLenum array )
{
#if defined( GL_CACHING )
  switch( array )
  {
    case GL_COLOR_ARRAY :
      GLDisableColorArray();
      break;
    case GL_FOG_COORDINATE_ARRAY :
      GLDisableFogCoordinateArray();
      break;
    case GL_NORMAL_ARRAY :
      GLDisableNormalArray();
      break;
    case GL_SECONDARY_COLOR_ARRAY :
      GLDisableSecondaryColorArray();
      break;
    case GL_TEXTURE_COORD_ARRAY :
      GLDisableTextureCoordArray();
      break;
    case GL_VERTEX_ARRAY :
      GLDisableVertexArray();
      break;
    default :
      _GLDisableClientState( array );
      break;
  }
#else
  _GLDisableClientState( array );
#endif
}

//! Replacement of glDrawArrays
/** This function asserts on the result before returning.  */
inline  void  GLDrawArrays( GLenum mode, GLint first, GLsizei count )
{
  NVSG_GL_STATS_FCT_COUNT();
  glDrawArrays( mode, first, count );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDrawBuffer
/** This function asserts on the result before returning.  */
inline  void  GLDrawBuffer( GLenum mode ) 
{ 
  NVSG_GL_STATS_FCT_COUNT();
  glDrawBuffer( mode );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDrawElements
/** This function asserts on the result before returning.  */
inline  void  GLDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )
{
  NVSG_GL_STATS_DRAW_ELEMENTS( mode, count, type, indices );
  ASSERT_GL_ARRAY_BUFFER_BINDING();
  ASSERT_GL_ELEMENT_ARRAY_BUFFER_BINDING();
  glDrawElements( mode, count, type, indices );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMultiDrawElements
/** This function asserts on the result before returning.  */
inline  void  GLMultiDrawElements( GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount )
{
  NVSG_GL_STATS_FCT_COUNT();
  ASSERT_GL_ARRAY_BUFFER_BINDING();
  ASSERT_GL_ELEMENT_ARRAY_BUFFER_BINDING();
  glContext()->m_glFunctions->glMultiDrawElements( mode, count, type, indices, primcount );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDrawPixels
/** This function asserts on the result before returning.  */
inline  void  GLDrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels )
{
  NVSG_GL_STATS_FCT_COUNT();
  glDrawPixels( width, height, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glDrawRangeElements
/** This function asserts on the result before returning.  */
inline  void  GLDrawRangeElements( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices )
{
  NVSG_GL_STATS_DRAW_RANGE_ELEMENTS( mode, start, end, count, type, indices );
  ASSERT_GL_ARRAY_BUFFER_BINDING();
  ASSERT_GL_ELEMENT_ARRAY_BUFFER_BINDING();
  glContext()->m_glFunctions->glDrawRangeElements( mode, start, end, count, type, indices );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEdgeFlagPointer
/** This function asserts on the result before returning.  */
inline  void  GLEdgeFlagPointer( GLsizei stride, const GLboolean *pointer )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEdgeFlagPointer( stride, pointer );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEdgeFlag
/** This function asserts on the result before returning.  */
inline  void  GLEdgeFlag( GLboolean flag )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEdgeFlag( flag );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glEdgeFlagv
/** This function asserts on the result before returning.  */
inline  void  GLEdgeFlagv( const GLboolean *flag )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEdgeFlagv( flag );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Cached version of GLEnable( GL_ALPHA_TEST )
/** This function caches the GL_ALPHA_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_ALPHA_TEST) directly. */
inline  void  GLEnableAlphaTest( void )
{
  GL_ENABLE( GL_ALPHA_TEST, &glContext()->m_colorBufferEnableAttributes.m_alphaTest
           , &glContext()->m_previousAttributes.m_alphaTest );
}

/*! \brief Helper function to reset the enable state of the alpha test.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the alpha test
 *  state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableAlphaTest, GLEnableAlphaTest, GLDisable, GLEnable */
inline void GLResetAlphaTest()
{
  GL_RESET( GL_ALPHA_TEST, &glContext()->m_colorBufferEnableAttributes.m_alphaTest
          , glContext()->m_previousAttributes.m_alphaTest );
}

//! Cached version of GLEnable( GL_AUTO_NORMAL )
/** This function caches the GL_AUTO_NORMAL state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_AUTO_NORMAL) directly. */
inline  void  GLEnableAutoNormal( void )
{
  GL_ENABLE( GL_AUTO_NORMAL, &glContext()->m_evalAttributes.m_autoNormal
           , &glContext()->m_previousAttributes.m_autoNormal );
}

/*! \brief Helper function to reset the enable state of the auto normal calculation.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the auto normal
 *  state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableAutoNormal, GLEnableAutoNormal, GLDisable, GLEnable */
inline void GLResetAutoNormal()
{
  GL_RESET( GL_AUTO_NORMAL, &glContext()->m_evalAttributes.m_autoNormal
          , glContext()->m_previousAttributes.m_autoNormal );
}

//! Cached version of GLEnable( GL_BLEND )
/** This function caches the GL_BLEND state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_BLEND) directly. */
inline  void  GLEnableBlend( void )
{
  GL_ENABLE( GL_BLEND, &glContext()->m_colorBufferEnableAttributes.m_blend
           , &glContext()->m_previousAttributes.m_blend );
}

/*! \brief Helper function to reset the enable state of the blending.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the blending
 *  state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableBlend, GLEnableBlend, GLDisable, GLEnable */
inline void GLResetBlend()
{
  GL_RESET( GL_BLEND, &glContext()->m_colorBufferEnableAttributes.m_blend
          , glContext()->m_previousAttributes.m_blend );
}

//! Cached version of GLEnable( GL_CLIP_PLANEi )
/** This function caches the GL_CLIP_PLANEi state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_CLIP_PLANEi) directly. */
inline  void  GLEnableClipPlane( GLenum plane )
{
#if defined( GL_CACHING )
  NVSG_ASSERT( plane-GL_CLIP_PLANE0 < glContext()->m_transformEnableAttributes.m_clipPlanes.size() );
#endif
  GL_ENABLE( plane, &glContext()->m_transformEnableAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0]
           , &glContext()->m_previousAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0] );
}

/*! \brief Helper function to reset the enable state of the clip plane \a plane.
 *  \param plane The id of the plane for which the the enable state is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the clip plane
 *  state for \a plane to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableClipPlane, GLEnableClipPlane, GLDisable, GLEnable */
inline void GLResetClipPlane( GLenum plane )
{
  GL_RESET( plane, &glContext()->m_transformEnableAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0]
          , glContext()->m_previousAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0] );
}

//! Cached version of GLEnable( GL_COLOR_LOGIC_OP )
/** This function caches the GL_COLOR_LOGIC_OP state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_COLOR_LOGIC_OP) directly. */
inline  void  GLEnableColorLogicOp( void )
{
  GL_ENABLE( GL_COLOR_LOGIC_OP, &glContext()->m_colorBufferEnableAttributes.m_colorLogicOp
           , &glContext()->m_previousAttributes.m_colorLogicOp );
}

/*! \brief Helper function to reset the enable state of the color logic operation.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the color logic
 *  operation state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableColorLogicOp, GLEnableColorLogicOp, GLDisable, GLEnable */
inline void GLResetColorLogicOp()
{
  GL_RESET( GL_COLOR_LOGIC_OP, &glContext()->m_colorBufferEnableAttributes.m_colorLogicOp
          , glContext()->m_previousAttributes.m_colorLogicOp );
}

//! Cached version of GLEnable( GL_COLOR_MATERIAL )
/** This function caches the GL_COLOR_MATERIAL state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_COLOR_MATERIAL) directly. */
inline  void  GLEnableColorMaterial( void )
{
  GL_ENABLE( GL_COLOR_MATERIAL, &glContext()->m_lightingAttributes.m_colorMaterial
           , &glContext()->m_previousAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the enable state of the color material.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the color
 *  material state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableColorMaterial, GLEnableColorMaterial, GLDisable, GLEnable */
inline  void  GLResetColorMaterial()
{
  GL_RESET( GL_COLOR_MATERIAL, &glContext()->m_lightingAttributes.m_colorMaterial
          , glContext()->m_previousAttributes.m_colorMaterial );
}

//! Cached version of GLEnable( GL_CULL_FACE )
/** This function caches the GL_CULL_FACE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_CULL_FACE) directly. */
inline  void  GLEnableCullFace( void )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_cullFace = ctx->m_polygonEnableAttributes.m_cullFace;
# endif
  if ( ! ctx->m_polygonEnableAttributes.m_cullFace )
#endif
  {
    if ( ! ctx->m_locks.m_cullFace )
    {
#if defined( GL_CACHING )
      NVSG_ASSERT( ! glIsEnabled( GL_CULL_FACE ) );
#endif
      _GLEnable( GL_CULL_FACE );
#if defined( GL_CACHING )
      ctx->m_polygonEnableAttributes.m_cullFace = GL_TRUE;
#endif
    }
  }
}

/*! \brief Helper function to reset the enable state of face culling.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the face
 *  culling state to it's previous state. Otherwise, it is enabled. Note, that this enabling face
 *  culling by default is NVSG specific and not like in OpenGL.
 *  \sa GLDisableCullFace, GLEnableCullFace, GLDisable, GLEnable */
inline void GLResetCullFace()
{
#if defined(GL_SUPPORT_RESET)
  glContext()->m_previousAttributes.m_cullFace ? GLEnableCullFace() : GLDisableCullFace();
#else
  GLEnableCullFace(); // NVSG specific! Not OpenGL default!
#endif
}

/*! \brief Helper function to enable face culling and lock it.
 *  \remarks This function enables face culling and locks it. Any subsequent calls to
 *  GLEnableCullFace, GLDisableCullFace, GLEnable(GL_CULL_FACE), or GLDisable(GL_CULL_FACE) will not
 *  change that setting, until the next call to GLEnableCullFaceUnlock or GLDisableCullFaceUnlock.
 *  \sa GLEnableCullFace, GLDisableCullFaceUnlock, GLEnableCullFaceUnlock */
inline void GLEnableCullFaceLock( void )
{
  NVSG_ASSERT( ! glContext()->m_locks.m_cullFace );
  GLEnableCullFace();
  glContext()->m_locks.m_cullFace = true;
}

/*! \brief Helper function to unlock the face culling and enable it.
 *  \remarks This function unlocks face culling and enables it.
 *  \sa GLEnableCullFace, GLEnableCullFaceLock, GLDisableCullFaceUnlock */
inline void GLEnableCullFaceUnlock( void )
{
  NVSG_ASSERT( glContext()->m_locks.m_cullFace );
  glContext()->m_locks.m_cullFace = false;
  GLEnableCullFace();
}

//! Cached version of GLEnable( GL_DEPTH_TEST )
/** This function caches the GL_DEPTH_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_DEPTH_TEST) directly. */
inline  void  GLEnableDepthTest( void )
{
  GL_ENABLE( GL_DEPTH_TEST, &glContext()->m_depthBufferEnableAttributes.m_depthTesting
           , &glContext()->m_previousAttributes.m_depthTesting );
}

/*! \brief Helper function to reset the enable state of depth testing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the depth test
 *  state to it's previous state. Otherwise, it is enabled. Note, that this enabling depth test by
 *  default is NVSG specific and not like in OpenGL.
 *  \sa GLDisableDepthTest, GLEnableDepthTest, GLDisable, GLEnable */
inline void GLResetDepthTest()
{
#if defined(GL_SUPPORT_RESET)
  glContext()->m_previousAttributes.m_depthTesting ? GLEnableDepthTest() : GLDisableDepthTest();
#else
  GLEnableDepthTest();  // NVSG specific! Not OpenGL default!
#endif
}

//! Cached version of GLEnable( GL_DITHER )
/** This function caches the GL_DITHER state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_DITHER) directly. */
inline  void  GLEnableDither( void )
{
  GL_ENABLE( GL_DITHER, &glContext()->m_colorBufferEnableAttributes.m_dither
           , &glContext()->m_previousAttributes.m_dither );
}

/*! \brief Helper function to reset the enable state of dithering.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the dither
 *  state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableDither, GLEnableDither, GLDisable, GLEnable */
inline void GLResetDither()
{
  GL_RESET( GL_DITHER, &glContext()->m_colorBufferEnableAttributes.m_dither
          , glContext()->m_previousAttributes.m_dither );
}

//! Cached version of GLEnable( GL_FOG )
/** This function caches the GL_DITHER state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_FOG) directly. */
inline  void  GLEnableFog( void )
{
  GL_ENABLE( GL_FOG, &glContext()->m_fogEnableAttributes.m_fog
           , &glContext()->m_previousAttributes.m_fog );
}

/*! \brief Helper function to reset the the enable state of fogging.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the fog state
 *  to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableFog, GLEnableFog, GLDisable, GLEnable */
inline void GLResetFog()
{
  GL_RESET( GL_FOG, &glContext()->m_fogEnableAttributes.m_fog
          , glContext()->m_previousAttributes.m_fog );
}

//! Cached version of GLEnable( GL_LIGHTi )
/** This function caches the GL_LIGHTi state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LIGHTi) directly. */
inline  void  GLEnableLight( GLenum light )
{
#if defined( GL_CACHING )
  NVSG_ASSERT( light-GL_LIGHT0 < glContext()->m_lightingEnableAttributes.m_lights.size() );
#endif
  GL_ENABLE( light, &glContext()->m_lightingEnableAttributes.m_lights[light-GL_LIGHT0]
           , &glContext()->m_previousAttributes.m_lights[light-GL_LIGHT0] );
}

/*! \brief Helper function to reset the enable state of the light \a light.
 *  \param light The id of the light for which the enable state is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the light state
 *  to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableLight, GLEnableLight, GLDisable, GLEnable */
inline void GLResetLight( GLenum light )
{
  GL_RESET( light, &glContext()->m_lightingEnableAttributes.m_lights[light-GL_LIGHT0]
          , glContext()->m_previousAttributes.m_lights[light-GL_LIGHT0] );
}

//! Cached version of GLEnable( GL_LIGHTING )
/** This function caches the GL_LIGHTING state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LIGHTING) directly. */
inline  void  GLEnableLighting( void )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lighting = ctx->m_lightingEnableAttributes.m_lighting;
# endif
  if ( ! ctx->m_lightingEnableAttributes.m_lighting )
#endif
  {
    if ( ! ctx->m_locks.m_lighting )
    {
#if defined( GL_CACHING )
      NVSG_ASSERT( ! glIsEnabled( GL_LIGHTING ) );
#endif
      _GLEnable( GL_LIGHTING );
#if defined( GL_CACHING )
      ctx->m_lightingEnableAttributes.m_lighting = GL_TRUE;
#endif
    }
  }
}

/*! \brief Helper function to reset the enable state of lighting.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the lighting
 *  state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableLighting, GLEnableLighting, GLDisable, GLEnable */
inline void GLResetLighting()
{
  GL_RESET( GL_LIGHTING, &glContext()->m_lightingEnableAttributes.m_lighting
          , glContext()->m_previousAttributes.m_lighting );
}

//! Cached version of GLEnable( GL_LINE_SMOOTH )
/** This function caches the GL_LINE_SMOOTH state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LINE_SMOOTH) directly. */
inline  void  GLEnableLineSmooth( void )
{
  GL_ENABLE( GL_LINE_SMOOTH, &glContext()->m_lineEnableAttributes.m_smooth
           , &glContext()->m_previousAttributes.m_lineSmooth );
}

/*! \brief Helper function to reset the enable state of line smoothing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the line
 *  smoothing state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableLineSmooth, GLEnableLineSmooth, GLDisable, GLEnable */
inline void GLResetLineSmooth()
{
  GL_RESET( GL_LINE_SMOOTH, &glContext()->m_lineEnableAttributes.m_smooth
          , glContext()->m_previousAttributes.m_lineSmooth );
}

//! Cached version of GLEnable( GL_LINE_STIPPLE )
/** This function caches the GL_LINE_STIPPLE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LINE_STIPPLE) directly. */
inline  void  GLEnableLineStipple( void )
{
  GL_ENABLE( GL_LINE_STIPPLE, &glContext()->m_lineEnableAttributes.m_stipple
           , &glContext()->m_previousAttributes.m_lineStipple );
}

/*! \brief Helper function to reset the enable state of line stippling.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the line
 *  stippling state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableLineStipple, GLEnableLineStipple, GLDisable, GLEnable */
inline void GLResetLineStipple()
{
  GL_RESET( GL_LINE_STIPPLE, &glContext()->m_lineEnableAttributes.m_stipple
          , glContext()->m_previousAttributes.m_lineStipple );
}

//! Cached version of GLEnable( GL_LOGIC_OP )
/** This function caches the GL_LOGIC_OP state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_LOGIC_OP) directly. */
inline  void  GLEnableLogicOp( void )
{
  GL_ENABLE( GL_LOGIC_OP, &glContext()->m_colorBufferEnableAttributes.m_logicOp
           , &glContext()->m_previousAttributes.m_logicOp );
}

/*! \brief Helper function to reset the enable state of logic operation.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the logic
 *  operation state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableLogicOp, GLEnableLogicOp, GLDisable, GLEnable */
inline void GLResetLogicOp()
{
  GL_RESET( GL_LOGIC_OP, &glContext()->m_colorBufferEnableAttributes.m_logicOp
          , glContext()->m_previousAttributes.m_logicOp );
}

//! Cached version of GLEnable( GL_NORMALIZE )
/** This function caches the GL_NORMALIZE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_NORMALIZE) directly. */
inline  void  GLEnableNormalize( void )
{
  GL_ENABLE( GL_NORMALIZE, &glContext()->m_transformEnableAttributes.m_normalize
           , &glContext()->m_previousAttributes.m_normalize );
}

/*! \brief Helper function to reset the enable state of normalizing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the normalize
 *  state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableNormalize, GLEnableNormalize, GLDisable, GLEnable */
inline void GLResetNormalize()
{
  GL_RESET( GL_NORMALIZE, &glContext()->m_transformEnableAttributes.m_normalize
          , glContext()->m_previousAttributes.m_normalize );
}

//! Cached version of GLEnable( GL_POINT_SMOOTH )
/** This function caches the GL_POINT_SMOOTH state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POINT_SMOOTH) directly. */
inline  void  GLEnablePointSmooth( void )
{
  GL_ENABLE( GL_POINT_SMOOTH, &glContext()->m_pointEnableAttributes.m_smooth
           , &glContext()->m_previousAttributes.m_pointSmooth );
}

/*! \brief Helper function to reset the enable state of point smoothing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the point
 *  smoothing state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisablePointSmooth, GLEnablePointSmooth, GLDisable, GLEnable */
inline void GLResetPointSmooth()
{
  GL_RESET( GL_POINT_SMOOTH, &glContext()->m_pointEnableAttributes.m_smooth
          , glContext()->m_previousAttributes.m_pointSmooth );
}

//! Cached version of GLEnable( GL_POLYGON_OFFSET_FILL )
/** This function caches the GL_POLYGON_OFFSET_FILL state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_OFFSET_FILL) directly. */
inline  void  GLEnablePolygonOffsetFill( void )
{
  GL_ENABLE( GL_POLYGON_OFFSET_FILL, &glContext()->m_polygonEnableAttributes.m_offsetFill
           , &glContext()->m_previousAttributes.m_offsetFill );
}

/*! \brief Helper function to reset the enable state of filled polygon offset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the polygon
 *  offset fill state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisablePolygonOffsetFill, GLEnablePolygonOffsetFill, GLDisable, GLEnable */
inline void GLResetPolygonOffsetFill()
{
  GL_RESET( GL_POLYGON_OFFSET_FILL, &glContext()->m_polygonEnableAttributes.m_offsetFill
          , glContext()->m_previousAttributes.m_offsetFill );
}

//! Cached version of GLEnable( GL_POLYGON_OFFSET_LINE )
/** This function caches the GL_POLYGON_OFFSET_LINE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_OFFSET_LINE) directly. */
inline  void  GLEnablePolygonOffsetLine( void )
{
  GL_ENABLE( GL_POLYGON_OFFSET_LINE, &glContext()->m_polygonEnableAttributes.m_offsetLine
           , &glContext()->m_previousAttributes.m_offsetLine );
}

/*! \brief Helper function to reset the enable state of lined polygon offset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the polygon
 *  offset line state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisablePolygonOffsetLine, GLEnablePolygonOffsetLine, GLDisable, GLEnable */
inline void GLResetPolygonOffsetLine()
{
  GL_RESET( GL_POLYGON_OFFSET_LINE, &glContext()->m_polygonEnableAttributes.m_offsetLine
          , glContext()->m_previousAttributes.m_offsetLine );
}

//! Cached version of GLEnable( GL_POLYGON_OFFSET_POINT )
/** This function caches the GL_POLYGON_OFFSET_POINT state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_OFFSET_POINT) directly. */
inline  void  GLEnablePolygonOffsetPoint( void )
{
  GL_ENABLE( GL_POLYGON_OFFSET_POINT, &glContext()->m_polygonEnableAttributes.m_offsetPoint
           , &glContext()->m_previousAttributes.m_offsetPoint );
}

/*! \brief Helper function to reset the enable state of pointed polygon offset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the polygon
 *  offset point state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisablePolygonOffsetPoint, GLEnablePolygonOffsetPoint, GLDisable, GLEnable */
inline void GLResetPolygonOffsetPoint()
{
  GL_RESET( GL_POLYGON_OFFSET_POINT, &glContext()->m_polygonEnableAttributes.m_offsetPoint
          , glContext()->m_previousAttributes.m_offsetPoint );
}

//! Cached version of GLEnable( GL_POLYGON_SMOOTH )
/** This function caches the GL_POLYGON_SMOOTH state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_SMOOTH) directly. */
inline  void  GLEnablePolygonSmooth( void )
{
  GL_ENABLE( GL_POLYGON_SMOOTH, &glContext()->m_polygonEnableAttributes.m_smooth
           , &glContext()->m_previousAttributes.m_polygonSmooth );
}

/*! \brief Helper function to reset the enable state of polygon smoothing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the polygon
 *  smooth state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisablePolygonSmooth, GLEnablePolygonSmooth, GLDisable, GLEnable */
inline  void  GLResetPolygonSmooth()
{
  GL_RESET( GL_POLYGON_SMOOTH, &glContext()->m_polygonEnableAttributes.m_smooth
          , glContext()->m_previousAttributes.m_polygonSmooth );
}

//! Cached version of GLEnable( GL_POLYGON_STIPPLE )
/** This function caches the GL_POLYGON_STIPPLE state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_POLYGON_STIPPLE) directly. */
inline  void  GLEnablePolygonStipple( void )
{
  GL_ENABLE( GL_POLYGON_STIPPLE, &glContext()->m_polygonEnableAttributes.m_stipple
           , &glContext()->m_previousAttributes.m_polygonStipple );
}

/*! \brief Helper function to reset the enable state of polygon stippling.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the polygon
 *  stippling state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisablePolygonStipple, GLEnablePolygonStipple, GLDisable, GLEnable */
inline  void  GLResetPolygonStipple()
{
  GL_RESET( GL_POLYGON_STIPPLE, &glContext()->m_polygonEnableAttributes.m_stipple
          , glContext()->m_previousAttributes.m_polygonStipple );
}

//! Cached version of GLEnable( GL_SCISSOR_TEST )
/** This function caches the GL_SCISSOR_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_SCISSOR_TEST) directly. */
inline  void  GLEnableScissorTest( void )
{
  GL_ENABLE( GL_SCISSOR_TEST, &glContext()->m_scissorEnableAttributes.m_scissorTest
           , &glContext()->m_previousAttributes.m_scissorTest );
}

/*! \brief Helper function to reset the enable state of scissor testing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the scissor
 *  test state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableScissorTest, GLEnableScissorTest, GLDisable, GLEnable */
inline  void  GLResetScissorTest()
{
  GL_RESET( GL_SCISSOR_TEST, &glContext()->m_scissorEnableAttributes.m_scissorTest
          , glContext()->m_previousAttributes.m_scissorTest );
}

//! Cached version of GLEnable( GL_STENCIL_TEST )
/** This function caches the GL_STENCIL_TEST state.
  * It is faster to call this method instead of calling GL[Disable|Enable](GL_STENCIL_TEST) directly. */
inline  void  GLEnableStencilTest( void )
{
  GL_ENABLE( GL_STENCIL_TEST, &glContext()->m_stencilBufferEnableAttributes.m_stencilTest
           , &glContext()->m_previousAttributes.m_stencilTest );
}

/*! \brief Helper function to reset the enable state of stencil testing.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the stencil
 *  test state to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableStencilTest, GLEnableStencilTest, GLDisable, GLEnable */
inline  void  GLResetStencilTest()
{
  GL_RESET( GL_STENCIL_TEST, &glContext()->m_stencilBufferEnableAttributes.m_stencilTest
          , glContext()->m_previousAttributes.m_stencilTest );
}

//! Cached version of GLEnable( [GL_TEXTURE_1D|GL_TEXTURE_2D|GL_TEXTURE_3D|GL_TEXTURE_CUBE_MAP|GL_TEXTURE_RECTANGLE_NV] )
/** This function caches the texture target type per texture unit. It is faster to call this method instead of calling 
  * GL[Disable|Enable]( [GL_TEXTURE_1D|GL_TEXTURE_2D|GL_TEXTURE_3D|GL_TEXTURE_CUBE_MAP|GL_TEXTURE_RECTANGLE_NV] ) directly. */
inline void GLEnableTexture( GLenum target )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_textureEnables[ctx->m_textureAttributes.m_toActivate].m_texture[target];
# endif
  NVSG_ASSERT(   ( target == GL_TEXTURE_1D ) || ( target == GL_TEXTURE_2D ) || ( target == GL_TEXTURE_3D )
             ||  ( target == GL_TEXTURE_CUBE_MAP ) || ( target == GL_TEXTURE_RECTANGLE_NV ) );
  if ( !ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_toActivate].m_texture[target] )
  {
    _GLUpdateActiveTexture();
    NVSG_ASSERT( ! glIsEnabled( target ) );
#endif
    _GLEnable( target );
#if defined( GL_CACHING )
    ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_active].m_texture[target] = GL_TRUE;
  }
#endif
}

/*! \brief Helper function to reset the enable state of texture target \a target.
 *  \param target The texture target for which the enable state has to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the texture
 *  target \a target to it's previous state. Otherwise, it is disabled.
 *  \sa GLDisableTexture, GLEnableTexture, GLDisable, GLEnable */
inline void GLResetTexture( GLenum target )
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLDisableTexture( ctx->m_previousAttributes.m_textureEnables[ctx->m_textureAttributes.m_toActivate].m_texture[target] );
#else
  GLDisableTexture( target );
#endif
}

//! Cached version of GLEnable( [GL_TEXTURE_GEN_S|GL_TEXTURE_GEN_T|GL_TEXTURE_GEN_R|GL_TEXTURE_GEN_Q] )
/** This function caches the texture generation type per texture unit. It is faster to call this method instead of calling 
  * GL[Disable|Enable]( [GL_TEXTURE_GEN_S|GL_TEXTURE_GEN_T|GL_TEXTURE_GEN_R|GL_TEXTURE_GEN_Q] ) directly. */
inline void GLEnableTextureGen( GLenum target )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  NVSG_ASSERT(    ( target == GL_TEXTURE_GEN_S )
              ||  ( target == GL_TEXTURE_GEN_T )
              ||  ( target == GL_TEXTURE_GEN_R )
              ||  ( target == GL_TEXTURE_GEN_Q ) );
  if ( ! ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_toActivate].m_gen[target-GL_TEXTURE_GEN_S] )
  {
    _GLUpdateActiveTexture();
    NVSG_ASSERT( ! glIsEnabled( target ) );
#endif
    _GLEnable( target );
#if defined( GL_CACHING )
    ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_active].m_gen[target-GL_TEXTURE_GEN_S] = GL_TRUE;
  }
#endif
}

//! Replacement of glEnable
/** This function caches on some cap values and asserts on the result before returning. */
inline  void  GLEnable( GLenum cap )
{
#if defined( GL_CACHING )
  switch( cap )
  {
    case GL_ALPHA_TEST :
      GLEnableAlphaTest();
      break;
    case GL_AUTO_NORMAL :
      GLEnableAutoNormal();
      break;
    case GL_BLEND :
      GLEnableBlend();
      break;
    case GL_CLIP_PLANE0 :
    case GL_CLIP_PLANE1 :
    case GL_CLIP_PLANE2 :
    case GL_CLIP_PLANE3 :
    case GL_CLIP_PLANE4 :
    case GL_CLIP_PLANE5 :
      GLEnableClipPlane( cap );
      break;
    case GL_COLOR_LOGIC_OP :
      GLEnableColorLogicOp();
      break;
    case GL_COLOR_MATERIAL :
      GLEnableColorMaterial();
      break;
    case GL_CULL_FACE :
      GLEnableCullFace();
      break;
    case GL_DEPTH_TEST :
      GLEnableDepthTest();
      break;
    case GL_DITHER :
      GLEnableDither();
      break;
    case GL_FOG :
      GLEnableFog();
      break;
    case GL_LIGHT0 :
    case GL_LIGHT1 :
    case GL_LIGHT2 :
    case GL_LIGHT3 :
    case GL_LIGHT4 :
    case GL_LIGHT5 :
    case GL_LIGHT6 :
    case GL_LIGHT7 :
      GLEnableLight( cap );
      break;
    case GL_LIGHTING :
      GLEnableLighting();
      break;
    case GL_LINE_SMOOTH :
      GLEnableLineSmooth();
      break;
    case GL_LINE_STIPPLE :
      GLEnableLineStipple();
      break;
    case GL_LOGIC_OP :
      GLEnableLogicOp();
      break;
    case GL_NORMALIZE :
      GLEnableNormalize();
      break;
    case GL_POINT_SMOOTH :
      GLEnablePointSmooth();
      break;
    case GL_POLYGON_OFFSET_FILL :
      GLEnablePolygonOffsetFill();
      break;
    case GL_POLYGON_OFFSET_LINE :
      GLEnablePolygonOffsetLine();
      break;
    case GL_POLYGON_OFFSET_POINT :
      GLEnablePolygonOffsetPoint();
      break;
    case GL_POLYGON_SMOOTH :
      GLEnablePolygonSmooth();
      break;
    case GL_POLYGON_STIPPLE :
      GLEnablePolygonStipple();
      break;
    case GL_SCISSOR_TEST :
      GLEnableScissorTest();
      break;
    case GL_STENCIL_TEST :
      GLEnableStencilTest();
      break;
    case GL_TEXTURE_1D :
    case GL_TEXTURE_2D :
    case GL_TEXTURE_3D :
    case GL_TEXTURE_CUBE_MAP :
    case GL_TEXTURE_RECTANGLE_NV :
      GLEnableTexture( cap );
      break;
    case GL_TEXTURE_GEN_S :
    case GL_TEXTURE_GEN_T :
    case GL_TEXTURE_GEN_R :
    case GL_TEXTURE_GEN_Q :
      GLEnableTextureGen( cap );
      break;
    default :
      _GLEnable( cap );
      break;
  }
#else
  switch( cap )
  {
    case GL_CULL_FACE :
      GLEnableCullFace();
      break;
    default :
      _GLEnable( cap );
      break;
  }
#endif
}

#if !defined(DOXYGEN_IGNORE)
#if defined( GL_CACHING )
inline void _GLEnableClientStateCached( GLenum cap, GLboolean * cache )
{
  if ( !*cache )
  {
    NVSG_ASSERT( !glIsEnabled( cap ) );
    _GLEnableClientState( cap );
    *cache = GL_TRUE;
  }
}
#define GL_ENABLE_CLIENT_STATE( cap, cache )  _GLEnableClientStateCached( cap, cache )
#else
#define GL_ENABLE_CLIENT_STATE( cap, cache )  _GLEnableClientState( cap );
#endif
#endif // DOXYGEN_IGNORE

//! Cached version of GLEnableClientState( GL_COLOR_ARRAY )
/** This function caches the GL_COLOR_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_COLOR_ARRAY ) directly. */
inline  void  GLEnableColorArray( void )
{
  GL_ENABLE_CLIENT_STATE( GL_COLOR_ARRAY, &glContext()->m_vertexArrayAttributes.m_colorArray );
}

//! Cached version of GLEnableClientState( GL_FOG_COORDINATE_ARRAY )
/** This function caches the GL_FOG_COORDINATE_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_FOG_COORDINATE_ARRAY ) directly. */
inline  void  GLEnableFogCoordinateArray( void )
{
  GL_ENABLE_CLIENT_STATE( GL_FOG_COORDINATE_ARRAY, &glContext()->m_vertexArrayAttributes.m_fogCoordArray );
}

//! Cached version of GLEnableClientState( GL_NORMAL_ARRAY )
/** This function caches the GL_NORMAL_ARRAY client state.
  * PIt is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_NORMAL_ARRAY ) directly. */
inline  void  GLEnableNormalArray( void )
{
  GL_ENABLE_CLIENT_STATE( GL_NORMAL_ARRAY, &glContext()->m_vertexArrayAttributes.m_normalArray );
}

//! Cached version of GLEnableClientState( GL_SECONDARY_COLOR_ARRAY )
/** This function caches the GL_SECONDARY_COLOR_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_SECONDARY_COLOR_ARRAY ) directly. */
inline  void  GLEnableSecondaryColorArray( void )
{
  GL_ENABLE_CLIENT_STATE( GL_SECONDARY_COLOR_ARRAY, &glContext()->m_vertexArrayAttributes.m_secondaryColorArray );
}

//! Cached version of GLEnableClientState( GL_TEXTURE_COORD_ARRAY )
/** This function caches the GL_TEXTURE_COORD_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_TEXTURE_COORD_ARRAY ) directly. */
inline  void  GLEnableTextureCoordArray( void )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  if ( ! ctx->m_vertexArrayAttributes.m_textureCoordArray[ctx->m_vertexArrayAttributes.m_clientTextureToActivate] )
  {
    _GLUpdateClientActiveTexture();
    NVSG_ASSERT( ! glIsEnabled( GL_TEXTURE_COORD_ARRAY ) );
#endif
    _GLEnableClientState( GL_TEXTURE_COORD_ARRAY );
#if defined( GL_CACHING )
    ctx->m_vertexArrayAttributes.m_textureCoordArray[ctx->m_vertexArrayAttributes.m_clientTextureActive] = GL_TRUE;
  }
#endif
}

//! Cached version of GLEnableClientState( GL_VERTEX_ARRAY )
/** This function caches the GL_VERTEX_ARRAY client state.
  * It is faster to call this method instead of calling GL[Disable|Enable]ClientState( GL_VERTEX_ARRAY ) directly. */
inline  void  GLEnableVertexArray( void )
{
  GL_ENABLE_CLIENT_STATE( GL_VERTEX_ARRAY, &glContext()->m_vertexArrayAttributes.m_vertexArray );
}

//! Replacement of glEnableClientState
/** This function caches on some cap values and asserts on the result before returning. */
inline  void  GLEnableClientState( GLenum array )
{
#if defined( GL_CACHING )
  switch( array )
  {
    case GL_COLOR_ARRAY :
      GLEnableColorArray();
      break;
    case GL_FOG_COORDINATE_ARRAY :
      GLEnableFogCoordinateArray();
      break;
    case GL_NORMAL_ARRAY :
      GLEnableNormalArray();
      break;
    case GL_SECONDARY_COLOR_ARRAY :
      GLEnableSecondaryColorArray();
      break;
    case GL_TEXTURE_COORD_ARRAY :
      GLEnableTextureCoordArray();
      break;
    case GL_VERTEX_ARRAY :
      GLEnableVertexArray();
      break;
    default :
      _GLEnableClientState( array );
      break;
  }
#else
  _GLEnableClientState( array );
#endif
}

//! Replacement of glEnd
/** This function asserts on the result before returning.  */
inline  void  GLEnd( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEnd();
#if !defined(NDEBUG)
  glContext()->m_inBeginEndLoop = false;
#endif
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEndList
/** This function asserts on the result before returning.  */
inline  void  GLEndList( void )
{
  NVSG_GL_STATS_END_LIST();
  glEndList();
  // NOTE: no assertion on GL_NO_ERROR here, because on glEndList the GL_OUT_OF_MEMORY is to be tested!
}

//! Replacement of glEndOcclusionQueryNV
/** This function hides the extension character of glEndOcclusionQueryNV and asserts on the result before returning.  */
inline  void  GLEndOcclusionQuery( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glEndOcclusionQueryNV );
  glContext()->m_glFunctions->glEndOcclusionQueryNV();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEndQueryARB
/** This function hides the extension character of glEndQueryARB and asserts on the result before returning. */
inline  void  GLEndQuery( GLenum target )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glEndQueryARB );
  glContext()->m_glFunctions->glEndQueryARB( target );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEvalCoord1d
/** This function asserts on the result before returning.  */
inline  void  GLEvalCoord1d( GLdouble u )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalCoord1d( u );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glEvalCoord1f
/** This function asserts on the result before returning.  */
inline  void  GLEvalCoord1f( GLfloat u )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalCoord1f( u );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glEvalCoord2d
/** This function asserts on the result before returning.  */
inline  void  GLEvalCoord2d( GLdouble u, GLdouble v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalCoord2d( u, v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glEvalCoord2f
/** This function asserts on the result before returning.  */
inline  void  GLEvalCoord2f( GLfloat u, GLfloat v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalCoord2f( u, v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glEvalMesh1
/** This function asserts on the result before returning.  */
inline  void  GLEvalMesh1( GLenum mode, GLint i1, GLint i2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalMesh1( mode, i1, i2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEvalMesh2
/** This function asserts on the result before returning.  */
inline  void  GLEvalMesh2( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalMesh2( mode, i1, i2, j1, j2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glEvalPoint1
/** This function asserts on the result before returning.  */
inline  void  GLEvalPoint1( GLint i )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalPoint1( i );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glEvalPoint2
/** This function asserts on the result before returning.  */
inline  void  GLEvalPoint2( GLint i, GLint j )
{
  NVSG_GL_STATS_FCT_COUNT();
  glEvalPoint2( i, j );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glFeedbackBuffer
/** This function asserts on the result before returning.  */
inline  void  GLFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFeedbackBuffer( size, type, buffer );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glFinish
/** This function asserts on the result before returning.  */
inline  void  GLFinish( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFinish();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glFlush
/** This function asserts on the result before returning.  */
inline  void  GLFlush( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFlush();
  ASSERT_GL_NO_ERROR;
}

#if !defined(DOXYGEN_IGNORE)
#if defined( GL_CACHING )
inline void _GLFogfCached( GLenum pname, GLfloat param, GLfloat * cache )
{
  if ( param != *cache )
  {
    _GLFogf( pname, param );
    *cache = param;
  }
}
inline void _GLFogfvCached( GLenum pname, const GLfloat * params, GLfloat * cache )
{
  if ( !equal4fv( params, cache ) )
  {
    _GLFogfv( pname, params );
    memcpy( cache, params, 4*sizeof(GLfloat) );
  }
}
inline void _GLFogiCached( GLenum pname, GLint param, GLint * cache )
{
  if ( param != *cache )
  {
    _GLFogi( pname, param );
    *cache = param;
  }
}
# if defined(GL_SUPPORT_RESET)
inline void _GLFogfCached( GLenum pname, GLfloat param, GLfloat * cache, GLfloat * prev )
{
  *prev = *cache;
  _GLFogfCached( pname, param, cache );
}
inline void _GLFogfvCached( GLenum pname, const GLfloat * params, GLfloat * cache, GLfloat * prev )
{
  memcpy( prev, cache, 4*sizeof(GLfloat) );
  _GLFogfvCached( pname, params, cache );
}
inline void _GLFogiCached( GLenum pname, GLint param, GLint * cache, GLint * prev )
{
  *prev = *cache;
  _GLFogiCached( pname, param, cache );
}
#  define GL_FOG_F( pname, param, cache, prev )   _GLFogfCached( pname, param, cache )
#  define GL_FOG_FV( pname, params, cache, prev ) _GLFogfvCached( pname, params, cache )
#  define GL_FOG_I( pname, param, cache, prev )   _GLFogiCached( pname, param, cache )
# else
#  define GL_FOG_F( pname, param, cache, prev )   _GLFogfCached( pname, param, cache )
#  define GL_FOG_FV( pname, params, cache, prev ) _GLFogfvCached( pname, params, cache )
#  define GL_FOG_I( pname, param, cache, prev )   _GLFogiCached( pname, param, cache )
# endif
#else
# define GL_FOG_F( pname, param, cache, prev )    _GLFogf( pname, param )
# define GL_FOG_FV( pname, params, cache, prev )  _GLFogfv( pname, params )
# define GL_FOG_I( pname, param, cache, prev )    _GLFogi( pname, param )
#endif
#endif // DOXYGEN_IGNORE

//! Cached version of GLFog[fv|iv]( GL_FOG_COLOR, params )
/** This function caches the GL_FOG_COLOR value.
  * It is faster to call this method instead of GLFog[fv|iv]( GL_FOG_COLOR, params ) directly. */
inline  void  GLFogColor( const GLfloat * color )
{
  GL_FOG_FV( GL_FOG_COLOR, color, &glContext()->m_fogAttributes.m_color[0]
           , &glContext()->m_previousAttributes.m_fogColor[0] );
}

/*! \brief Helper function to reset the fog color
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the fog color
 *  to it's previous state. Otherwise, it is set to (0,0,0,0).
 *  \sa GLFogColor */
inline void GLResetFogColor()
{
#if defined(GL_SUPPORT_RESET)
  GLFogColor( &glContext()->m_previousAttributes.m_fogColor[0] );
#else
  static nvmath::Vec4f fc(0.0f,0.0f,0.0f,0.0f);
  GLFogColor( &fc[0] );
#endif
}

//! Replacement of glFogCoordPointer
/** This function caches the fog coordinate pointer setting and asserts on the result before returning.  */
inline  void  GLFogCoordPointer( GLenum type, GLsizei stride, const GLvoid *pointer )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
  GLContext::VertexArrayParameters parameters( 1, type, stride, pointer );
  if ( ctx->m_vertexArrayAttributes.m_fogCoordArrayParams != parameters )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    ctx->m_glFunctions->glFogCoordPointer( type, stride, pointer );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_vertexArrayAttributes.m_fogCoordArrayParams = parameters;
#endif
  }
}

//! Cached version of GLFog[f|fv|i|iv]( GL_FOG_DENSITY, param[s] )
/** This function caches the GL_FOG_DENSITY value.
  * It is faster to call this method instead of GLFog[f|fv|i|iv]( GL_FOG_MODE, param[s] ) directly. */
inline  void  GLFogDensity( GLfloat density )
{
  GL_FOG_F( GL_FOG_DENSITY, density, &glContext()->m_fogAttributes.m_density
          , &glContext()->m_previousAttributes.m_fogDensity );
}

/*! \brief Helper function to reset the fog density.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the fog density
 *  to it's previous state. Otherwise, it is set to 1.0f.
 *  \sa GLFogDensity */
inline void GLResetFogDensity()
{
#if defined(GL_SUPPORT_RESET)
  GLFogDensity( glContext()->m_previousAttributes.m_fogDensity );
#else
  GLFogDensity( 1.0f );
#endif
}

//! Cached version of GLFog[f|fv|i|iv]( GL_FOG_END, param[s] )
/** This function caches the GL_FOG_END value.
  * It is faster to call this method instead of GLFog[f|fv|i|iv]( GL_FOG_END, param[s] ) directly. */
inline  void  GLFogEnd( GLfloat end )
{
  GL_FOG_F( GL_FOG_END, end, &glContext()->m_fogAttributes.m_end
          , &glContext()->m_previousAttributes.m_fogEnd );
}

/*! \brief Helper function to reset the fog end value.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the fog end
 *  value to it's previous state. Otherwise, it is set to 1.0f.
 *  \sa GLFogDensity */
inline void GLResetFogEnd()
{
#if defined(GL_SUPPORT_RESET)
  GLFogEnd( glContext()->m_previousAttributes.m_fogEnd );
#else
  GLFogEnd( 1.0f );
#endif
}

//! Replacement of glFogf
/** This function asserts on the result before returning.  */
inline  void  GLFogf( GLenum pname, GLfloat param ) 
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_FOG_DENSITY :
      GLFogDensity( param );
      break;
    case GL_FOG_END :
      GLFogEnd( param );
      break;
    case GL_FOG_MODE :
      GLFogMode( (GLenum)param );
      break;
    case GL_FOG_START :
      GLFogStart( param );
      break;
    default :
      _GLFogf( pname, param );
      break;
  }
#else
  _GLFogf( pname, param );
#endif
}

//! Replacement of glFogfv
/** This function asserts on the result before returning.  */
inline  void  GLFogfv( GLenum pname, const GLfloat *params )  
{ 
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_FOG_COLOR :
      GLFogColor( params );
      break;
    case GL_FOG_DENSITY :
      GLFogDensity( *params );
      break;
    case GL_FOG_END :
      GLFogEnd( *params );
      break;
    case GL_FOG_MODE :
      GLFogMode( (GLenum)*params );
      break;
    case GL_FOG_START :
      GLFogStart( *params );
      break;
    default :
      _GLFogfv( pname, params );
      break;
  }
#else
  _GLFogfv( pname, params );
#endif
}

//! Replacement of glFogi
/** This function asserts on the result before returning.  */
inline  void  GLFogi( GLenum pname, GLint param ) 
{ 
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_FOG_DENSITY :
      GLFogDensity( (GLfloat)param );
      break;
    case GL_FOG_END :
      GLFogEnd( (GLfloat)param );
      break;
    case GL_FOG_MODE :
      GLFogMode( param );
      break;
    case GL_FOG_START :
      GLFogStart( (GLfloat)param );
      break;
    default :
      _GLFogi( pname, param );
      break;
  }
#else
  _GLFogi( pname, param );
#endif
}

//! Replacement of glFogiv
/** This function asserts on the result before returning.  */
inline  void  GLFogiv( GLenum pname, const GLint *params ) 
{ 
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_FOG_COLOR :
      {
        GLfloat fParams[4];
        for ( size_t i=0 ; i<4 ; i++ )
        {
          fParams[i] = (GLfloat)params[i];
        }
        GLFogColor( fParams );
      }
      break;
    case GL_FOG_DENSITY :
      GLFogDensity( (GLfloat)*params );
      break;
    case GL_FOG_END :
      GLFogEnd( (GLfloat)*params );
      break;
    case GL_FOG_MODE :
      GLFogMode( *params );
      break;
    case GL_FOG_START :
      GLFogStart( (GLfloat)*params );
      break;
    default :
      _GLFogiv( pname, params );
      break;
  }
#else
  _GLFogiv( pname, params );
#endif
}

//! Cached version of GLFog[f|fv|i|iv]( GL_FOG_MODE, param[s] )
/** This function caches the GL_FOG_MODE value.
  * It is faster to call this method instead of GLFog[f|fv|i|iv]( GL_FOG_MODE, param[s] ) directly. */
inline  void  GLFogMode( GLenum mode )
{
  GL_FOG_I( GL_FOG_MODE, mode, (GLint*)&glContext()->m_fogAttributes.m_mode
          , (GLint*) &glContext()->m_previousAttributes.m_fogMode );
}

/*! \brief Helper function to reset the fog mode.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the fog mode
 *  to it's previous state. Otherwise, it is set to GL_EXP.
 *  \sa GLFogMode */
inline void GLResetFogMode()
{
#if defined(GL_SUPPORT_RESET)
  GLFogMode( glContext()->m_previousAttributes.m_fogMode );
#else
  GLFogMode( GL_EXP );
#endif
}

//! Cached version of GLFog[f|fv|i|iv]( GL_FOG_START, param[s] )
/** This function caches the GL_FOG_START value.
  * It is faster to call this method instead of GLFog[f|fv|i|iv]( GL_FOG_START, param[s] ) directly. */
inline  void  GLFogStart( GLfloat start )
{
  GL_FOG_F( GL_FOG_START, start, &glContext()->m_fogAttributes.m_start
          , glContext()->m_previousAttributes.m_fogStart );
}

/*! \brief Helper function to reset the fog start value.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the fog start
 *  value to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLFogStart */
inline void GLResetFogStart()
{
#if defined(GL_SUPPORT_RESET)
  GLFogStart( glContext()->m_previousAttributes.m_fogStart );
#else
  GLFogStart( 0.0f );
#endif
}

//! Replacement of glFrontFace
/** This function caches the front face setting and asserts on the result before returning.  */
inline  void  GLFrontFace( GLenum mode )
{
#if defined(GL_CACHING)
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_frontFaceMode = ctx->m_polygonAttributes.m_frontFaceMode;
# endif
  if ( mode != ctx->m_polygonAttributes.m_frontFaceMode )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glFrontFace( mode );
    ASSERT_GL_NO_ERROR;
#if defined(GL_CACHING)
    ctx->m_polygonAttributes.m_frontFaceMode = mode;
#endif
  }
}

/*! \brief Helper function to reset the front face state.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the front face
 *  state to it's previous state. Otherwise, it is set to GL_CCW.
 *  \sa GLFrontFace */
inline void GLResetFrontFace()
{
#if defined(GL_SUPPORT_RESET)
  GLFrontFace( glContext()->m_previousAttributes.m_frontFaceMode );
#else
  GLFrontFace(GL_CCW);
#endif
}

//! Replacement of glFrustum
/** This function asserts on the result before returning.  */
inline  void  GLFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
  NVSG_GL_STATS_FRUSTUM( left, right, bottom, top, zNear, zFar );
  glFrustum( left, right, bottom, top, zNear, zFar );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGenLists
/** This function asserts on the result and updates some caches. */
inline  GLuint  GLGenLists( GLsizei range )
{
  NVSG_GL_STATS_GEN_LISTS( range );
  GLuint  firstList = glGenLists( range );
  glContext()->addLists(range, firstList);
  ASSERT_GL_NO_ERROR;
  return( firstList );
}

//! Replacement of glGenOcclusionQueriesNV
/** This function hides the extension character of glGenOcclusionQueriesNV and asserts on the result, and updates some caches. */
inline  void  GLGenOcclusionQueries( GLsizei n, GLuint *ids )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glGenOcclusionQueriesNV );
  ctx->m_glFunctions->glGenOcclusionQueriesNV( n, ids );
  ctx->addOcclusionQueries(n, ids);
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGenProgramsNV
/** This function hides the extension character of glGenProgramsNV and asserts on the result, and updates some caches.  */
inline  void  GLGenPrograms( GLsizei n, GLuint *ids )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glGenProgramsNV );
  ctx->m_glFunctions->glGenProgramsNV( n, ids );
  ctx->addPrograms(n, ids);
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGenQueriesARB
/** This function hides the extension character of glGenQueriesARB and asserts on the result, and updates some caches. */
inline  void  GLGenQueries( GLsizei n, GLuint *ids )
{
  NVSG_GL_STATS_FCT_COUNT();
  GLContext * ctx = glContext();
  NVSG_ASSERT( ctx->m_glFunctions && ctx->m_glFunctions->glGenQueriesARB );
  ctx->m_glFunctions->glGenQueriesARB( n, ids );
  ctx->addQueries( n, ids );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGenTextures
/** This function asserts on the result and updates some caches. */
inline  void  GLGenTextures( GLsizei n, GLuint *textures )
{
  NVSG_GL_STATS_GEN_TEXTURES( n, textures );
  glGenTextures( n, textures );
  glContext()->addTextures(n, textures);
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetBooleanv
/** This function asserts on the result before returning.  */
inline  void  GLGetBooleanv( GLenum pname, GLboolean *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetBooleanv( pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetDoublev
/** This function asserts on the result before returning.  */
inline  void  GLGetDoublev( GLenum pname, GLdouble *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetDoublev( pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetFloatv
/** This function asserts on the result before returning.  */
inline  void  GLGetFloatv( GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetFloatv( pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetIntegerv
/** This function asserts on the result before returning.  */
inline  void  GLGetIntegerv( GLenum pname, GLint *params )
{
  NVSG_GL_STATS_GET_INTEGERV( pname, params );
  glGetIntegerv( pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetClipPlane
/** This function asserts on the result before returning.  */
inline  void  GLGetClipPlane( GLenum plane, GLdouble *equation )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetClipPlane( plane, equation );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetLightfv
/** This function asserts on the result before returning.  */
inline  void  GLGetLightfv( GLenum light, GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetLightfv( light, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetLightiv
/** This function asserts on the result before returning.  */
inline  void  GLGetLightiv( GLenum light, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetLightiv( light, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetMapdv
/** This function asserts on the result before returning.  */
inline  void  GLGetMapdv( GLenum target, GLenum query, GLdouble *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetMapdv( target, query, v );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetMapfv
/** This function asserts on the result before returning.  */
inline  void  GLGetMapfv( GLenum target, GLenum query, GLfloat *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetMapfv( target, query, v );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetMapiv
/** This function asserts on the result before returning.  */
inline  void  GLGetMapiv( GLenum target, GLenum query, GLint *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetMapiv( target, query, v );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetMaterialfv
/** This function asserts on the result before returning.  */
inline  void  GLGetMaterialfv( GLenum face, GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetMaterialfv( face, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetMaterialiv
/** This function asserts on the result before returning.  */
inline  void  GLGetMaterialiv( GLenum face, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetMaterialiv( face, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Get the maximal number of texture coordinate slots.
inline GLint GLGetMaxTexCoords( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  return( glContext()->m_maxTexCoords );
}

//! Get the maximal number of texture units.
inline GLint GLGetMaxTexUnits( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  return( glContext()->m_maxTexUnits );
}

//! Get the maximal number of texture image units.
inline GLint GLGetMaxTexImageUnits( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_maxTexImageUnits;
}

//! Replacement of glGetOcclusionQueryuivNV
/** This function hides the extension character of glGetOcclusionQueryuivNV and asserts on the result before returning.  */
inline  void  GLGetOcclusionQueryuiv( GLuint id, GLenum pname, GLuint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glGetOcclusionQueryuivNV );
  glContext()->m_glFunctions->glGetOcclusionQueryuivNV( id, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetPixelMapfv
/** This function asserts on the result before returning.  */
inline  void  GLGetPixelMapfv( GLenum map, GLfloat *values )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetPixelMapfv( map, values );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetPixelMapuiv
/** This function asserts on the result before returning.  */
inline  void  GLGetPixelMapuiv( GLenum map, GLuint *values )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetPixelMapuiv( map, values );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetPixelMapusv
/** This function asserts on the result before returning.  */
inline  void  GLGetPixelMapusv( GLenum map, GLushort *values )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetPixelMapusv( map, values );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetPointerv
/** This function asserts on the result before returning.  */
inline  void  GLGetPointerv( GLenum pname, GLvoid* *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetPointerv( pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetPolygonStipple
/** This function asserts on the result before returning.  */
inline  void  GLGetPolygonStipple( GLubyte *mask )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetPolygonStipple( mask );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetQueryivARB
/** This function hides the extension character of glGetQueryivARB and asserts on the result before returning. */
inline  void  GLGetQueryiv( GLenum target, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glGetQueryivARB );
  glContext()->m_glFunctions->glGetQueryivARB( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetQueryObjectivARB
/** This function hides the extension character of glGetQueryObjectivARB and asserts on the result before returning. */
inline  void  GLGetQueryObjectiv( GLuint id, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glGetQueryObjectivARB );
  glContext()->m_glFunctions->glGetQueryObjectivARB( id, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetQueryObjectuivARB
/** This function hides the extension character of glGetQueryObjectuivARB and asserts on the result before returning. */
inline  void  GLGetQueryObjectuiv( GLuint id, GLenum pname, GLuint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glGetQueryObjectuivARB );
  glContext()->m_glFunctions->glGetQueryObjectuivARB( id, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetString
/** This function asserts on the result before returning.  */
inline  const GLubyte * GLGetString( GLenum name )
{
  NVSG_GL_STATS_FCT_COUNT();
  const GLubyte * ret = glGetString( name );
  ASSERT_GL_NO_ERROR;
  return( ret );
}

//! Replacement of glGetTexEnvfv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexEnvfv( GLenum target, GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexEnvfv( target,pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexEnviv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexEnviv( GLenum target, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexEnviv( target,pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexGendv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexGendv( GLenum coord, GLenum pname, GLdouble *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexGendv( coord, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexGenfv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexGenfv( GLenum coord, GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexGenfv( coord, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexGeniv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexGeniv( GLenum coord, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexGeniv( coord, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexImage
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLGetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexImage( target, level, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexLevelParameterfv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexLevelParameterfv( GLenum target, GLint level, GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexLevelParameterfv( target, level, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexLevelParameteriv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexLevelParameteriv( GLenum target, GLint level, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexLevelParameteriv( target, level, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexParameterfv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexParameterfv( GLenum target, GLenum pname, GLfloat *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexParameterfv( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glGetTexParameteriv
/** This function asserts on the result before returning.  */
inline  void  GLGetTexParameteriv( GLenum target, GLenum pname, GLint *params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glGetTexParameteriv( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glHint
/** This function asserts on the result before returning.  */
inline  void  GLHint( GLenum target, GLenum mode )
{
  NVSG_GL_STATS_FCT_COUNT();
  glHint( target, mode );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glIndexd
/** This function asserts on the result before returning.  */
inline  void  GLIndexd( GLdouble c )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexd( c );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glIndexf
/** This function asserts on the result before returning.  */
inline  void  GLIndexf( GLfloat c )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexf( c );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glIndexi
/** This function asserts on the result before returning.  */
inline  void  GLIndexi( GLint c )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexi( c );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glIndexMask
/** This function asserts on the result before returning.  */
inline  void  GLIndexMask( GLuint mask )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexMask( mask );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glIndexPointer
/** This function asserts on the result before returning.  */
inline  void  GLIndexPointer( GLenum type, GLsizei stride, const GLvoid *pointer )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexPointer( type, stride, pointer );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glIndexs
/** This function asserts on the result before returning.  */
inline  void  GLIndexs( GLshort c )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexs( c );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glIndexub
/** This function asserts on the result before returning.  */
inline  void  GLIndexub( GLubyte c )
{
  NVSG_GL_STATS_FCT_COUNT();
  glIndexub( c );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glInitNames
/** This function asserts on the result before returning.  */
inline  void  GLInitNames( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  glInitNames();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glInterleavedArrays
/** This function asserts on the result before returning.  */
inline  void  GLInterleavedArrays( GLenum format, GLsizei stride, const GLvoid *pointer )
{
  NVSG_GL_STATS_FCT_COUNT();
  glInterleavedArrays( format, stride, pointer );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glIsEnabled
/** This function returns the state of the queried capability. */
inline  GLboolean  GLIsEnabled( GLenum cap ) 
{
  GLboolean isEnabled;
#if defined( GL_CACHING )
  switch( cap )
  {
    case GL_ALPHA_TEST :
      isEnabled = GLIsEnabledAlphaTest();
      break;
    case GL_AUTO_NORMAL :
      isEnabled = GLIsEnabledAutoNormal();
      break;
    case GL_BLEND :
      isEnabled = GLIsEnabledBlend();
      break;
    case GL_CLIP_PLANE0 :
    case GL_CLIP_PLANE1 :
    case GL_CLIP_PLANE2 :
    case GL_CLIP_PLANE3 :
    case GL_CLIP_PLANE4 :
    case GL_CLIP_PLANE5 :
      isEnabled = GLIsEnabledClipPlane( cap );
      break;
    case GL_COLOR_LOGIC_OP :
      isEnabled = GLIsEnabledColorLogicOp();
      break;
    case GL_COLOR_MATERIAL :
      isEnabled = GLIsEnabledColorMaterial();
      break;
    case GL_CULL_FACE :
      isEnabled = GLIsEnabledCullFace();
      break;
    case GL_DEPTH_TEST :
      isEnabled = GLIsEnabledDepthTest();
      break;
    case GL_DITHER :
      isEnabled = GLIsEnabledDither();
      break;
    case GL_FOG :
      isEnabled = GLIsEnabledFog();
      break;
    case GL_LIGHT0 :
    case GL_LIGHT1 :
    case GL_LIGHT2 :
    case GL_LIGHT3 :
    case GL_LIGHT4 :
    case GL_LIGHT5 :
    case GL_LIGHT6 :
    case GL_LIGHT7 :
      isEnabled = GLIsEnabledLight( cap );
      break;
    case GL_LIGHTING :
      isEnabled = GLIsEnabledLighting();
      break;
    case GL_LINE_SMOOTH :
      isEnabled = GLIsEnabledLineSmooth();
      break;
    case GL_LINE_STIPPLE :
      isEnabled = GLIsEnabledLineStipple();
      break;
    case GL_LOGIC_OP :
      isEnabled = GLIsEnabledLogicOp();
      break;
    case GL_NORMALIZE :
      isEnabled = GLIsEnabledNormalize();
      break;
    case GL_POINT_SMOOTH :
      isEnabled = GLIsEnabledPointSmooth();
      break;
    case GL_POLYGON_OFFSET_FILL :
      isEnabled = GLIsEnabledPolygonOffsetFill();
      break;
    case GL_POLYGON_OFFSET_LINE :
      isEnabled = GLIsEnabledPolygonOffsetLine();
      break;
    case GL_POLYGON_OFFSET_POINT :
      isEnabled = GLIsEnabledPolygonOffsetPoint();
      break;
    case GL_POLYGON_SMOOTH :
      isEnabled = GLIsEnabledPolygonSmooth();
      break;
    case GL_POLYGON_STIPPLE :
      isEnabled = GLIsEnabledPolygonStipple();
      break;
    case GL_SCISSOR_TEST :
      isEnabled = GLIsEnabledScissorTest();
      break;
    case GL_STENCIL_TEST :
      isEnabled = GLIsEnabledStencilTest();
      break;
    case GL_TEXTURE_1D :
    case GL_TEXTURE_2D :
    case GL_TEXTURE_3D :
    case GL_TEXTURE_CUBE_MAP :
    case GL_TEXTURE_RECTANGLE_NV :
      isEnabled = GLIsEnabledTexture( cap );
      break;
    case GL_TEXTURE_GEN_S :
    case GL_TEXTURE_GEN_T :
    case GL_TEXTURE_GEN_R :
    case GL_TEXTURE_GEN_Q :
      isEnabled = GLIsEnabledTextureGen( cap );
      break;
    default :
      isEnabled = _GLIsEnabled( cap );
      break;
  }
#else
  isEnabled = _GLIsEnabled( cap );
#endif
  return( isEnabled );
}

//! Replacement of glIsEnabled( GL_ALPHA_TEST )
/** This function returns the state of the GL_ALPHA_TEST capability. */
inline GLboolean GLIsEnabledAlphaTest()
{
  return( GL_IS_ENABLED( GL_ALPHA_TEST, glContext()->m_colorBufferEnableAttributes.m_alphaTest ) );
}

//! Replacement of glIsEnabled( GL_AUTO_NORMAL )
/** This function returns the state of the GL_AUTO_NORMAL capability. */
inline GLboolean GLIsEnabledAutoNormal()
{
  return( GL_IS_ENABLED( GL_AUTO_NORMAL, glContext()->m_evalAttributes.m_autoNormal ) );
}

//! Replacement of glIsEnabled( GL_BLEND )
/** This function returns the state of the GL_BLEND capability. */
inline GLboolean GLIsEnabledBlend()
{
  return( GL_IS_ENABLED( GL_BLEND, glContext()->m_colorBufferEnableAttributes.m_blend ) );
}

//! Replacement of glIsEnabled( GL_CLIP_PLANEi )
/** This function returns the state of the GL_CLIP_PLANEi capability. */
inline GLboolean GLIsEnabledClipPlane( GLenum plane )
{
#if defined( GL_CACHING )
  NVSG_ASSERT( plane-GL_CLIP_PLANE0 < glContext()->m_transformEnableAttributes.m_clipPlanes.size() );
#endif
  return( GL_IS_ENABLED( plane, glContext()->m_transformEnableAttributes.m_clipPlanes[plane-GL_CLIP_PLANE0] ) );
}

//! Replacement of glIsEnabled( GL_COLOR_LOGIC_OP )
/** This function returns the state of the GL_COLOR_LOGIC_OP capability. */
inline GLboolean GLIsEnabledColorLogicOp()
{
  return( GL_IS_ENABLED( GL_COLOR_LOGIC_OP, glContext()->m_colorBufferEnableAttributes.m_colorLogicOp ) );
}

//! Replacement of glIsEnabled( GL_COLOR_MATERIAL )
/** This function returns the state of the GL_COLOR_MATERIAL capability. */
inline  GLboolean GLIsEnabledColorMaterial()
{
  return( GL_IS_ENABLED( GL_COLOR_MATERIAL, glContext()->m_lightingAttributes.m_colorMaterial ) );
}

//! Replacement of glIsEnabled( GL_CULL_FACE )
/** This function returns the state of the GL_CULL_FACE capability. */
inline GLboolean GLIsEnabledCullFace()
{
  return( GL_IS_ENABLED( GL_CULL_FACE, glContext()->m_polygonEnableAttributes.m_cullFace ) );
}

//! Replacement of glIsEnabled( GL_DEPTH_TEST )
/** This function returns the state of the GL_DEPTH_TEST capability. */
inline GLboolean GLIsEnabledDepthTest()
{
  return( GL_IS_ENABLED( GL_DEPTH_TEST, glContext()->m_depthBufferEnableAttributes.m_depthTesting ) );
}

//! Replacement of glIsEnabled( GL_DITHER )
/** This function returns the state of the GL_DITHER capability. */
inline GLboolean GLIsEnabledDither()
{
  return( GL_IS_ENABLED( GL_DITHER, glContext()->m_colorBufferEnableAttributes.m_dither ) );
}

//! Replacement of glIsEnabled( GL_FOG )
/** This function returns the state of the GL_FOG capability. */
inline GLboolean GLIsEnabledFog()
{
  return( GL_IS_ENABLED( GL_FOG, glContext()->m_fogEnableAttributes.m_fog ) );
}

//! Replacement of glIsEnabled( GL_LIGHTi )
/** This function returns the state of the GL_LIGHTi capability. */
inline GLboolean GLIsEnabledLight( GLenum light )
{
#if defined( GL_CACHING )
  NVSG_ASSERT( light-GL_LIGHT0 < glContext()->m_lightingEnableAttributes.m_lights.size() );
#endif
  return( GL_IS_ENABLED( light, glContext()->m_lightingEnableAttributes.m_lights[light-GL_LIGHT0] ) );
}

//! Replacement of glIsEnabled( GL_LIGHTING )
/** This function returns the state of the GL_LIGHTING capability. */
inline GLboolean GLIsEnabledLighting()
{
  return( GL_IS_ENABLED( GL_LIGHTING, glContext()->m_lightingEnableAttributes.m_lighting ) );
}

//! Replacement of glIsEnabled( GL_LINE_SMOOTH )
/** This function returns the state of the GL_LINE_SMOOTH capability. */
inline GLboolean GLIsEnabledLineSmooth()
{
  return( GL_IS_ENABLED( GL_LINE_SMOOTH, glContext()->m_lineEnableAttributes.m_smooth ) );
}

//! Replacement of glIsEnabled( GL_LINE_STIPPLE )
/** This function returns the state of the GL_LINE_STIPPLE capability. */
inline GLboolean GLIsEnabledLineStipple()
{
  return( GL_IS_ENABLED( GL_LINE_STIPPLE, glContext()->m_lineEnableAttributes.m_stipple ) );
}

//! Replacement of glIsEnabled( GL_LOGIC_OP )
/** This function returns the state of the GL_LOGIC_OP capability. */
inline GLboolean GLIsEnabledLogicOp()
{
  return( GL_IS_ENABLED( GL_LOGIC_OP, glContext()->m_colorBufferEnableAttributes.m_logicOp ) );
}

//! Replacement of glIsEnabled( GL_NORMALIZE )
/** This function returns the state of the GL_NORMALIZE capability. */
inline GLboolean GLIsEnabledNormalize()
{
  return( GL_IS_ENABLED( GL_NORMALIZE, glContext()->m_transformEnableAttributes.m_normalize ) );
}

//! Replacement of glIsEnabled( GL_POINT_SMOOTH )
/** This function returns the state of the GL_POINT_SMOOTH capability. */
inline GLboolean GLIsEnabledPointSmooth()
{
  return( GL_IS_ENABLED( GL_POINT_SMOOTH, glContext()->m_pointEnableAttributes.m_smooth ) );
}

//! Replacement of glIsEnabled( GL_POLYGON_OFFSET_FILL )
/** This function returns the state of the GL_POLYGON_OFFSET_FILL capability. */
inline GLboolean GLIsEnabledPolygonOffsetFill()
{
  return( GL_IS_ENABLED( GL_POLYGON_OFFSET_FILL, glContext()->m_polygonEnableAttributes.m_offsetFill ) );
}

//! Replacement of glIsEnabled( GL_POLYGON_OFFSET_LINE )
/** This function returns the state of the GL_POLYGON_OFFSET_LINE capability. */
inline GLboolean GLIsEnabledPolygonOffsetLine()
{
  return( GL_IS_ENABLED( GL_POLYGON_OFFSET_LINE, glContext()->m_polygonEnableAttributes.m_offsetLine ) );
}

//! Replacement of glIsEnabled( GL_POLYGON_OFFSET_POINT )
/** This function returns the state of the GL_POLYGON_OFFSET_POINT capability. */
inline GLboolean GLIsEnabledPolygonOffsetPoint()
{
  return( GL_IS_ENABLED( GL_POLYGON_OFFSET_POINT, glContext()->m_polygonEnableAttributes.m_offsetPoint ) );
}

//! Replacement of glIsEnabled( GL_POLYGON_SMOOTH )
/** This function returns the state of the GL_POLYGON_SMOOTH capability. */
inline GLboolean GLIsEnabledPolygonSmooth()
{
  return( GL_IS_ENABLED( GL_POLYGON_SMOOTH, glContext()->m_polygonEnableAttributes.m_smooth ) );
}

//! Replacement of glIsEnabled( GL_POLYGON_STIPPLE )
/** This function returns the state of the GL_POLYGON_STIPPLE capability. */
inline GLboolean GLIsEnabledPolygonStipple()
{
  return( GL_IS_ENABLED( GL_POLYGON_STIPPLE, glContext()->m_polygonEnableAttributes.m_stipple ) );
}

//! Replacement of glIsEnabled( GL_SCISSOR_TEST )
/** This function returns the state of the GL_SCISSOR_TEST capability. */
inline GLboolean GLIsEnabledScissorTest()
{
  return( GL_IS_ENABLED( GL_SCISSOR_TEST, glContext()->m_scissorEnableAttributes.m_scissorTest ) );
}

//! Replacement of glIsEnabled( GL_STENCIL_TEST )
/** This function returns the state of the GL_STENCIL_TEST capability. */
inline GLboolean GLIsEnabledStencilTest()
{
  return( GL_IS_ENABLED( GL_STENCIL_TEST, glContext()->m_stencilBufferEnableAttributes.m_stencilTest ) );
}

//! Replacement of glIsEnabled( [GL_TEXTURE_1D|GL_TEXTURE_2D|GL_TEXTURE_3D|GL_TEXTURE_CUBE_MAP|GL_TEXTURE_RECTANGLE_NV] )
/** This function returns the state of the [GL_TEXTURE_1D|GL_TEXTURE_2D|GL_TEXTURE_3D|GL_TEXTURE_CUBE_MAP|GL_TEXTURE_RECTANGLE_NV] capability. */
inline GLboolean GLIsEnabledTexture( GLenum target )
{
  NVSG_ASSERT(    ( target == GL_TEXTURE_1D ) || ( target == GL_TEXTURE_2D ) || ( target == GL_TEXTURE_3D )
              ||  ( target == GL_TEXTURE_CUBE_MAP ) || ( target == GL_TEXTURE_RECTANGLE_NV ) );
  GLContext * ctx = glContext();
  return( GL_IS_ENABLED( target, ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_toActivate].m_texture[target] ) );
}

//! Replacement of glIsEnabled( [GL_TEXTURE_GEN_S|GL_TEXTURE_GEN_T|GL_TEXTURE_GEN_R|GL_TEXTURE_GEN_Q] )
/** This function returns the state of the [GL_TEXTURE_GEN_S|GL_TEXTURE_GEN_T|GL_TEXTURE_GEN_R|GL_TEXTURE_GEN_Q] capability. */
inline GLboolean GLIsEnabledTextureGen( GLenum target )
{
  NVSG_ASSERT(    ( target == GL_TEXTURE_GEN_S )
              ||  ( target == GL_TEXTURE_GEN_T )
              ||  ( target == GL_TEXTURE_GEN_R )
              ||  ( target == GL_TEXTURE_GEN_Q ) );
  GLContext * ctx = glContext();
  return( GL_IS_ENABLED( target, ctx->m_textureEnableAttributes.m_unit[ctx->m_textureAttributes.m_toActivate].m_gen[target-GL_TEXTURE_GEN_S] ) );
}

//! Returns the version of the OpenGL available on the target system.
/** \returns The floating point version of the OpenGL with the representation 
  * major.minor is returned as an integer calculated as 10*major+minor.\n\n
  * The OpenGL version 1.4, for example, will be reported as 14. */
inline GLint GLGetVersion()
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext() && glContext()->m_glFunctions);
  return glContext()->m_glFunctions->getVersion();
}

//! Test if a given extension is available on the current context.
/** \note Not all exported extensions might be available.
  * \returns  \c true, if all functions of \a extension are available, otherwise \c false  */
inline GLboolean GLIsExtensionAvailable( const std::string& extension )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext() && glContext()->m_glFunctions);
  return glContext()->m_glFunctions->isAvailable(extension);
}

//! Test if a given extension is exported on the current context.
/** \note Not all exported extensions might be available.
  * \returns  \c true, if \a extension is exported, otherwise \c false  */
inline GLboolean GLIsExtensionExported( const std::string& extension )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext() && glContext()->m_glFunctions);
  return glContext()->m_glFunctions->isExported(extension);
}

//! Replacement of glIsList
/** This function tests if for the current context, l\a ist denotes a valid display list and asserts on the result before returning. */
inline  GLboolean GLIsList( GLuint list )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext() && glContext()->m_glFunctions);
  NVSG_ASSERT(!!glContext()->isList(list)==!!glIsList(list));
  return glContext()->isList(list);
}

//! Replacement of glIsOcclusionQueryNV
/** This function tests if for the current context, \a id denotes a valid query and asserts on the result before returning. */
inline  GLboolean GLIsOcclusionQuery( GLuint id )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext()->m_glFunctions && glContext()->m_glFunctions->glIsOcclusionQueryNV);
  NVSG_ASSERT(!!glContext()->isOcclusionQuery(id)==!!glContext()->m_glFunctions->glIsOcclusionQueryNV(id));
  return( glContext()->isOcclusionQuery( id ) );
}

//! Replacement of glIsProgramNV
/** This function tests if for the current context, \a id denotes a valid program and asserts on the result before returning. */
inline  GLboolean GLIsProgram(GLuint id)
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext()->m_glFunctions && glContext()->m_glFunctions->glIsProgramNV);
  NVSG_ASSERT(!!glContext()->isProgram(id)==!!glContext()->m_glFunctions->glIsProgramNV(id));
  return glContext()->isProgram(id);
}

//! Replacement of glIsQueryARB
/** This function tests if for the current context, \a id denotes a valid query and asserts on the result before returning. */
inline  GLboolean GLIsQuery( GLuint id )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext()->m_glFunctions && glContext()->m_glFunctions->glIsQueryARB);
  NVSG_ASSERT(!!glContext()->isQuery(id)==!!glContext()->m_glFunctions->glIsQueryARB(id));
  return glContext()->isQuery(id);
}

//! Replacement of glIsTexture
/** This function tests if for the current context, \a texture denotes a valid texture and asserts on the result before returning. */
inline  GLboolean GLIsTexture( GLuint texture )
{
  NVSG_GL_STATS_IS_TEXTURE( texture );
  NVSG_ASSERT(glContext() && glContext()->m_glFunctions);
  NVSG_ASSERT(!!glContext()->isTexture(texture)==!!glIsTexture(texture));
  return(glContext()->isTexture(texture));
}

#if !defined(DOXYGEN_IGNORE)
#if defined( GL_CACHING )
inline void _GLLightfCached( GLenum light, GLenum pname, GLfloat param, GLfloat * cache )
{
  if ( param != *cache )
  {
#if !defined(NDEBUG)
    GLfloat f;
    glGetLightfv( light, pname, &f );
    NVSG_ASSERT( f == *cache );
#endif
    _GLLightf( light, pname, param );
    *cache = param;
  }
}
inline void _GLLightfvCached( GLenum light, GLenum pname, const GLfloat * params, GLfloat * cache )
{
  if ( !equal4fv( params, cache ) )
  {
#if !defined(NDEBUG)
    GLfloat f[4];
    glGetLightfv( light, pname, f );
    NVSG_ASSERT( equal4fv( f, cache ) );
#endif
    _GLLightfv( light, pname, params );
    memcpy( cache, params, 4*sizeof(GLfloat) );
  }
}
# if defined(GL_SUPPORT_RESET)
inline void _GLLightfCached( GLenum light, GLenum pname, GLfloat param, GLfloat * cache, GLfloat * prev )
{
  *prev = *cache;
  _GLLightfCached( light, pname, param, cache );
}
inline void _GLLightfvCached( GLenum light, GLenum pname, const GLfloat * params, GLfloat * cache, GLfloat * prev )
{
  memcpy( prev, cache, 4*sizeof(GLfloat) );
  _GLLightfvCached( light, pname, params, cache );
}
#  define GL_LIGHT_F( light, pname, param, cache, prev )    _GLLightfCached( light, pname, param, cache, prev )
#  define GL_LIGHT_FV( light, pname, params, cache, prev )  _GLLightfvCached( light, pname, params, cache, prev )
# else
#  define GL_LIGHT_F( light, pname, param, cache, prev )    _GLLightfCached( light, pname, param, cache )
#  define GL_LIGHT_FV( light, pname, params, cache, prev )  _GLLightfvCached( light, pname, params, cache )
# endif
#else
# define GL_LIGHT_F( light, pname, param, cache, prev )     _GLLightf( light, pname, param )
# define GL_LIGHT_FV( light, pname, params, cache, prev )   _GLLightfv( light, pname, params )
#endif
#endif // DOXYGEN_IGNORE

//! Cached version of GLLightfv( light, GL_AMBIENT, param )
/** This function caches the GL_AMBIENT value on the light.
  * It is faster to call this method instead of GLLightfv( light, GL_AMBIENT, param ) directly. */
inline  void  GLLightAmbient( GLenum light, const GLfloat *params )
{
  GL_LIGHT_FV( light, GL_AMBIENT, params
             , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_ambient[0]
             , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_ambient[0] );
}

/*! \brief Helper function to reset the ambient light color.
 *  \param light The id of the light for which the ambient color is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the ambient
 *  light to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLLightAmbient */
inline void GLResetLightAmbient( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightAmbient( light, &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_ambient[0] );
#else
  static nvmath::Vec4f al(0.0f,0.0f,0.0f,1.0f);
  GLLightAmbient(light, &al[0]);
#endif
}

//! Cached version of GLLightf( light, GL_CONSTANT_ATTENUATION, param )
/** This function caches the GL_CONSTANT_ATTENUATION value on the light.
  * It is faster to call this method instead of GLLightf( light, GL_CONSTANT_ATTENUATION, param ) directly. */
inline  void  GLLightConstantAttenuation( GLenum light, GLfloat param )
{
  GL_LIGHT_F( light, GL_CONSTANT_ATTENUATION, param
            , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[0]
            , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[0] );
}

/*! \brief Helper function to reset the constant attenuation of the light \a light.
 *  \param light The id of the light for which the constant attenuation is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the constant
 *  attenuation of the light \a light to it's previous state. Otherwise, it is set to 1.0f.
 *  \sa GLLightConstantAttenuation */
inline void GLResetLightConstantAttenuation( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightConstantAttenuation( light, glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[0] );
#else
  GLLightConstantAttenuation( light, 1.0f );
#endif
}

//! Cached version of GLLightfv( light, GL_DIFFUSE, param )
/** This function caches the GL_DIFFUSE value on the light.
  * It is faster to call this method instead of GLLightfv( light, GL_DIFFUSE, param ) directly. */
inline  void  GLLightDiffuse( GLenum light, const GLfloat *params )
{
  GL_LIGHT_FV( light, GL_DIFFUSE, params
             , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_diffuse[0]
             , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_diffuse[0] );
}

/*! \brief Helper function to reset the diffuse color of the light \a light.
 *  \param light The id of the light for which the diffuse color is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the diffuse
 *  lighting to it's previous state. Otherwise, it is set to (1,1,1,1) if \a light is GL_LIGHT0, or
 *  (0,0,0,1) otherwise.
 *  \sa GLLightDiffuse */
inline void GLResetLightDiffuse( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightDiffuse( light, &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_diffuse[0] );
#else
  static Vec4f ld[2] = { Vec4f(1.0f,1.0f.1.0f,1.0f), Vec4f(0.0f,0.0f,0.0f,1.0f) };
  GLLightDiffuse( light, &ld[light!=GL_LIGHT0][0] );
#endif
}

//! Cached version of GLLightf( light, GL_LINEAR_ATTENUATION, param )
/** This function caches the GL_LINEAR_ATTENUATION value on the light.
  * It is faster to call this method instead of GLLightf( light, GL_LINEAR_ATTENUATION, param ) directly. */
inline  void  GLLightLinearAttenuation( GLenum light, GLfloat param )
{
  GL_LIGHT_F( light, GL_LINEAR_ATTENUATION, param
            , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[1]
            , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[1] );
}

/*! \brief Helper function to reset the linear attenuation of the light \a light.
 *  \param light The id of the light for which the linear attenuation is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the linear
 *  attenuation of the light \a light to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLLightLinearAttenuation */
inline void GLResetLightLinearAttenuation( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightLinearAttenuation( light, glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[1] );
#else
  GLLightLinearAttenuation( light, 0.0f );
#endif
}

//! Cached version of GLLightfv( light, GL_POSITION, param )
/** The light position is not cached, and it should not be. This function just exists for completeness. */
inline  void  GLLightPosition( GLenum light, const GLfloat *params )
{
#if defined( GL_CACHING )
# if defined( GL_SUPPORT_RESET )
  GLContext * ctx = glContext();
  ctx->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_position = ctx->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_position;
  memcpy( &ctx->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_position[0], params, 4*sizeof(GLfloat) );
# endif
  _GLLightfv( light, GL_POSITION, params );
#else
  GLLightfv( light, GL_POSITION, params );
#endif
}

/*! \brief Helper function to reset the position of the light \a light.
 *  \param light The id of the light for which the position is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the position
 *  of the light \a light to it's previous state. Otherwise, it is set to (0,0,1,0).
 *  \sa GLLightPosition */
inline void GLResetLightPosition( GLenum light )
{
#if defined( GL_SUPPORT_RESET )
  GLLightPosition( light, &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_position[0] );
#else
  static nvmath::Vec4f lp(0.0f,0.0f,1.0f,0.0f);
  GLLightPosition( light, &lp[0] );
#endif
}

//! Cached version of GLLightf( light, GL_QUADRATIC_ATTENUATION, param )
/** This function caches the GL_QUADRATIC_ATTENUATION value on the light.
  * It is faster to call this method instead of GLLightf( light, GL_QUADRATIC_ATTENUATION, param ) directly. */
inline  void  GLLightQuadraticAttenuation( GLenum light, GLfloat param )
{
  GL_LIGHT_F( light, GL_QUADRATIC_ATTENUATION, param
            , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[2]
            , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[2] );
}

/*! \brief Helper function to reset the quadratic attenuation of the light \a light.
 *  \param light The id of the light for which the quadratic attenuation is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the quadratic
 *  attenuation of the light \a light to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLLightQuadraticAttenuation */
inline void GLResetLightQuadraticAttenuation( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightQuadraticAttenuation( light, glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_attenuation[2] );
#else
  GLLightQuadraticAttenuation( light, 0.0f );
#endif
}

//! Cached version of GLLightfv( light, GL_SPECULAR, param )
/** This function caches the GL_SPECULAR value on the light.
  * It is faster to call this method instead of GLLightfv( light, GL_SPECULAR, param ) directly. */
inline  void  GLLightSpecular( GLenum light, const GLfloat *params )
{
  GL_LIGHT_FV( light, GL_SPECULAR, params
             , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_specular[0]
             , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_specular[0] );
}

/*! \brief Helper function to reset the specular color of the light \a light.
 *  \param light The id of the light for which the specular color is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the specular
 *  color of the light \a light to it's previous state. Otherwise, it is set to (1,1,1,1) if \a
 *  light is GL_LIGHT0, or (0,0,0,1) otherwise.
 *  \sa GLLightSpecular */
inline void GLResetLightSpecular( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightSpecular( light, &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_specular[0] );
#else
  static Vec4f ls[2] = { Vec4f(1.0f,1.0f.1.0f,1.0f), Vec4f(0.0f,0.0f,0.0f,1.0f) };
  GLLightSpecular( light, &ls[light!=GL_LIGHT0][0] );
#endif
}

//! Cached version of GLLightf( light, GL_SPOT_CUTOFF, param )
/** This function caches the GL_SPOT_CUTOFF value on the light.
  * It is faster to call this method instead of GLLightf( light, GL_SPOT_CUTOFF, param ) directly. */
inline  void  GLLightSpotCutoff( GLenum light, GLfloat param )
{
  GL_LIGHT_F( light, GL_SPOT_CUTOFF, param
            , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_spotCutoff
            , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_spotCutoff );
}

/*! \brief Helper function to reset the spot cutoff of the light \a light.
 *  \param light The id of the light for which the spot cutoff is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the spot cutoff
 *  of the light \a light to it's previous state. Otherwise, it is set to 180.0f.
 *  \sa GLLightSpotCutoff */
inline void GLResetLightSpotCutoff( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightSpotCutoff( light, glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_spotCutoff );
#else
  GLLightSpotCutoff( light, 180.0f );
#endif
}

//! Cached version of GLLightfv( light, GL_SPOT_DIRECTION, param )
/** The light spot direction is not cached, and it should not be. This function just exists for completeness. */
inline  void  GLLightSpotDirection( GLenum light, const GLfloat *params )
{
#if defined( GL_CACHING )
# if defined( GL_SUPPORT_RESET )
  GLContext * ctx = glContext();
  ctx->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_spotDirection = ctx->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_spotDirection;
  memcpy( &ctx->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_spotDirection[0], params, 4*sizeof(GLfloat) );
# endif
  _GLLightfv( light, GL_SPOT_DIRECTION, params );
#else
  GLLightfv( light, GL_SPOT_DIRECTION, params );
#endif
}

/*! \brief Helper function to reset the spot cutoff of the light \a light.
 *  \param light The id of the light for which the spot cutoff is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the spot cutoff
 *  of the light \a light to it's previous state. Otherwise, it is set to 180.0f.
 *  \sa GLLightSpotCutoff */
inline void GLResetLightSpotDirection( GLenum light )
{
#if defined( GL_SUPPORT_RESET )
  GLLightSpotDirection( light
                      , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_spotDirection[0] );
#else
  static nvmath::Vec4f lsd(0.0f,0.0f,-1.0f,1.0f);
  GLLightSpotDirection( light, &lsd[0] );
#endif
}

//! Cached version of GLLightf( light, GL_SPOT_EXPONENT, param )
/** This function caches the GL_SPOT_EXPONENT value on the light.
  * It is faster to call this method instead of GLLightf( light, GL_SPOT_EXPONENT, param ) directly. */
inline  void  GLLightSpotExponent( GLenum light, GLfloat param )
{
  GL_LIGHT_F( light, GL_SPOT_EXPONENT, param
            , &glContext()->m_lightingAttributes.m_lightParameters[light-GL_LIGHT0].m_spotExponent
            , &glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_spotExponent );
}

/*! \brief Helper function to reset the spot exponent of the light \a light.
 *  \param light The id of the light for which the spot exponent is to be reset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the spot
 *  exponent of the light \a light to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLLightSpotExponent */
inline void GLResetLightSpotExponent( GLenum light )
{
#if defined(GL_SUPPORT_RESET)
  GLLightSpotExponent( light, glContext()->m_previousAttributes.m_lightParameters[light-GL_LIGHT0].m_spotExponent );
#else
  GLLightSpotExponent( light, 0.0f );
#endif
}

//! Replacement of glLightf
/** This function caches on some pname values and asserts on the result. */
inline  void  GLLightf( GLenum light, GLenum pname, GLfloat param ) 
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_CONSTANT_ATTENUATION :
      GLLightConstantAttenuation( light, param );
      break;
    case GL_LINEAR_ATTENUATION :
      GLLightLinearAttenuation( light, param );
      break;
    case GL_QUADRATIC_ATTENUATION :
      GLLightQuadraticAttenuation( light, param );
      break;
    case GL_SPOT_CUTOFF :
      GLLightSpotCutoff( light, param );
      break;
    case GL_SPOT_EXPONENT :
      GLLightSpotExponent( light, param );
      break;
    default :
      _GLLightf( light, pname, param );
      break;
  }
#else
  _GLLightf( light, pname, param );
#endif
}

//! Replacement of glLightfv
/** This function caches on some pname values and asserts on the result. */
inline  void  GLLightfv( GLenum light, GLenum pname, const GLfloat *params ) 
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_AMBIENT :
      GLLightAmbient( light, params );
      break;
    case GL_CONSTANT_ATTENUATION :
      GLLightConstantAttenuation( light, params[0] );
      break;
    case GL_DIFFUSE :
      GLLightDiffuse( light, params );
      break;
    case GL_LINEAR_ATTENUATION :
      GLLightLinearAttenuation( light, params[0] );
      break;
    case GL_POSITION :
      GLLightPosition( light, params );
      break;
    case GL_QUADRATIC_ATTENUATION :
      GLLightQuadraticAttenuation( light, params[0] );
      break;
    case GL_SPECULAR :
      GLLightSpecular( light, params );
      break;
    case GL_SPOT_CUTOFF :
      GLLightSpotCutoff( light, params[0] );
      break;
    case GL_SPOT_DIRECTION :
      GLLightSpotDirection( light, params );
      break;
    case GL_SPOT_EXPONENT :
      GLLightSpotExponent( light, params[0] );
      break;
    default :
      _GLLightfv( light, pname, params );
      break;
  }
#else
  _GLLightfv( light, pname, params );
#endif
}

//! Replacement of glLighti
/** This function caches on some pname values and asserts on the result. */
inline  void  GLLighti( GLenum light, GLenum pname, GLint param ) 
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_CONSTANT_ATTENUATION :
      GLLightConstantAttenuation( light, (GLfloat)param );
      break;
    case GL_LINEAR_ATTENUATION :
      GLLightLinearAttenuation( light, (GLfloat)param );
      break;
    case GL_QUADRATIC_ATTENUATION :
      GLLightQuadraticAttenuation( light, (GLfloat)param );
      break;
    case GL_SPOT_CUTOFF :
      GLLightSpotCutoff( light, (GLfloat)param );
      break;
    case GL_SPOT_EXPONENT :
      GLLightSpotExponent( light, (GLfloat)param );
      break;
    default :
      _GLLighti( light, pname, param );
      break;
  }
#else
  _GLLighti( light, pname, param );
#endif
}

//! Convert GLInt to GLFloat
inline  void  GLintToGLfloat( const GLint *params, GLfloat *fParams )
{
  for ( size_t i=0 ; i<4 ; i++ )
  {
    fParams[i] = (GLfloat)params[i] / INT_MAX;
  }
}

//! Replacement of glLightiv
/** This function caches on some pname values and asserts on the result. */
inline  void  GLLightiv( GLenum light, GLenum pname, GLint *params ) 
{ 
#if defined( GL_CACHING )
  GLfloat fParams[4];
  switch( pname )
  {
    case GL_AMBIENT :
      GLintToGLfloat( params, fParams );
      GLLightAmbient( light, fParams );
      break;
    case GL_CONSTANT_ATTENUATION :
      GLLightConstantAttenuation( light, (GLfloat)params[0] );
      break;
    case GL_DIFFUSE :
      GLintToGLfloat( params, fParams );
      GLLightDiffuse( light, fParams );
      break;
    case GL_LINEAR_ATTENUATION :
      GLLightLinearAttenuation( light, (GLfloat)params[0] );
      break;
    case GL_POSITION :
      for ( size_t i=0 ; i<4 ; i++ )
      {
        fParams[i] = (GLfloat)params[i];
      }
      GLLightPosition( light, fParams );
      break;
    case GL_QUADRATIC_ATTENUATION :
      GLLightQuadraticAttenuation( light, (GLfloat)params[0] );
      break;
    case GL_SPECULAR :
      GLintToGLfloat( params, fParams );
      GLLightSpecular( light, fParams );
      break;
    case GL_SPOT_CUTOFF :
      GLLightSpotCutoff( light, (GLfloat)params[0] );
      break;
    case GL_SPOT_DIRECTION :
      for ( size_t i=0 ; i<4 ; i++ )
      {
        fParams[i] = (GLfloat)params[i];
      }
      GLLightSpotDirection( light, fParams );
      break;
    case GL_SPOT_EXPONENT :
      GLLightSpotExponent( light, (GLfloat)params[0] );
      break;
    default :
      _GLLightiv( light, pname, params );
      break;
  }
#else
  _GLLightiv( light, pname, params );
#endif
}

//! Replacement of glLightModelfv( GL_LIGHT_MODEL_AMBIENT, params )
/** This function caches the light model settings and asserts on the result before returning.  */
inline  void  GLLightModelAmbient( const GLfloat * params )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lightModelAmbient = ctx->m_lightingAttributes.m_lightModelAmbient;
# endif
  if ( !equal4fv( &ctx->m_lightingAttributes.m_lightModelAmbient[0], params ) )
#endif
  {
    _GLLightModelfv( GL_LIGHT_MODEL_AMBIENT, params );
#if defined( GL_CACHING )
    memcpy( &ctx->m_lightingAttributes.m_lightModelAmbient[0], params, 4*sizeof(float) );
#endif
  }
}

/*! \brief Helper function to reset the ambient light color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the ambient
 *  light color to it's previous state. Otherwise, it is set to (0.2,0.2,0.2,1.0).
 *  \sa GLLightSpotExponent */
inline void GLResetLightModelAmbient()
{
#if defined(GL_SUPPORT_RESET)
  GLLightModelAmbient( &glContext()->m_previousAttributes.m_lightModelAmbient[0] );
#else
  static nvmath::Vec4f lma(0.2f,0.2f,0.2f,1.0f);
  GLLightModelAmbient( &lma[0] );
#endif
}

//! Replacement of GLLightModelLocalViewer
/** This function caches the light model settings and asserts on the result before returning.  */
inline  void  GLLightModelLocalViewer( GLint param )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lightModelLocalViewer = ctx->m_lightingAttributes.m_lightModelLocalViewer;
# endif
  if ( ctx->m_lightingAttributes.m_lightModelLocalViewer != !!param )
  {
# if !defined(NDEBUG)
    GLboolean lv;
    glGetBooleanv( GL_LIGHT_MODEL_LOCAL_VIEWER, &lv );
    NVSG_ASSERT( ctx->m_lightingAttributes.m_lightModelLocalViewer == !!lv );
# endif
#endif
    _GLLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, param );
#if defined( GL_CACHING )
    ctx->m_lightingAttributes.m_lightModelLocalViewer = !!param;
  }
#endif
}

/*! \brief Helper function to reset the local viewer setting
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the local
 *  viewer to it's previous state. Otherwise, it is set to GL_FALSE.
 *  \sa GLLightModelLocalViewer */
inline void GLResetLightModelLocalViewer()
{
#if defined(GL_SUPPORT_RESET)
  GLLightModelLocalViewer( glContext()->m_previousAttributes.m_lightModelLocalViewer );
#else
  GLLightModelLocalViewer( GL_FALSE );
#endif
}

//! Replacement of GLLightModelTwoSide
/** This function caches the light model settings and asserts on the result before returning.  */
inline  void  GLLightModelTwoSide( GLint param )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lightModelTwoSide = ctx->m_lightingAttributes.m_lightModelTwoSide;
# endif
  if ( ctx->m_lightingAttributes.m_lightModelTwoSide != !!param )
#endif
  {
    if ( ! ctx->m_locks.m_lightModelTwoSide )
    {
#if !defined( NDEBUG ) && defined( GL_CACHING )
      GLboolean lv;
      glGetBooleanv( GL_LIGHT_MODEL_TWO_SIDE, &lv );
      NVSG_ASSERT( ctx->m_lightingAttributes.m_lightModelTwoSide == !!lv );
#endif
      _GLLightModeli( GL_LIGHT_MODEL_TWO_SIDE, param );
#if defined( GL_CACHING )
      ctx->m_lightingAttributes.m_lightModelTwoSide = !!param;
#endif
    }
  }
}

/*! \brief Helper function to reset the two sided lighting state.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the two sided
 *  lighting to it's previous state. Otherwise, it is set to GL_FALSE.
 *  \sa GLLightModelTwoSide */
inline void GLResetLightModelTwoSide()
{
#if defined(GL_SUPPORT_RESET)
  GLLightModelTwoSide( glContext()->m_previousAttributes.m_lightModelTwoSide );
#else
  GLLightModelTwoSide( GL_FALSE );
#endif
}

/*! \brief Helper function to set two sided lighting and lock it.
 *  \param param The value to set and lock two sided lighting to.
 *  \remarks This function sets two sided lighting to \a param and locks it. Any subsequent calls to
 *  GLLightModelTwoSide or GLLightModel{if}(GL_LIGHT_MODEL_TWO_SIDE,) will not change that setting,
 *  until the next call to GLLightModelTwoSideUnlock.
 *  \sa GLLightModelTwoSide, GLLightModelTwoSideUnlock */
inline void GLLightModelTwoSideLock( GLint param )
{
  NVSG_ASSERT( !glContext()->m_locks.m_lightModelTwoSide );
  GLLightModelTwoSide( param );
  glContext()->m_locks.m_lightModelTwoSide = true;
}

/*! \brief Helper function to unlock the two sided lighting and set it.
 *  \param param The value to set the unlocked two sided lighting to.
 *  \remarks This function unlocks two sided lighting and sets it to \a param.
 *  \sa GLLightModelTwoSide, GLLightModelTwoSideLock */
inline void GLLightModelTwoSideUnlock( GLint param )
{
  NVSG_ASSERT( glContext()->m_locks.m_lightModelTwoSide );
  glContext()->m_locks.m_lightModelTwoSide = false;
  GLLightModelTwoSide( param );
}

//! Replacement of glLightModelf
/** This function caches some of the light model settings and asserts on the result before returning.  */
inline  void  GLLightModelf( GLenum pname, GLfloat param )  
{
  switch( pname )
  {
    case GL_LIGHT_MODEL_LOCAL_VIEWER :
      GLLightModelLocalViewer( (GLint)param );
      break;
    case GL_LIGHT_MODEL_TWO_SIDE :
      GLLightModelTwoSide( (GLint)param );
      break;
    default :
      _GLLightModelf( pname, param );
      break;
  }
}

//! Replacement of glLightModelfv
/** This function caches some the light model settings and asserts on the result before returning.  */
inline  void  GLLightModelfv( GLenum pname, const GLfloat *params ) 
{ 
  switch( pname )
  {
    case GL_LIGHT_MODEL_AMBIENT :
      GLLightModelAmbient( params );
      break;
    case GL_LIGHT_MODEL_LOCAL_VIEWER :
      GLLightModelLocalViewer( (GLint)params[0] );
      break;
    case GL_LIGHT_MODEL_TWO_SIDE :
      GLLightModelTwoSide( (GLint)params[0] );
      break;
    default :
      _GLLightModelfv( pname, params );
      break;
  }
}

//! Replacement of glLightModeli
/** This function caches some of the light model settings and asserts on the result before returning.  */
inline  void  GLLightModeli( GLenum pname, GLint param )  
{
  switch( pname )
  {
    case GL_LIGHT_MODEL_LOCAL_VIEWER :
      GLLightModelLocalViewer( param );
      break;
    case GL_LIGHT_MODEL_TWO_SIDE :
      GLLightModelTwoSide( param );
      break;
    default :
      _GLLightModeli( pname, param );
      break;
  }
}

//! Replacement of glLightModeliv
/** This function caches some of the light model settings and asserts on the result before returning.  */
inline  void  GLLightModeliv( GLenum pname, const GLint *params ) 
{ 
  switch( pname )
  {
    case GL_LIGHT_MODEL_LOCAL_VIEWER :
      GLLightModelLocalViewer( params[0] );
      break;
    case GL_LIGHT_MODEL_TWO_SIDE :
      GLLightModelTwoSide( params[0] );
      break;
    default :
      _GLLightModeliv( pname, params );
      break;
  }
}

//! Replacement of glLineStipple
/** This function caches the line stipple settings and asserts on the result before returning.  */
inline  void  GLLineStipple( GLint factor, GLushort pattern )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lineStipplePattern = ctx->m_lineAttributes.m_stipplePattern;
  ctx->m_previousAttributes.m_lineStippleRepeat  = ctx->m_lineAttributes.m_stippleRepeat;
# endif
  if ( ( ctx->m_lineAttributes.m_stippleRepeat != factor ) || ( ctx->m_lineAttributes.m_stipplePattern != pattern ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glLineStipple( factor, pattern );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_lineAttributes.m_stippleRepeat = factor;
    ctx->m_lineAttributes.m_stipplePattern = pattern;
#endif
  }
}

/*! \brief Helper function to reset line stipple repeat and pattern.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the line
 *  stipple repeat and pattern to it's previous state. Otherwise, it is set to 1 and 0xFFFF,
 *  respectively.
 *  \sa GLLineStipple */
inline void GLResetLineStippleDefinition()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLLineStipple( ctx->m_previousAttributes.m_lineStippleRepeat
               , ctx->m_previousAttributes.m_lineStipplePattern );
#else
  GLLineStipple( 1, (GLushort)0xFFFF );
#endif
}

//! Replacement of glLineWidth
/** This function caches the line width setting and asserts on the result before returning.  */
inline  void  GLLineWidth( GLfloat width )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_lineWidth = ctx->m_lineAttributes.m_width;
# endif
  if ( ctx->m_lineAttributes.m_width != width )
  {
# if !defined(NDEBUG)
    GLfloat f;
    glGetFloatv( GL_LINE_WIDTH, &f );
    NVSG_ASSERT( ctx->m_lineAttributes.m_width == f );
# endif
#endif
    NVSG_GL_STATS_LINE_WIDTH( width );
    glLineWidth( width );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_lineAttributes.m_width = width;
  }
#endif
}

/*! \brief Helper function to reset the line width.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the line
 *  width to it's previous state. Otherwise, it is set to 1.0f.
 *  \sa GLLineWidth */
inline void GLResetLineWidth()
{
#if defined(GL_SUPPORT_RESET)
  GLLineWidth( glContext()->m_previousAttributes.m_lineWidth );
#else
  GLLineWidth( 1.0f );
#endif
}

//! Replacement of glListBase
/** This function asserts on the result before returning.  */
inline  void  GLListBase( GLuint base )
{
  NVSG_GL_STATS_FCT_COUNT();
  glListBase( base );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadItentity
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLLoadIdentity( void )
{
  NVSG_GL_STATS_LOAD_IDENTITY();
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glLoadIdentity();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadMatrixd
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLLoadMatrixd( const GLdouble *m )
{
  NVSG_GL_STATS_LOAD_MATRIXD( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glLoadMatrixd( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadMatrixf
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLLoadMatrixf( const GLfloat *m )
{
  NVSG_GL_STATS_LOAD_MATRIXF( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glLoadMatrixf( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadTransposeMatrixd
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLLoadTransposedMatrixd( const GLdouble *m )
{
  NVSG_GL_STATS_LOAD_MATRIXD( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glLoadTransposeMatrixd );
  glContext()->m_glFunctions->glLoadTransposeMatrixd( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadTransposMatrixf
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLLoadTransposeMatrixf( const GLfloat *m )
{
  NVSG_GL_STATS_LOAD_MATRIXF( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glLoadTransposeMatrixf );
  glContext()->m_glFunctions->glLoadTransposeMatrixf( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadName
/** This function asserts on the result before returning.  */
inline  void  GLLoadName( GLuint name )
{
  NVSG_GL_STATS_FCT_COUNT();
  glLoadName( name );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glLoadProgramNV
/** This function hides the extension character of glLoadProgramNV and asserts on the result before returning.  */
inline  void  GLLoadProgram( GLenum target, GLuint id, GLsizei len, const GLubyte *program )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glLoadProgramNV );
  glContext()->m_glFunctions->glLoadProgramNV( target, id, len, program );
#if !defined(NDEBUG)
  ASSERT_GL_NO_ERROR;
  GLint errorPos;
  glGetIntegerv( GL_PROGRAM_ERROR_POSITION_NV, &errorPos );
  NVSG_ASSERT( errorPos == -1 );
#endif
}

//! Replacement of glLogicOp
/** This function asserts on the result before returning.  */
inline  void  GLLogicOp( GLenum opcode )
{
  NVSG_GL_STATS_FCT_COUNT();
  glLogicOp( opcode );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMap1d
/** This function asserts on the result before returning.  */
inline  void  GLMap1d( GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMap1d( target, u1, u2, stride, order, points );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMap1f
/** This function asserts on the result before returning.  */
inline  void  GLMap1f( GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMap1f( target, u1, u2, stride, order, points );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMap2d
/** This function asserts on the result before returning.  */
inline  void  GLMap2d( GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMap2d( target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMap2f
/** This function asserts on the result before returning.  */
inline  void  GLMap2f( GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMap2f( target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMapGrid1d
/** This function asserts on the result before returning.  */
inline  void  GLMapGrid1d( GLint un, GLdouble u1, GLdouble u2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMapGrid1d( un, u1, u2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMapGrid1f
/** This function asserts on the result before returning.  */
inline  void  GLMapGrid1f( GLint un, GLfloat u1, GLfloat u2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMapGrid1f( un, u1, u2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMapGrid2d
/** This function asserts on the result before returning.  */
inline  void  GLMapGrid2d( GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMapGrid2d( un, u1, u2, vn, v1, v2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMapGrid2f
/** This function asserts on the result before returning.  */
inline  void  GLMapGrid2f( GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glMapGrid2f( un, u1, u2, vn, v1, v2 );
  ASSERT_GL_NO_ERROR;
}

#if !defined(DOXYGEN_IGNORE)
#if defined( GL_CACHING )
inline void _GLMaterialfCached( GLenum face, GLenum pname, GLfloat param, GLfloat * cache )
{
  if ( !equalf( param, *cache ) )
  {
#if !defined(NDEBUG)
    GLfloat f;
    glGetMaterialfv( face, pname, &f );
    NVSG_ASSERT( *cache == f );
#endif
    _GLMaterialf( face, pname, param );
    *cache = param;
  }
}
inline void _GLMaterialfCached( GLenum pname, GLfloat param, GLfloat * cacheFront, GLfloat * cacheBack )
{
  if ( !equalf( param, *cacheFront ) || !equalf( param, *cacheBack ) )
  {
#if !defined(NDEBUG)
    GLfloat f0, f1;
    glGetMaterialfv( GL_FRONT, pname, &f0 );
    glGetMaterialfv( GL_BACK, pname, &f1 );
    NVSG_ASSERT( ( *cacheFront == f0 ) && ( *cacheBack == f1 ) );
#endif
    _GLMaterialf( GL_FRONT_AND_BACK, pname, param );
    *cacheFront = param;
    *cacheBack = param;
  }
}
inline void _GLMaterialfvCached( GLenum face, GLenum pname, const GLfloat * params, GLfloat * cache )
{
  if ( !equal4fv( params, cache ) )
  {
#if !defined(NDEBUG)
    GLfloat p[4];
    glGetMaterialfv( face, pname, p );
    NVSG_ASSERT( equal4fv( cache, p ) );
#endif
    _GLMaterialfv( face, pname, params );
    memcpy( cache, params, 4*sizeof(float) );
  }
}
inline void _GLMaterialfvCached( GLenum face, GLenum pname, const GLfloat * params, GLfloat * cache, bool colorMaterial )
{
  if ( colorMaterial )
  {
    _GLMaterialfv( face, pname, params );
  }
  else
  {
    _GLMaterialfvCached( face, pname, params, cache );
  }
}
inline void _GLMaterialfvCached( GLenum pname, const GLfloat * params, GLfloat * cacheFront, GLfloat * cacheBack )
{
  if ( !equal4fv( params, cacheFront ) || !equal4fv( params, cacheBack ) )
  {
#if !defined(NDEBUG)
    GLfloat p0[4], p1[4];
    glGetMaterialfv( GL_FRONT, pname, p0 );
    glGetMaterialfv( GL_BACK, pname, p1 );
    NVSG_ASSERT( equal4fv( cacheFront, p0 ) && equal4fv( cacheBack, p1 ) );
#endif
    _GLMaterialfv( GL_FRONT_AND_BACK, pname, params );
    memcpy( cacheFront, params, 4*sizeof(float) );
    memcpy( cacheBack, params, 4*sizeof(float) );
  }
}
inline void _GLMaterialfvCached( GLenum pname, const GLfloat * params, GLfloat * cacheFront, GLfloat * cacheBack, bool colorMaterial )
{
  if ( colorMaterial )
  {
    _GLMaterialfv( GL_FRONT_AND_BACK, pname, params );
  }
  else
  {
    _GLMaterialfvCached( pname, params, cacheFront, cacheBack );
  }
}
# if defined(GL_SUPPORT_RESET)
inline void _GLMaterialfCachedR( GLenum face, GLenum pname, GLfloat param, GLfloat * cache, GLfloat * prev )
{
  *prev = *cache;
  _GLMaterialfCached( face, pname, param, cache );
}
inline void _GLMaterialfCachedR( GLenum pname, GLfloat param, GLfloat * cacheFront, GLfloat * cacheBack, GLfloat * prevFront, GLfloat * prevBack )
{
  *prevFront = *cacheFront;
  *prevBack = *cacheBack;
  _GLMaterialfCached( pname, param, cacheFront, cacheBack );
}
inline void _GLMaterialfvCachedR( GLenum face, GLenum pname, const GLfloat * params, GLfloat * cache, GLfloat * prev )
{
  memcpy( prev, cache, 4*sizeof(GLfloat) );
  _GLMaterialfvCached( face, pname, params, cache );
}
inline void _GLMaterialfvCachedR( GLenum face, GLenum pname, const GLfloat * params, GLfloat * cache, GLfloat * prev, bool colorMaterial )
{
  memcpy( prev, cache, 4*sizeof(GLfloat) );
  _GLMaterialfvCached( face, pname, params, cache, colorMaterial );
}
inline void _GLMaterialfvCachedR( GLenum pname, const GLfloat * params, GLfloat * cacheFront, GLfloat * cacheBack, GLfloat * prevFront, GLfloat * prevBack )
{
  memcpy( prevFront, cacheFront, 4*sizeof(GLfloat) );
  memcpy( prevBack, cacheBack, 4*sizeof(GLfloat) );
  _GLMaterialfvCached( pname, params, cacheFront, cacheBack );
}
inline void _GLMaterialfvCachedR( GLenum pname, const GLfloat * params, GLfloat * cacheFront, GLfloat * cacheBack, GLfloat * prevFront, GLfloat * prevBack, bool colorMaterial )
{
  memcpy( prevFront, cacheFront, 4*sizeof(GLfloat) );
  memcpy( prevBack, cacheBack, 4*sizeof(GLfloat) );
  _GLMaterialfvCached( pname, params, cacheFront, cacheBack, colorMaterial );
}
#  define GL_MATERIAL_F( face, pname, param, cache, prev )                                    _GLMaterialfCachedR( face, pname, param, cache, prev )
#  define GL_MATERIAL_F2( pname, param, cacheFront, cacheBack, prevFront, prevBack )          _GLMaterialfCachedR( pname, param, cacheFront, cacheBack, prevFront, prevBack )
#  define GL_MATERIAL_FV( face, pname, params, cache, prev )                                  _GLMaterialfvCachedR( face, pname, params, cache, prev )
#  define GL_MATERIAL_FV2( pname, params, cacheFront, cacheBack, prevFront, prevBack )        _GLMaterialfvCachedR( pname, params, cacheFront, cacheBack, prevFront, prevBack )
#  define GL_MATERIAL_FV_CM( face, pname, params, cache, prev, cm )                           _GLMaterialfvCachedR( face, pname, params, cache, prev, cm )
#  define GL_MATERIAL_FV2_CM( pname, params, cacheFront, cacheBack, prevFront, prevBack, cm ) _GLMaterialfvCachedR( pname, params, cacheFront, cacheBack, prevFront, prevBack, cm )
# else
#  define GL_MATERIAL_F( face, pname, param, cache, prev )                                    _GLMaterialfCached( face, pname, param, cache )
#  define GL_MATERIAL_F2( pname, param, cacheFront, cacheBack, prevFront, prevBack )          _GLMaterialfCached( pname, param, cacheFront, cacheBack )
#  define GL_MATERIAL_FV( face, pname, params, cache, prev )                                  _GLMaterialfvCached( face, pname, params, cache )
#  define GL_MATERIAL_FV2( pname, params, cacheFront, cacheBack, prevFront, prevBack )        _GLMaterialfvCached( pname, params, cacheFront, cacheBack )
#  define GL_MATERIAL_FV_CM( face, pname, params, cache, prev, cm )                           _GLMaterialfvCached( face, pname, params, cache, cm )
#  define GL_MATERIAL_FV2_CM( pname, params, cacheFront, cacheBack, prevFront, prevBack, cm ) _GLMaterialfvCached( pname, params, cacheFront, cacheBack, cm )
# endif
#else
# define GL_MATERIAL_F( face, pname, param, cache, prevFront )                                _GLMaterialf( face, pname, param )
# define GL_MATERIAL_F2( pname, param, cacheFront, cacheBack, prevFront, prevBack )           _GLMaterialf( GL_FRONT_AND_BACK, pname, param )
# define GL_MATERIAL_FV( face, pname, params, cache, prev )                                   _GLMaterialfv( face, pname, params )
# define GL_MATERIAL_FV2( pname, params, cacheFront, cacheBack, prevFront, prevBack )         _GLMaterialfv( GL_FRONT_AND_BACK, pname, params )
# define GL_MATERIAL_FV_CM( face, pname, params, cache, prev, cm )                            _GLMaterialfv( face, pname, params )
# define GL_MATERIAL_FV2_CM( pname, params, cacheFront, cacheBack, prevFront, prevBack, cm )  _GLMaterialfv( GL_FRONT_AND_BACK, pname, params )
#endif

#if defined(GL_SUPPORT_RESET)
inline void _GLResetMaterialf( GLenum pname, GLfloat defParam, GLfloat * cacheFront, GLfloat * cacheBack, GLfloat * prevFront, GLfloat * prevBack )
{
  if ( *prevFront == *prevBack )
  {
    _GLMaterialfCachedR( pname, *prevFront, cacheFront, cacheBack, prevFront, prevBack );
  }
  else
  {
    _GLMaterialfCachedR( GL_FRONT, pname, *prevFront, cacheFront, prevFront );
    _GLMaterialfCachedR( GL_BACK, pname, *prevBack, cacheBack, prevBack );
  }
}
inline void _GLResetMaterialfv( GLenum face, GLenum pname, GLfloat * cache, GLfloat * prev )
{
  GLfloat tmp[4];
  memcpy( &tmp[0], prev, 4*sizeof(GLfloat) );
  _GLMaterialfvCachedR( face, pname, tmp, cache, prev );
}
inline void _GLResetMaterialfv( GLenum pname, GLfloat * cacheFront, GLfloat * cacheBack, GLfloat * prevFront, GLfloat * prevBack )
{
  if ( equal4fv( prevFront, prevBack ) )
  {
    GLfloat tmp[4];
    memcpy( &tmp[0], prevFront, 4*sizeof(GLfloat) );
    _GLMaterialfvCachedR( pname, tmp, cacheFront, cacheBack, prevFront, prevBack );
  }
  else
  {
    _GLResetMaterialfv( GL_FRONT, pname, cacheFront, prevFront );
    _GLResetMaterialfv( GL_BACK, pname, cacheBack, prevBack );
  }
}
inline void _GLResetMaterialfv( GLenum face, GLenum pname, const GLfloat * defParams, GLfloat * cache, GLfloat * prev, bool colorMaterial )
{
  if ( colorMaterial )
  {
    _GLMaterialfv( face, pname, defParams );
  }
  else
  {
    _GLResetMaterialfv( face, pname, cache, prev );
  }
}
inline void _GLResetMaterialfv( GLenum pname, const GLfloat * defParams, GLfloat * cacheFront, GLfloat * cacheBack, GLfloat * prevFront, GLfloat * prevBack, bool colorMaterial )
{
  if ( colorMaterial )
  {
    _GLMaterialfv( GL_FRONT_AND_BACK, pname, defParams );
  }
  else
  {
    _GLResetMaterialfv( pname, cacheFront, cacheBack, prevFront, prevBack );
  }
}
# define GL_RESET_MATERIAL_F( face, pname, defParam, cache, prev )                                    _GLMaterialfCachedR( face, pname, *prev, cache, prev )
# define GL_RESET_MATERIAL_F2( pname, defParam, cacheFront, cacheBack, prevFront, prevBack )          _GLResetMaterialf( pname, defParam, cacheFront, cacheBack, prevFront, prevBack )
# define GL_RESET_MATERIAL_FV( face, pname, defParams, cache, prev )                                  _GLResetMaterialfv( face, pname, cache, prev )
# define GL_RESET_MATERIAL_FV2( pname, defParams, cacheFront, cacheBack, prevFront, prevBack )        _GLResetMaterialfv( pname, cacheFront, cacheBack, prevFront, prevBack )
# define GL_RESET_MATERIAL_FV_CM( face, pname, defParams, cache, prev, cm )                           _GLResetMaterialfv( face, pname, defParams, cache, prev, cm )
# define GL_RESET_MATERIAL_FV2_CM( pname, defParams, cacheFront, cacheBack, prevFront, prevBack, cm ) _GLResetMaterialfv( pname, defParams, cacheFront, cacheBack, prevFront, prevBack, cm )
#else
# define GL_RESET_MATERIAL_F( face, pname, defParam, cache, prev )                                    _GLMaterialf( face, pname, defParam )
# define GL_RESET_MATERIAL_F2( pname, defParam, cacheFront, cacheBack, prevFront, prevBack )          _GLMaterialf( GL_FRONT_AND_BACK, pname, defParam )
# define GL_RESET_MATERIAL_FV( face, pname, defParams, cache, prev )                                  _GLMaterialfv( face, pname, defParams )
# define GL_RESET_MATERIAL_FV2( pname, defParams, cacheFront, cacheBack, prevFront, prevBack )        _GLMaterialfv( GL_FRONT_AND_BACK, pname, defParams )
# define GL_RESET_MATERIAL_FV_CM( face, pname, defParams, cache, prev, cm )                           _GLMaterialfv( face, pname, defParams )
# define GL_RESET_MATERIAL_FV2_CM( pname, defParams, cacheFront, cacheBack, prevFront, prevBack, cm ) _GLMaterialfv( GL_FRONT_AND_BACK, pname, defParams )
#endif
#endif // DOXYGEN_IGNORE

//! Cached version of GLMaterialf( GL_BACK, GL_SHININESS, param )
/** This function caches the GL_SHININESS value on the GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_BACK, GL_SHININESS, param ) directly. */
inline  void  GLMaterialShininessBack( GLfloat param )
{
  GL_MATERIAL_F( GL_BACK, GL_SHININESS, param
               , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_BACK]
               , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_BACK] );
}

/*! \brief Helper function to reset the material back face shininess.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back face shininess to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLMaterialShininessBack */
inline void GLResetMaterialShininessBack()
{
  GL_RESET_MATERIAL_F( GL_BACK, GL_SHININESS, 0.0f
                     , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_BACK]
                     , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_BACK] );
}

//! Cached version of GLMaterialf( GL_FRONT, GL_SHININESS, param )
/** This function caches the GL_SHININESS value on the GL_FRONT face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT, GL_SHININESS, param ) directly. */
inline  void  GLMaterialShininessFront( GLfloat param )
{
  GL_MATERIAL_F( GL_FRONT, GL_SHININESS, param
               , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_FRONT]
               , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_FRONT] );
}

/*! \brief Helper function to reset the material front face shininess.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  front face shininess to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLMaterialShininessFront */
inline void GLResetMaterialShininessFront()
{
  GL_RESET_MATERIAL_F( GL_FRONT, GL_SHININESS, 0.0f
                     , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_FRONT]
                     , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_FRONT] );
}

//! Cached version of GLMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, param )
/** This function caches the GL_SHININESS value on both the GL_FRONT and GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, param ) directly. */
inline  void  GLMaterialShininessFrontAndBack( GLfloat param )
{
  GL_MATERIAL_F2( GL_SHININESS, param
                , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_FRONT]
                , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_BACK]
                , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_FRONT]
                , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_BACK] );
}

/*! \brief Helper function to reset the material back and front face shininess.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back and front face shininess to it's previous state. Otherwise, it is set to 0.0f.
 *  \sa GLMaterialShininessFrontAndBack */
inline void GLResetMaterialShininessFrontAndBack()
{
  GL_RESET_MATERIAL_F2( GL_SHININESS, 0.0f
                      , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_FRONT]
                      , &glContext()->m_lightingAttributes.m_materialShininess[NVSG_CACHE_BACK]
                      , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_FRONT]
                      , &glContext()->m_previousAttributes.m_materialShininess[NVSG_CACHE_BACK] );
}

//! Cached version of GLMaterialf( face, GL_SHININESS, param )
/** This function caches the GL_SHININESS value of the material.
  * It is faster to call this method instead of GLMaterialf(face,GL_SHININESS,param) directly. */
inline  void  GLMaterialShininess( GLenum face, GLfloat param )
{
#if defined( GL_CACHING )
  switch( face )
  {
    case GL_BACK :
      GLMaterialShininessBack( param );
      break;
    case GL_FRONT :
      GLMaterialShininessFront( param );
      break;
    case GL_FRONT_AND_BACK :
      GLMaterialShininessFrontAndBack( param );
      break;
    default :
      NVSG_ASSERT( false );
      break;
  }
#else
  GLMaterialf( face, GL_SHININESS, param );
#endif
}

//! Cached version of GLMaterialf( GL_BACK, GL_AMBIENT, param )
/** This function caches the GL_AMBIENT value on the GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_BACK, GL_AMBIENT, param ) directly. */
inline  void  GLMaterialAmbientBack( const GLfloat *params )
{
  GL_MATERIAL_FV_CM( GL_BACK, GL_AMBIENT, params
                   , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                   , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                   , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the material back face ambient color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back face ambient color to it's previous state. Otherwise, it is set to (0.2,0.2,0.2,1.0).
 *  \sa GLMaterialAmbientBack */
inline void GLResetMaterialAmbientBack()
{
  static nvmath::Vec4f ma(0.2f,0.2f,0.2f,1.0f);
  GL_RESET_MATERIAL_FV_CM( GL_BACK, GL_AMBIENT, &ma[0]
                         , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                         , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                         , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

//! Cached version of GLMaterialf( GL_FRONT, GL_AMBIENT, param )
/** This function caches the GL_AMBIENT value on the GL_FRONT face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT, GL_AMBIENT, param ) directly. */
inline  void  GLMaterialAmbientFront( const GLfloat *params )
{
  GL_MATERIAL_FV_CM( GL_FRONT, GL_AMBIENT, params
                   , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                   , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                   , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the material front face ambient color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  front face ambient color to it's previous state. Otherwise, it is set to (0.2,0.2,0.2,1.0).
 *  \sa GLMaterialAmbientFront */
inline void GLResetMaterialAmbientFront()
{
  static nvmath::Vec4f ma(0.2f,0.2f,0.2f,1.0f);
  GL_RESET_MATERIAL_FV_CM( GL_FRONT, GL_AMBIENT, &ma[0]
                         , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                         , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                         , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

//! Cached version of GLMaterialf( GL_FRONT_AND_BACK, GL_AMBIENT, param )
/** This function caches the GL_AMBIENT value on both the GL_FRONT and GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT_AND_BACK, GL_AMBIENT, param ) directly. */
inline  void  GLMaterialAmbientFrontAndBack( const GLfloat *params )
{
  GL_MATERIAL_FV2_CM( GL_AMBIENT, params
                    , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                    , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                    , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                    , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                    , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the material back and front face ambient color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back and front face ambient color to it's previous state. Otherwise, it is set to
 *  (0.2,0.2,0.2,1.0).
 *  \sa GLMaterialAmbientFrontAndBack */
inline void GLResetMaterialAmbientFrontAndBack()
{
  static nvmath::Vec4f ma(0.2f,0.2f,0.2f,1.0f);
  GL_RESET_MATERIAL_FV2_CM( GL_AMBIENT, &ma[0]
                          , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                          , &glContext()->m_lightingAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                          , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_FRONT][0]
                          , &glContext()->m_previousAttributes.m_materialAmbient[NVSG_CACHE_BACK][0]
                          , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

//! Cached version of GLMaterialfv( face, GL_AMBIENT, params )
/** This function caches the GL_AMBIENT value of the material.
  * It is faster to call this method instead of GLMaterialfv(face,GL_AMBIENT,params) directly. */
inline  void  GLMaterialAmbient( GLenum face, const GLfloat *params )
{
#if defined( GL_CACHING )
  switch( face )
  {
    case GL_BACK :
      GLMaterialAmbientBack( params );
      break;
    case GL_FRONT :
      GLMaterialAmbientFront( params );
      break;
    case GL_FRONT_AND_BACK :
      GLMaterialAmbientFrontAndBack( params );
      break;
    default :
      NVSG_ASSERT( false );
      break;
  }
#else
  GLMaterialfv( face, GL_AMBIENT, params );
#endif
}

//! Cached version of GLMaterialf( GL_BACK, GL_DIFFUSE, param )
/** This function caches the GL_DIFFUSE value on the GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_BACK, GL_DIFFUSE, param ) directly. */
inline  void  GLMaterialDiffuseBack( const GLfloat *params )
{
  GL_MATERIAL_FV_CM( GL_BACK, GL_DIFFUSE, params
                   , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                   , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                   , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the material back face diffuse color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back face diffuse color to it's previous state. Otherwise, it is set to (0.8,0.8,0.8,1.0).
 *  \sa GLMaterialDiffuseBack */
inline void GLResetMaterialDiffuseBack()
{
  static nvmath::Vec4f md(0.8f,0.8f,0.8f,1.0f);
  GL_RESET_MATERIAL_FV_CM( GL_BACK, GL_DIFFUSE, &md[0]
                          , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                          , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                          , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

//! Cached version of GLMaterialf( GL_FRONT, GL_DIFFUSE, param )
/** This function caches the GL_DIFFUSE value on the GL_FRONT face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT, GL_DIFFUSE, param ) directly. */
inline  void  GLMaterialDiffuseFront( const GLfloat *params )
{
  GL_MATERIAL_FV_CM( GL_FRONT, GL_DIFFUSE, params
                   , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                   , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                   , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the material front face diffuse color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  front face diffuse color to it's previous state. Otherwise, it is set to (0.8,0.8,0.8,1.0).
 *  \sa GLMaterialDiffuseFront */
inline void GLResetMaterialDiffuseFront()
{
  static nvmath::Vec4f md(0.8f,0.8f,0.8f,1.0f);
  GL_RESET_MATERIAL_FV_CM( GL_FRONT, GL_DIFFUSE, &md[0]
                         , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                         , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                         , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

//! Cached version of GLMaterialf( GL_FRONT_AND_BACK, GL_DIFFUSE, param )
/** This function caches the GL_DIFFUSE value on both the GL_FRONT and GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT_AND_BACK, GL_DIFFUSE, param ) directly. */
inline  void  GLMaterialDiffuseFrontAndBack( const GLfloat *params )
{
  GL_MATERIAL_FV2_CM( GL_DIFFUSE, params
                    , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                    , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                    , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                    , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                    , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

/*! \brief Helper function to reset the material back and front face diffuse color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back and front face diffuse color to it's previous state. Otherwise, it is set to
 *  (0.8,0.8,0.8,1.0).
 *  \sa GLMaterialDiffuseFrontAndBack */
inline void GLResetMaterialDiffuseFrontAndBack()
{
  static nvmath::Vec4f md(0.8f,0.8f,0.8f,1.0f);
  GL_RESET_MATERIAL_FV2_CM( GL_DIFFUSE, &md[0]
                          , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                          , &glContext()->m_lightingAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                          , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_FRONT][0]
                          , &glContext()->m_previousAttributes.m_materialDiffuse[NVSG_CACHE_BACK][0]
                          , !!glContext()->m_lightingAttributes.m_colorMaterial );
}

//! Cached version of GLMaterialfv( face, GL_DIFFUSE, params )
/** This function caches the GL_DIFFUSE value of the material.
  * It is faster to call this method instead of GLMaterialfv(face,GL_DIFFUSE,params) directly. */
inline  void  GLMaterialDiffuse( GLenum face, const GLfloat *params )
{
#if defined( GL_CACHING )
  switch( face )
  {
    case GL_BACK :
      GLMaterialDiffuseBack( params );
      break;
    case GL_FRONT :
      GLMaterialDiffuseFront( params );
      break;
    case GL_FRONT_AND_BACK :
      GLMaterialDiffuseFrontAndBack( params );
      break;
    default :
      NVSG_ASSERT( false );
      break;
  }
#else
  GLMaterialfv( face, GL_DIFFUSE, params );
#endif
}

//! Cached version of GLMaterialf( GL_BACK, GL_EMISSION, param )
/** This function caches the GL_EMISSION value on the GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_BACK, GL_EMISSION, param ) directly. */
inline  void  GLMaterialEmissionBack( const GLfloat *params )
{
  GL_MATERIAL_FV( GL_BACK, GL_EMISSION, params
                , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_BACK][0]
                , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_BACK][0] );
}

/*! \brief Helper function to reset the material back face emission color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back face emission color to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLMaterialEmissionBack */
inline void GLResetMaterialEmissionBack()
{
  static nvmath::Vec4f me(0.0f,0.0f,0.0f,1.0f);
  GL_RESET_MATERIAL_FV( GL_BACK, GL_EMISSION, &me[0]
                      , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_BACK][0]
                      , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_BACK][0] );
}

//! Cached version of GLMaterialf( GL_FRONT, GL_EMISSION, param )
/** This function caches the GL_EMISSION value on the GL_FRONT face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT, GL_EMISSION, param ) directly. */
inline  void  GLMaterialEmissionFront( const GLfloat *params )
{
  GL_MATERIAL_FV( GL_FRONT, GL_EMISSION, params
                , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_FRONT][0]
                , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_FRONT][0] );
}

/*! \brief Helper function to reset the material front face emission color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  front face emission color to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLMaterialEmissionFront */
inline void GLResetMaterialEmissionFront()
{
  static nvmath::Vec4f me(0.0f,0.0f,0.0f,1.0f);
  GL_RESET_MATERIAL_FV( GL_FRONT, GL_EMISSION, &me[0]
                      , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_FRONT][0]
                      , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_FRONT][0] );
}

//! Cached version of GLMaterialf( GL_FRONT_AND_BACK, GL_EMISSION, param )
/** This function caches the GL_EMISSION value on both the GL_FRONT and GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT_AND_BACK, GL_EMISSION, param ) directly. */
inline  void  GLMaterialEmissionFrontAndBack( const GLfloat *params )
{
  GL_MATERIAL_FV2( GL_EMISSION, params
                 , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_FRONT][0]
                 , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_BACK][0]
                 , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_FRONT][0]
                 , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_BACK][0] );
}

/*! \brief Helper function to reset the material back and front face emission color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back and front face emission color to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLMaterialEmissionFrontAndBack */
inline void GLResetMaterialEmissionFrontAndBack()
{
  static nvmath::Vec4f me(0.0f,0.0f,0.0f,1.0f);
  GL_RESET_MATERIAL_FV2( GL_EMISSION, &me[0]
                       , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_FRONT][0]
                       , &glContext()->m_lightingAttributes.m_materialEmission[NVSG_CACHE_BACK][0]
                       , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_FRONT][0]
                       , &glContext()->m_previousAttributes.m_materialEmission[NVSG_CACHE_BACK][0] );
}

//! Cached version of GLMaterialfv( face, GL_EMISSION, params )
/** This function caches the GL_EMISSION value of the material.
  * It is faster to call this method instead of GLMaterialfv(face,GL_EMISSION,params) directly. */
inline  void  GLMaterialEmission( GLenum face, const GLfloat *params )
{
#if defined( GL_CACHING )
  switch( face )
  {
    case GL_BACK :
      GLMaterialEmissionBack( params );
      break;
    case GL_FRONT :
      GLMaterialEmissionFront( params );
      break;
    case GL_FRONT_AND_BACK :
      GLMaterialEmissionFrontAndBack( params );
      break;
    default :
      NVSG_ASSERT( false );
      break;
  }
#else
  GLMaterialfv( face, GL_EMISSION, params );
#endif
}

//! Cached version of GLMaterialf( GL_BACK, GL_SPECULAR, param )
/** This function caches the GL_SPECULAR value on the GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_BACK, GL_SPECULAR, param ) directly. */
inline  void  GLMaterialSpecularBack( const GLfloat *params )
{
  GL_MATERIAL_FV( GL_BACK, GL_SPECULAR, params
                , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_BACK][0]
                , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_BACK][0] );
}

/*! \brief Helper function to reset the material back face specular color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back face specular color to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLMaterialSpecularBack */
inline void GLResetMaterialSpecularBack()
{
  static nvmath::Vec4f ms(0.0f,0.0f,0.0f,1.0f);
  GL_RESET_MATERIAL_FV( GL_BACK, GL_SPECULAR, &ms[0]
                      , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_BACK][0]
                      , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_BACK][0] );
}

//! Cached version of GLMaterialf( GL_FRONT, GL_SPECULAR, param )
/** This function caches the GL_SPECULAR value on the GL_FRONT face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT, GL_SPECULAR, param ) directly. */
inline  void  GLMaterialSpecularFront( const GLfloat *params )
{
  GL_MATERIAL_FV( GL_FRONT, GL_SPECULAR, params
                , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0]
                , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0] );
}

/*! \brief Helper function to reset the material front face specular color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  front face specular color to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLMaterialSpecularFront */
inline void GLResetMaterialSpecularFront()
{
  static nvmath::Vec4f ms(0.0f,0.0f,0.0f,1.0f);
  GL_RESET_MATERIAL_FV( GL_FRONT, GL_SPECULAR, &ms[0]
                      , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0]
                      , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0] );
}

//! Cached version of GLMaterialf( GL_FRONT_AND_BACK, GL_SPECULAR, param )
/** This function caches the GL_SPECULAR value on both the GL_FRONT and GL_BACK face of the material.
  * It is faster to call this method instead of GLMaterialf( GL_FRONT_AND_BACK, GL_SPECULAR, param ) directly. */
inline  void  GLMaterialSpecularFrontAndBack( const GLfloat *params )
{
  GL_MATERIAL_FV2( GL_SPECULAR, params
                 , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0]
                 , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_BACK][0]
                 , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0]
                 , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_BACK][0] );
}

/*! \brief Helper function to reset the material back and front face specular color.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the material
 *  back and front face specular color to it's previous state. Otherwise, it is set to (0,0,0,1).
 *  \sa GLMaterialSpecularFrontAndBack */
inline void GLResetMaterialSpecularFrontAndBack()
{
  static nvmath::Vec4f ms(0.0f,0.0f,0.0f,1.0f);
  GL_RESET_MATERIAL_FV2( GL_SPECULAR, &ms[0]
                       , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0]
                       , &glContext()->m_lightingAttributes.m_materialSpecular[NVSG_CACHE_BACK][0]
                       , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_FRONT][0]
                       , &glContext()->m_previousAttributes.m_materialSpecular[NVSG_CACHE_BACK][0] );
}

//! Cached version of GLMaterialfv( face, GL_SPECULAR, params )
/** This function caches the GL_SPECULAR value of the material.
  * It is faster to call this method instead of GLMaterialfv(face,GL_SPECULAR,params) directly. */
inline  void  GLMaterialSpecular( GLenum face, const GLfloat *params )
{
#if defined( GL_CACHING )
  switch( face )
  {
    case GL_BACK :
      GLMaterialSpecularBack( params );
      break;
    case GL_FRONT :
      GLMaterialSpecularFront( params );
      break;
    case GL_FRONT_AND_BACK :
      GLMaterialSpecularFrontAndBack( params );
      break;
    default :
      NVSG_ASSERT( false );
      break;
  }
#else
  GLMaterialfv( face, GL_SPECULAR, params );
#endif
}

//! Replacement of glMaterialf
/** This function caches on some face and pname values and asserts on the result before returning. */
inline  void  GLMaterialf( GLenum face, GLenum pname, GLfloat param )
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_SHININESS :
      GLMaterialShininess( face, param );
      break;
    default :
      _GLMaterialf( face, pname, param );
      break;
  }
#else
  _GLMaterialf( face, pname, param );
#endif
}

//! Replacement of glMaterialfv
/** This function caches on some face and pname values and asserts on the result before returning. */
inline  void  GLMaterialfv( GLenum face, GLenum pname, const GLfloat *params )  
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_AMBIENT :
      GLMaterialAmbient( face, params );
      break;
    case GL_DIFFUSE :
      GLMaterialDiffuse( face, params );
      break;
    case GL_EMISSION :
      GLMaterialEmission( face, params );
      break;
    case GL_SPECULAR :
      GLMaterialSpecular( face, params );
      break;
    case GL_SHININESS :
      GLMaterialShininess( face, params[0] );
      break;
    default :
      _GLMaterialfv( face, pname, params );
      break;
  }
#else
  _GLMaterialfv( face, pname, params );
#endif
}

//! Replacement of glMateriali
/** This function caches on some face and pname values and asserts on the result before returning. */
inline  void  GLMateriali( GLenum face, GLenum pname, GLint param ) 
{ 
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_SHININESS :
      GLMaterialShininess( face, (GLfloat)param );
      break;
    default :
      _GLMateriali( face, pname, param );
      break;
  }
#else
  _GLMateriali( face, pname, param );
#endif
}

//! Replacement of glMaterialiv
/** This function caches on some face and pname values and asserts on the result before returning. */
inline  void  GLMaterialiv( GLenum face, GLenum pname, const GLint *params )  
{
#if defined( GL_CACHING )
  GLfloat fParams[4];
  switch( pname )
  {
    case GL_AMBIENT :
      GLintToGLfloat( params, fParams );
      GLMaterialAmbient( face, fParams );
      break;
    case GL_DIFFUSE :
      GLintToGLfloat( params, fParams );
      GLMaterialDiffuse( face, fParams );
      break;
    case GL_EMISSION :
      GLintToGLfloat( params, fParams );
      GLMaterialEmission( face, fParams );
      break;
    case GL_SPECULAR :
      GLintToGLfloat( params, fParams );
      GLMaterialSpecular( face, fParams );
      break;
    case GL_SHININESS :
      GLMaterialShininess( face, (GLfloat)params[0] );
      break;
    default :
      _GLMaterialiv( face, pname, params );
      break;
  }
#else
  _GLMaterialiv( face, pname, params );
#endif
}

//! Replacement of glMatrixMode
/** This function caches the matrix mode setting and asserts on the result before returning.  */
inline  void  GLMatrixMode( GLenum mode )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  if ( ctx->m_transformAttributes.m_matrixMode != mode )
#endif
  {
    NVSG_GL_STATS_MATRIX_MODE( mode );
    glMatrixMode( mode );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_transformAttributes.m_matrixMode = mode;
#endif
  }
}

//! Replacement of glMultiTexCoord2f
/** This function hides the extension character of glMultiTexCoord2f and asserts on the result before returning.  */
inline  void  GLMultiTexCoord2f( GLenum texture, GLfloat tx, GLfloat ty )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glMultiTexCoord2f );
  glContext()->m_glFunctions->glMultiTexCoord2f( texture, tx, ty );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glMultMatrixd
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLMultMatrixd( const GLdouble *m )
{
  NVSG_GL_STATS_MULT_MATRIXD( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glMultMatrixd( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMultMatrixf
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLMultMatrixf( const GLfloat *m )
{
  NVSG_GL_STATS_MULT_MATRIXF( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glMultMatrixf( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMultTransposeMatrixd
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLMultTransposeMatrixd( const GLdouble *m )
{
  NVSG_GL_STATS_MULT_MATRIXD( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glMultTransposeMatrixd );
  glContext()->m_glFunctions->glMultTransposeMatrixd( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glMultTransposeMatrixf
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLMultTransposeMatrixf( const GLfloat *m )
{
  NVSG_GL_STATS_MULT_MATRIXF( m );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glMultTransposeMatrixf );
  glContext()->m_glFunctions->glMultTransposeMatrixf( m );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glNewList
/** This function asserts on the result before returning.  */
inline  void  GLNewList( GLuint list, GLenum mode )
{
  NVSG_GL_STATS_NEW_LIST( list, mode );
  glNewList( list, mode );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glNormal3b
/** This function asserts on the result before returning.  */
inline  void  GLNormal3b( GLbyte nx, GLbyte ny, GLbyte nz )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3b( nx, ny, nz );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3bv
/** This function asserts on the result before returning.  */
inline  void  GLNormal3bv( const GLbyte *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3bv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3d
/** This function asserts on the result before returning.  */
inline  void  GLNormal3d( GLdouble nx, GLdouble ny, GLdouble nz )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3d( nx, ny, nz );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3dv
/** This function asserts on the result before returning.  */
inline  void  GLNormal3dv( const GLdouble *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3dv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3f
/** This function asserts on the result before returning.  */
inline  void  GLNormal3f( GLfloat nx, GLfloat ny, GLfloat nz )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3f( nx, ny, nz );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3fv
/** This function asserts on the result before returning.  */
inline  void  GLNormal3fv( const GLfloat *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3fv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3i
/** This function asserts on the result before returning.  */
inline  void  GLNormal3i( GLint nx, GLint ny, GLint nz )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3i( nx, ny, nz );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3iv
/** This function asserts on the result before returning.  */
inline  void  GLNormal3iv( const GLint *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3iv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3s
/** This function asserts on the result before returning.  */
inline  void  GLNormal3s( GLshort nx, GLshort ny, GLshort nz )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3s( nx, ny, nz );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormal3sv
/** This function asserts on the result before returning.  */
inline  void  GLNormal3sv( const GLshort *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glNormal3sv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glNormalPointer
/** This function caches the normal pointer and asserts on the result before returning.  */
inline  void  GLNormalPointer( GLenum type, GLsizei stride, const GLvoid *pointer )
{
#if defined(GL_CACHING)
  GLContext::VertexArrayParameters parameters( 3, type, stride, pointer );
  GLContext * ctx = glContext();
  if ( ctx->m_vertexArrayAttributes.m_normalArrayParams != parameters )
#endif
  {
    NVSG_GL_STATS_NORMAL_POINTER( type, stride, pointer );
    glNormalPointer( type, stride, pointer );    
    ASSERT_GL_NO_ERROR;
#if defined(GL_CACHING)
    ctx->m_vertexArrayAttributes.m_normalArrayParams = parameters;
#endif
  }
}

//! Replacement of glOrtho
/** This function asserts on the result before returning.  */
inline  void  GLOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
  NVSG_GL_STATS_ORTHO( left, right, bottom, top, zNear, zFar );
  glOrtho( left, right, bottom, top, zNear, zFar );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPassThrough
/** This function asserts on the result before returning.  */
inline  void  GLPassThrough( GLfloat token )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPassThrough( token );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelMapfv
/** This function asserts on the result before returning.  */
inline  void  GLPixelMapfv( GLenum map, GLsizei mapsize, const GLfloat *values )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelMapfv( map, mapsize, values );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelMapuiv
/** This function asserts on the result before returning.  */
inline  void  GLPixelMapuiv( GLenum map, GLsizei mapsize, const GLuint *values )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelMapuiv( map, mapsize, values );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelMapusv
/** This function asserts on the result before returning.  */
inline  void  GLPixelMapusv( GLenum map, GLsizei mapsize, const GLushort *values )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelMapusv( map, mapsize, values );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelStoref
/** This function asserts on the result before returning.  */
inline  void  GLPixelStoref( GLenum pname, GLfloat param )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelStoref( pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelStorei
/** This function asserts on the result before returning.  */
inline  void  GLPixelStorei( GLenum pname, GLint param )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelStorei( pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelTransferf
/** This function asserts on the result before returning.  */
inline  void  GLPixelTransferf( GLenum pname, GLfloat param )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelTransferf( pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelTransferi
/** This function asserts on the result before returning.  */
inline  void  GLPixelTransferi( GLenum pname, GLint param )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelTransferi( pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPixelZoom
/** This function asserts on the result before returning.  */
inline  void  GLPixelZoom( GLfloat xfactor, GLfloat yfactor )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPixelZoom( xfactor, yfactor );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPointSize
/** This function caches the point size setting and asserts on the result before returning.  */
inline  void  GLPointSize( GLfloat size )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_pointSize = ctx->m_pointAttributes.m_size;
# endif
  if ( !equalf(ctx->m_pointAttributes.m_size, size) )
  {
# if !defined(NDEBUG)
    GLfloat f;
    glGetFloatv( GL_POINT_SIZE, &f );
    NVSG_ASSERT( equalf(ctx->m_pointAttributes.m_size, f) );
# endif
#endif
    NVSG_GL_STATS_POINT_SIZE( size );
    glPointSize( size );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_pointAttributes.m_size = size;
  }
#endif
}

/*! \brief Helper function to reset the point size.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the point size
 *  to it's previous state. Otherwise, it is set to 1.0f.
 *  \sa GLPointSize */
inline void GLResetPointSize()
{
#if defined(GL_SUPPORT_RESET)
  GLPointSize( glContext()->m_previousAttributes.m_pointSize );
#else
  GLPointSize( 1.0f );
#endif
}

//! Cached version of GLPolygonMode( GL_BACK, mode )
/** This function caches the polygon mode on the GL_BACK face of polygon.
  * It is faster to call this method instead of GLPolygonMode( GL_BACK, mode ) directly. */
inline  void  GLPolygonModeBack( GLenum mode )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_BACK] = ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK];
# endif
  if ( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK] != mode )
#endif
  {
    if ( ! ctx->m_locks.m_polygonMode )
    {
#if defined( GL_CACHING) && !defined( NDEBUG )
      GLint p[2];
      glGetIntegerv( GL_POLYGON_MODE, p );
      NVSG_ASSERT( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK] == p[1] );
#endif
      _GLPolygonMode( GL_BACK, mode );
#if defined( GL_CACHING )
      ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK] = mode;
#endif
    }
  }
}

/*! \brief Helper function to reset back face polygon mode.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the back face
 *  polygon mode to it's previous state. Otherwise, it is set to GL_FILL.
 *  \sa GLPolygonModeBack */
inline void GLResetPolygonModeBack()
{
#if defined(GL_SUPPORT_RESET)
  GLPolygonModeBack( glContext()->m_previousAttributes.m_rasterMode[NVSG_CACHE_BACK] );
#else
  GLPolygonModeBack( GL_FILL );
#endif
}

//! Cached version of GLPolygonMode( GL_FRONT, mode )
/** This function caches the polygon mode on the GL_FRONT face of polygon.
  * It is faster to call this method instead of GLPolygonMode( GL_FRONT, mode ) directly. */
inline  void  GLPolygonModeFront( GLenum mode )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_FRONT] = ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT];
# endif
  if ( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT] != mode )
#endif
  {
    if ( ! ctx->m_locks.m_polygonMode )
    {
#if defined( GL_CACHING ) && !defined( NDEBUG )
      GLint p[2];
      glGetIntegerv( GL_POLYGON_MODE, p );
      NVSG_ASSERT( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT] == p[0] );
#endif
      _GLPolygonMode( GL_FRONT, mode );
#if defined( GL_CACHING )
      ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT] = mode;
#endif
    }
  }
}

/*! \brief Helper function to reset front face polygon mode.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the front face
 *  polygon mode to it's previous state. Otherwise, it is set to GL_FILL.
 *  \sa GLPolygonModeFront */
inline void GLResetPolygonModeFront()
{
#if defined(GL_SUPPORT_RESET)
  GLPolygonModeFront( glContext()->m_previousAttributes.m_rasterMode[NVSG_CACHE_FRONT] );
#else
  GLPolygonModeFront( GL_FILL );
#endif
}

//! Cached version of GLPolygonMode( GL_FRONT_AND_BACK, mode )
/** This function caches the polygon mode on both the GL_FRONT and GL_BACK face of polygon.
  * It is faster to call this method instead of GLPolygonMode( GL_FRONT_AND_BACK, mode ) directly. */
inline  void  GLPolygonModeFrontAndBack( GLenum mode )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_FRONT] = ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT];
  ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_BACK] = ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK];
# endif
  if (    ( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT] != mode )
      ||  ( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK] != mode ) )
#endif
  {
    if ( ! ctx->m_locks.m_polygonMode )
    {
#if defined( GL_CACHING ) && !defined( NDEBUG )
      GLint p[2];
      glGetIntegerv( GL_POLYGON_MODE, p );
      NVSG_ASSERT(    ( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK] == p[NVSG_CACHE_BACK] )
                  &&  ( ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT] == p[NVSG_CACHE_FRONT] ) );
#endif
      _GLPolygonMode( GL_FRONT_AND_BACK, mode );
#if defined( GL_CACHING )
      ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_BACK] = mode;
      ctx->m_polygonAttributes.m_rasterMode[NVSG_CACHE_FRONT] = mode;
#endif
    }
  }
}

/*! \brief Helper function to reset back and front face polygon mode.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the back and
 *  front face polygon mode to it's previous state. Otherwise, it is set to GL_FILL.
 *  \sa GLPolygonModeFrontAndBack, GLPolygonModeFront, GLPolygonMode Back */
inline void GLResetPolygonModeFrontAndBack()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  if ( ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_FRONT] == ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_BACK] )
  {
    GLPolygonModeFrontAndBack( ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_FRONT] );
  }
  else
  {
    GLPolygonModeFront( ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_FRONT] );
    GLPolygonModeBack( ctx->m_previousAttributes.m_rasterMode[NVSG_CACHE_BACK] );
  }
#else
  GLPolygonModeFrontAndBack( GL_FILL );
#endif
}

//! Cached version of GLPolygonMode( face, mode )
/** This function caches the polygon mode and assert on the result. */
inline  void  GLPolygonMode( GLenum face, GLenum mode )
{
#if defined( GL_CACHING )
  switch( face )
  {
    case GL_BACK :
      GLPolygonModeBack( mode );
      break;
    case GL_FRONT :
      GLPolygonModeFront( mode );
      break;
    case GL_FRONT_AND_BACK :
      GLPolygonModeFrontAndBack( mode );
      break;
    default :
      NVSG_ASSERT( false );
      break;
  }
#else
  _GLPolygonMode( face, mode );
#endif
}

/*! \brief Helper function to set the polygon mode and lock it.
 *  \param face The face for which the polygon mode is to set and lock.
 *  \param mode The polygon mode to set and lock.
 *  \remarks This function sets the polygon mode for \a face to \a mode and locks it. Any subsequent
 *  calls to GLPolygonMode, GLPolygonModeBack, or GLPolygonModeFront on the same \a face will not
 *  change that setting, until the next call to GLPolygonModeUnlock.
 *  \sa GLPolygonMode, GLPolygonModeUnlock */
inline void GLPolygonModeLock( GLenum face, GLenum mode )
{
  NVSG_ASSERT( ! glContext()->m_locks.m_polygonMode );
  GLPolygonMode( face, mode );
  glContext()->m_locks.m_polygonMode = true;
}

/*! \brief Helper function to unlock the polygon mode and set it.
 *  \param face The face for which the polygon mode is to be unlocked and set.
 *  \param mode The polygon mode to set.
 *  \remarks This function unlocks the polygon mode for \a face and sets it to \a mode.
 *  \sa GLPolygonMode, GLPolygonModeLock */
inline void GLPolygonModeUnlock( GLenum face, GLenum mode )
{
  glContext()->m_locks.m_polygonMode = false;
  GLPolygonMode( face, mode );
}

//! Replacement of glPolygonOffset
/** This function caches the polygon offset settings and asserts on the result before returning.  */
inline  void  GLPolygonOffset( GLfloat factor, GLfloat units )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_polygonOffsetFactor = ctx->m_polygonAttributes.m_offsetFactor;
  ctx->m_previousAttributes.m_polygonOffsetBias   = ctx->m_polygonAttributes.m_offsetBias;
# endif
  if (  !equalf(ctx->m_polygonAttributes.m_offsetFactor, factor) 
     || !equalf(ctx->m_polygonAttributes.m_offsetBias, units) )
#endif
  {
    NVSG_GL_STATS_POLYGON_OFFSET( factor, units );
    glPolygonOffset( factor, units );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_polygonAttributes.m_offsetFactor  = factor;
    ctx->m_polygonAttributes.m_offsetBias   = units;
#endif
  }
}

/*! \brief Helper function to reset the polygon offset.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the polygon
 *  offset to it's previous state. Otherwise, it is set to (0,0).
 *  \sa GLPolygonOffset */
inline void GLResetPolygonOffset()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLPolygonOffset( ctx->m_previousAttributes.m_polygonOffsetFactor
                 , ctx->m_previousAttributes.m_polygonOffsetBias );
#else
  GLPolygonOffset( 0.0f, 0.0f );
#endif
}

//! Replacement of glPolygonStipple
/** This function asserts on the result before returning.  */
inline  void  GLPolygonStipple( const GLubyte *mask )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPolygonStipple( mask );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPopAttrib
/** This function asserts on the result before returning.
  * \note Currently we don't hold a stack of cached GL states, so GLPushAttrib/GLPopAttrib doesn't affect our cached values.
  * It is therefore recommended to used GLPushAttrib/GLPopAttrib very local and to not use other potentially caching
  * GL* functions (with uppercase GL), inbetween such a pair. */
inline  void  GLPopAttrib( void )
{
  NVSG_GL_STATS_POP_ATTRIB();
  glPopAttrib();
  ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  GLbitfield mask = (GLbitfield)ctx->m_pushMasks.top();
  if ( mask == GL_ALL_ATTRIB_BITS )
  {
    ctx->m_colorBufferAttributes = ctx->m_colorBufferAttributesStack.top();
    ctx->m_colorBufferAttributesStack.pop();
    ctx->m_colorBufferEnableAttributes = ctx->m_colorBufferEnableAttributesStack.top();
    ctx->m_colorBufferEnableAttributesStack.pop();
    ctx->m_depthBufferAttributes = ctx->m_depthBufferAttributesStack.top();
    ctx->m_depthBufferAttributesStack.pop();
    ctx->m_depthBufferEnableAttributes = ctx->m_depthBufferEnableAttributesStack.top();
    ctx->m_depthBufferEnableAttributesStack.pop();
    ctx->m_evalAttributes = ctx->m_evalAttributesStack.top();
    ctx->m_evalAttributesStack.pop();
    ctx->m_fogAttributes = ctx->m_fogAttributesStack.top();
    ctx->m_fogAttributesStack.pop();
    ctx->m_fogEnableAttributes = ctx->m_fogEnableAttributesStack.top();
    ctx->m_fogEnableAttributesStack.pop();
    ctx->m_lightingAttributes = ctx->m_lightingAttributesStack.top();
    ctx->m_lightingAttributesStack.pop();
    ctx->m_lightingEnableAttributes = ctx->m_lightingEnableAttributesStack.top();
    ctx->m_lightingEnableAttributesStack.pop();
    ctx->m_lineAttributes = ctx->m_lineAttributesStack.top();
    ctx->m_lineAttributesStack.pop();
    ctx->m_lineEnableAttributes = ctx->m_lineEnableAttributesStack.top();
    ctx->m_lineEnableAttributesStack.pop();
    ctx->m_pointAttributes = ctx->m_pointAttributesStack.top();
    ctx->m_pointAttributesStack.pop();
    ctx->m_pointEnableAttributes = ctx->m_pointEnableAttributesStack.top();
    ctx->m_pointEnableAttributesStack.pop();
    ctx->m_polygonAttributes = ctx->m_polygonAttributesStack.top();
    ctx->m_polygonAttributesStack.pop();
    ctx->m_polygonEnableAttributes = ctx->m_polygonEnableAttributesStack.top();
    ctx->m_polygonEnableAttributesStack.pop();
    ctx->m_scissorAttributes = ctx->m_scissorAttributesStack.top();
    ctx->m_scissorAttributesStack.pop();
    ctx->m_scissorEnableAttributes = ctx->m_scissorEnableAttributesStack.top();
    ctx->m_scissorEnableAttributesStack.pop();
    ctx->m_stencilBufferAttributes = ctx->m_stencilBufferAttributesStack.top();
    ctx->m_stencilBufferAttributesStack.pop();
    ctx->m_stencilBufferEnableAttributes = ctx->m_stencilBufferEnableAttributesStack.top();
    ctx->m_stencilBufferEnableAttributesStack.pop();
    ctx->m_textureAttributes = ctx->m_textureAttributesStack.top();
    ctx->m_textureAttributesStack.pop();
    ctx->m_textureEnableAttributes = ctx->m_textureEnableAttributesStack.top();
    ctx->m_textureEnableAttributesStack.pop();
    ctx->m_transformAttributes = ctx->m_transformAttributesStack.top();
    ctx->m_transformAttributesStack.pop();
    ctx->m_transformEnableAttributes = ctx->m_transformEnableAttributesStack.top();
    ctx->m_transformEnableAttributesStack.pop();
    ctx->m_viewportAttributes = ctx->m_viewportAttributesStack.top();
    ctx->m_viewportAttributesStack.pop();
  }
  else
  {
    if ( mask & GL_COLOR_BUFFER_BIT )
    {
      ctx->m_colorBufferAttributes = ctx->m_colorBufferAttributesStack.top();
      ctx->m_colorBufferAttributesStack.pop();
    }
    if ( mask & GL_DEPTH_BUFFER_BIT )
    {
      ctx->m_depthBufferAttributes = ctx->m_depthBufferAttributesStack.top();
      ctx->m_depthBufferAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT ) )
    {
      ctx->m_colorBufferEnableAttributes = ctx->m_colorBufferEnableAttributesStack.top();
      ctx->m_colorBufferEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT ) )
    {
      ctx->m_depthBufferEnableAttributes = ctx->m_depthBufferEnableAttributesStack.top();
      ctx->m_depthBufferEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_FOG_BIT ) )
    {
      ctx->m_fogEnableAttributes = ctx->m_fogEnableAttributesStack.top();
      ctx->m_fogEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_LIGHTING_BIT ) )
    {
      ctx->m_lightingEnableAttributes = ctx->m_lightingEnableAttributesStack.top();
      ctx->m_lightingEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_LINE_BIT ) )
    {
      ctx->m_lineEnableAttributes = ctx->m_lineEnableAttributesStack.top();
      ctx->m_lineEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_POINT_BIT ) )
    {
      ctx->m_pointEnableAttributes = ctx->m_pointEnableAttributesStack.top();
      ctx->m_pointEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_POLYGON_BIT ) )
    {
      ctx->m_polygonEnableAttributes = ctx->m_polygonEnableAttributesStack.top();
      ctx->m_polygonEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_SCISSOR_BIT ) )
    {
      ctx->m_scissorEnableAttributes = ctx->m_scissorEnableAttributesStack.top();
      ctx->m_scissorEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_STENCIL_BUFFER_BIT ) )
    {
      ctx->m_stencilBufferEnableAttributes = ctx->m_stencilBufferEnableAttributesStack.top();
      ctx->m_stencilBufferEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_TEXTURE_BIT ) )
    {
      ctx->m_textureEnableAttributes = ctx->m_textureEnableAttributesStack.top();
      ctx->m_textureEnableAttributesStack.pop();
    }
    if ( mask & ( GL_ENABLE_BIT | GL_TRANSFORM_BIT ) )
    {
      ctx->m_transformEnableAttributes = ctx->m_transformEnableAttributesStack.top();
      ctx->m_transformEnableAttributesStack.pop();
    }
    if ( mask & GL_EVAL_BIT )
    {
      ctx->m_evalAttributes = ctx->m_evalAttributesStack.top();
      ctx->m_evalAttributesStack.pop();
    }
    if ( mask & GL_FOG_BIT )
    {
      ctx->m_fogAttributes = ctx->m_fogAttributesStack.top();
      ctx->m_fogAttributesStack.pop();
    }
    if ( mask & GL_LIGHTING_BIT )
    {
      ctx->m_lightingAttributes = ctx->m_lightingAttributesStack.top();
      ctx->m_lightingAttributesStack.pop();
    }
    if ( mask & GL_LINE_BIT )
    {
      ctx->m_lineAttributes = ctx->m_lineAttributesStack.top();
      ctx->m_lineAttributesStack.pop();
    }
    if ( mask & GL_POINT_BIT )
    {
      ctx->m_pointAttributes = ctx->m_pointAttributesStack.top();
      ctx->m_pointAttributesStack.pop();
    }
    if ( mask & GL_POLYGON_BIT )
    {
      ctx->m_polygonAttributes = ctx->m_polygonAttributesStack.top();
      ctx->m_polygonAttributesStack.pop();
    }
    if ( mask & GL_SCISSOR_BIT )
    {
      ctx->m_scissorAttributes = ctx->m_scissorAttributesStack.top();
      ctx->m_scissorAttributesStack.pop();
    }
    if ( mask & GL_STENCIL_BUFFER_BIT )
    {
      ctx->m_stencilBufferAttributes = ctx->m_stencilBufferAttributesStack.top();
      ctx->m_stencilBufferAttributesStack.pop();
    }
    if ( mask & GL_TEXTURE_BIT )
    {
      ctx->m_textureAttributes = ctx->m_textureAttributesStack.top();
      ctx->m_textureAttributesStack.pop();
    }
    if ( mask & GL_TRANSFORM_BIT )
    {
      ctx->m_transformAttributes = ctx->m_transformAttributesStack.top();
      ctx->m_transformAttributesStack.pop();
    }
    if ( mask & GL_VIEWPORT_BIT )
    {
      ctx->m_viewportAttributes = ctx->m_viewportAttributesStack.top();
      ctx->m_viewportAttributesStack.pop();
    }
  }
  ctx->m_pushMasks.pop();
#endif
}

//! Replacement of glPopClientAttrib
/** This function asserts on the result before returning.
  * \note Currently we don't hold a stack of cached GL states, so GLPushClientAttrib/GLPopClientAttrib doesn't affect our cached
  * values. It is therefore recommended to used GLPushAttrib/GLPopAttrib very local and to not use other potentially caching
  * GL* functions (with uppercase GL), inbetween such a pair. */
inline  void  GLPopClientAttrib( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPopClientAttrib();
  ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  GLbitfield mask = (GLbitfield)ctx->m_clientPushMasks.top();
  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT )
  {
    // note: vertex buffer binding states will be pushed/popped along with the vertex array client state
    GLContext::VertexBufferBinding const& binding = ctx->m_vertexBufferBindingAttributesStack.top();
    ctx->m_bufferBindingAttributes.setVertexBufferBinding(binding);
    ctx->m_vertexBufferBindingAttributesStack.pop();

    ctx->m_vertexArrayAttributes = ctx->m_vertexArrayAttributesStack.top();
    ctx->m_vertexArrayAttributesStack.pop();
  }

  if ( mask & GL_CLIENT_PIXEL_STORE_BIT ) 
  {
    // note: pixel buffer binding states will be pushed/popped along with the pixel store client state
    GLContext::PixelBufferBinding const& binding = ctx->m_pixelBufferBindingAttributesStack.top();
    ctx->m_bufferBindingAttributes.setPixelBufferBinding(binding);
    ctx->m_pixelBufferBindingAttributesStack.pop();
  }

  ctx->m_clientPushMasks.pop();
#endif
}

//! Replacement of glPopMatrix
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLPopMatrix( void )
{
  NVSG_GL_STATS_POP_MATRIX();
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glPopMatrix();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPopName
/** This function asserts on the result before returning.  */
inline  void  GLPopName( void )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPopName();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPrimitiveRestartIndex
/** This function caches the primitive restart index and asserts on the result before returning.  */
inline  void  GLPrimitiveRestartIndex( GLuint index )
{
  GLContext * ctx = glContext();
#if defined( GL_CACHING )
  if ( ctx->m_vertexArrayAttributes.m_primitiveRestartIndex != index )
#endif
  {
    NVSG_GL_STATS_PRIMITIVE_RESTART_INDEX( index );
    ctx->m_glFunctions->glPrimitiveRestartIndexNV( index );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_vertexArrayAttributes.m_primitiveRestartIndex = index;
#endif
  }
}

//! Replacement of glPrioritizeTextures
/** This function asserts on the result before returning.  */
inline  void  GLPrioritizeTextures( GLsizei n, const GLuint *textures, const GLclampf *priorities )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPrioritizeTextures( n, textures, priorities );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glProgramNamedParameter4fNV
/** This function hides the extension character of glProgramNamedParameter4fNV and asserts on the result before returning.  */
inline  void  GLProgramNamedParameter4f( GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glProgramNamedParameter4fNV );
  glContext()->m_glFunctions->glProgramNamedParameter4fNV( id, len, name, x, y, z, w );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPushAttrib
/** This function asserts on the result before returning.
  * \note Currently we don't hold a stack of cached GL states, so GLPushAttrib/GLPopAttrib doesn't affect our cached values.
  * It is therefore recommended to used GLPushAttrib/GLPopAttrib very local and to not use other potentially caching
  * GL* functions (with uppercase GL), inbetween such a pair. */
inline  void  GLPushAttrib( GLbitfield mask )
{
  NVSG_GL_STATS_PUSH_ATTRIB( mask );
  glPushAttrib( mask );
  ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  ctx->m_pushMasks.push( mask );
  if ( mask == GL_ALL_ATTRIB_BITS )
  {
    ctx->m_colorBufferAttributesStack.push( ctx->m_colorBufferAttributes );
    ctx->m_colorBufferEnableAttributesStack.push( ctx->m_colorBufferEnableAttributes );
    ctx->m_depthBufferAttributesStack.push( ctx->m_depthBufferAttributes );
    ctx->m_depthBufferEnableAttributesStack.push( ctx->m_depthBufferEnableAttributes );
    ctx->m_evalAttributesStack.push( ctx->m_evalAttributes );
    ctx->m_fogAttributesStack.push( ctx->m_fogAttributes );
    ctx->m_fogEnableAttributesStack.push( ctx->m_fogEnableAttributes );
    ctx->m_lightingAttributesStack.push( ctx->m_lightingAttributes );
    ctx->m_lightingEnableAttributesStack.push( ctx->m_lightingEnableAttributes );
    ctx->m_lineAttributesStack.push( ctx->m_lineAttributes );
    ctx->m_lineEnableAttributesStack.push( ctx->m_lineEnableAttributes );
    ctx->m_pointAttributesStack.push( ctx->m_pointAttributes );
    ctx->m_pointEnableAttributesStack.push( ctx->m_pointEnableAttributes );
    ctx->m_polygonAttributesStack.push( ctx->m_polygonAttributes );
    ctx->m_polygonEnableAttributesStack.push( ctx->m_polygonEnableAttributes );
    ctx->m_scissorAttributesStack.push( ctx->m_scissorAttributes );
    ctx->m_scissorEnableAttributesStack.push( ctx->m_scissorEnableAttributes );
    ctx->m_stencilBufferAttributesStack.push( ctx->m_stencilBufferAttributes );
    ctx->m_stencilBufferEnableAttributesStack.push( ctx->m_stencilBufferEnableAttributes );
    ctx->m_textureAttributesStack.push( ctx->m_textureAttributes );
    ctx->m_textureEnableAttributesStack.push( ctx->m_textureEnableAttributes );
    ctx->m_transformAttributesStack.push( ctx->m_transformAttributes );
    ctx->m_transformEnableAttributesStack.push( ctx->m_transformEnableAttributes );
    ctx->m_viewportAttributesStack.push( ctx->m_viewportAttributes );
  }
  else
  {
    if ( mask & GL_COLOR_BUFFER_BIT )
    {
      ctx->m_colorBufferAttributesStack.push( ctx->m_colorBufferAttributes );
    }
    if ( mask & GL_DEPTH_BUFFER_BIT )
    {
      ctx->m_depthBufferAttributesStack.push( ctx->m_depthBufferAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT ) )
    {
      ctx->m_colorBufferEnableAttributesStack.push( ctx->m_colorBufferEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT ) )
    {
      ctx->m_depthBufferEnableAttributesStack.push( ctx->m_depthBufferEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_FOG_BIT ) )
    {
      ctx->m_fogEnableAttributesStack.push( ctx->m_fogEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_LIGHTING_BIT ) )
    {
      ctx->m_lightingEnableAttributesStack.push( ctx->m_lightingEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_LINE_BIT ) )
    {
      ctx->m_lineEnableAttributesStack.push( ctx->m_lineEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_POINT_BIT ) )
    {
      ctx->m_pointEnableAttributesStack.push( ctx->m_pointEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_POLYGON_BIT ) )
    {
      ctx->m_polygonEnableAttributesStack.push( ctx->m_polygonEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_SCISSOR_BIT ) )
    {
      ctx->m_scissorEnableAttributesStack.push( ctx->m_scissorEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_STENCIL_BUFFER_BIT ) )
    {
      ctx->m_stencilBufferEnableAttributesStack.push( ctx->m_stencilBufferEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_TEXTURE_BIT ) )
    {
      ctx->m_textureEnableAttributesStack.push( ctx->m_textureEnableAttributes );
    }
    if ( mask & ( GL_ENABLE_BIT | GL_TRANSFORM_BIT ) )
    {
      ctx->m_transformEnableAttributesStack.push( ctx->m_transformEnableAttributes );
    }
    if ( mask & GL_EVAL_BIT )
    {
      ctx->m_evalAttributesStack.push( ctx->m_evalAttributes );
    }
    if ( mask & GL_FOG_BIT )
    {
      ctx->m_fogAttributesStack.push( ctx->m_fogAttributes );
    }
    if ( mask & GL_LIGHTING_BIT )
    {
      ctx->m_lightingAttributesStack.push( ctx->m_lightingAttributes );
    }
    if ( mask & GL_LINE_BIT )
    {
      ctx->m_lineAttributesStack.push( ctx->m_lineAttributes );
    }
    if ( mask & GL_POINT_BIT )
    {
      ctx->m_pointAttributesStack.push( ctx->m_pointAttributes );
    }
    if ( mask & GL_POLYGON_BIT )
    {
      ctx->m_polygonAttributesStack.push( ctx->m_polygonAttributes );
    }
    if ( mask & GL_SCISSOR_BIT )
    {
      ctx->m_scissorAttributesStack.push( ctx->m_scissorAttributes );
    }
    if ( mask & GL_STENCIL_BUFFER_BIT )
    {
      ctx->m_stencilBufferAttributesStack.push( ctx->m_stencilBufferAttributes );
    }
    if ( mask & GL_TEXTURE_BIT )
    {
      ctx->m_textureAttributesStack.push( ctx->m_textureAttributes );
    }
    if ( mask & GL_TRANSFORM_BIT )
    {
      ctx->m_transformAttributesStack.push( ctx->m_transformAttributes );
    }
    if ( mask & GL_VIEWPORT_BIT )
    {
      ctx->m_viewportAttributesStack.push( ctx->m_viewportAttributes );
    }
  }
#endif
}

//! Replacement of glPushClientAttrib
/** This function asserts on the result before returning.
  * \note Currently we don't hold a stack of cached GL states, so GLPushClientAttrib/GLPopClientAttrib doesn't affect our cached
  * values. It is therefore recommended to used GLPushAttrib/GLPopAttrib very local and to not use other potentially caching
  * GL* functions (with uppercase GL), inbetween such a pair. */
inline  void  GLPushClientAttrib( GLbitfield mask )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPushClientAttrib( mask );
  ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
  ctx->m_clientPushMasks.push( mask );
  if ( mask & GL_CLIENT_VERTEX_ARRAY_BIT )
  {
    // note: vertex buffer binding states will be pushed/popped along with the vertex array client state
    ctx->m_vertexBufferBindingAttributesStack.push(ctx->m_bufferBindingAttributes.getVertexBufferBinding());
    ctx->m_vertexArrayAttributesStack.push( ctx->m_vertexArrayAttributes );
  }
  if ( mask & GL_CLIENT_PIXEL_STORE_BIT )
  {
    // note: pixel buffer binding states will be pushed/popped along with the pixel store client state
    ctx->m_pixelBufferBindingAttributesStack.push(ctx->m_bufferBindingAttributes.getPixelBufferBinding());
  }
#endif
}

//! Replacement of glPushMatrix
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLPushMatrix( void )
{
  NVSG_GL_STATS_PUSH_MATRIX();
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glPushMatrix();
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glPushName
/** This function asserts on the result before returning.  */
inline  void  GLPushName( GLuint name )
{
  NVSG_GL_STATS_FCT_COUNT();
  glPushName( name );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos2d
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos2d( GLdouble x, GLdouble y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos2d( x, y );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos2f
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos2f( GLfloat x, GLfloat y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos2f( x, y );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos2i
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos2i( GLint x, GLint y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos2i( x, y );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos2s
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos2s( GLshort x, GLshort y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos2s( x, y );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos3d
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos3d( GLdouble x, GLdouble y, GLdouble z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos3d( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos3f
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos3f( GLfloat x, GLfloat y, GLfloat z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos3f( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos3i
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos3i( GLint x, GLint y, GLint z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos3i( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos3s
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos3s( GLshort x, GLshort y, GLshort z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos3s( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos4d
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos4d( x, y, z, w );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos4f
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos4f( x, y, z, w );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos4i
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos4i( GLint x, GLint y, GLint z, GLint w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos4i( x, y, z, w );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRasterPos4s
/** This function asserts on the result before returning.  */
inline  void  GLRasterPos4s( GLshort x, GLshort y, GLshort z, GLshort w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRasterPos4s( x, y, z, w );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glReadBuffer
/** This function asserts on the result before returning.  */
inline  void  GLReadBuffer( GLenum mode ) 
{ 
  NVSG_GL_STATS_FCT_COUNT();
  glReadBuffer( mode );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glReadPixels
/** This function asserts on the result before returning.  */
inline  void  GLReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels )
{
  NVSG_GL_STATS_FCT_COUNT();
  glReadPixels( x, y, width, height, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRectd
/** This function asserts on the result before returning.  */
inline  void  GLRectd( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRectd( x1, y1, x2, y2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRectf
/** This function asserts on the result before returning.  */
inline  void  GLRectf( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRectf( x1, y1, x2, y2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRecti
/** This function asserts on the result before returning.  */
inline  void  GLRecti( GLint x1, GLint y1, GLint x2, GLint y2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRecti( x1, y1, x2, y2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRects
/** This function asserts on the result before returning.  */
inline  void  GLRects( GLshort x1, GLshort y1, GLshort x2, GLshort y2 )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRects( x1, y1, x2, y2 );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRenderMode
/** This function asserts on the result before returning.  */
inline  void  GLRenderMode( GLenum mode )
{
  NVSG_GL_STATS_FCT_COUNT();
  glRenderMode( mode );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRotated
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLRotated( GLdouble angle, GLdouble x, GLdouble y, GLdouble z )
{
  NVSG_GL_STATS_ROTATED( angle, x, y, z );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glRotated( angle, x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glRotatef
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLRotatef( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
  NVSG_GL_STATS_ROTATEF( angle, x, y, z );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glRotatef( angle, x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glScaled
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLScaled( GLdouble x, GLdouble y, GLdouble z )
{
  NVSG_GL_STATS_SCALED( x, y, z );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glScaled( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glScalef
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLScalef( GLfloat x, GLfloat y, GLfloat z )
{
  NVSG_GL_STATS_SCALEF( x, y, z );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glScalef( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glScissor
/** This function caches the scissor settings and asserts on the result before returning.  */
inline  void  GLScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_scissorX      = ctx->m_scissorAttributes.m_x;
  ctx->m_previousAttributes.m_scissorY      = ctx->m_scissorAttributes.m_y;
  ctx->m_previousAttributes.m_scissorWidth  = ctx->m_scissorAttributes.m_width;
  ctx->m_previousAttributes.m_scissorHeight = ctx->m_scissorAttributes.m_height;
# endif
  if (    ( ctx->m_scissorAttributes.m_x != x )
      ||  ( ctx->m_scissorAttributes.m_y != y )
      ||  ( ctx->m_scissorAttributes.m_width != width )
      ||  ( ctx->m_scissorAttributes.m_height != height ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glScissor( x, y, width, height );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_scissorAttributes.m_x = x;
    ctx->m_scissorAttributes.m_y = y;
    ctx->m_scissorAttributes.m_width = width;
    ctx->m_scissorAttributes.m_height = height;
#endif
  }
}

/*! \brief Helper function to reset the scissor rectangle.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the scissor
 *  rectangle to it's previous state. Otherwise, it does nothing.
 *  \sa GLScissor */
inline void GLResetScissor()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLScissor( ctx->m_previousAttributes.m_scissorX, ctx->m_previousAttributes.m_scissorY
           , ctx->m_previousAttributes.m_scissorWidth, ctx->m_previousAttributes.m_scissorHeight );
#else
#endif
}

//! Replacement of glSecondaryColorPointer
/** This function caches the secondary color pointer setting and asserts on the result before returning.  */
inline  void  GLSecondaryColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer )
{
#if defined( GL_CACHING )
  GLContext::VertexArrayParameters parameters( size, type, stride, pointer );
  if ( glContext()->m_vertexArrayAttributes.m_secondaryColorArrayParams != parameters )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glContext()->m_glFunctions->glSecondaryColorPointer( size, type, stride, pointer );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    glContext()->m_vertexArrayAttributes.m_secondaryColorArrayParams = parameters;
#endif
  }
}

//! Replacement of glSelectBuffer
/** This function asserts on the result before returning.  */
inline  void  GLSelectBuffer( GLsizei size, GLuint *buffer )
{
  NVSG_GL_STATS_FCT_COUNT();
  glSelectBuffer( size, buffer );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glSetWindowStereoModeNV
/** This function hides the extension character of glSetWindowStereoModeNV and asserts on the result before returning.  */
inline  void  GLSetWindowStereoMode( GLboolean displayMode )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions );
  if ( glContext()->m_glFunctions->glSetWindowStereoModeNV )
  {
    glContext()->m_glFunctions->glSetWindowStereoModeNV( displayMode );
    ASSERT_GL_NO_ERROR;
  }
}

//! Replacement of glShadeModel
/** This function caches the shade model setting and asserts on the result before returning.  */
inline  void  GLShadeModel( GLenum mode )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_shadeModel = ctx->m_lightingAttributes.m_shadeModel;
# endif
  if ( ctx->m_lightingAttributes.m_shadeModel != mode )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glShadeModel( mode );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_lightingAttributes.m_shadeModel = mode;
#endif
  }
}

/*! \brief Helper function to reset the shade model.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the shade model
 *  to it's previous state. Otherwise, it is set to GL_SMOOTH.
 *  \sa GLShadeModel */
inline void GLResetShadeModel()
{
#if defined(GL_SUPPORT_RESET)
  GLShadeModel( glContext()->m_previousAttributes.m_shadeModel );
#else
  GLShadeModel( GL_SMOOTH ); 
#endif
}

//! Replacement of glStencilFunc
/** This function caches the stencil function setting and asserts on the result before returning.  */
inline  void  GLStencilFunc( GLenum func, GLint ref, GLuint mask )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_stencilFunc      = ctx->m_stencilBufferAttributes.m_stencilFunc;
  ctx->m_previousAttributes.m_stencilRef       = ctx->m_stencilBufferAttributes.m_stencilRef;
  ctx->m_previousAttributes.m_stencilValueMask = ctx->m_stencilBufferAttributes.m_stencilValueMask;
# endif
  if (    ( ctx->m_stencilBufferAttributes.m_stencilFunc != func )
      ||  ( ctx->m_stencilBufferAttributes.m_stencilRef != ref )
      ||  ( ctx->m_stencilBufferAttributes.m_stencilValueMask != mask ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glStencilFunc( func, ref, mask );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_stencilBufferAttributes.m_stencilFunc = func;
    ctx->m_stencilBufferAttributes.m_stencilRef = ref;
    ctx->m_stencilBufferAttributes.m_stencilValueMask = mask;
#endif
  }
}

/*! \brief Helper function to reset the stencil function values.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the stencil
 *  function values to it's previous state. Otherwise, it sets the function, reference value,
 *  and mask to GL_ALWAYS, 0, and ~0, respectively.
 *  \sa GLStencilFunc */
inline void GLResetStencilFunc()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLStencilFunc( ctx->m_previousAttributes.m_stencilFunc, ctx->m_previousAttributes.m_stencilRef
               , ctx->m_previousAttributes.m_stencilValueMask );
#else
  GLStencilFunc( GL_ALWAYS, 0, ~0 );
#endif
}

//! Replacement of glStencilMask
/** This function caches the stencil mask setting and asserts on the result before returning.  */
inline  void  GLStencilMask( GLuint mask )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_stencilWriteMask = ctx->m_stencilBufferAttributes.m_stencilWriteMask;
# endif
  if ( ctx->m_stencilBufferAttributes.m_stencilWriteMask != mask )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glStencilMask( mask );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_stencilBufferAttributes.m_stencilWriteMask = mask;
#endif
  }
}

/*! \brief Helper function to reset the stencil mask.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the stencil
 *  mask to it's previous state. Otherwise, it is set to ~0.
 *  \sa GLStencilMask */
inline void GLResetStencilMask()
{
#if defined(GL_SUPPORT_RESET)
  GLStencilMask( glContext()->m_previousAttributes.m_stencilWriteMask );
#else
  GLStencilMask( ~0 );
#endif
}

//! Replacement of glStencilOp
/** This function caches the stencil operation setting and asserts on the result before returning.  */
inline  void  GLStencilOp( GLenum fail, GLenum zfail, GLenum zpass )
{
#if defined( GL_CACHING )
  GLContext * ctx = glContext();
# if defined(GL_SUPPORT_RESET)
  ctx->m_previousAttributes.m_stencilFail          = ctx->m_stencilBufferAttributes.m_stencilFail;
  ctx->m_previousAttributes.m_stencilPassDepthFail = ctx->m_stencilBufferAttributes.m_stencilPassDepthFail;
  ctx->m_previousAttributes.m_stencilPassDepthPass = ctx->m_stencilBufferAttributes.m_stencilPassDepthPass;
# endif
  if (    ( ctx->m_stencilBufferAttributes.m_stencilFail != fail )
      ||  ( ctx->m_stencilBufferAttributes.m_stencilPassDepthFail != zfail )
      ||  ( ctx->m_stencilBufferAttributes.m_stencilPassDepthPass != zpass ) )
#endif
  {
    NVSG_GL_STATS_FCT_COUNT();
    glStencilOp( fail, zfail, zpass );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    ctx->m_stencilBufferAttributes.m_stencilFail = fail;
    ctx->m_stencilBufferAttributes.m_stencilPassDepthFail = zfail;
    ctx->m_stencilBufferAttributes.m_stencilPassDepthPass = zpass;
#endif
  }
}

/*! \brief Helper function to reset the stencil operation values.
 *  \remarks If GL_SUPPORT_RESET is defined (per default, it is), this function sets the stencil
 *  operation values to it's previous state. Otherwise, it sets each value (fail, zfail, zpass) to
 *  GL_KEEP.
 *  \sa GLStencilMask */
inline void GLResetStencilOp()
{
#if defined(GL_SUPPORT_RESET)
  GLContext * ctx = glContext();
  GLStencilOp( ctx->m_previousAttributes.m_stencilFail
             , ctx->m_previousAttributes.m_stencilPassDepthFail
             , ctx->m_previousAttributes.m_stencilPassDepthPass );
#else
  GLStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
#endif
}

//! Replacement of glTexCoord1d
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord1d( GLdouble s )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord1d( s );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord1f
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord1f( GLfloat s )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord1f( s );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord1i
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord1i( GLint s )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord1i( s );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord1s
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord1s( GLshort s )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord1s( s );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord2d
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord2d( GLdouble s, GLdouble t )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord2d( s, t );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord2f
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord2f( GLfloat s, GLfloat t )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord2f( s, t );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord2fv
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord2fv( const GLfloat *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord2fv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord2i
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord2i( GLint s, GLint t )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord2i( s, t );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord2s
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord2s( GLshort s, GLshort t )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord2s( s, t );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord3d
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord3d( GLdouble s, GLdouble t, GLdouble r )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord3d( s, t, r );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord3f
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord3f( GLfloat s, GLfloat t, GLfloat r )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord3f( s, t, r );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord3i
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord3i( GLint s, GLint t, GLint r )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord3i( s, t, r );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord3s
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord3s( GLshort s, GLshort t, GLshort r )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord3s( s, t, r );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord4d
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord4d( GLdouble s, GLdouble t, GLdouble r, GLdouble q )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord4d( s, t, r, q );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord4f
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord4f( GLfloat s, GLfloat t, GLfloat r, GLfloat q )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord4f( s, t, r, q );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord4i
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord4i( GLint s, GLint t, GLint r, GLint q )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord4i( s, t, r, q );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoord4s
/** This function asserts on the result before returning.  */
inline  void  GLTexCoord4s( GLshort s, GLshort t, GLshort r, GLshort q )
{
  NVSG_GL_STATS_FCT_COUNT();
  glTexCoord4s( s, t, r, q );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glTexCoordPointer
/** This function caches the tex coord pointer setting and asserts on the result before returning.  */
inline  void  GLTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer )
{
#if defined( GL_CACHING )
  _GLUpdateClientActiveTexture();
  NVSG_ASSERT(glContext()->m_vertexArrayAttributes.m_clientTextureActive<8);
  size_t i = glContext()->m_vertexArrayAttributes.m_clientTextureActive;
  GLContext::VertexArrayParameters parameters( size, type, stride, pointer );
  if ( glContext()->m_vertexArrayAttributes.m_textureCoordArrayParams[i] != parameters )
#endif
  {
    NVSG_GL_STATS_TEX_COORD_POINTER( size, type, stride, pointer );
    glTexCoordPointer( size, type, stride, pointer );    
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    glContext()->m_vertexArrayAttributes.m_textureCoordArrayParams[i] = parameters;
#endif
  }
}

//! Cached version of GLTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, params )
/** This function caches the GL_TEXTURE_ENV_COLOR value of the active texture unit.
  * It is faster to call this method instead of GLTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, params) directly. */
inline  void  GLTextureEnvColor( const GLfloat * params )
{
#if defined( GL_CACHING )
  if ( !equal4fv( &glContext()->m_textureAttributes.m_unit[glContext()->m_textureAttributes.m_toActivate].m_envColor[0], params ) )
  {
    _GLUpdateActiveTexture();
# if !defined(NDEBUG)
    GLfloat tmp[4];
    glGetTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, tmp );
    NVSG_ASSERT( equal4fv(&glContext()->m_textureAttributes.m_unit[glContext()->m_textureAttributes.m_toActivate].m_envColor[0], params ) );
# endif
#endif
    _GLTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, params );
#if defined( GL_CACHING )
    memcpy( &glContext()->m_textureAttributes.m_unit[glContext()->m_textureAttributes.m_toActivate].m_envColor[0], params, 4*sizeof(float) );
  }
#endif
}

//! Cached version of GLTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param )
/** This function caches the GL_TEXTURE_ENV_MODE value of the active texture unit.
* It is faster to call this method instead of GLTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param) directly. */
inline  void  GLTextureEnvMode( GLint param )
{
#if defined( GL_CACHING )
  if ( glContext()->m_textureAttributes.m_unit[glContext()->m_textureAttributes.m_toActivate].m_envMode != param )
  {
    _GLUpdateActiveTexture();
# if !defined(NDEBUG)
    GLint tem;
    glGetTexEnviv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &tem );
    NVSG_ASSERT( glContext()->m_textureAttributes.m_unit[glContext()->m_textureAttributes.m_toActivate].m_envMode == tem );
# endif
#endif
    _GLTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param );
#if defined( GL_CACHING )
    glContext()->m_textureAttributes.m_unit[glContext()->m_textureAttributes.m_toActivate].m_envMode = param;
  }
#endif
}

//! Cached version of glTexEnvf( target, pname, param )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexEnvf( GLenum target, GLenum pname, GLfloat param )
{
  NVSG_ASSERT( ( target == GL_TEXTURE_ENV ) );
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_ENV_MODE :
      GLTextureEnvMode( (GLint)param );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexEnvf( target, pname, param );
      break;
  }
#else
  _GLTexEnvf( target, pname, param );
#endif
}

//! Cached version of glTexEnvf( target, pname, param )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexEnvfv( GLenum target, GLenum pname, const GLfloat *params )
{
  NVSG_ASSERT( ( target == GL_TEXTURE_ENV ) );
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_ENV_COLOR :
      GLTextureEnvColor( params );
      break;
    case GL_TEXTURE_ENV_MODE :
      GLTextureEnvMode( (GLint)params[0] );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexEnvfv( target, pname, params );
      break;
  }
#else
  _GLTexEnvfv( target, pname, params );
#endif
}

//! Cached version of glTexEnvi( target, pname, param )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexEnvi( GLenum target, GLenum pname, GLint param )
{
  NVSG_ASSERT( ( target == GL_TEXTURE_ENV ) );
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_ENV_MODE :
      GLTextureEnvMode( param );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexEnvi( target, pname, param );
      break;
  }
#else
  _GLTexEnvi( target, pname, param );
#endif
}

//! Cached version of glTexEnviv( target, pname, param )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexEnviv( GLenum target, GLenum pname, const GLint *params )
{
  NVSG_ASSERT( ( target == GL_TEXTURE_ENV ) );
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_ENV_MODE :
      GLTextureEnvMode( params[0] );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexEnviv( target, pname, params );
      break;
  }
#else
  _GLTexEnviv( target, pname, params );
#endif
}

//! Replacement of glTexGend
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexGend( GLenum coord, GLenum pname, GLdouble param )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexGend( coord, pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexGenf
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexGenf( GLenum coord, GLenum pname, GLfloat param )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexGenf( coord, pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexGenfv
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexGenfv( GLenum coord, GLenum pname, const GLfloat *param )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexGenfv( coord, pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexGeni
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexGeni( GLenum coord, GLenum pname, GLint param )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexGeni( coord, pname, param );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexImage1D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexImage1D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexImage1D( target, level, internalformat, width, border, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexImage2D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border
                          , GLenum format, GLenum type, const GLvoid *pixels )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_TEX_IMAGE2D( target, level, internalformat, width, height, border, format, type, pixels );
  glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexImage3D
/** This function hides the extension character of glTexImage3D, updates the active texture,
  * and asserts on the result before returning.  */
inline  void  GLTexImage3D( GLenum target, int level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, int border, GLenum format, GLenum type, const void* pixels)
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(GLGetVersion()>=12 || GLIsExtensionAvailable("GL_EXT_texture3D"));
  NVSG_ASSERT( glContext()->m_glFunctions && glContext()->m_glFunctions->glTexImage3D );
  glContext()->m_glFunctions->glTexImage3D( target, level, internalformat, width, height, depth, border, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Cached version of GLTexParameterfv( target, GL_TEXTURE_BORDER_COLOR, params )
/** This function caches the GL_TEXTURE_BORDER_COLOR values of the texture currently bound to target.
  * It is faster to call this method instead of GLTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, params) directly. */
inline  void  GLTextureBorderColor( GLenum target, const GLfloat *params )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  _GLTexParameterfv( target, GL_TEXTURE_BORDER_COLOR, params );
}

//! Cached version of GLTexParameteri( target, GL_TEXTURE_MAG_FILTER, param )
/** This function caches the GL_TEXTURE_MAG_FILTER values of the texture currently bound to target.
  * It is faster to call this method instead of GLTexParameteri(target, GL_TEXTURE_MAG_FILTER, param) directly. */
inline  void  GLTextureMagFilter( GLenum target, GLint param )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  _GLTexParameteri( target, GL_TEXTURE_MAG_FILTER, param );
}

//! Cached version of GLTexParameteri( target, GL_TEXTURE_MIN_FILTER, param )
/** This function caches the GL_TEXTURE_MIN_FILTER values of the texture currently bound to target.
  * It is faster to call this method instead of GLTexParameteri(target, GL_TEXTURE_MIN_FILTER, param) directly. */
inline  void  GLTextureMinFilter( GLenum target, GLint param )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  _GLTexParameteri( target, GL_TEXTURE_MIN_FILTER, param );
}

//! Cached version of GLTexParameteri( target, GL_TEXTURE_WRAP_R, param )
/** This function caches the GL_TEXTURE_WRAP_R values of the texture currently bound to target.
  * It is faster to call this method instead of GLTexParameteri(target, GL_TEXTURE_WRAP_R, param) directly. */
inline  void  GLTextureWrapR( GLenum target, GLint param )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  _GLTexParameteri( target, GL_TEXTURE_WRAP_R, param );
}

//! Cached version of GLTexParameteri( target, GL_TEXTURE_WRAP_S, param )
/** This function caches the GL_TEXTURE_WRAP_S values of the texture currently bound to target.
  * It is faster to call this method instead of GLTexParameteri(target, GL_TEXTURE_WRAP_S, param) directly. */
inline  void  GLTextureWrapS( GLenum target, GLint param )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  _GLTexParameteri( target, GL_TEXTURE_WRAP_S, param );
}

//! Cached version of GLTexParameteri( target, GL_TEXTURE_WRAP_T, param )
/** This function caches the GL_TEXTURE_WRAP_T values of the texture currently bound to target.
  * It is faster to call this method instead of GLTexParameteri(target, GL_TEXTURE_WRAP_T, param) directly. */
inline  void  GLTextureWrapT( GLenum target, GLint param )
{
#if defined( GL_CACHING )
  _GLUpdateActiveTexture();
#endif
  _GLTexParameteri( target, GL_TEXTURE_WRAP_T, param );
}

//! Cached version of glTexParameterf( target, pname, param )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexParameterf( GLenum target, GLenum pname, GLfloat param )
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_BORDER_COLOR :
      NVSG_ASSERT( false );
      break;
    case GL_TEXTURE_MAG_FILTER :
      GLTextureMagFilter( target, (GLint)param );
      break;
    case GL_TEXTURE_MIN_FILTER :
      GLTextureMinFilter( target, (GLint)param );
      break;
    case GL_TEXTURE_WRAP_R :
      GLTextureWrapR( target, (GLint)param );
      break;
    case GL_TEXTURE_WRAP_S :
      GLTextureWrapS( target, (GLint)param );
      break;
    case GL_TEXTURE_WRAP_T :
      GLTextureWrapT( target, (GLint)param );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexParameterf( target, pname, param );
      break;
  }
#else
  _GLTexParameterf( target, pname, param );
#endif
}

//! Cached version of glTexParameterfv( target, pname, params )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexParameterfv( GLenum target, GLenum pname, const GLfloat *params )
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_BORDER_COLOR :
      GLTextureBorderColor( target, params );
      break;
    case GL_TEXTURE_MAG_FILTER :
      GLTextureMagFilter( target, (GLint)params[0] );
      break;
    case GL_TEXTURE_MIN_FILTER :
      GLTextureMinFilter( target, (GLint)params[0] );
      break;
    case GL_TEXTURE_WRAP_R :
      GLTextureWrapR( target, (GLint)params[0] );
      break;
    case GL_TEXTURE_WRAP_S :
      GLTextureWrapS( target, (GLint)params[0] );
      break;
    case GL_TEXTURE_WRAP_T :
      GLTextureWrapT( target, (GLint)params[0] );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexParameterfv( target, pname, params );
      break;
  }
#else
  _GLTexParameterfv( target, pname, params );
#endif
}

//! Cached version of glTexParameteri( target, pname, param )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexParameteri( GLenum target, GLenum pname, GLint param )
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_BORDER_COLOR :
      NVSG_ASSERT( false );
      break;
    case GL_TEXTURE_MAG_FILTER :
      GLTextureMagFilter( target, param );
      break;
    case GL_TEXTURE_MIN_FILTER :
      GLTextureMinFilter( target, param );
      break;
    case GL_TEXTURE_WRAP_R :
      GLTextureWrapR( target, param );
      break;
    case GL_TEXTURE_WRAP_S :
      GLTextureWrapS( target, param );
      break;
    case GL_TEXTURE_WRAP_T :
      GLTextureWrapT( target, param );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexParameteri( target, pname, param );
      break;
  }
#else
  _GLTexParameteri( target, pname, param );
#endif
}

//! Cached version of glTexParameteriv( target, pname, params )
/** This function caches some of the texture environment parameters and asserts on the result before returning. */
inline  void  GLTexParameteriv( GLenum target, GLenum pname, const GLint *params )
{
#if defined( GL_CACHING )
  switch( pname )
  {
    case GL_TEXTURE_BORDER_COLOR :
      {
        GLfloat fParams[4];
        GLintToGLfloat( params, fParams );
        GLTextureBorderColor( target, fParams );
      }
      break;
    case GL_TEXTURE_MAG_FILTER :
      GLTextureMagFilter( target, params[0] );
      break;
    case GL_TEXTURE_MIN_FILTER :
      GLTextureMinFilter( target, params[0] );
      break;
    case GL_TEXTURE_WRAP_R :
      GLTextureWrapR( target, params[0] );
      break;
    case GL_TEXTURE_WRAP_S :
      GLTextureWrapS( target, params[0] );
      break;
    case GL_TEXTURE_WRAP_T :
      GLTextureWrapT( target, params[0] );
      break;
    default :
      _GLUpdateActiveTexture();
      _GLTexParameteriv( target, pname, params );
      break;
  }
#else
  _GLTexParameteriv( target, pname, params );
#endif
}

//! Replacement of glTexSubImage1D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexSubImage1D( target, level, xoffset, width, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexSubImage2D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTexSubImage3D
/** This function updates the active texture and asserts on the result before returning.  */
inline  void  GLTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels )
{
#if defined(GL_CACHING)
  _GLUpdateActiveTexture();
#endif
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(GLGetVersion()>=12 || GLIsExtensionAvailable("GL_EXT_texture3D"));
  NVSG_ASSERT(glContext()->m_glFunctions && glContext()->m_glFunctions->glTexSubImage3D);
  glContext()->m_glFunctions->glTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTranslated
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLTranslated( GLdouble x, GLdouble y, GLdouble z )
{
  NVSG_GL_STATS_TRANSLATED( x, y, z );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glTranslated( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glTranslatef
/** This function updates the active texture if the matrix mode is GL_TEXTURE and asserts on the result before returning.  */
inline  void  GLTranslatef( GLfloat x, GLfloat y, GLfloat z )
{
  NVSG_GL_STATS_TRANSLATEF( x, y, z );
#if defined( GL_CACHING )
  if ( GL_TEXTURE == glContext()->m_transformAttributes.m_matrixMode )
  { // note: we have a texture matrix stack per unit
    _GLUpdateActiveTexture();
  }
#endif
  glTranslatef( x, y, z );
  ASSERT_GL_NO_ERROR;
}

//! Replacement of glVertex2d
/** This function asserts on the result before returning.  */
inline  void  GLVertex2d( GLdouble x, GLdouble y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex2d( x, y );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex2f
/** This function asserts on the result before returning.  */
inline  void  GLVertex2f( GLfloat x, GLfloat y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex2f( x, y );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex2i
/** This function asserts on the result before returning.  */
inline  void  GLVertex2i( GLint x, GLint y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex2i( x, y );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex2s
/** This function asserts on the result before returning.  */
inline  void  GLVertex2s( GLshort x, GLshort y )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex2s( x, y );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex3d
/** This function asserts on the result before returning.  */
inline  void  GLVertex3d( GLdouble x, GLdouble y, GLdouble z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex3d( x, y, z );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex3f
/** This function asserts on the result before returning.  */
inline  void  GLVertex3f( GLfloat x, GLfloat y, GLfloat z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex3f( x, y, z );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex3fv
/** This function asserts on the result before returning.  */
inline  void  GLVertex3fv( const GLfloat *v )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex3fv( v );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex3i
/** This function asserts on the result before returning.  */
inline  void  GLVertex3i( GLint x, GLint y, GLint z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex3i( x, y, z );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex3s
/** This function asserts on the result before returning.  */
inline  void  GLVertex3s( GLshort x, GLshort y, GLshort z )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex3s( x, y, z );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex4d
/** This function asserts on the result before returning.  */
inline  void  GLVertex4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex4d( x, y, z, w );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex4f
/** This function asserts on the result before returning.  */
inline  void  GLVertex4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex4f( x, y, z, w );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex4i
/** This function asserts on the result before returning.  */
inline  void  GLVertex4i( GLint x, GLint y, GLint z, GLint w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex4i( x, y, z, w );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertex4s
/** This function asserts on the result before returning.  */
inline  void  GLVertex4s( GLshort x, GLshort y, GLshort z, GLshort w )
{
  NVSG_GL_STATS_FCT_COUNT();
  glVertex4s( x, y, z, w );
  ASSERT_GL_NO_ERROR_IN_BEGIN_END;
}

//! Replacement of glVertexPointer
/** This function caches the vertex pointer setting and asserts on the result before returning.  */
inline  void  GLVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer )
{
#if defined( GL_CACHING )
  GLContext::VertexArrayParameters parameters( size, type, stride, pointer );
  if ( glContext()->m_vertexArrayAttributes.m_vertexArrayParams != parameters )
#endif
  {
    NVSG_GL_STATS_VERTEX_POINTER( size, type, stride, pointer );
    glVertexPointer( size, type, stride, pointer );
    ASSERT_GL_NO_ERROR;
#if defined( GL_CACHING )
    glContext()->m_vertexArrayAttributes.m_vertexArrayParams = parameters;
#endif
  }
}

//! Replacement of glViewport
/** This function asserts on the result before returning.  */
inline  void  GLViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
  NVSG_GL_STATS_VIEWPORT( x, y, width, height );
  glViewport( x, y, width, height );
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glIsRenderbufferEXT.
 *  \param renderbuffer The id to test.
 *  \return \c TRUE, if \a renderbuffer is the id of a render buffer, otherwise \c FALSE.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLBindRenderbuffer, GLDeleteRenderbuffers, GLGenRenderbuffers, GLRenderbufferStorage,
 *  GLGetRenderbufferParameteriv */
inline GLboolean GLIsRenderbuffer(GLuint renderbuffer)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  GLboolean isRenderBuffer = glContext()->m_glFunctions->glIsRenderbufferEXT(renderbuffer);
  ASSERT_GL_NO_ERROR;
  return isRenderBuffer;
}

/*! \brief Replacement of glBindRenderbufferEXT.
 *  \param target The id of the target to bind the render buffer to. Must be GL_RENDERBUFFER_EXT.
 *  \param renderbuffer The id of the unused render buffer to bind.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsRenderbuffer, GLDeleteRenderbuffers, GLGenRenderbuffers, GLRenderbufferStorage,
 *  GLGetRenderbufferParameteriv */
inline void GLBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glBindRenderbufferEXT(target, renderbuffer);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glDeleteRenderbuffersEXT.
 *  \param n The number of render buffer ids to delete.
 *  \param renderbuffers A pointer to the render buffer ids to delete.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsRenderbuffer, GLBindRenderbuffer, GLGenRenderbuffers, GLRenderbufferStorage,
 *  GLGetRenderbufferParameteriv */
inline void GLDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glDeleteRenderbuffersEXT(n, renderbuffers);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glGenRenderbuffersEXT.
 *  \param n The number of render buffer ids to generate.
 *  \param renderbuffers A pointer to the memory to fill with the generated render buffer ids.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsRenderbuffer, GLBindRenderbuffer, GLDeleteRenderbuffers, GLRenderbufferStorage,
 *  GLGetRenderbufferParameteriv */
inline void GLGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glGenRenderbuffersEXT(n, renderbuffers);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glRenderbufferStorageEXT.
 *  \param target Must be GL_RENDERBUFFER_EXT.
 *  \param internalformat Must be color-renderable, depth-renderable, or stencil-renderable.
 *  \param width The width of the render buffer in pixels.
 *  \param height The height of the render buffer in pixels.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsRenderbuffer, GLBindRenderbuffer, GLDeleteRenderbuffers, GLGenRenderbuffers,
 *  GLGetRenderbufferParameteriv */
inline void GLRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glRenderbufferStorageEXT(target, internalformat, width, height);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glGetRenderbufferParameterivEXT.
 *  \param target Must be GL_RENDERBUFFER_EXT.
 *  \param pname The id of the value to get.
 *  \param params Pointer to the memory to write the queried value to.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsRenderbuffer, GLBindRenderbuffer, GLDeleteRenderbuffers, GLGenRenderbuffers,
 *  GLRenderbufferStorage */
inline void GLGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glGetRenderbufferParameterivEXT(target, pname, params);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glIsFramebufferEXT.
 *  \param framebuffer The id to test.
 *  \return \c TRUE, if \a framebuffer is the id of a frame buffer, otherwise \c FALSE.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers, GLCheckFramebufferStatus,
 *  GLFramebufferTexture1D, GLFramebufferTexture2D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline GLboolean GLIsFramebuffer(GLuint framebuffer)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  GLboolean isFramebuffer = glContext()->m_glFunctions->glIsFramebufferEXT(framebuffer);
  ASSERT_GL_NO_ERROR;
  return isFramebuffer;
}

/*! \brief Replacement of glBindFramebufferEXT.
 *  \param target The id of the target to bind the frame buffer to. Must be GL_FRAMEBUFFER_EXT.
 *  \param framebuffer The id of the unused frame buffer to bind.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers, GLCheckFramebufferStatus,
 *  GLFramebufferTexture1D, GLFramebufferTexture2D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline void GLBindFramebuffer(GLenum target, GLuint framebuffer)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glBindFramebufferEXT(target, framebuffer);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glDeleteFramebuffersEXT.
 *  \param n The number of frame buffer ids to delete.
 *  \param framebuffers A pointer to the frame buffer ids to delete.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLGenFramebuffers, GLCheckFramebufferStatus,
 *  GLFramebufferTexture1D, GLFramebufferTexture2D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline void GLDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glDeleteFramebuffersEXT(n, framebuffers);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glGenFramebuffersEXT.
 *  \param n The number of frame buffer ids to generate.
 *  \param framebuffers A pointer to the memory to fill with the generated frame buffer ids.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLCheckFramebufferStatus,
 *  GLFramebufferTexture1D, GLFramebufferTexture2D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline void GLGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glGenFramebuffersEXT(n, framebuffers);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glCheckFramebufferStatusEXT.
 *  \param target Must be GL_FRAMEBUFFER_EXT.
 *  \return A symbolic value describing the frame buffer status.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers,
 *  GLFramebufferTexture1D, GLFramebufferTexture2D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline GLenum GLCheckFramebufferStatus(GLenum target)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  GLenum status = glContext()->m_glFunctions->glCheckFramebufferStatusEXT(target);
  ASSERT_GL_NO_ERROR;
  return status;
}

/*! \brief Replacement of glFramebufferTexture1DEXT.
 *  \param target Must be GL_FRAMEBUFFER_EXT.
 *  \param attachment One of the attachment points of the frame buffer.
 *  \param textarget The texture target.
 *  \param texture The id of an existing texture with an target of \a textarget.
 *  \param level The mipmap level of the texture image to be attached.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers,
 *  GLCheckFramebufferStatus, GLFramebufferTexture2D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline void GLFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glFramebufferTexture1DEXT(target, attachment, textarget, texture, level);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glFramebufferTexture2DEXT.
 *  \param target Must be GL_FRAMEBUFFER_EXT.
 *  \param attachment One of the attachment points of the frame buffer.
 *  \param textarget The texture target.
 *  \param texture The id of an existing texture with an target of \a textarget.
 *  \param level The mipmap level of the texture image to be attached.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers,
 *  GLCheckFramebufferStatus, GLFramebufferTexture1D, GLFramebufferTexture3D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline void GLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glFramebufferTexture3DEXT.
 *  \param target Must be GL_FRAMEBUFFER_EXT.
 *  \param attachment One of the attachment points of the frame buffer.
 *  \param textarget The texture target.
 *  \param texture The id of an existing texture with an target of \a textarget.
 *  \param level The mipmap level of the texture image to be attached.
 *  \param zoffset Specifies the z-offset of a 2-dimensional image within a 3-dimensional texture.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers,
 *  GLCheckFramebufferStatus, GLFramebufferTexture1D, GLFramebufferTexture2D,
 *  GLFramebufferRenderbuffer, GLGetFramebufferAttachmentParameteriv */
inline void GLFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glFramebufferTexture3DEXT(target, attachment, textarget, texture, level, zoffset);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glFramebufferRenderbufferEXT.
 *  \param target Must be GL_FRAMEBUFFER_EXT.
 *  \param attachment One of the attachment points of the frame buffer.
 *  \param renderbuffertarget Must be GL_RENDERBUFFER_EXT.
 *  \param renderbuffer The id of the render buffer to be attached to the frame buffer.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers,
 *  GLCheckFramebufferStatus, GLFramebufferTexture1D, GLFramebufferTexture2D,
 *  GLFramebufferTexture3D, GLGetFramebufferAttachmentParameteriv */
inline void GLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glGetFramebufferAttachmentParameterivEXT.
 *  \param target Must be GL_FRAMEBUFFER_EXT.
 *  \param attachment One of the attachment points of the frame buffer.
 *  \param pname The symbolic values of the attachment parameters to query.
 *  \param params A pointer to the memory where result of the query is written to.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error.
 *  \sa GLIsFramebuffer, GLBindFramebuffer, GLDeleteFramebuffers, GLGenFramebuffers,
 *  GLCheckFramebufferStatus, GLFramebufferTexture1D, GLFramebufferTexture2D,
 *  GLFramebufferTexture3D, GLFramebufferRenderbuffer */
inline void GLGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glGetFramebufferAttachmentParameterivEXT(target, attachment, pname, params);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Replacement of glGenerateMipmapEXT
 *  \param target The symbolic value of the texture target to generate mipmaps for.
 *  \note It is assumed that the extension GL_EXT_framebuffer_object is supported.
 *  \note This function asserts on no GL error. */
inline void GLGenerateMipmap(GLenum target)
{
  NVSG_ASSERT(GLIsExtensionAvailable("GL_EXT_framebuffer_object"));
  glContext()->m_glFunctions->glGenerateMipmapEXT(target);
  ASSERT_GL_NO_ERROR;
}

/*! \brief Determine the texture image format out of a PixelFormat and a PixelDataType.
 *  \param fmt The PixelFormat of an image.
 *  \param type The PixelDataType of an image.
 *  \return A pointer to the constant texture image description corresponding to \a fmt and \a type.
 *  \remarks The texture image description returned is some kind of "best match" to \a fmt and \a
 *  type on the current OpenGL context. */
inline const GLTexImageFmt* GLGetTexImageFmt(nvsg::Image::PixelFormat fmt, nvsg::Image::PixelDataType type)
{
  // give a hint while debugging. though, need a run-time check below
  NVSG_ASSERT(fmt!=nvsg::Image::IMG_UNKNOWN_FORMAT || type!=nvsg::Image::IMG_UNKNOWN_TYPE);  

  if( fmt==nvsg::Image::IMG_UNKNOWN_FORMAT || type==nvsg::Image::IMG_UNKNOWN_TYPE )  
  {
    return NULL;
  }

  return &glContext()->m_glTexImageFmts[fmt][type];
}

#if defined(_WIN32)
//! Replacement of wglBindTexImageARB
/** This function hides the extension character of wglBindTexImageARB and asserts on its result before returning. */
inline  BOOL  WGLBindTexImage( HPBUFFERARB hPbuffer, int iBuffer )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglBindTexImageARB );
  return glContext()->m_glFunctions->wglBindTexImageARB( hPbuffer, iBuffer );
}

//! Replacement of wglChoosePixelFormatARB
/** This function hides the extension character of wglChoosePixelFormatARB and asserts on its result before returning. */
inline  BOOL  WGLChoosePixelFormat( HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglChoosePixelFormatARB );
  return glContext()->m_glFunctions->wglChoosePixelFormatARB( hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats );
}

//! Replacement of wglCreateContext.
/** If this function is used in conjunction with WGLCreateContext, WGLMakeCurrent, and the functions with
  * uppercase GL prefix, then the states and attributes are cached per context. Moreover, the extension functions of
  * OpenGL are used per context (as they really should!). */
inline  HGLRC WGLCreateContext( HDC hdc )
{
  HGLRC hglrc = wglCreateContext( hdc );
  WGLAttachContext(hdc, hglrc);
  return hglrc;
}

//! Replacement of wglCreatePbufferARB
/** This function hides the extension character of wglCreatePbufferARB. 
  * \note The creation of a PBuffer might fail dependent on available hardware resources. 
  * In this case a NULL handle will be returned. */
inline  HPBUFFERARB WGLCreatePbuffer( HDC hdc, int pixelFormat, int width, int height, const int *attribList )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( wglGetCurrentContext() != NULL );
  NVSG_ASSERT( glContext()->m_glFunctions->wglCreatePbufferARB );
  HPBUFFERARB hPbuffer = glContext()->m_glFunctions->wglCreatePbufferARB( hdc, pixelFormat, width, height, attribList );
  return( hPbuffer );
}

//! Replacement of wglDeleteContext.
/** If this function is used in conjunction with WGLDeleteContext, WGLMakeCurrent, and functions with
  * uppercase 'GL' prefix, the states and attributes are cached per context. 
  * Moreover, OpenGL extension functions are used per context (as they really should!).
  */
inline  BOOL  WGLDeleteContext( HGLRC hglrc )
{
  NVSG_GL_STATS_FCT_COUNT();
  WGLDetachContext(hglrc);
  return wglDeleteContext(hglrc);
}

//! Replacement of wglDestroyPbufferARB
/** This function hides the extension character of wglDestroyPbufferARB and asserts on its result before returning. */
inline  BOOL  WGLDestroyPbuffer( HPBUFFERARB hPbuffer )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT(glContext()->m_glFunctions->wglDestroyPbufferARB);
  return glContext()->m_glFunctions->wglDestroyPbufferARB( hPbuffer );
}

//! Replacement for wglGetCurrenContext
/** The function obtains a handle to the current OpenGL rendering context of the calling thread. 
  * \returns A handle to the current OpenGL context of the calling thread, or NULL if the calling thread 
  * has no current OpenGL context.
  */
inline  HGLRC  WGLGetCurrentContext(void)
{
  // !! NOTE: no NVSG_GL_STATS_FCT_COUNT macro call here, because it relies on a valid glContext !!
  ASSERT_GL_CURRENT_CONTEXT_CONFORMITY();
  return wglGetCurrentContext();
}

//! Replacement for wglGetCurrentDC
/** The function obtains a handle to the device context that is associated with the current OpenGL rendering context 
  * of the calling thread. 
  * \returns A handle to the device context which is currently associated with the current OpenGL context of the 
  * calling thread by means of WGLMakeCurrent, or NULL if the calling thread has no current OpenGL context.
  */
inline  HDC  WGLGetCurrentDC(void)
{
  NVSG_GL_STATS_FCT_COUNT();
  return wglGetCurrentDC();
}

//! Replacement of wglGetPbufferDCARB
/** This function hides the extension character of wglGetPbufferDCARB and asserts on its result before returning. */
inline  HDC WGLGetPbufferDC( HPBUFFERARB hPbuffer )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglGetPbufferDCARB );
  return glContext()->m_glFunctions->wglGetPbufferDCARB( hPbuffer );
}

//! Replacement of wglQueryPbufferARB
/** This function hides the extension character of wglQueryPbufferARB and asserts on its result before returning. */
inline  BOOL  WGLQueryPbuffer( HPBUFFERARB hPbuffer, int attribute, int *value )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglQueryPbufferARB );
  return glContext()->m_glFunctions->wglQueryPbufferARB( hPbuffer, attribute, value );
}

//! Replacement of wglReleasePbufferDCARB
/** This function hides the extension character of wglReleasePbufferDCARB and asserts on its result before returning. */
inline  int WGLReleasePbufferDC( HPBUFFERARB hPbuffer, HDC hdc )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglReleasePbufferDCARB );
  int i = glContext()->m_glFunctions->wglReleasePbufferDCARB( hPbuffer, hdc );
  NVSG_ASSERT( i == 1 );
  return( i );
}

//! Replacement of wglReleaseTexImageARB
/** This function hides the extension character of wglReleaseTexImageARB and asserts on its result before returning. */
inline  BOOL  WGLReleaseTexImage( HPBUFFERARB hPbuffer, int iBuffer )
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglReleaseTexImageARB );
  return glContext()->m_glFunctions->wglReleaseTexImageARB( hPbuffer, iBuffer );
}

//! Replacement of wglJointSwapGroupNV
inline BOOL WGLJoinSwapGroupNV(HDC hDC, GLuint group)
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_glFunctions->wglJoinSwapGroupNV(hDC, group);
}

//! Replacement of wglBindSwapBarrierNV
inline BOOL WGLBindSwapBarrierNV(GLuint group, GLuint barrier)
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_glFunctions->wglBindSwapBarrierNV(group, barrier);
}

//! Replacement of wglQuerySwapGroupNV
inline BOOL WGLQuerySwapGroupNV(HDC hDC, GLuint * group, GLuint * barrier)
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_glFunctions->wglQuerySwapGroupNV(hDC, group, barrier);
}

//! Replacement of wglQueryMaxSwapGroupsNV
inline BOOL WGLQueryMaxSwapGroupsNV(HDC hDC, GLuint * maxGroups, GLuint * maxBarriers)
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_glFunctions->wglQueryMaxSwapGroupsNV(hDC, maxGroups, maxBarriers);
}

//! Replacement of wglQueryFrameCountNV
inline BOOL WGLQueryFrameCountNV(HDC hDC, GLuint * count)
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_glFunctions->wglQueryFrameCountNV(hDC, count);
}

//! Replacement of wglResetFrameCountNV
inline BOOL WGLResetFrameCountNV(HDC hDC)
{
  NVSG_GL_STATS_FCT_COUNT();
  return glContext()->m_glFunctions->wglResetFrameCountNV(hDC);
}

//! Replacement of wglEnumGpusNV
inline BOOL WGLEnumGpus(UINT iIndex, HGPUNV *hGpu)
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglEnumGpusNV );
  return glContext()->m_glFunctions->wglEnumGpusNV(iIndex, hGpu);
}

//! Replacement of wglEnumGpuDevicesNV
inline BOOL WGLEnumGpuDevices(HGPUNV hGpu, UINT iIndex, PGPU_DEVICE pGpuDevice)
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglEnumGpuDevicesNV );
  return glContext()->m_glFunctions->wglEnumGpuDevicesNV(hGpu, iIndex, pGpuDevice);
}

//! Replacement for wglCreateAffinityDVNV
inline HDC  WGLCreateAffinityDC(const HGPUNV *pGpuList)
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglCreateAffinityDCNV );
  return glContext()->m_glFunctions->wglCreateAffinityDCNV(pGpuList);
}

//! Replacement for wglEnumGpusFromAffinityDCNV
inline BOOL WGLEnumGpusFromAffinityDC(HDC hAffinityDC, UINT iIndex, HGPUNV *hGpu)
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglEnumGpusFromAffinityDCNV );
  return glContext()->m_glFunctions->wglEnumGpusFromAffinityDCNV(hAffinityDC, iIndex, hGpu);
}

//! Replacement for wglDeleteDCNV
inline BOOL WGLDeleteDC(HDC hAffinityDC)
{
  NVSG_GL_STATS_FCT_COUNT();
  NVSG_ASSERT( glContext()->m_glFunctions->wglDeleteDCNV );
  return glContext()->m_glFunctions->wglDeleteDCNV(hAffinityDC);
}

#endif

#if defined(LINUX)
//! Replacement of glXCreateContext
inline GLXContext GLXCreateContext(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct)
{
  GLXContext ctx = glXCreateContext(dpy, vis, shareList, direct);
  GLXAttachContext(dpy, ctx);
  return ctx;
}

//! Replacement of glXDestroyContext
inline void GLXDestroyContext(Display *dpy, GLXContext ctx)
{
  NVSG_GL_STATS_FCT_COUNT();
  GLXDetachContext(ctx);
  return glXDestroyContext(dpy, ctx);
}

//! Replacement of glXGetCurrentContext
inline GLXContext GLXGetCurrentContext()
{
  ASSERT_GL_CURRENT_CONTEXT_CONFORMITY();
  return glXGetCurrentContext();
}

//! Replacement of glXGetCurrentDrawable
inline GLXDrawable GLXGetCurrentDrawable()
{
  NVSG_GL_STATS_FCT_COUNT();
  return glXGetCurrentDrawable();
}

#endif // LINUX

enum 
{
  GL_INVALID_BUFFER_TARGET = -1
  // specialized targets for vertex buffers 
, GL_VERTEX_BUFFER_INTERLEAVED_GENERIC = 0
, GL_VERTEX_BUFFER_FIRST = GL_VERTEX_BUFFER_INTERLEAVED_GENERIC
, GL_VERTEX_BUFFER_INTERLEAVED_V3F_N3F
, GL_VERTEX_BUFFER_INTERLEAVED_V3F_N3F_T2F
, GL_VERTEX_BUFFER_INTERLEAVED_V3F_N3F_VC4F_T2F
, GL_VERTEX_BUFFER_INTERLEAVED_V3F_N3F_T2F_TG3F_BN3F
, GL_VERTEX_BUFFER_LAST = GL_VERTEX_BUFFER_INTERLEAVED_V3F_N3F_T2F_TG3F_BN3F
, GL_INDEX_BUFFER_GENERIC
, GL_INDEX_BUFFER_FIRST = GL_INDEX_BUFFER_GENERIC
, GL_INDEX_BUFFER_UNSIGNED_SHORT
, GL_INDEX_BUFFER_UNSIGNED_INT
, GL_INDEX_BUFFER_LAST = GL_INDEX_BUFFER_UNSIGNED_INT
, GL_NUM_BUFFER_TARGETS
};

//! Generates a vertex buffer of requested size
/** The function creates a buffer object for vertex data and 
  * returns the buffer identifier in the buffer pointed to by \a buffer. */
NVSG_API void GLGenVertexBuffer(GLenum target, GLsizeiptr size, GLuint * buffer);

//! Generates an index buffer of requested size
/** The function creates a buffer object for index data and 
  * returns the buffer identifier in the buffer pointed to by \a buffer. */
NVSG_API void GLGenIndexBuffer(GLenum target, GLsizeiptr size, GLuint * buffer);

//! Deletes a vertex buffer
/** The function deletes the buffer object identified by \a buffer. 
  * The behavior is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenVertexBuffer. */
NVSG_API void GLDeleteVertexBuffer(GLuint buffer);

//! Deletes an index buffer
/** The function deletes the buffer object identified by \a buffer. 
* The behavior is undefined if an invalid identifier is passed through \a buffer,
* that is - an identifier that was not returned by GLGenIndexBuffer. */
NVSG_API void GLDeleteIndexBuffer(GLuint buffer);

//! Binds the buffer name associated with the identifier \a buffer to the GL_ARRAY_ELEMENT target.
/** The behavior is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenVertexBuffer. */
NVSG_API void GLBindVertexBuffer(GLuint buffer);

//! Binds the buffer name associated with the identifier \a buffer to the GL_ELEMENT_ARRAY_ELEMENT target.
/** The behavior is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenIndexBuffer. */
NVSG_API void GLBindIndexBuffer(GLuint buffer);

//! Uploads all pending buffers 
/** The function uploads the data copied to not yet uploaded vertex or index buffers 
  * to Vertex Buffer Objects (VBO) for fast rendering. 
  *
  * \note This function must be called immediately before rendering if vertex or index 
  * buffers are used for caching strategies. Omitting this call certainly results in
  * undefined runtime behavior! */
NVSG_API void GLUploadBuffers();

//! Maps the vertex buffer identified by \a buffer into process memory for write-only access.
/** The returned pointer can be used to write vertex data to the vertex buffer.
  *
  * Attempts to read back data from the mapped buffer might result in undefined behavior.
  * Writing more bytes to the buffer than it was specified at buffer creation, results in undefined behavior.
  * The behavior also is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenVertexBuffer.
  *
  * After data has been written to the buffer, and before the data will be dereferenced by any GL commands,
  * the mapping must be relinquished by calling GLUnmapVertexData.
  *
  * \returns A pointer to the vertex buffer if the memory could be mapped into process memory.
  * Otherwise a null pointer is returned. */
NVSG_API GLvoid * GLMapVertexBuffer(GLuint buffer);

//! Maps the index buffer identified by \a buffer into process memory for write-only access.
/** The returned pointer can be used to write index data to the vertex buffer.
  *
  * Attempts to read back data from the mapped buffer might result in undefined behavior.
  * Writing more bytes to the buffer than it was specified at buffer creation, results in undefined behavior.
  * The behavior also is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenVertexBuffer.
  *
  * After data has been written to the buffer, and before the data will be dereferenced by any GL commands,
  * the mapping must be relinquished by calling GLUnmapIndexData.
  *
  * \returns A pointer to the index buffer if the memory could be mapped into process memory.
  * Otherwise a null pointer is returned. */
NVSG_API GLvoid * GLMapIndexBuffer(GLuint buffer);

//! Relinquishes a vertex buffer mapping
/** The function relinquishes the mapping for the buffer identified by \a buffer.
  * The behavior is undefined if a mapping for the specified buffer was not initiated
  * before by a call to GLMapVertexBuffer.
  * The behavior also is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenVertexBuffer. 
  * \returns The function returns GL_TRUE unless data values in the data store of the buffer
  * have become corrupted during the period that the buffer was mapped. */
NVSG_API GLboolean GLUnmapVertexBuffer(GLuint buffer);

//! Relinquishes an index buffer mapping
/** The function relinquishes the mapping for the buffer identified by \a buffer.
  * The behavior is undefined if a mapping for the specified buffer was not initiated
  * before by a call to GLMapIndexBuffer.
  * The behavior also is undefined if an invalid identifier is passed through \a buffer,
  * that is - an identifier that was not returned by GLGenIndexBuffer. 
  * \returns The function returns GL_TRUE unless data values in the data store of the buffer
  * have become corrupted during the period that the buffer was mapped. */
NVSG_API GLboolean GLUnmapIndexBuffer(GLuint buffer);

//! Returns the start offset for the specified buffer
/** Vertex buffers might be collected in a higher-level buffer object.
  * This function returns the start offset inside the higher-level buffer object.
  * \returns The start offset inside the higher-level vertex buffer object. */
NVSG_API GLsizeiptr GLGetVertexBufferOffset(GLuint buffer);

//! Returns the start offset for the specified buffer
/** Index buffers might be collected in a higher-level buffer object.
  * This function returns the start offset inside the higher-level buffer object.
  * \returns The start offset inside the higher-level vertex buffer object. */
NVSG_API GLsizeiptr GLGetIndexBufferOffset(GLuint buffer);

//! Returns whether \a buffer identifies a valid vertex buffer for the current context.
/** \returns GL_TRUE if \a buffer identifies a valid vertex buffer for the current context.
  * GL_FALSE otherwise. */
NVSG_API GLboolean GLIsVertexBuffer(GLuint buffer);

//! Returns whether \a buffer identifies a valid index buffer for the current context.
/** \returns GL_TRUE if \a buffer identifies a valid index buffer for the current context.
  * GL_FALSE otherwise. */
NVSG_API GLboolean GLIsIndexBuffer(GLuint buffer);

//! Returns the base buffer offset.
/** The base buffer offset is NVSG module dependent. 
  * For a given NVSG module it is non-varying and equal for all vertex and index buffers. 
  * \returns The base buffer offset used with the actual NVSG module. */
NVSG_API GLsizeiptr GLGetBaseBufferOffset();

#if !defined(DOXYGEN_IGNORE)
inline void _GLDisable( GLenum cap ) 
{
  NVSG_GL_STATS_STATE( cap, false );
  glDisable( cap );
  ASSERT_GL_NO_ERROR;
}

inline void _GLDisableClientState( GLenum array ) 
{
  NVSG_GL_STATS_CLIENT_STATE( array, false );
  glDisableClientState( array );
  ASSERT_GL_NO_ERROR;
}

inline void _GLEnable( GLenum cap ) 
{
  NVSG_GL_STATS_STATE( cap, true );
  glEnable( cap );
  ASSERT_GL_NO_ERROR;
}

inline void _GLEnableClientState( GLenum array ) 
{
  NVSG_GL_STATS_CLIENT_STATE( array, true );
  glEnableClientState( array );
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLFogf( GLenum pname, GLfloat param )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFogf( pname, param );
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLFogfv( GLenum pname, const GLfloat * params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFogfv( pname, params );
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLFogi( GLenum pname, GLint param )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFogi(pname, param);
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLFogiv( GLenum pname, const GLint * params )
{
  NVSG_GL_STATS_FCT_COUNT();
  glFogiv(pname, params);
  ASSERT_GL_NO_ERROR;
}

inline GLboolean _GLIsEnabled( GLenum cap ) 
{
  NVSG_GL_STATS_FCT_COUNT();
  GLboolean isEnabled = glIsEnabled( cap );
  ASSERT_GL_NO_ERROR;
  return( isEnabled );
}

inline  void  _GLLightf( GLenum light, GLenum pname, GLfloat param ) 
{ 
  NVSG_GL_STATS_LIGHTF( light, pname, param );
  glLightf( light, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLightfv( GLenum light, GLenum pname, const GLfloat *params ) 
{
  NVSG_GL_STATS_LIGHTFV( light, pname, params );
  glLightfv( light, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLighti( GLenum light, GLenum pname, GLint param ) 
{ 
  NVSG_GL_STATS_LIGHTI( light, pname, param );
  glLighti( light, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLightiv( GLenum light, GLenum pname, GLint *params ) 
{ 
  NVSG_GL_STATS_LIGHTIV( light, pname, params );
  glLightiv( light, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLightModelf( GLenum pname, GLfloat param )
{
  NVSG_GL_STATS_LIGHT_MODELF( pname, param );
  glLightModelf( pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLightModelfv( GLenum pname, const GLfloat *params )
{
  NVSG_GL_STATS_LIGHT_MODELFV( pname, params );
  glLightModelfv( pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLightModeli( GLenum pname, GLint param )
{
  NVSG_GL_STATS_LIGHT_MODELI( pname, param );
  glLightModeli( pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLLightModeliv( GLenum pname, const GLint *params )
{
  NVSG_GL_STATS_LIGHT_MODELIV( pname, params );
  glLightModeliv( pname, params );
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLMaterialf( GLenum face, GLenum pname, GLfloat param ) 
{
  NVSG_GL_STATS_MATERIALF( face, pname, param );
  glMaterialf( face, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLMaterialfv( GLenum face, GLenum pname, const GLfloat *params ) 
{
  NVSG_GL_STATS_MATERIALFV( face, pname, params );
  glMaterialfv( face, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLMateriali( GLenum face, GLenum pname, GLint param ) 
{
  NVSG_GL_STATS_MATERIALI( face, pname, param );
  glMateriali( face, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLMaterialiv( GLenum face, GLenum pname, const GLint *params )
{
  NVSG_GL_STATS_MATERIALIV( face, pname, params );
  glMaterialiv( face, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLPolygonMode( GLenum face, GLenum mode )
{
  NVSG_GL_STATS_POLYGON_MODE( face, mode );
  glPolygonMode( face, mode );
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLTexEnvf( GLenum target, GLenum pname, GLfloat param )
{
  NVSG_GL_STATS_TEX_ENVF( target, pname, param );
  glTexEnvf( target, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLTexEnvfv( GLenum target, GLenum pname, const GLfloat *params )
{
  NVSG_GL_STATS_TEX_ENVFV( target, pname, params );
  glTexEnvfv( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLTexEnvi( GLenum target, GLenum pname, GLint param )
{
  NVSG_GL_STATS_TEX_ENVI( target, pname, param );
  glTexEnvi( target, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLTexEnviv( GLenum target, GLenum pname, const GLint *params )
{
  NVSG_GL_STATS_TEX_ENVIV( target, pname, params );
  glTexEnviv( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline  void  _GLTexParameterf( GLenum target, GLenum pname, GLfloat param )
{
  NVSG_GL_STATS_TEX_PARAMETERF( target, pname, param );
  glTexParameterf( target, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLTexParameterfv( GLenum target, GLenum pname, const GLfloat *params )
{
  NVSG_GL_STATS_TEX_PARAMETERFV( target, pname, params );
  glTexParameterfv( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

inline void _GLTexParameteri( GLenum target, GLenum pname, GLint param )
{
  NVSG_GL_STATS_TEX_PARAMETERI( target, pname, param );
  glTexParameteri( target, pname, param );
  ASSERT_GL_NO_ERROR;
}

inline void _GLTexParameteriv( GLenum target, GLenum pname, const GLint *params )
{
  NVSG_GL_STATS_TEX_PARAMETERIV( target, pname, params );
  glTexParameteriv( target, pname, params );
  ASSERT_GL_NO_ERROR;
}

#if defined( GL_CACHING )

inline  void  _GLUpdateActiveTexture( void )
{
  GLContext * ctx = glContext();
  if ( ctx->m_textureAttributes.m_toActivate != ctx->m_textureAttributes.m_active )
  {
#if !defined(NDEBUG)
    GLint at;
    glGetIntegerv( GL_ACTIVE_TEXTURE, &at );
    NVSG_ASSERT( at == GL_TEXTURE0 + ctx->m_textureAttributes.m_active );
#endif
    ctx->m_textureAttributes.m_active = ctx->m_textureAttributes.m_toActivate;
    NVSG_GL_STATS_ACTIVE_TEXTURE( GL_TEXTURE0 + ctx->m_textureAttributes.m_active );
    ctx->m_glFunctions->glActiveTexture( GL_TEXTURE0 + ctx->m_textureAttributes.m_active );
  }
}

inline  void  _GLUpdateClientActiveTexture( void )
{
  GLContext * ctx = glContext();
  if ( ctx->m_vertexArrayAttributes.m_clientTextureToActivate != ctx->m_vertexArrayAttributes.m_clientTextureActive )
  {
#if !defined(NDEBUG)
    GLint cat;
    glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE, &cat );
    NVSG_ASSERT( cat == GL_TEXTURE0 + ctx->m_vertexArrayAttributes.m_clientTextureActive );
#endif
    ctx->m_vertexArrayAttributes.m_clientTextureActive = ctx->m_vertexArrayAttributes.m_clientTextureToActivate;
    NVSG_GL_STATS_CLIENT_ACTIVE_TEXTURE( GL_TEXTURE0 + ctx->m_vertexArrayAttributes.m_clientTextureActive );
    ctx->m_glFunctions->glClientActiveTexture( GL_TEXTURE0 + ctx->m_vertexArrayAttributes.m_clientTextureActive );
  }
}

#endif

#endif

} // namespace nvgl
