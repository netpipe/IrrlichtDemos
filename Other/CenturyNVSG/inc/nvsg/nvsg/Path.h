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
#include "nvutil/RCObject.h" // base class definition
#include "nvsg/CoreTypes.h"

namespace nvmath
{
  class Mat44f; // forward declaration
}

namespace nvsg 
{
   /*! \brief Class that represents a path through the tree.
    *  \par Namespace: nvsg
    *  \remarks  A Path represents a scene graph or subgraph. It contains
    *  pointers to a chain of objects, each being a child of the
    *  previous object. \n
    *  A path can hold any kind of Object-derived objects (even Drawables). */
  class Path : public nvutil::RCObject
  {
    public:
      /*! \brief Construct a Path object. */
      NVSG_API Path();

      /*! \brief Copy constructor.
       *  \param Reference to a constant Path object.
       *  \remarks This copy constructor creates an exact copy of the provided Path object.\n
       *  Internally this class holds a vector of node pointer representing the path through the 
       *  graph. This copy constructor does not perform a deep copy on the pointers in the vector. 
       *  It simply copies the pointers.*/
      NVSG_API Path(const Path& rhs);

      /*! \brief Get the length of the path through the graph.
       *  \return The number of nodes in the path chain. 
       *  \remarks The path length is the exact number of nodes in the path chain. */
      NVSG_API unsigned int getLength() const;

      /*! \brief Test an empty path.
       *  \return When there is no node in the path chain, this function returns true.
       *  \remarks Testing on emptiness with this function is possibly faster than testing 
       *  the length. */
      NVSG_API bool isEmpty() const;  
      
      /*! \brief Get the head object of the path.
       *  \return This function returns a pointer to the constant head object or NULL when the
       *  path is empty.
       *  \remarks The head object is the first object in the path chain. 
       *  the call Path::getFromHead(0) is equivalent to Path::getHead().
       *  \sa getTail, getFromHead, getFromTail */
      NVSG_API ObjectHandle * getHead() const;

      /*! \brief Get the tail object of the path.
       *  \return This function returns a pointer to the constant tail object or NULL when the
       *  path is empty.
       *  \remarks The tail object is the last object in the path chain. 
       *  the call Path::getFromTail(0) is equivalent to Path::getTail().
       *  \sa getHead, getFromHead, getFromTail */
      NVSG_API ObjectHandle * getTail() const;

      /*! \brief Get an object on the path. 
       *  \param i Null based index of the node to get from the path. The behavior of this 
       *  function is undefined for an invalid index.
       *  \return Pointer to the constant object with the index i.
       *  \remarks The returned object is the object at the requested position. Providing an index
       *  of 0 corresponds to the call to Path::getHead.
       *  \sa getHead, getFromHead, getFromTail */ 
      NVSG_API ObjectHandle * getFromHead(size_t i) const;

      /*! \brief Get an object on the path. 
       *  \param i Null based index of the node to get from the path. The behavior of this 
       *  function is undefined for an invalid index.
       *  \return Pointer to the constant object with the index i.
       *  \remarks The returned object is the object at the requested position. Providing an index
       *  of 0 corresponds to the call to Path::getTail.
       *  \sa getTail, getFromHead, getFromTail */ 
      NVSG_API ObjectHandle * getFromTail(size_t i) const;
      
      /*! \brief Remove the last object from the path. 
       *  \remarks This function simply removes the last (tail) object from the current path chain.
       *  It also handles decrementing the reference count of the removed object.\n
       *  Calling this function on an empty path will lead to undefined behavior.
       *  \sa push, truncate */
      NVSG_API void pop();

      /*! \brief Append an object to the path chain. 
       *  \param Pointer to an constant Object. Providing an invalid pointer or a NULL pointer
       *  will lead to undefined behavior.
       *  \remarks This function appends the given Object to the end of the path chain. 
       *  It also correctly increments the reference count of the provide object. 
       *  \sa pop, truncate*/
      NVSG_API void push(ObjectHandle * pObject);
           
      /*! \brief Remove several objects from the path chain. 
       *  \param start The start index where the removal starts. Providing an invalid start 
       *  index will lead to undefined behavior.
       *  \remarks This function removes all following objects beginning from the start index
       *  from the path chain, including the object at the start index. It correctly
       *  decrements the reference counts of all the removed objects.\n
       *  Calling Path::truncate(0) will remove all objects from the path chain.
       *  \sa pop, push*/
      NVSG_API void truncate(size_t start);

      /*! \brief Get the ModelToWorld and WorldToModel matrices along the path. 
       *  \param modelToWorld Reference for returning the ModelToWorld matrix, excluding the  
       *  tail node transformation.
       *  \param worldToModel Reference for returning the WorldToModel matrix, excluding the 
       *  tail node transformation.
       *  \remarks The accumulated matrices do not contain the tail node transformation. 
       *  These matrices are very handy in the nvui::TransformManipulator derived classes.
       *  nvui::TransformManipulator */
      NVSG_API void getModelToWorldMatrix( nvmath::Mat44f & modelToWorld
                                         , nvmath::Mat44f & worldToModel) const;

    protected:
      //! Prohibit explicit creation on stack by making the destructor protected.
      NVSG_API virtual ~Path();
      
    private:
      std::vector<ObjectHandle *> m_path;   //!< Vector of objects representing a path chain.
  };

  // - - - - - - - - - - - - - - - - - - -
  // inlines
  // - - - - - - - - - - - - - - - - - - -

  inline ObjectHandle * Path::getHead() const
  {
    NVSG_TRACE();
    return( m_path.empty() ? NULL : m_path.front() );
  }

  inline ObjectHandle * Path::getTail() const
  {
    NVSG_TRACE();
    return( m_path.empty() ? NULL : m_path.back() );
  }

  inline ObjectHandle * Path::getFromHead( size_t i ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(0 <= i && i < m_path.size());
    return( m_path[i] );
  }

  inline ObjectHandle * Path::getFromTail( size_t i ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(0 <= i && i < m_path.size());
    return( m_path[m_path.size() - i-1] );
  }

  inline unsigned int Path::getLength() const
  {
    NVSG_TRACE();
    return (unsigned int) m_path.size();
  }

  inline void Path::pop()
  {
    NVSG_TRACE();
    NVSG_ASSERT(getLength() > 0);
    m_path.back()->removeRef();
    m_path.pop_back();
  }

  inline bool Path::isEmpty() const
  {
    NVSG_TRACE();
    return m_path.empty();
  }
} //  namspace nvsg
