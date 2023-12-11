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
/** \file */

#include "nvsg/DAL.h"
#include "nvgl/GLContext.h"
#include "nvsg/VertexAttributeSet.h"
#include <vector>
#include <set>

namespace nvgl
{
  // interface to provide lazy cleanup of dangling GL resources
  class GLResourceDeleter
  {
  public:
    GLResourceDeleter(GLsizei n, GLuint * names) : m_names(&names[0], &names[n]) {}
    virtual ~GLResourceDeleter() {}
    virtual void release() { m_names.clear(); delete this; }
  protected:
    std::vector<GLuint> m_names;
  };

  // identifier for device abstraction layer data
  enum
  {
    GL_BUFFER = 0x00000001
    , GL_DL  = 0x00000002
    , GL_TEX = 0x00000004
  };

  class GLDALDataCreator : public nvsg::DALDataCreator
  {
  public:
    NVSG_API ~GLDALDataCreator();
    NVSG_API void onReleaseDAL(nvsg::HDAL); // overrides DALDataCreator::onReleaseDAL
    NVSG_API void exit();  // performs cleanup on exit

    // add/release lists 
    NVSG_API void addDisplayList(nvsg::HDAL);
    NVSG_API void releaseDisplayLists();
    // add/release buffers
    NVSG_API void addBuffer(nvsg::HDAL);
    NVSG_API void releaseBuffers();
    // add/release textures 
    NVSG_API void addTexture(nvsg::HDAL);
    NVSG_API void releaseTextures();
    // add/release dangling resources
    NVSG_API void addDanglingResource(GLResourceDeleter*);
    NVSG_API void releaseDanglingResources();
  
  private:

    // DAL resources
    std::set<nvsg::HDAL> m_lists;
    std::set<nvsg::HDAL> m_buffers;
    std::set<nvsg::HDAL> m_textures;
  
    // dangling resource management
    nvutil::SWMRSync m_lock;
    void enterCS() { m_lock.lockExclusive(); }
    void leaveCS() { m_lock.unlockExclusive(); }
    std::vector<GLResourceDeleter*> m_danglingResources;
  };

  struct GLDALData : public nvsg::DALData
  {
    NVSG_API GLDALData(GLDALDataCreator * _creator);
    NVSG_API virtual ~GLDALData();
    GLContext * context; 
    GLDALDataCreator * creator;
  };

  // DAL data carrying device dependent texture IDs  
  struct GLTextureCache : public GLDALData
  {
    NVSG_API GLTextureCache(GLDALDataCreator * creator);
    NVSG_API GLTextureCache(GLDALDataCreator * creator, GLuint tex, GLenum tgt);
    NVSG_API ~GLTextureCache();

    struct Deleter : public GLResourceDeleter
    {
      Deleter(GLuint name) : GLResourceDeleter(1, &name) {}
      void release();
    };

    struct GLImageFormat
    {
      GLImageFormat() 
        : levels(~0)
        , width(0)
        , height(0)
        , depth(0)
        , format(~0)
        , type(~0) 
      {}

      GLsizei levels; // # mipmap levels specfied
      GLsizei width;  // width of level 0 image in pixels
      GLsizei height; // height of level 0 image in pixels
      GLsizei depth;  // depth of level 0 image in pixels
      GLenum  format; // user format identifier
      GLenum  type;   // data type identifier
    };

    GLuint  texture; // GL name of the texture object
    GLenum  target;  // texture target
    GLImageFormat imgFmt; // describes the image as specified by GLTexImage.
                          // this is going to be evaluated for re-use purposes
  };

  struct GLDisplayListCache : public GLDALData
  {
    NVSG_API GLDisplayListCache(GLDALDataCreator * creator); 
    NVSG_API ~GLDisplayListCache();

    struct Deleter : public GLResourceDeleter
    {
      Deleter(GLuint name) : GLResourceDeleter(1, &name) {}
      void release();
    };

    GLuint listID;
  };

	struct GLArrayBufferCache : public GLDALData
	{
	  GLArrayBufferCache(GLDALDataCreator * creator); 
	  ~GLArrayBufferCache(); 

    struct Deleter : public GLResourceDeleter
    {
      Deleter(GLuint name) : GLResourceDeleter(1, &name) {}
      void release();
    };

    GLuint vbo;   // vertex buffer identifier
	  GLsizei stride;         // byte offset between consecutive vertex attribute data of same type
	  GLsizeiptr attribOffsets[nvsg::VertexAttributeSet::NUMBER_OF_VERTEX_ATTRIBUTES]; // per vertex attribute offset into vertex buffer
	  GLenum attribType[nvsg::VertexAttributeSet::NUMBER_OF_VERTEX_ATTRIBUTES]; // per attribute type
	};

	struct GLElementArrayBufferCache : public GLDALData
	{
	  GLElementArrayBufferCache(GLDALDataCreator * creator); 
	  ~GLElementArrayBufferCache(); 

    struct Deleter : public GLResourceDeleter
    {
      Deleter(GLuint name) : GLResourceDeleter(1, &name) {}
      void release();
    };

    GLuint ibo;   // index buffer identifier
	  GLsizeiptr indexOffset; // offest into index buffer
	  GLenum indexType;      // type used with indices (either GL_UNSIGNED_INT or GL_UNSIGNED_SHORT)
	  GLsizei numIndices;     // total number of indices
	  GLuint  startIndex;   // start - end range for glDrawRangeElements
	  GLuint  endIndex;     // ...
	  // multi draw elements related
	  GLvoid ** primitiveOffsets;    // per primitive offset into index buffer 
	  GLsizei * primitiveIndexCount; // per primitive index count
	};

  struct GLPixelBufferCache : public GLDALData
  {
    GLPixelBufferCache(GLDALDataCreator * creator);
    ~GLPixelBufferCache();

    struct Deleter : public GLResourceDeleter
    {
      Deleter(GLuint * names) : GLResourceDeleter(2, names) {}
      void release();
    };

    GLuint pbo[2];
    GLint  target; // 0 or 1, toggling
  };

}
