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
#include "nvmath/Mat44f.h"
#include "nvsg/Group.h"
#include "nvsg/Billboard.h"

namespace nvtraverser
{
  /*! \brief Helper class to hold transformation information used in the render lists.
   *  \remarks This class holds the current modelToWorld and worldToModel transformations, a flag
   *  indicating if the transformation is length preserving, a flag indicating that the
   *  transformation is mirroring, and finally a flag, indicating that the transformation is fixed,
   *  that is, it's not expected to change between two frames.
   *  \sa BaseRenderElement */
  class TransformElement : public BaseRenderElement
  {
    public:
      /*! \brief Default constructor.
       *  \remarks The default constructor sets the model to world and the world to model
       *  transformations to the identity matrix, links to no parent or TransformHandle, and marks
       *  the transformation that no normalization is needed, no mirroring takes place, and it is
       *  expected to be fixed between consecutive frames. */
      NVSG_API TransformElement( void );

      /*! \brief Constructor of a TransformElement.
       *  \param mtw A reference to the constant model to world matrix.
       *  \param wtm A reference to the constant world to model matrix.
       *  \param parent A pointer to the TransformElement that represents the parent of this
       *  transformation, in the scene graph.
       *  \param transform A pointer to the GroupHandle, giving the transformation.
       *  \param lengthPreserving A flag, that specifies, if \c true, that this transformation
       *  preserves the length of vectors, such that they need not be renormalized before usage.
       *  \param mirror A flag, that specifies, if \c true, that this transformation mirrors vectors.
       *  \param fixed A flag, that specifies, if \c true, that this transformation is not to be
       *  expected to vary from frame to frame, and thus can be cached.
       *  \remarks The constructor sets all the values of a TransformElement as the constructor
       *  arguments specify. */
      NVSG_API TransformElement( const nvmath::Mat44f &mtw, const nvmath::Mat44f &wtm
                               , TransformElement *parent, nvsg::GroupHandle * transform
                               , bool lengthPreserving, bool mirror, bool fixed );

      /*! \brief Get the TransformElement that represents the parent of this transformation, in the
       *  scene graph.
       *  \return A pointer to the TransformElement that represents the parent of this
       *  transformation, in the scene graph.
       *  \sa getTransform */
      NVSG_API TransformElement * getParent() const;

      /*! \brief Get the model-to-world transformation from the top of the scene graph to this object.
       *  \return A constant reference to the combined transformation from top of the scene graph to
       *  this object.
       *  \sa getWorldToModel */
      NVSG_API const nvmath::Mat44f & getModelToWorld() const;

      /*! \brief Get the handle of the object that this TransformElement represents.
       *  \return A pointer to the handle of the object that this TransformElement represents.
       *  \sa getParent */
      NVSG_API nvsg::GroupHandle * getTransform() const;

      /*! \brief Get the world-to-model transformation from the top of the scene graph to this object.
       *  \return A constant reference to the combined transformation from top the scene graph to
       *  this object.
       *  \sa getModelToWorld */
      NVSG_API const nvmath::Mat44f & getWorldToModel() const;

      /*! \brief Query, if the matrices of this TransformElement are likely to change on each frame.
       *  \return \c true, if the matrices of this TransformElement can be considered to be constant
       *  from frame to frame, otherwise \c false.
       *  \remarks The transformation of a TransformElement is likely to change on each frame, if
       *  it represents the transformation of a Billboard, for example, or it is a direct or indirect
       *  child of a Billboard.
       *  \sa getTransform, isLengthPreserving, isMirroring */
      NVSG_API bool isFixed() const;

      /*! \brief Query, if the matrices of this TransformElement are length preserving.
       *  \return \c true, if the matrices of this TransformElement are length preserving, otherwise
       *  \c false.
       *  \remarks If a matrix is not length preserving, any vector that is multiplied with it may
       *  need to be renormalized before further usage.
       *  \sa isFixed, isMirroring */
      NVSG_API bool isLengthPreserving() const;

      /*! \brief Query, if the matrices of this TransformElement are mirroring.
       *  \return \c true, if the matrices of this TransformElement are mirroring.
       *  \sa isFixed, isLengthPreserving */
      NVSG_API bool isMirroring() const;

      /*! \brief Update the matrices of this TransformElement 
       *  \param matrix The local model-to-world transformation.
       *  \param inverse The local world-to-model transformation.
       *  \remarks The parent's modelToWorld matrix is premultiplied with \a matrix, and the
       *  parent's worldToModel matrix is postmultiplied with \a inverse to get the combined
       *  modelToWorld and worldToModel matrices, respectively, represented by this
       *  TransformElement. */
      NVSG_API void updateMatrices( const nvmath::Mat44f & matrix, const nvmath::Mat44f & inverse );

    private:
      nvmath::Mat44f      m_modelToWorld;
      nvmath::Mat44f      m_worldToModel;
      TransformElement  * m_parent;
      nvsg::GroupHandle * m_transform;
      bool                m_lengthPreserving;
      bool                m_mirror;
      bool                m_fixed;
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \brief Helper function to create a default TransformElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the TransformElement in.
   *  \return A pointer to the newly created TransformElement.
   *  \sa constructRenderElement */
  inline TransformElement * createTransformElement(RenderElementPool * pool)
  {
    return constructRenderElement<TransformElement>(pool);
  }

  /*! \brief Helper function to create a TransformElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the TransformElement in.
   *  \param mtw A reference to the constant model to world matrix.
   *  \param wtm A reference to the constant world to model matrix.
   *  \param parent A pointer to the TransformElement that represents the parent of this
   *  transformation, in the scene graph.
   *  \param transform A pointer to the GroupHandle, giving the transformation.
   *  \param lengthPreserving A flag, that specifies, if \c true, that this transformation
   *  preserves the length of vectors, such that they need not be renormalized before usage.
   *  \param mirror A flag, that specifies, if \c true, that this transformation mirrors vectors.
   *  \param fixed A flag, that specifies, if \c true, that this transformation is not to be
   *  expected to vary from frame to frame, and thus can be cached.
   *  \return A pointer to the newly created TransformElement.
   *  \sa constructRenderElement */
  inline TransformElement * createTransformElement( RenderElementPool * pool
                                                  , const nvmath::Mat44f& mtw
                                                  , const nvmath::Mat44f& wtm
                                                  , TransformElement * parent
                                                  , nvsg::GroupHandle * transform
                                                  , bool lengthPreserving
                                                  , bool mirror
                                                  , bool fixed )
  {
    return constructRenderElement<TransformElement
                                 , const nvmath::Mat44f&
                                 , const nvmath::Mat44f&
                                 , TransformElement*
                                 , nvsg::GroupHandle *
                                 , bool
                                 , bool
                                 , bool>( pool, mtw, wtm, parent, transform, lengthPreserving, mirror, fixed );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline TransformElement::TransformElement( void )
    : m_modelToWorld( nvmath::cIdentity44f )
    , m_worldToModel( nvmath::cIdentity44f )
    , m_parent( NULL )
    , m_transform( NULL )
    , m_lengthPreserving( true )
    , m_mirror( false )
    , m_fixed( true )
  {
  }

  inline TransformElement::TransformElement( const nvmath::Mat44f &mw
                                           , const nvmath::Mat44f &wm
                                           , TransformElement *parent
                                           , nvsg::GroupHandle * transform
                                           , bool lengthPreserving
                                           , bool mirror
                                           , bool fixed )
    : m_modelToWorld( mw )
    , m_worldToModel( wm )
    , m_parent( parent )
    , m_transform( transform )
    , m_lengthPreserving( lengthPreserving )
    , m_mirror( mirror )
    , m_fixed( fixed && ( ! parent || parent->m_fixed ) )
  {
  }

  inline TransformElement * TransformElement::getParent() const
  {
    return( m_parent );
  }

  inline const nvmath::Mat44f & TransformElement::getModelToWorld() const
  {
    return( m_modelToWorld );
  }

  inline nvsg::GroupHandle * TransformElement::getTransform() const
  {
    return( m_transform );
  }

  inline const nvmath::Mat44f & TransformElement::getWorldToModel() const
  {
    return( m_worldToModel );
  }

  inline bool TransformElement::isFixed() const
  {
    return( m_fixed );
  }

  inline bool TransformElement::isLengthPreserving() const
  {
    return( m_lengthPreserving );
  }

  inline bool TransformElement::isMirroring() const
  {
    return( m_mirror );
  }

  inline void TransformElement::updateMatrices( const nvmath::Mat44f & matrix
                                              , const nvmath::Mat44f & inverse )
  {
    m_modelToWorld = matrix * m_parent->m_modelToWorld;
    m_worldToModel = m_parent->m_worldToModel * inverse;
  }
}