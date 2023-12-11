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
/** @file */

#include "nvsgcommon.h"

#if ! defined( DOXYGEN_IGNORE )   // no need to document this
//#if !defined(NDEBUG)
//# define GL_STATISTICS
//#endif

#if defined( GL_STATISTICS )

#include "nvgl/nvgl.h"
#include "nvmath/Mat44f.h"
#include "nvmath/Vec3f.h"
#include "nvmath/Vec4f.h"
#include <stack>

# define  NVSG_GL_STATS_FCT_COUNT()                                      glContext()->m_glStats->m_fctCount[__FUNCTION__]++
# define  NVSG_GL_STATS_ACTIVE_TEXTURE( texture )                        glContext()->m_glStats->activeTexture( __FUNCTION__, texture )
# define  NVSG_GL_STATS_BIND_BUFFER( target, id )                        glContext()->m_glStats->bindBuffer( __FUNCTION__, target, id )
# define  NVSG_GL_STATS_BIND_TEXTURE( target, texture )                  glContext()->m_glStats->bindTexture( __FUNCTION__, target, texture )
# define  NVSG_GL_STATS_BLEND_FUNC( sfactor, dfactor )                   glContext()->m_glStats->blendFunc( __FUNCTION__, sfactor, dfactor )
# define  NVSG_GL_STATS_BUFFER_DATA( target, size, data, usage )         glContext()->m_glStats->bufferData( __FUNCTION__, target, size, data, usage )
# define  NVSG_GL_STATS_CALL_LIST( list )                                glContext()->m_glStats->callList( __FUNCTION__, list )
# define  NVSG_GL_STATS_CLEAR( mask )                                    glContext()->m_glStats->clear( __FUNCTION__, mask )
# define  NVSG_GL_STATS_CLEAR_COLOR( red, green, blue, alpha )           glContext()->m_glStats->clearColor( __FUNCTION__, red, green, blue, alpha );
# define  NVSG_GL_STATS_CLIENT_ACTIVE_TEXTURE( texture )                 glContext()->m_glStats->clientActiveTexture( __FUNCTION__, texture )
# define  NVSG_GL_STATS_CLIENT_STATE( array, on )                        glContext()->m_glStats->clientState( __FUNCTION__, array, on )
# define  NVSG_GL_STATS_COLOR3B( red, green, blue )                      glContext()->m_glStats->color3b( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3BV( v )                                    glContext()->m_glStats->color3bv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3D( red, green, blue )                      glContext()->m_glStats->color3d( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3DV( v )                                    glContext()->m_glStats->color3dv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3F( red, green, blue )                      glContext()->m_glStats->color3f( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3FV( v )                                    glContext()->m_glStats->color3fv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3I( red, green, blue )                      glContext()->m_glStats->color3i( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3IV( v )                                    glContext()->m_glStats->color3iv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3S( red, green, blue )                      glContext()->m_glStats->color3s( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3SV( v )                                    glContext()->m_glStats->color3sv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3UB( red, green, blue )                     glContext()->m_glStats->color3ub( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3UBV( v )                                   glContext()->m_glStats->color3ubv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3UI( red, green, blue )                     glContext()->m_glStats->color3ui( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3UIV( v )                                   glContext()->m_glStats->color3uiv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR3US( red, green, blue )                     glContext()->m_glStats->color3us( __FUNCTION__, red, green, blue )
# define  NVSG_GL_STATS_COLOR3USV( v )                                   glContext()->m_glStats->color3usv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4B( red, green, blue, alpha )               glContext()->m_glStats->color4b( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4BV( v )                                    glContext()->m_glStats->color4bv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4D( red, green, blue, alpha )               glContext()->m_glStats->color4d( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4DV( v )                                    glContext()->m_glStats->color4dv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4F( red, green, blue, alpha )               glContext()->m_glStats->color4f( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4FV( v )                                    glContext()->m_glStats->color4fv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4I( red, green, blue, alpha )               glContext()->m_glStats->color4i( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4IV( v )                                    glContext()->m_glStats->color4iv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4S( red, green, blue, alpha )               glContext()->m_glStats->color4s( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4SV( v )                                    glContext()->m_glStats->color4sv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4UB( red, green, blue, alpha )              glContext()->m_glStats->color4ub( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4UBV( v )                                   glContext()->m_glStats->color4ubv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4UI( red, green, blue, alpha )              glContext()->m_glStats->color4ui( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4UIV( v )                                   glContext()->m_glStats->color4uiv( __FUNCTION__, v )
# define  NVSG_GL_STATS_COLOR4US( red, green, blue, alpha )              glContext()->m_glStats->color4us( __FUNCTION__, red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4USV( v )                                   glContext()->m_glStats->color4usv( __FUNCTION__, v )
# define  NVSG_GL_STATS_DELETE_BUFFERS( n, buffers )                     glContext()->m_glStats->deleteBuffers( __FUNCTION__, n, buffers )
# define  NVSG_GL_STATS_DELETE_LISTS( list, range )                      glContext()->m_glStats->deleteLists( __FUNCTION__, list, range )
# define  NVSG_GL_STATS_DELETE_TEXTURES( n, textures )                   glContext()->m_glStats->deleteTextures( __FUNCTION__, n, textures )
# define  NVSG_GL_STATS_DEPTH_FUNC( func )                               glContext()->m_glStats->depthFunc( __FUNCTION__, func )
# define  NVSG_GL_STATS_DEPTH_MASK( flag )                               glContext()->m_glStats->depthMask( __FUNCTION__, flag )
# define  NVSG_GL_STATS_DRAW_ELEMENTS( mode, count, type, indices )      glContext()->m_glStats->drawElements( __FUNCTION__, mode, count, type, indices )
# define  NVSG_GL_STATS_DRAW_RANGE_ELEMENTS( mode, start, end, count, type, indices )  \
                                                                         glContext()->m_glStats->drawRangeElements( __FUNCTION__, mode, start, end, count, type, indices )
# define  NVSG_GL_STATS_END_LIST()                                       glContext()->m_glStats->endList( __FUNCTION__ );
# define  NVSG_GL_STATS_FRUSTUM( left, right, bottom, top, zNear, zFar ) glContext()->m_glStats->frustum( __FUNCTION__, left, right, bottom, top, zNear, zFar )
# define  NVSG_GL_STATS_GEN_BUFFERS( n, buffers )                        glContext()->m_glStats->genBuffers( __FUNCTION__, n, buffers )
# define  NVSG_GL_STATS_GEN_LISTS( range )                               glContext()->m_glStats->genLists( __FUNCTION__, range )
# define  NVSG_GL_STATS_GEN_TEXTURES( n, textures )                      glContext()->m_glStats->genTextures( __FUNCTION__, n, textures )
# define  NVSG_GL_STATS_GET_INTEGERV( pname, params )                    glContext()->m_glStats->getIntegerv( __FUNCTION__, pname, params )
# define  NVSG_GL_STATS_IS_TEXTURE( texture )                            glContext()->m_glStats->isTexture( __FUNCTION__, texture )
# define  NVSG_GL_STATS_LIGHTF( light, pname, param )                    glContext()->m_glStats->lightf( __FUNCTION__, light, pname, param )
# define  NVSG_GL_STATS_LIGHTFV( light, pname, params )                  glContext()->m_glStats->lightfv( __FUNCTION__, light, pname, params )
# define  NVSG_GL_STATS_LIGHTI( light, pname, param )                    glContext()->m_glStats->lighti( __FUNCTION__, light, pname, param )
# define  NVSG_GL_STATS_LIGHTIV( light, pname, params )                  glContext()->m_glStats->lightiv( __FUNCTION__, light, pname, params )
# define  NVSG_GL_STATS_LIGHT_MODELF( pname, param )                     glContext()->m_glStats->lightModelf( __FUNCTION__, pname, param )
# define  NVSG_GL_STATS_LIGHT_MODELFV( pname, params )                   glContext()->m_glStats->lightModelfv( __FUNCTION__, pname, params )
# define  NVSG_GL_STATS_LIGHT_MODELI( pname, param )                     glContext()->m_glStats->lightModeli( __FUNCTION__, pname, param )
# define  NVSG_GL_STATS_LIGHT_MODELIV( pname, params )                   glContext()->m_glStats->lightModeliv( __FUNCTION__, pname, params )
# define  NVSG_GL_STATS_LINE_WIDTH( width )                              glContext()->m_glStats->lineWidth( __FUNCTION__, width )
# define  NVSG_GL_STATS_LOAD_IDENTITY()                                  glContext()->m_glStats->loadIdentity( __FUNCTION__ )
# define  NVSG_GL_STATS_LOAD_MATRIXD( m )                                glContext()->m_glStats->loadMatrixd( __FUNCTION__, m )
# define  NVSG_GL_STATS_LOAD_MATRIXF( m )                                glContext()->m_glStats->loadMatrixf( __FUNCTION__, m )
# define  NVSG_GL_STATS_MATERIALF( face, pname, param )                  glContext()->m_glStats->materialf( __FUNCTION__, face, pname, param )
# define  NVSG_GL_STATS_MATERIALFV( face, pname, params )                glContext()->m_glStats->materialfv( __FUNCTION__, face, pname, params )
# define  NVSG_GL_STATS_MATERIALI( face, pname, param )                  glContext()->m_glStats->materiali( __FUNCTION__, face, pname, param )
# define  NVSG_GL_STATS_MATERIALIV( face, pname, params )                glContext()->m_glStats->materialiv( __FUNCTION__, face, pname, params )
# define  NVSG_GL_STATS_MATRIX_MODE( mode )                              glContext()->m_glStats->matrixMode( __FUNCTION__, mode )
# define  NVSG_GL_STATS_MULT_MATRIXD( m )                                glContext()->m_glStats->multMatrixd( __FUNCTION__, m )
# define  NVSG_GL_STATS_MULT_MATRIXF( m )                                glContext()->m_glStats->multMatrixf( __FUNCTION__, m )
# define  NVSG_GL_STATS_NEW_LIST( list, mode )                           glContext()->m_glStats->newList( __FUNCTION__, list, mode )
# define  NVSG_GL_STATS_NORMAL_POINTER( type, stride, pointer )          glContext()->m_glStats->normalPointer( __FUNCTION__, type, stride, pointer )
# define  NVSG_GL_STATS_ORTHO( left, right, bottom, top, zNear, zFar )   glContext()->m_glStats->ortho( __FUNCTION__, left, right, bottom, top, zNear, zFar )
# define  NVSG_GL_STATS_POINT_SIZE( size )                               glContext()->m_glStats->pointSize( __FUNCTION__, size )
# define  NVSG_GL_STATS_POLYGON_MODE( face, mode )                       glContext()->m_glStats->polygonMode( __FUNCTION__, face, mode )
# define  NVSG_GL_STATS_POLYGON_OFFSET( factor, units )                  glContext()->m_glStats->polygonOffset( __FUNCTION__, factor, units )
# define  NVSG_GL_STATS_POP_ATTRIB()                                     glContext()->m_glStats->popAttrib( __FUNCTION__ )
# define  NVSG_GL_STATS_POP_MATRIX()                                     glContext()->m_glStats->popMatrix( __FUNCTION__ )
# define  NVSG_GL_STATS_PRIMITIVE_RESTART_INDEX( index )                 glContext()->m_glStats->primitiveRestartIndex( __FUNCTION__, index )
# define  NVSG_GL_STATS_PUSH_ATTRIB( mask )                              glContext()->m_glStats->pushAttrib( __FUNCTION__, mask )
# define  NVSG_GL_STATS_PUSH_MATRIX()                                    glContext()->m_glStats->pushMatrix( __FUNCTION__ )
# define  NVSG_GL_STATS_ROTATED( angle, x, y, z )                        glContext()->m_glStats->rotated( __FUNCTION__, angle, x, y, z )
# define  NVSG_GL_STATS_ROTATEF( angle, x, y, z )                        glContext()->m_glStats->rotatef( __FUNCTION__, angle, x, y, z )
# define  NVSG_GL_STATS_SCALED( x, y, z )                                glContext()->m_glStats->scaled( __FUNCTION__, x, y, z )
# define  NVSG_GL_STATS_SCALEF( x, y, z )                                glContext()->m_glStats->scalef( __FUNCTION__, x, y, z )
# define  NVSG_GL_STATS_STATE( cap, on )                                 glContext()->m_glStats->state( __FUNCTION__, cap, on )
# define  NVSG_GL_STATS_TEX_COORD_POINTER( size, type, stride, pointer ) glContext()->m_glStats->texCoordPointer( __FUNCTION__, size, type, stride, pointer )
# define  NVSG_GL_STATS_TEX_ENVF( target, pname, param )                 glContext()->m_glStats->texEnvf( __FUNCTION__, target, pname, param )
# define  NVSG_GL_STATS_TEX_ENVFV( target, pname, params )               glContext()->m_glStats->texEnvfv( __FUNCTION__, target, pname, params )
# define  NVSG_GL_STATS_TEX_ENVI( target, pname, param )                 glContext()->m_glStats->texEnvi( __FUNCTION__, target, pname, param )
# define  NVSG_GL_STATS_TEX_ENVIV( target, pname, params )               glContext()->m_glStats->texEnviv( __FUNCTION__, target, pname, params )
# define  NVSG_GL_STATS_TEX_IMAGE2D( target, level, internalformat, width, height, border, format, type, pixels )  \
                                                                         glContext()->m_glStats->texImage2d( __FUNCTION__, target, level, internalformat, width, height, border, format, type, pixel
s )
# define  NVSG_GL_STATS_TEX_PARAMETERF( target, pname, param )           glContext()->m_glStats->texParameterf( __FUNCTION__, target, pname, param )
# define  NVSG_GL_STATS_TEX_PARAMETERFV( target, pname, params )         glContext()->m_glStats->texParameterfv( __FUNCTION__, target, pname, params )
# define  NVSG_GL_STATS_TEX_PARAMETERI( target, pname, param )           glContext()->m_glStats->texParameteri( __FUNCTION__, target, pname, param )
# define  NVSG_GL_STATS_TEX_PARAMETERIV( target, pname, params )         glContext()->m_glStats->texParameteriv( __FUNCTION__, target, pname, params )
# define  NVSG_GL_STATS_TRANSLATED( x, y, z )                            glContext()->m_glStats->translated( __FUNCTION__, x, y, z )
# define  NVSG_GL_STATS_TRANSLATEF( x, y, z )                            glContext()->m_glStats->translatef( __FUNCTION__, x, y, z )
# define  NVSG_GL_STATS_VERTEX_POINTER( size, type, stride, pointer )    glContext()->m_glStats->vertexPointer( __FUNCTION__, size, type, stride, pointer )
# define  NVSG_GL_STATS_VIEWPORT( x, y, width, height )                  glContext()->m_glStats->viewport( __FUNCTION__, x, y, width, height )


class GLStatsEnable
{
public:
  GLStatsEnable( void )
    : enableState(false)
    , enableCalls(0)
    , disableCalls(0)
    , redundantEnables(0)
    , redundantDisables(0)
  {
  }

public:
  bool    enableState;
  size_t  enableCalls;
  size_t  disableCalls;
  size_t  redundantEnables;
  size_t  redundantDisables;
};

class GLStatsCount
{
public:
  GLStatsCount( void )
    : calls(0)
    , redundantCalls(0)
  {
  }

public:
  size_t  calls;
  size_t  redundantCalls;
};

class GLStatsFloat : public GLStatsCount
{
public:
  GLStatsFloat( void )
    : value(-1.0f)
  {
  }

public:
  float   value;
};

class GLStatsVec3f : public GLStatsCount
{
public:
  GLStatsVec3f( void )
    : value( -1.0f, -1.0f, -1.0f )
  {
  }

public:
  nvmath::Vec3f value;
};

class GLStatsVec4f : public GLStatsCount
{
public:
  GLStatsVec4f( void )
    : value( -1.0f, -1.0f, -1.0f, -1.0f )
  {
  }

public:
  nvmath::Vec4f value;
};

class GLStatistics
{
public:
  GLStatistics( void );

  void  dump( void );
  void  dumpHistogram( FILE *fh, const std::map<size_t,size_t> &hist, char *prefix );
  void  activeTexture( const char *fctName, GLenum texture );
  void  bindBuffer( const char *fctName, GLenum target, GLuint id );
  void  bindTexture( const char *fctName, GLenum target, GLuint texture );
  void  blendFunc( const char *fctName, GLenum sfactor, GLenum dfactor );
  void  bufferData( const char *fctName, GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage );
  void  callList( const char *fctName, GLuint list );
  void  clear( const char *fctName, GLbitfield mask );
  void  clearColor( const char *fctName, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
  void  clientActiveTexture( const char *fctName, GLenum texture );
  void  clientState( const char *fctName, GLenum array, bool enable );
  void  color3b( const char *fctName, GLbyte red, GLbyte green, GLbyte blue );
  void  color3bv( const char *fctName, const GLbyte *v );
  void  color3d( const char *fctName, GLdouble red, GLdouble green, GLdouble blue );
  void  color3dv( const char *fctName, const GLdouble *v );
  void  color3f( const char *fctName, GLfloat red, GLfloat green, GLfloat blue );
  void  color3fv( const char *fctName, const GLfloat *v );
  void  color3i( const char *fctName, GLint red, GLint green, GLint blue );
  void  color3iv( const char *fctName, const GLint *v );
  void  color3s( const char *fctName, GLshort red, GLshort green, GLshort blue );
  void  color3sv( const char *fctName, const GLshort *v );
  void  color3ub( const char *fctName, GLubyte red, GLubyte green, GLubyte blue );
  void  color3ubv( const char *fctName, const GLubyte *v );
  void  color3ui( const char *fctName, GLuint red, GLuint green, GLuint blue );
  void  color3uiv( const char *fctName, const GLuint *v );
  void  color3us( const char *fctName, GLushort red, GLushort green, GLushort blue );
  void  color3usv( const char *fctName, const GLushort *v );
  void  color4b( const char *fctName, GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha );
  void  color4bv( const char *fctName, const GLbyte *v );
  void  color4d( const char *fctName, GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha );
  void  color4dv( const char *fctName, const GLdouble *v );
  void  color4f( const char *fctName, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
  void  color4fv( const char *fctName, const GLfloat *v );
  void  color4i( const char *fctName, GLint red, GLint green, GLint blue, GLint alpha );
  void  color4iv( const char *fctName, const GLint *v );
  void  color4s( const char *fctName, GLshort red, GLshort green, GLshort blue, GLshort alpha );
  void  color4sv( const char *fctName, const GLshort *v );
  void  color4ub( const char *fctName, GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha );
  void  color4ubv( const char *fctName, const GLubyte *v );
  void  color4ui( const char *fctName, GLuint red, GLuint green, GLuint blue, GLuint alpha );
  void  color4uiv( const char *fctName, const GLuint *v );
  void  color4us( const char *fctName, GLushort red, GLushort green, GLushort blue, GLushort alpha );
  void  color4usv( const char *fctName, const GLushort *v );
  void  deleteBuffers( const char *fctName, GLsizei n, const GLuint *buffers );
  void  deleteLists( const char *fctName, GLuint list, GLsizei range );
  void  deleteTextures( const char *fctName, GLsizei n, const GLuint *textures );
  void  depthFunc( const char *fctName, GLenum func );
  void  depthMask( const char *fctName, GLboolean flag );
  void  drawElements( const char *fctName, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices );
  void  drawRangeElements( const char *fctName, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
  void  endList( const char *fctName );
  void  frustum( const char *fctName, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
  void  initLightDefault( GLenum light, GLenum pname );
  void  initLightModelDefault( GLenum pname );
  void  initMaterialDefault( GLenum face, GLenum pname );
  void  initMatrixDefault( void );
  void  initPolygonMode( GLenum face );
  void  initStateDefault( GLenum cap );
  void  initTexEnv( GLenum target, GLenum pname );
  void  initTexParameter( GLenum target, GLenum pname );
  void  genBuffers( const char *fctName, GLsizei n, GLuint *buffers );
  void  genLists( const char *fctName, GLsizei range );
  void  genTextures( const char *fctName, GLsizei n, GLuint *textures );
  void  getIntegerv( const char *fctName, GLenum pname, GLint *params );
  void  isTexture( const char *fctName, GLuint texture );
  void  lightf( const char *fctName, GLenum light, GLenum pname, GLfloat param );
  void  lightfv( const char *fctName, GLenum light, GLenum pname, const GLfloat *params );
  void  lighti( const char *fctName, GLenum light, GLenum pname, GLint param );
  void  lightiv( const char *fctName, GLenum light, GLenum pname, const GLint *params );
  void  lightModelf( const char *fctName, GLenum pname, GLfloat param );
  void  lightModelfv( const char *fctName, GLenum pname, const GLfloat *params );
  void  lightModeli( const char *fctName, GLenum pname, GLint param );
  void  lightModeliv( const char *fctName, GLenum pname, const GLint *params );
  void  lineWidth( const char *fctName, GLfloat width );
  void  loadIdentity( const char *fctName );
  void  loadMatrixd( const char *fctName, const GLdouble *m );
  void  loadMatrixf( const char *fctName, const GLfloat *m );
  void  materialf( const char *fctName, GLenum face, GLenum pname, GLfloat param );
  void  materialfv( const char *fctName, GLenum face, GLenum pname, const GLfloat *params );
  void  materiali( const char *fctName, GLenum face, GLenum pname, GLint param );
  void  materialiv( const char *fctName, GLenum face, GLenum pname, const GLint *params );
  void  matrixMode( const char *fctName, GLenum mode );
  void  multMatrixd( const char *fctName, const GLdouble *m );
  void  multMatrixf( const char *fctName, const GLfloat *m );
  void  newList( const char *fctName, GLuint list, GLenum mode );
  void  normalPointer( const char *fctName, GLenum type, GLsizei stride, const GLvoid *pointer );
  void  ortho( const char *fctName, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear
             , GLdouble zFar );
  void  pointSize( const char *fctName, GLfloat size );
  void  polygonMode( const char *fctName, GLenum face, GLenum mode );
  void  polygonOffset( const char *fctName, GLfloat factor, GLfloat units );
  void  popAttrib( const char *fctName );
  void  popMatrix( const char *fctName );
  void  primitiveRestartIndex( const char *fctName, GLuint index );
  void  pushAttrib( const char *fctName, GLbitfield mask );
  void  pushMatrix( const char *fctName );
  void  rotated( const char *fctName , GLdouble angle, GLdouble x, GLdouble y, GLdouble z );
  void  rotatef( const char *fctName , GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
  void  scaled( const char *fctName, GLdouble x, GLdouble y, GLdouble z );
  void  scalef( const char *fctName, GLfloat x, GLfloat y, GLfloat z );
  void  state( const char *fctName, GLenum cap, bool on );
  void  texCoordPointer( const char *fctName, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  void  texEnvf( const char *fctName, GLenum target, GLenum pname, GLfloat param );
  void  texEnvfv( const char *fctName, GLenum target, GLenum pname, const GLfloat *params );
  void  texEnvi( const char *fctName, GLenum target, GLenum pname, GLint param );
  void  texEnviv( const char *fctName, GLenum target, GLenum pname, const GLint *params );
  void  texImage2d( const char *fctName, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height
                  , GLint border, GLenum format, GLenum type, const GLvoid *pixels );
  void  texParameterf( const char *fctName, GLenum target, GLenum pname, GLfloat param );
  void  texParameterfv( const char *fctName, GLenum target, GLenum pname, const GLfloat *params );
  void  texParameteri( const char *fctName, GLenum target, GLenum pname, GLint param );
  void  texParameteriv( const char *fctName, GLenum target, GLenum pname, const GLint *params );
  void  translated( const char *fctName, GLdouble x, GLdouble y, GLdouble z );
  void  translatef( const char *fctName, GLfloat x, GLfloat y, GLfloat z );
  void  vertexPointer( const char *fctName, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
  void  viewport( const char *fctName, GLint x, GLint y, GLsizei width, GLsizei height );

public:
  std::map<std::string,size_t>                        m_fctCount;
  std::map<std::string,GLStatsCount>                  m_fctStats;
  GLenum                                              m_activeTexture;
  GLenum                                              m_blendFuncDFactor;
  GLenum                                              m_blendFuncSFactor;
  std::map<GLenum,GLuint>                             m_boundBuffer;
  std::vector<std::map<GLenum,GLuint> >               m_boundTexture;
  std::map<GLuint,std::vector<char> >                 m_bufferData;
  std::map<GLuint,GLenum>                             m_bufferDataUsage;
  nvmath::Vec4f                                       m_clearColor;
  GLenum                                              m_clientActiveTexture;
  nvmath::Vec4f                                       m_color;
  GLenum                                              m_depthFunc;
  GLboolean                                           m_depthMask;
  std::map<size_t,size_t>                             m_drawElementsHistogram;
  std::map<size_t,size_t>                             m_drawElementsIndexDistanceHistogram;
  std::map<GLenum,std::map<GLenum,GLfloat> >          m_lightf;
  std::map<GLenum,std::map<GLenum,nvmath::Vec3f> >    m_light3f;
  std::map<GLenum,std::map<GLenum,nvmath::Vec4f> >    m_light4f;
  std::map<GLenum,nvmath::Vec4f>                      m_lightModel4f;
  std::map<GLenum,GLint>                              m_lightModeli;
  GLfloat                                             m_lineWidth;
  std::map<GLenum,std::map<GLenum,GLfloat> >          m_materialf;
  std::map<GLenum,std::map<GLenum,nvmath::Vec4f> >    m_material4f;
  std::map<GLenum,std::stack<nvmath::Mat44f> >        m_matrixf;
  GLenum                                              m_matrixMode;
  GLfloat                                             m_pointSize;
  std::map<GLenum,GLenum>                             m_polygonMode;
  GLfloat                                             m_polygonOffsetFactor;
  GLfloat                                             m_polygonOffsetUnits;
  GLuint                                              m_primitiveRestartIndex;
  std::map<GLenum,bool>                               m_state;
  std::map<GLuint,std::map<GLint,GLint> >             m_texImage2dInternalFormat;
  std::map<GLuint,std::map<GLenum,GLint> >            m_texParameter;
  std::map<GLuint,nvmath::Vec4f>                      m_textureBorderColor;
  std::map<GLenum,bool>                               m_textureCoordArrayState;
  std::map<GLenum,nvmath::Vec4f>                      m_textureEnvColor;
  std::map<GLenum,GLint>                              m_textureEnvMode;
  std::map<GLuint,GLfloat>                            m_texturePriority;
  GLsizei                                             m_viewportHeight;
  GLsizei                                             m_viewportWidth;
  GLint                                               m_viewportX;
  GLint                                               m_viewportY;
};

#else

# define  NVSG_GL_STATS_FCT_COUNT()
# define  NVSG_GL_STATS_ACTIVE_TEXTURE( texture )
# define  NVSG_GL_STATS_BIND_BUFFER( target, id )
# define  NVSG_GL_STATS_BIND_TEXTURE( target, texture )
# define  NVSG_GL_STATS_BLEND_FUNC( sfactor, dfactor )
# define  NVSG_GL_STATS_BUFFER_DATA( target, size, data, usage )
# define  NVSG_GL_STATS_CALL_LIST( list )
# define  NVSG_GL_STATS_CLEAR( mask )
# define  NVSG_GL_STATS_CLEAR_COLOR( red, green, blue, alpha )
# define  NVSG_GL_STATS_CLIENT_ACTIVE_TEXTURE( texture )
# define  NVSG_GL_STATS_CLIENT_STATE( array, on )
# define  NVSG_GL_STATS_COLOR3B( red, green, blue )
# define  NVSG_GL_STATS_COLOR3BV( v )
# define  NVSG_GL_STATS_COLOR3D( red, green, blue )
# define  NVSG_GL_STATS_COLOR3DV( v )
# define  NVSG_GL_STATS_COLOR3F( red, green, blue )
# define  NVSG_GL_STATS_COLOR3FV( v )
# define  NVSG_GL_STATS_COLOR3I( red, green, blue )
# define  NVSG_GL_STATS_COLOR3IV( v )
# define  NVSG_GL_STATS_COLOR3S( red, green, blue )
# define  NVSG_GL_STATS_COLOR3SV( v )
# define  NVSG_GL_STATS_COLOR3UB( red, green, blue )
# define  NVSG_GL_STATS_COLOR3UBV( v )
# define  NVSG_GL_STATS_COLOR3UI( red, green, blue )
# define  NVSG_GL_STATS_COLOR3UIV( v )
# define  NVSG_GL_STATS_COLOR3US( red, green, blue )
# define  NVSG_GL_STATS_COLOR3USV( v )
# define  NVSG_GL_STATS_COLOR4B( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4BV( v )
# define  NVSG_GL_STATS_COLOR4D( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4DV( v )
# define  NVSG_GL_STATS_COLOR4F( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4FV( v )
# define  NVSG_GL_STATS_COLOR4I( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4IV( v )
# define  NVSG_GL_STATS_COLOR4S( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4SV( v )
# define  NVSG_GL_STATS_COLOR4UB( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4UBV( v )
# define  NVSG_GL_STATS_COLOR4UI( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4UIV( v )
# define  NVSG_GL_STATS_COLOR4US( red, green, blue, alpha )
# define  NVSG_GL_STATS_COLOR4USV( v )
# define  NVSG_GL_STATS_DELETE_BUFFERS( n, buffers )
# define  NVSG_GL_STATS_DELETE_LISTS( list, range )
# define  NVSG_GL_STATS_DELETE_TEXTURES( n, textures )
# define  NVSG_GL_STATS_DEPTH_FUNC( func )
# define  NVSG_GL_STATS_DEPTH_MASK( flag )
# define  NVSG_GL_STATS_DRAW_ELEMENTS( mode, count, type, indices )
# define  NVSG_GL_STATS_DRAW_RANGE_ELEMENTS( mode, start, end, count, type, indices )
# define  NVSG_GL_STATS_END_LIST()
# define  NVSG_GL_STATS_FRUSTUM( left, right, bottom, top, zNear, zFar )
# define  NVSG_GL_STATS_GEN_BUFFERS( n, buffers )
# define  NVSG_GL_STATS_GEN_LISTS( range )
# define  NVSG_GL_STATS_GEN_TEXTURES( n, textures )
# define  NVSG_GL_STATS_GET_INTEGERV( pname, params )
# define  NVSG_GL_STATS_IS_TEXTURE( texture )
# define  NVSG_GL_STATS_LIGHTF( light, pname, param )
# define  NVSG_GL_STATS_LIGHTFV( light, pname, params )
# define  NVSG_GL_STATS_LIGHTI( light, pname, param )
# define  NVSG_GL_STATS_LIGHTIV( light, pname, params )
# define  NVSG_GL_STATS_LIGHT_MODELF( pname, param )
# define  NVSG_GL_STATS_LIGHT_MODELFV( pname, params )
# define  NVSG_GL_STATS_LIGHT_MODELI( pname, param )
# define  NVSG_GL_STATS_LIGHT_MODELIV( pname, params )
# define  NVSG_GL_STATS_LINE_WIDTH( width )
# define  NVSG_GL_STATS_LOAD_IDENTITY()
# define  NVSG_GL_STATS_LOAD_MATRIXD( m )
# define  NVSG_GL_STATS_LOAD_MATRIXF( m )
# define  NVSG_GL_STATS_MATERIALF( face, pname, param )
# define  NVSG_GL_STATS_MATERIALFV( face, pname, params )
# define  NVSG_GL_STATS_MATERIALI( face, pname, param )
# define  NVSG_GL_STATS_MATERIALIV( face, pname, params )
# define  NVSG_GL_STATS_MATRIX_MODE( mode )
# define  NVSG_GL_STATS_MULT_MATRIXD( m )
# define  NVSG_GL_STATS_MULT_MATRIXF( m )
# define  NVSG_GL_STATS_NEW_LIST( list, mode )
# define  NVSG_GL_STATS_NORMAL_POINTER( type, stride, pointer )
# define  NVSG_GL_STATS_ORTHO( left, right, bottom, top, zNear, zFar )
# define  NVSG_GL_STATS_POINT_SIZE( size )
# define  NVSG_GL_STATS_POLYGON_MODE( face, mode )
# define  NVSG_GL_STATS_POLYGON_OFFSET( factor, units )
# define  NVSG_GL_STATS_POP_ATTRIB()
# define  NVSG_GL_STATS_POP_MATRIX()
# define  NVSG_GL_STATS_PRIMITIVE_RESTART_INDEX( index )
# define  NVSG_GL_STATS_PUSH_ATTRIB( mask )
# define  NVSG_GL_STATS_PUSH_MATRIX()
# define  NVSG_GL_STATS_ROTATED( angle, x, y, z )
# define  NVSG_GL_STATS_ROTATEF( angle, x, y, z )
# define  NVSG_GL_STATS_SCALED( x, y, z )
# define  NVSG_GL_STATS_SCALEF( x, y, z )
# define  NVSG_GL_STATS_STATE( cap, on )
# define  NVSG_GL_STATS_TEX_COORD_POINTER( size, type, stride, pointer )
# define  NVSG_GL_STATS_TEX_ENVF( target, pname, param )
# define  NVSG_GL_STATS_TEX_ENVFV( target, pname, params )
# define  NVSG_GL_STATS_TEX_ENVI( target, pname, param )
# define  NVSG_GL_STATS_TEX_ENVIV( target, pname, params )
# define  NVSG_GL_STATS_TEX_IMAGE2D( target, level, internalformat, width, height, border, format, type, pixels )
# define  NVSG_GL_STATS_TEX_PARAMETERF( target, pname, param )
# define  NVSG_GL_STATS_TEX_PARAMETERFV( target, pname, params )
# define  NVSG_GL_STATS_TEX_PARAMETERI( target, pname, param )
# define  NVSG_GL_STATS_TEX_PARAMETERIV( target, pname, params )
# define  NVSG_GL_STATS_TRANSLATED( x, y, z )
# define  NVSG_GL_STATS_TRANSLATEF( x, y, z )
# define  NVSG_GL_STATS_VERTEX_POINTER( size, type, stride, pointer )
# define  NVSG_GL_STATS_VIEWPORT( x, y, width, height )

#endif

#endif  // DOXYGON_IGNORE