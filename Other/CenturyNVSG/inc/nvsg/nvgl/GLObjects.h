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

#include "nvsgcommon.h"

#include <set>
#include "nvgl/nvgl.h"
#include "nvutil/RCObject.h"


namespace nvgl
{
  class GLDALDataCreator;

  //! The \c GLObjects class is a container of OpenGL objects.
  /** Just like a \c GLContext, you will never have to use this class explicitly. A \c GLContext holds a \c GLObjects member
    * to cache information about OpenGL objects. OpenGL objects for example are display lists, texture objects, or vertex
    * buffer objects. */
  class GLObjects
  {
    friend class GLContext;

  public:
      //! Constructor
      GLObjects();
      //! Destructor
      ~GLObjects();

  private:
    // Collection of shareable objects
    struct GLShareables : public nvutil::RCObject
    {
      GLShareables(); 
      ~GLShareables(); // for cleanup purposes
      void exit(); // cleanup

      NameSet m_buffers;
      NameSet m_lists;
      NameSet m_programs;
      NameSet m_textures;

      GLDALDataCreator * m_DALDataCreator;
    };
    
    // shareable objects
    GLShareables * m_shareables;
    // non-shareable objects
    NameSet m_occlusionQueries;
    NameSet m_queries;
  };
} //  namespace nvgl
