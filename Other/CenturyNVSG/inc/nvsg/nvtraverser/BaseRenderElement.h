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

namespace nvtraverser
{
  /*! \brief Helper class to serve as a base class for the render list elements, like RenderElement,
   *  LightsElement, or ClipPlanesElement.
   *  \remarks This class provides the reference counting for the render list elements.
   *  \sa ClipPlanesElement, HighlightElement, LightsElement, RenderElement, TransformElement */
  class BaseRenderElement
  {
    public:
      /*! \brief Increments the BaseRenderElement's reference count.
       *  \remarks For user code to ensure that the data of a reference counted object is valid as
       *  long as it uses the object, it should first increment the objects reference count. After
       *  usage, client code should decrement the objects reference count to avoid resource leaks.
       *  \note The reference count of a newly created BaseRenderElement initially is zero.
       *  \sa removeRef */
      void addRef();

      /*! \brief Decrements the BaseRenderElement's reference count. 
       *  \remarks It is prohibited to explicitly delete a BaseRenderElement. The object will be
       *  automatically deleted, if the reference count changes from one to zero.
       *  \sa addRef */
      void removeRef();

#if !defined(NDEBUG)
      /*! \brief Get the current reference count of a BaseRenderElement.
       *  \return The current reference count of the BaseRenderElement.
       *  \remarks This function is available in the debug build only! */
      size_t getRefCnt() const;
#endif

    protected:
      /*! \brief Protected default constructor to prevent explicit instantiation.
       *  \remarks Initializes the reference count to zero.
       *  \note A BaseRenderElement is intended to serve as a base class only. That is, a
       *  BaseRenderElement can be instantiated only as an object of a class derived from
       *  BaseRenderElement. */
      BaseRenderElement();

      /*! \brief Protected copy constructor to prevent explicit instantiation.
       *  \remarks A new BaseRenderElement is created, and hence, the reference count will be
       *  initialized to zero because no one but the creator is currently referencing this object.
       *  \note A BaseRenderElement is intended to serve as a base class only. That is, a
       *  BaseRenderElement can be instantiated only as an object of a class derived from
       *  BaseRenderElement. */
      BaseRenderElement( const BaseRenderElement & rhs );

      /*! \brief Destructor of a BaseRenderElement
       *  \remarks A BaseRenderElement will be automatically deleted when its reference count
       *  changes from one to zero.
       *  \sa removeRef */
      virtual ~BaseRenderElement();

    private:
      size_t m_refCnt;
  };


  inline BaseRenderElement::BaseRenderElement()
    : m_refCnt(0)
  {
  }

  inline BaseRenderElement::BaseRenderElement( const BaseRenderElement & rhs )
    : m_refCnt(0)
  {
  }

  inline BaseRenderElement::~BaseRenderElement()
  {
  }

  inline void BaseRenderElement::addRef()
  {
    m_refCnt++;
  }

  inline void BaseRenderElement::removeRef()
  {
    if ( --m_refCnt == 0 )
    {
      // note: this object was created in a pool using placement new, that is -
      // we must not call the delete operator here but explicitely call the destructor
      this->~BaseRenderElement();
    }
  }

#if !defined(NDEBUG)
  inline size_t BaseRenderElement::getRefCnt() const
  {
    return( m_refCnt );
  }
#endif
}