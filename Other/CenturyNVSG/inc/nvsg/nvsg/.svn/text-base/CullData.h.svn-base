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
#include  "nvutil/RCObject.h"

namespace nvsg
{  
  /*! \brief Type to describe the cull code of an object.
   *  \par Namespace: nvsg
   *  \remarks With a CullCode, an object can be marked to be completely out (CC_OUT), completely
   *  in (CC_IN) or partially in and out (CC_PART) with respect to a view volume.
   *  \sa CullData */
  typedef enum
  {
    CC_OUT,   //!< an object is completely outside
    CC_IN,    //!< an object is completely inside
    CC_PART   //!< an object is partially in and partially out
  } CullCode;

  /*! \brief Class holding culling information of an object and it's children.
   *  \par Namespace: nvsg
   *  \remarks With CullData, a CullTraverser can build a (somewhat simplified) mirror of the
   *  scene tree. Each CullData object corresponds to one object in a scene tree. Such a CullData
   *  tree is used in the Traverser to determine if parts of the scene tree are to be traversed.
   *  \sa CullCode, nvtraverser::CullTraverser, nvtraverser::FrustumCullTraverser,
   *  nvtraverser::Traverser */
  class CullData : public nvutil::RCObject
  {
    public:
      /*! \brief Constructor with nvsg::CullCode.
       *  \param cullCode Code to be associated with this CullData.
       *  \sa CullCode */
      NVSG_API CullData( CullCode cullCode );

    public:
      /*! \brief Balance the tree beneath this CullData.
       *  \remarks After having constructed a CullData tree, it might be of value to improve it.
       *  When this CullData has a nvsg::CullCode of CC_PART, and all it's children have a
       *  nvsg::CullCode of CC_IN, this function removes these children and changes the CullData
       *  to CC_IN, thus simplifying the CullData tree.
       *  \sa addChild, getChild, getCullCode */
      NVSG_API void balance( void );

      /*! \brief Add a child to this CullData.
       *  \param cd A pointer to the CullData to add to the back of this CullData's childrens.
       *  \remarks CullData represent a somewhat simplified mirror of a scene tree. If \a this is
       *  the CullData of an object in a scene tree with the nvsg::CullCode CC_PART, and that
       *  object has children, \a this should get the same number of children as that object in
       *  the scene tree, each representing the CullData of the corresponding child.\n
       *  If the nvsg::CullCode of \a this is CC_IN or CC_OUT, it need not have to have
       *  corresponding children, because the in/out determination is obviously valid for all
       *  children of that scene tree object.
       *  \sa getChild, getCullCode */
      NVSG_API void addChild( CullData *cd );

      /*! \brief Get the \a i'th child of this CullData.
       *  \param i The Index of the child to get.
       *  \return A pointer to the constant CullData of the \a i'th child.
       *  \sa addChild */
      NVSG_API const CullData * getChild( size_t i ) const;

      /*! \brief Get the nvsg::CullCode of this CullData.
       *  \return The nvsg::CullCode of this CullData. */
      NVSG_API CullCode getCullCode( void ) const;

      /*! \brief Get the number of children of this CullData.
       *  \return The number of children of this CullData.
       *  \remarks The number of children of this CullDat should be zero (if the nvsg::CullCode is
       *  CC_IN or CC_OUT), or the same as the number of children of the scene tree object that
       *  this CullData has the nvsg::CullCode for.
       *  \sa addChild, getChild, getCullCode */
      NVSG_API size_t getNumberOfChildren( void ) const;

    protected:
      /*! \brief Destructor. */
      NVSG_API virtual ~CullData( void );

    private:
      NVSG_API CullData();  // private default constructor to prevent default creation

    private:
      CullCode          m_cullCode;
      std::vector<CullData*> m_children;
  };

  /*! \relates CullData
   *  \brief Clone a CullData by performing deep copy operation.
   *  \param rhs A pointer to the constant CullData to clone.
   *  \return A pointer to the cloned CullData. */
  NVSG_API CullData * clone( const CullData * rhs );

  /*! \relates CullData
   *  \brief Test two CullData for beeing equal
   *  \param lhs A reference to the constant left hand side of the comparison.
   *  \param rhs A reference to the constant right hand side of the comparison.
   *  \return \c true, if the two CullData are equal, otherwise \c false.
   *  \remarks Two CullData are considered to be equal, if they have the same nvsg::CullCode and
   *  the same number of children, and each of the children are pairwise equal.
   *  \sa CullData */
  bool operator==( const CullData &lhs, const CullData &rhs );

  /*! \relates CullData
   *  \brief Test two CullData for beeing unequal.
   *  \param lhs A reference to the constant left hand side of the comparison.
   *  \param rhs A reference to the constant right hand side of the comparison.
   *  \return \c true, if the two CullData are unequal, otherwise \c false.
   *  \remarks Two CullData are considered to be equal, if they have the same nvsg::CullCode and
   *  the same number of children, and each of the children are pairwise equal.
   *  \sa CullData */
  bool operator!=( const CullData &lhs, const CullData &rhs );

  // - - - - - - - - - - - - - - - - - - - - -
  // inlines
  // - - - - - - - - - - - - - - - - - - - - -

  inline  void  CullData::addChild( CullData *cd )
  {
    NVSG_TRACE();
    NVSG_ASSERT(cd);
    cd->addRef();
    m_children.push_back( cd );
  }

  inline  const CullData  * CullData::getChild( size_t i ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( i < m_children.size() );
    return( m_children[i] );
  }

  inline  CullCode  CullData::getCullCode( void ) const
  {
    NVSG_TRACE();
    return( m_cullCode );
  }

  inline  size_t    CullData::getNumberOfChildren( void ) const
  {
    NVSG_TRACE();
    return( m_children.size() );
  }

} //  namespace nvsg
