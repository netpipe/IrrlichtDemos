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
#include "nvtraverser/ClipPlanesElement.h"
#include "nvtraverser/TransformElement.h"
#include "nvutil/RCObject.h"
#include <algorithm>

namespace nvtraverser
{
  /*! \brief Helper class to hold highlight information used in the render lists.
   *  \remarks This class holds a number of PrimitiveSets representing the highlight objects,
   *  together with the current ClipPlanesElement and TransformElement.
   *  \sa BaseRenderElement */
  class HighlightElement : public BaseRenderElement
  {
  public: 
    /*! \brief Default constructor.
     *  \param cpe A pointer to the ClipPlanesElement to use.
     *  \param te A pointer to the TransformElement to use.
     *  \remarks The default constructor increases the reference count of the construction
     *  arguments \a cpe and \a te. */
    NVSG_API HighlightElement( ClipPlanesElement * cpe, TransformElement * te );

    /*! \brief Default destructor.
     *  \remarks The default destructor decreases the reference count of the associated
     *  ClipPlanesElement and TransformElement, as well as of each used PrimitiveSet.
     *  \sa addHighlightObject */
    NVSG_API ~HighlightElement();

    /*! \brief Add a PrimitiveSet as a highlight object.
     *  \param psh A pointer to the PrimitiveSetHandle representing a highlight object.
     *  \remarks The PrimitiveSetHandle \a psh is stored and its reference count in incremented.
     *  \sa getHighlightObject, getNumberOfHighlightObjects */
    NVSG_API void addHighlightObject( nvsg::PrimitiveSetHandle * psh );

    /*! \brief Get the associated ClipPlanesElement.
     *  \return The pointer to the ClipPlanesElement, this HighlightElement was created with.
     *  \sa getTransform */
    NVSG_API ClipPlanesElement * getClipPlanes();

    /*! \brief Get the \a index's highlight object.
     *  \param index The index of the highlight object to get.
     *  \return A pointer to the PrimitiveSetHandle representing the \a index's highlight object.
     *  \remarks
     *  \note The behavior is undefined if \a index is larger or equal to the number of highlight
     *  objects stored in this HighlightElement.
     *  \sa getNumberOfHighlightObjects */
    NVSG_API nvsg::PrimitiveSetHandle * getHighlightObject( size_t index );

    /*! \brief Get the number of stored highlight objects.
     *  \return The number of stored highlight objects.
     *  \sa addHighlightObject, getHighlightObject */
    NVSG_API size_t getNumberOfHighlightObjects();

    /*! \brief Get the associated TransformElement.
     *  \return A pointer to the TransformElement, this HighlightElement was created with.
     *  \sa getClipPlanes */
    NVSG_API TransformElement * getTransform();

  private:
    std::vector<nvsg::PrimitiveSetHandle*>    m_highlightObjects;
    ClipPlanesElement                       * m_clipPlanes;
    TransformElement                        * m_transform;
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \brief Helper function to create a HighlightElement using a given RenderElementPool.
   *  \param pool The memory pool to create the the HighlightElement in.
   *  \param cpe A pointer to the ClipPlanesElement to use.
   *  \param te A pointer to the TransformElement to use.
   *  \return A pointer to the newly created HighlightElement.
   *  \sa constructRenderElement */
  inline HighlightElement * createHighlightElement( RenderElementPool * pool, ClipPlanesElement * cpe
    , TransformElement* te)
  {
    NVSG_TRACE();
    return constructRenderElement<HighlightElement, ClipPlanesElement*, TransformElement*>(pool, cpe, te);
  }


  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline HighlightElement::HighlightElement( ClipPlanesElement * cpe, TransformElement * te )
    : m_clipPlanes(cpe)
    , m_transform(te)
  {
    NVSG_TRACE();
    NVSG_ASSERT(m_clipPlanes && m_transform);
    m_clipPlanes->addRef();
    m_transform->addRef();
  }

  inline HighlightElement::~HighlightElement()
  {
    NVSG_TRACE();
    NVSG_ASSERT(m_clipPlanes && m_transform);
    m_clipPlanes->removeRef();
    m_transform->removeRef();
    std::for_each(m_highlightObjects.begin(),m_highlightObjects.end(), nvutil::removeReference());
  }

  inline void HighlightElement::addHighlightObject( nvsg::PrimitiveSetHandle * psh )
  {
    NVSG_TRACE();
    NVSG_ASSERT( psh );
    psh->addRef();
    m_highlightObjects.push_back(psh);
  }

  inline ClipPlanesElement * HighlightElement::getClipPlanes()
  {
    NVSG_TRACE();
    return( m_clipPlanes );
  }

  inline nvsg::PrimitiveSetHandle * HighlightElement::getHighlightObject( size_t index )
  {
    NVSG_TRACE();
    NVSG_ASSERT( index < m_highlightObjects.size() );
    return( m_highlightObjects[index] );
  }

  inline size_t HighlightElement::getNumberOfHighlightObjects()
  {
    NVSG_TRACE();
    return( m_highlightObjects.size() );
  }

  inline TransformElement * HighlightElement::getTransform()
  {
    NVSG_TRACE();
    return( m_transform );
  }
}
