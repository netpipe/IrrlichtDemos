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
#include "nvutil/RCObject.h"
#include "nvutil/Handle.h"
#include "nvsg/Object.h"
#include "nvsg/StateVariant.h"
#include "nvsg/StatePass.h"
#include "nvsg/Material.h"
#include "nvsg/PrimitiveSet.h"
#include "nvmath/Sphere3f.h"
#include "nvmath/Vec3f.h"
#include "nvmath/Vec4f.h"
#include "nvmath/Mat44f.h"
#include "nvtraverser/ClipPlanesElement.h"
#include "nvtraverser/LightsElement.h"
#include "nvtraverser/TransformElement.h"
#include <algorithm>

namespace nvtraverser
{
  /*! \brief Helper class to hold all the state informations valid for a number of PrimitiveSets.
   *  \remarks This class holds the complete state information valid for a number of PrimitiveSets.
   *  This information contains the current StateVariant, and conglomerates informations on the
   *  clip planes (ClipPlanesElement), light sources (LightsElement), and transformations
   *  (TransformElement).
   *  \sa BaseRenderElement */
  class RenderElement : public BaseRenderElement
  {
    public:
      /*! \brief Constructor of a RenderElement.
       *  \param svh A pointer to the current StateVariant to be used. May by NULL.
       *  \param cpe A pointer to the current ClipPlanesElement to be used.
       *  \param le A pointer to the current LightsElement to be used.
       *  \param te A pointer to the current TransformElement to be used.
       *  \param culling \c true, if the PrimitiveSets of this RenderElement are allowed to be
       *  culled, otherwise \c false.
       *  \remarks To render a scene, all PrimitiveSets in that scene are gathered into a list of
       *  RenderElements. Each RenderElement holds a number of PrimitiveSets for which the same
       *  states - like clipping, lighting, or transformation - are valid.
       *  The StateVariantHandle might be NULL, if there is no valid StateVariant for the following
       *  geometries (PrimitiveSets). The other pointers (\a cpe, \a le, and \a te) are not allowed
       *  to be NULL, but have to be valid pointers. The reference count of each of those objects
       *  is increased. */
      NVSG_API RenderElement( nvsg::StateVariantHandle * svh, ClipPlanesElement * cpe
                            , LightsElement *le, TransformElement *te, bool culling );

      /*! \brief Destructor of a RenderElement.
       *  \remarks The reference counts of all the constructor arguments and the added PrimitiveSets
       *  are decremented.
       *  \sa addPrimitiveSet */
      NVSG_API ~RenderElement();

      /*! \brief Add a PrimitiveSet to this RenderElement.
       *  \param psh A pointer to the PrimitiveSet to add.
       *  \remarks The reference count of \a psh is increased.
       *  \sa getNumberOfPrimitiveSets, getPrimitiveSet */
      NVSG_API void addPrimitiveSet( nvsg::PrimitiveSetHandle * psh );

      /*! \brief Get a pointer to the associated ClipPlanesElement.
       *  \return A pointer to the associated ClipPlanesElement.
       *  \sa getLights, getStateVariant, getTransform */
      NVSG_API ClipPlanesElement * getClipPlanes() const;

      /*! \brief Get a pointer to the associated LightsElement.
       *  \return A pointer to the associated LightsElement.
       *  \sa getClipPlanes, getStateVariant, getTransform */
      NVSG_API LightsElement * getLights() const;

      /*! \brief Get the number of PrimitiveSets in this RenderElement.
       *  \return The number of PrimitiveSets in this RenderElement.
       *  \sa getPrimitiveSet, addPrimitiveSet */
      NVSG_API size_t getNumberOfPrimitiveSets() const;

      /*! \brief Get the occlusion cull ID associated with this RenderElement.
       *  \return The occlusion cull ID associated with this RenderElement.
       *  \remarks A RenderElement can have an associated occlusion cull ID. This ID is set and used
       *  in the RenderTraverser::occlusionTest, if occlusion culling is on.
       *  \sa setOcclusionCullID */
      NVSG_API unsigned int getOcclusionCullID() const;

      /*! \brief Get the PrimitiveSet with index \a i.
       *  \param i Index of the PrimitiveSet to get.
       *  \return A pointer to the PrimitiveSetHandle that was stored at the \a i-th position in
       *  this RenderElement.
       *  \note The behavior is undefined if i is greater or equal to the number of PrimitiveSets in
       *  this RenderElement.
       *  \sa getNumberOfPrimitiveSets, addPrimitveSet */
      NVSG_API nvsg::PrimitiveSetHandle * getPrimitiveSet( size_t i ) const;

      /*! \brief Get the squared distance of the center of the bounding sphere to the camera.
       *  \param worldToView A constant reference to the transformation matrix from world to view
       *  space.
       *  \return The squared distance from the center of the bounding sphere of this RenderElement
       *  to the camera.
       *  \remarks It is assumed, that each call of this function is used with the same \a
       *  worldToView matrix, until the squared distance is explicitly invalidated using
       *  invalidateSquaredDistance. This is used, for example, to sort the RenderElements of a
       *  render list by distance. In that sorting process, the world to view transformation does
       *  not change. That way, the distance can be calculated once, and there is no need to store
       *  or compare the transformation matrix.
       *  \sa invalidateSquaredDistance, isSquaredDistanceValid */
      NVSG_API float getSquaredDistance( const nvmath::Mat44f &worldToView );

      /*! \brief Get a pointer to the associated StateVariantHandle.
       *  \return A pointer to the associated StateVariantHandle.
       *  \remarks This StateVariant is valid for each PrimitiveSet in this RenderElement.
       *  \sa getLights, getClipPlanes, getTransform */
      NVSG_API nvsg::StateVariantHandle * getStateVariant() const;

      /*! \brief Get a pointer to the associated TransformElement.
       *  \return A pointer to the associated TransformElement.
       *  \remarks The TransformElement holds the transformation informations valid for all
       *  PrimitiveSets in this RenderElement.
       *  \sa getLights, getClipPlanes, getStateVariant */
      NVSG_API TransformElement * getTransform() const;

      /*! \brief Mark the squared distance information as invalid.
       *  \remarks Between two frames, the squared distance of each RenderElement should be marked
       *  as invalid. That way, they can be sorted by distance to the camera efficiently, without
       *  the need to store or compare a world to view transformation.
       *  \sa getSquaredDistance, isSquaredDistanceValid */
      NVSG_API void invalidateSquaredDistance();

      /*! \brief Query if the PrimitiveSets of this RenderElement are supposed to be culled.
       *  \return \a true, if the PrimitiveSets of this RenderElement are supposed to be culled,
       *  otherwise \c false.
       *  \sa getOcclusionCullID, setOcclusionCullID */
      NVSG_API bool isSubjectToCull() const;

      /*! \brief Query if the squared distance information is valid.
       *  \return \c true, if the squared-distance is of this RenderElement is marked as valid,
       *  otherwise \c false.
       *  \remarks While sorting RenderElements by distance, the squared distance to the camera can
       *  be queried multiple times, each time with the same world to view transformation matrix.
       *  On first calculation of that squared distance, it is marked as valid to make sure the
       *  calculation isn't done anymore, until it is explicitly marked as invalid again.
       *  \sa getSquaredDistance, invalidateSquaredDistance */
      NVSG_API bool isSquaredDistanceValid() const;

      /*! \brief Sets the occlusion cull ID for this RenderElement.
       *  \param id The occlusion cull ID associated to this RenderElement.
       *  \remarks A RenderElement can have an associated occlusion cull ID. This ID is set and used
       *  in the RenderTraverser::occlusionTest, if occlusion culling is on.
       *  \sa getOcclusionCullID */
      NVSG_API void setOcclusionCullID( unsigned int id );

    private:
      nvmath::Sphere3f                          m_boundingSphere;
      bool                                      m_boundingSphereValid;
      ClipPlanesElement                       * m_clipPlanes;
      LightsElement                           * m_lights;
      bool                                      m_culling;
      unsigned int                              m_occlusionCullID;
      std::vector<nvsg::PrimitiveSetHandle *>   m_primitiveSets;
      float                                     m_squaredDist;
      nvsg::StateVariantHandle                * m_stateVariant;
      TransformElement                        * m_transform;
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \brief Helper function to create a RenderElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the RenderElement in.
   *  \param svh A pointer to the current StateVariant to be used. May by NULL.
   *  \param cpe A pointer to the current ClipPlanesElement to be used.
   *  \param le A pointer to the current LightsElement to be used.
   *  \param te A pointer to the current TransformElement to be used.
   *  \return A pointer to the newly created LightsElement.
   *  \sa constructRenderElement, RenderElementPool */
  inline RenderElement * createRenderElement( RenderElementPool * pool
                                            , nvsg::StateVariantHandle * svh
                                            , ClipPlanesElement * cpe
                                            , LightsElement* le
                                            , TransformElement* te
                                            , bool culling )
  {
    return constructRenderElement<RenderElement, nvsg::StateVariantHandle *, ClipPlanesElement *
      , LightsElement*, TransformElement*, bool>(pool, svh, cpe, le, te, culling);
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // comparison functors
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \brief Comparison functor to sort RenderElements from back to front.
   *  \remarks This functor class is used to sort RenderElements from back to front. It is used for
   *  example to sort the transparent objects in a scene.
   *  \sa RenderElementCompareFrontToBack */
  class RenderElementCompareBackToFront
  {
    public:
      /*! \brief Constructor of the functor with world-to-view transformation matrix.
       *  \param worldToView A reference to the constant world-to-view transformation matrix.
       *  \remarks The matrix \a worldToView is used to determine the distances of the
       *  RenderElements to the camera.
       *  \sa RenderElementCompareFrontToBack */
      NVSG_API RenderElementCompareBackToFront( const nvmath::Mat44f &worldToView );

      /*! \brief Function-call operator to compare back to front on distance.
       *  \param re0 A pointer to the first RenderElement to compare.
       *  \param re1 A pointer to the second RenderElement to compare.
       *  \return \c true, if the squareDistance of re0 is larger than that of re1, otherwise \c
       *  false. */
      NVSG_API bool operator()( RenderElement * re0, RenderElement *re1 ) const;

    private:
      const nvmath::Mat44f  m_worldToView;
  };

  /*! \brief Comparison functor to sort RenderElements from front to back.
   *  \remarks This functor class is used to sort RenderElements from front to back. It is used for
   *  example to sort the opaque objects in a scene, when depth sorting is enabled.
   *  \sa RenderElementCompareFrontToBack */
  class RenderElementCompareFrontToBack
  {
    public:
      /*! \brief Constructor of the functor with world-to-view transformation matrix.
       *  \param worldToView A reference to the constant world-to-view transformation matrix.
       *  \remarks The matrix \a worldToView is used to determine the distances of the
       *  RenderElements to the camera.
       *  \sa RenderElementCompareBackToFront */
      NVSG_API RenderElementCompareFrontToBack( nvmath::Mat44f &worldToView );

      /*! \brief Function-call operator to compare front to back on distance
      *  \param re0 A pointer to the first RenderElement to compare.
      *  \param re1 A pointer to the second RenderElement to compare.
      *  \return \c true, if the squareDistance of re0 is smaller than that of re1, otherwise \c
      *  false. */
      NVSG_API bool operator()( RenderElement * re0, RenderElement *re1 ) const;

    private:
      nvmath::Mat44f  m_worldToView;
  };

  /*! \brief Comparison functor to sort RenderElements by material.
   *  \remarks This functor class is used to sort RenderElements from front to back. It is used for
   *  example to sort the opaque objects in a scene, when depth sorting is enabled.
   *  \sa RenderElementCompareFrontToBack, RenderElementBackToFront */
  class RenderElementCompareStateAttributes
  {
    public:
      /*! \brief Function-call operator to compare on StateAttributes in the RenderElements.
       *  \param re0 A pointer to the first RenderElement to compare.
       *  \param re1 A pointer to the second RenderElement to compare.
       *  \return \c true, if re0 is considered to be "less than" re1, in terms of StateAttributes,
       *  otherwise \c false. */
      NVSG_API bool operator()( RenderElement * re0, RenderElement *re1 ) const;

    private:
      NVSG_API bool isLess( RenderElement * re0, RenderElement *re1 ) const;
      NVSG_API bool lessTest( const nvsg::StateVariant * sv0, const nvsg::StateVariant * sv1
                            , unsigned int objectCode, bool & less ) const;
  };


  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline void RenderElement::addPrimitiveSet( nvsg::PrimitiveSetHandle * psh )
  {
    NVSG_ASSERT( psh );
    psh->addRef();
    m_primitiveSets.push_back( psh );
    m_boundingSphereValid = false;
  }

  inline ClipPlanesElement * RenderElement::getClipPlanes() const
  {
    return( m_clipPlanes );
  }

  inline LightsElement * RenderElement::getLights() const
  {
    return( m_lights );
  }

  inline size_t RenderElement::getNumberOfPrimitiveSets() const
  {
    return( m_primitiveSets.size() );
  }

  inline unsigned int RenderElement::getOcclusionCullID() const
  {
    return( m_occlusionCullID );
  }

  inline nvsg::PrimitiveSetHandle * RenderElement::getPrimitiveSet( size_t index ) const
  {
    NVSG_ASSERT( index < m_primitiveSets.size() );
    return( m_primitiveSets[index] );
  }

  inline nvsg::StateVariantHandle * RenderElement::getStateVariant() const
  {
    return( m_stateVariant );
  }

  inline TransformElement * RenderElement::getTransform() const
  {
    return( m_transform );
  }

  inline void RenderElement::invalidateSquaredDistance()
  {
    m_squaredDist = -1.0f;
  }

  inline bool RenderElement::isSubjectToCull() const
  {
    return( m_culling );
  }

  inline bool RenderElement::isSquaredDistanceValid() const
  {
    return( 0.0f <= m_squaredDist );
  }

  inline void RenderElement::setOcclusionCullID( unsigned int id )
  {
    m_occlusionCullID = id;
  }


  inline bool RenderElementCompareBackToFront::operator()( RenderElement * re0, RenderElement *re1 ) const
  {
    return( re1->getSquaredDistance( m_worldToView ) < re0->getSquaredDistance( m_worldToView ) );
  }

  inline bool RenderElementCompareFrontToBack::operator()( RenderElement * re0, RenderElement *re1 ) const
  {
    return( re0->getSquaredDistance( m_worldToView ) < re1->getSquaredDistance( m_worldToView ) );
  }

  inline bool RenderElementCompareStateAttributes::operator()( RenderElement * re0, RenderElement *re1 ) const
  {
    NVSG_ASSERT( re0 && re1 );
    bool less = isLess( re0, re1 );
    NVSG_ASSERT( !( less && isLess(re1,re0) ) );  // assure that not both re0<re1 and re1<re0
    return( less );
  }
}