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

#include "nvtraverser/BaseRenderElement.h"
#include "nvtraverser/RenderElementPool.h"
#include "nvutil/RCObject.h"
#include "nvsg/Group.h"
#include "nvmath/Mat44f.h"
#include "nvmath/Plane3f.h"
#include <vector>

namespace nvtraverser
{
  /*! \brief Helper class to hold the clip planes valid for a number of PrimitiveSets.
   *  \remarks This class holds the complete clip plane information valid for the objects being
   *  beneath a Group in the scene tree.
   *  \sa BaseRenderElement */
  class ClipPlanesElement : public BaseRenderElement
  {
    public:
      /*! \brief Default constructor.
       *  \remarks The default constructor initializes the corresponding Group to be NULL. It is
       *  used to initialize the stack of ClipPlanesElements maintained by the RenderTraverser. */
      NVSG_API ClipPlanesElement();

      /*! \brief Constructor of a ClipPlanesElement.
       *  \param group A pointer to the Group that adds at least one clip plane.
       *  \param modelToWorld A reference to the corresponding constant model to world
       *  transformation matrix.
       *  \param rhs A reference to the constant ClipPlanesElement to get the first clip planes from.
       *  \remarks The RenderTraverser maintains a stack of ClipPlaneElmenents. Whenever a Group
       *  with at least one clip plane is encountered on traversal, a new ClipPlaneElement is pushed
       *  on that stack, holding all the clip planes being valid at that point, plus the clip planes
       *  defined in that Group. */
      NVSG_API ClipPlanesElement( nvsg::GroupHandle * group, const nvmath::Mat44f & modelToWorld
                                , const ClipPlanesElement &rhs );

      /*! \brief Get the clip plane at index \a index.
       *  \param index The index of the clip plane to get.
       *  \return The clip plane at index \a index.
       *  \note The behavior is undefined, if \a index is larger or equal to the number of clip
       *  planes in this ClipPlanesElement.
       *  \sa getNumberOfClipPlanes, getModelToWorld */
      NVSG_API const nvmath::Plane3f & getClipPlane( size_t index ) const;

      /*! \brief Get the GroupHandle corresponding to this ClipPlanesElement.
       *  \return A pointer to the GroupHandle corresponding to this ClipPlanesElement.
       *  \remarks This is used in RenderTraverser::postTraverseGroup to ensure that this is a
       *  Group that added some clip planes. */
      NVSG_API nvsg::GroupHandle * getGroup() const;

      /*! \brief Get the model-to-world transformation matrix at index \a index.
       *  \param index The index of the model-to-world transformation matrix to get.
       *  \return The model-to-world transformation matrix corresponding to the clip plane at index
       *  \a index.
       *  \note The behavior is undefined, if \a index is larger or equal to the number of clip
       *  planes in this ClipPlanesElement.
       *  \sa getNumberOfClipPlanes, getClipPlane */
      NVSG_API const nvmath::Mat44f & getModelToWorld( size_t index ) const;

      /*! \brief Get the number of clip planes in this ClipPlanesElement.
       *  \return The number of clip planes in this ClipPlanesElement.
       *  \remarks For each of the clip planes, there is a corresponding model-to-world
       *  transformation matrix.
       *  \sa getClipPlane, getModelToWorld */
      NVSG_API size_t getNumberOfClipPlanes() const;

    private:
      nvsg::GroupHandle            * m_group;
      std::vector<nvmath::Plane3f>   m_clipPlanes;
      std::vector<nvmath::Mat44f>    m_modelToWorld;
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \brief Helper function to create an empty ClipPlanesElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the RenderElement in.
   *  \return A pointer to the newly created ClipPlanesElement.
   *  \sa constructRenderElement, RenderElementPool */
  inline ClipPlanesElement * createClipPlanesElement( RenderElementPool * pool )
  {
    return( constructRenderElement<ClipPlanesElement>( pool ) );
  }

  /*! \brief Helper function to create a ClipPlanesElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the RenderElement in.
   *  \param group A pointer to the Group that adds at least one clip plane.
   *  \param modelToWorld A reference to the corresponding constant model to world
   *  transformation matrix.
   *  \param rhs A reference to the constant ClipPlanesElement to get the first clip planes from.
   *  \return A pointer to the newly created ClipPlanesElement.
   *  \sa constructRenderElement, RenderElementPool */
  inline ClipPlanesElement * createClipPlanesElement( RenderElementPool * pool
                                                    , nvsg::GroupHandle * group
                                                    , const nvmath::Mat44f & modelToWorld
                                                    , const ClipPlanesElement & rhs )
  {
    return( constructRenderElement<ClipPlanesElement,nvsg::GroupHandle *, const nvmath::Mat44f &
                                  , const ClipPlanesElement &>( pool, group, modelToWorld, rhs ) );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline ClipPlanesElement::ClipPlanesElement()
    : m_group(NULL)
  {
  }

  inline ClipPlanesElement::ClipPlanesElement( nvsg::GroupHandle * group
                                             , const nvmath::Mat44f & modelToWorld
                                             , const ClipPlanesElement &rhs )
    : BaseRenderElement(rhs)
    , m_clipPlanes(rhs.m_clipPlanes)
    , m_modelToWorld(rhs.m_modelToWorld)
    , m_group(group)
  {
    NVSG_ASSERT(m_group);
    nvutil::ReadableObject<nvsg::Group> g(m_group);
    for ( size_t i=0 ; i<g->getNumberOfClipPlanes() ; i++ )
    {
      if ( g->isClipPlaneActive(i) )
      {
        m_clipPlanes.push_back( g->getClipPlane(i) );
        m_modelToWorld.push_back( modelToWorld );
      }
    }
  }

  inline const nvmath::Plane3f & ClipPlanesElement::getClipPlane( size_t index ) const
  {
    NVSG_ASSERT( index < m_clipPlanes.size() );
    return( m_clipPlanes[index] );
  }

  inline nvsg::GroupHandle * ClipPlanesElement::getGroup() const
  {
    return( m_group );
  }

  inline const nvmath::Mat44f & ClipPlanesElement::getModelToWorld( size_t index ) const
  {
    NVSG_ASSERT( index < m_modelToWorld.size() );
    return( m_modelToWorld[index] );
  }

  inline size_t ClipPlanesElement::getNumberOfClipPlanes() const
  {
    return( m_clipPlanes.size() );
  }
}