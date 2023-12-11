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

#include "nvsgcommon.h"
#include "nvtraverser/BaseRenderElement.h"
#include "nvmath/Mat44f.h"
#include "nvsg/Group.h"
#include "nvsg/LightSource.h"
#include <algorithm> // transform, for_each
#include <iterator> // back_inserter

namespace nvtraverser
{
  /*! \brief Helper class to hold the vector of lights that are active while traversing the tree.
   *  \remarks This class holds the vector of lights that are active while traversing the tree,
   *  the corresponding modelToWorld and worldToModel transformations, and the GroupHandle that
   *  activated the latest light(s) in this LightsElement.
   *  \sa BaseRenderElement */
  class LightsElement : public BaseRenderElement
  {
    public:
      /*! \brief Default constructor.
       *  \remarks The default constructor sets the owning group to NULL. The vector of lights and
       *  the vectors transformation matrices are empty */
      NVSG_API LightsElement();

      /*! \brief Constructor of a LightsElement.
       *  \param group The handle of the Group that is going to add one or more light sources.
       *  \param rhs A reference to the constant current LightsElement valid up to now.
       *  \remarks The constructor increments the reference counter of the Group \a group and all
       *  light sources in \a rhs. All light sources and the corresponding modelToWorld and
       *  worldToModel matrices are copied over to this new LightsElement. */
      NVSG_API LightsElement( nvsg::GroupHandle * group, const LightsElement &rhs );

      /*! \brief Destructor of a LightsElement
       *  \remarks If there is a non-zero Group registered for this LightsElement, it's reference
       *  is removed. For each LightSource in this LightsElement, their reference is removed. */
      NVSG_API ~LightsElement();

      /*! \brief Add a light to this LightsElement
       *  \param lsh A pointer to the LightsSourceHandle to add.
       *  \param modelToWorld A constant reference to the corresponding model to world transformation.
       *  \param worldToModel A constant reference to the corresponding world to model transformation.
       *  \remarks This function stores the LightsSourceHandle \a lsh and the corresponding
       *  transformations matrices, and increases the reference count of \a lsh.
       *  \sa getNumberOfLights, getLight, getModelToWorld, getWorldToModel */
      NVSG_API void addLight( nvsg::LightSourceHandle * lsh, const nvmath::Mat44f & modelToWorld
                            , const nvmath::Mat44f & worldToModel );

      /*! \brief Get the Group associated with this LightsElement.
       *  \return A pointer to the GroupHandle that did add the last one or more light sources to
       *  this LightsElement.
       *  \remarks On traversal, this information is used to determine if the top element of the
       *  stack of LightsElement has to be popped. */
      NVSG_API nvsg::GroupHandle * getGroup() const;

      /*! \brief Get the light source with index \a i.
       *  \param i Index of the light source to get.
       *  \return A pointer to the LightSourceHandle that was stored as the \a i-th light source.
       *  \note The behavior is undefined if i is greater or equal to the number of stored light
       *  sources.
       *  \sa getNumberOfLights, getModelToWorld, getWorldToModel, addLight */
      NVSG_API nvsg::LightSourceHandle * getLight( size_t i ) const;

      /*! \brief Get the model-to-world transformation with index \a i.
       *  \param i Index of the model-to-world transformation to get.
       *  \return A constant reference to the model-to-world transformation at index \a i.
       *  \note The behavior is undefined if i is greater or equal to the number of stored light
       *  sources.
       *  \sa getNumberOfLights, getLight, getWorldToModel, addLight */
      NVSG_API const nvmath::Mat44f & getModelToWorld( size_t i ) const;

      /*! \brief Get the number of lights in this LightsElement.
       *  \return The number of LightSourceHandles, with corresponding transformations, stored.
       *  \sa addLight, getLight, getModelToWorld, getWorldToModel */
      NVSG_API size_t getNumberOfLights() const;

      /*! \brief Get the world-to-model transformation with index \a i.
       *  \param i Index of the world-to-model transformation to get.
       *  \return A constant reference to the world-to-model transformation at index \a i.
       *  \note The behavior is undefined if i is greater or equal to the number of stored light
       *  sources.
       *  \sa getNumberOfLights, getLight, getModelToWorld, addLight */
      NVSG_API const nvmath::Mat44f & getWorldToModel( size_t i ) const;

    private:
      //  private copy constructor to prevent accidental usage
      LightsElement(const LightsElement &rhs);

    private:
      nvsg::GroupHandle *                   m_group;
      std::vector<nvsg::LightSourceHandle*> m_lights;
      std::vector<nvmath::Mat44f>           m_modelToWorlds;
      std::vector<nvmath::Mat44f>           m_worldToModels;
  };


  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \brief Helper function to create a default LightsElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the LightsElement in.
   *  \return A pointer to the newly created LightsElement.
   *  \sa LightsElement, constructRenderElement, RenderElementPool */
  inline LightsElement * createLightsElement( RenderElementPool* pool )
  {
    return constructRenderElement<LightsElement>(pool);
  }

  /*! \brief Helper function to create a LightsElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the LightsElement in.
   *  \param group The handle of the Group that is going to add one or more light sources.
   *  \param rhs A reference to the constant current LightsElement valid up to now.
   *  \return A pointer to the newly created LightsElement.
   *  \sa LightsElement, constructRenderElement, RenderElementPool */
  inline LightsElement * createLightsElement( RenderElementPool* pool, nvsg::GroupHandle* group
                                            , const LightsElement& rhs)
  {
    return constructRenderElement<LightsElement, nvsg::GroupHandle*, const LightsElement&>(pool, group, rhs);
  }


  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline LightsElement::LightsElement()
    : m_group(NULL)
  {
  }

  inline LightsElement::LightsElement( nvsg::GroupHandle * group, const LightsElement &rhs )
    : m_group(group)
    , m_modelToWorlds(rhs.m_modelToWorlds) // copy matrices
    , m_worldToModels(rhs.m_worldToModels) // copy matrices
  {
    NVSG_ASSERT( m_group );
    m_group->addRef();

    // copy lights from rhs and add reference for each
    std::transform( rhs.m_lights.begin(), rhs.m_lights.end(), std::back_inserter(m_lights)
                  , nvutil::addReference() );
  }

  inline LightsElement::~LightsElement()
  {
    if ( m_group )
    {
      m_group->removeRef();
    }
    std::for_each( m_lights.begin(), m_lights.end(), nvutil::removeReference() );
  }

  inline void LightsElement::addLight( nvsg::LightSourceHandle * lsh, const nvmath::Mat44f & modelToWorld
                              , const nvmath::Mat44f & worldToModel )
  {
    NVSG_ASSERT( lsh );
    lsh->addRef();
    m_lights.push_back( lsh );
    m_modelToWorlds.push_back( modelToWorld );
    m_worldToModels.push_back( worldToModel );
  }

  inline nvsg::GroupHandle * LightsElement::getGroup() const
  {
    return( m_group );
  }

  inline nvsg::LightSourceHandle * LightsElement::getLight( size_t i ) const
  {
    NVSG_ASSERT( i < m_lights.size() );
    return( m_lights[i] );
  }

  inline const nvmath::Mat44f & LightsElement::getModelToWorld( size_t i ) const
  {
    NVSG_ASSERT( i < m_modelToWorlds.size() );
    return( m_modelToWorlds[i] );
  }

  inline size_t LightsElement::getNumberOfLights() const
  {
    return( m_lights.size() );
  }

  inline const nvmath::Mat44f & LightsElement::getWorldToModel( size_t i ) const
  {
    NVSG_ASSERT( i < m_worldToModels.size() );
    return( m_worldToModels[i] );
  }

}
